#include "minishell.h"

/**
 *  @brief export builtin without options.
 *  @result modify env list
 * 			or print env list + export but unset variables
 *	@param cmd command to execute with cmd->cmd_exe
 *	@param sh shell structure
 */
int export_builtin(t_ast_command *cmd, t_shell *sh)
{
	bool name_error;
	
	cmd->cmd_exe++;
	if (!*(cmd->cmd_exe))
		return(print_env(true, sh->env, sh));
	name_error = false;
	while (*(cmd->cmd_exe))
	{
		export_one(*(cmd->cmd_exe), &name_error, sh);
		cmd->cmd_exe++;
	}
	if (name_error)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

/**
 *  @brief export one variable
 *  @result modify env variable
 * 			or add env variable to the end of env list
 *	@param assignment string of the form "name=value"
 *	@param name_error pointer to a boolean to set to true if there is a name error
 *	@param sh shell structure
 *  @todo check input `export =value` and add to mpanic
 */
void	export_one(char *assignment, bool *name_error, t_shell *sh)
{
	char		*equal_sign;
	char		*name;
	t_list		*env_var;
	bool		add;

    equal_sign = ft_strchr(assignment, '=');
	add = false;
	if (ft_strnstr(assignment, "+=", equal_sign - assignment + 1))
		add = true;
	name = s_alloc(ft_substr(assignment, 0, equal_sign - add - assignment), ONLY_CHECK, sh);
	if (!is_valid_name(name))
		return (report_name_error(assignment, name_error, sh));
	env_var = get_env_var(name, sh->env);
	if (env_var && equal_sign)
		edit_env_value(env_var, equal_sign + 1, add, sh);
	else if (!env_var && !equal_sign)
		add_new_env_var(name, NULL, &(sh->env), sh);
	else if (!env_var && equal_sign)
		add_new_env_var(name, equal_sign + 1, &(sh->env), sh);
	free(name);
}

void report_name_error(char *assignment, bool *name_error, t_shell *sh)
{
	s_write_fd("minishell: export: `", STDERR_FILENO, sh);
	s_write_fd(assignment, STDERR_FILENO, sh);
	s_write_fd("': not a valid identifier\n", STDERR_FILENO, sh);
	*name_error = true;
}

void	edit_env_value(t_list *env_var, char *new_value, bool add, t_shell *sh)
{
	t_env_var *var;
	char *tmp;

	var = (t_env_var *)(env_var->content);
	tmp = var->value;
	if (add)
		var->value = s_alloc(ft_strjoin(var->value, new_value), ONLY_CHECK, sh);
	else
		var->value = s_alloc(ft_strdup(new_value), ONLY_CHECK, sh);
	free(tmp);
}

bool is_valid_name(char *name)
{
	if (!ft_isalpha(*name) && *name != '_')
		return (false);
	name++;
	while (*name)
	{
		if (!ft_isalnum(*name) && *name != '_')
			return (false);
		name++;
	}
	return (true);
}
