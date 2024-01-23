#include "minishell.h"


t_list*	get_env_var(char *name, t_list *env);
void	remove_env_var(t_list *env, t_shell *sh);
void	free_env_var(void *env);

/**
 *  @brief unset builtin without options.
 *  @result remove env variable from env list
 *	@param cmd command to execute with cmd->argv
 *	@param sh shell structure
 */
int unset_builtin(t_ast_command *cmd, t_shell *sh)
{
	while (*(cmd->cmd_exe))
	{
		check_for_modified_path(*(cmd->cmd_exe), sh);
		remove_env_var(get_env_var(*(cmd->cmd_exe), sh->env), sh);
		cmd->cmd_exe++;
	}
	return (EXIT_SUCCESS);
}

/**
 *  @brief remove env variable from env list
 *	@param env a non-null node from env list
 *	@param sh shell structure
 *  @return
 */
void	remove_env_var(t_list *env, t_shell *sh)
{
	if (!env)
		return ;
	if (env->next && env->prev)
		env->prev->next = env->next;
	else if (env->next && !env->prev)
		sh->env = env->next;
	else if (!env->next && env->prev)
		env->prev->next = NULL;
	ft_lstdelone(env, free_env_var);
}

void	free_env_var(void *content)
{
	t_env_var *var;
	
	var = (t_env_var *)content;
	free (var->name);
	free (var->value);
	free (var);
}
