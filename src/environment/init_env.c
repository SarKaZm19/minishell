#include "minishell.h"

t_list	*init_env(char **env, t_shell *sh)
{
	char **assignment;
	t_list		*env_list;

	env_list = NULL;
	while (*env)
	{
		assignment = ft_split(*env, '='); // TODO: check ft_split return
		// todo: what append if there is more than 2 "="?
		add_new_env_var(assignment[0], assignment[1], &env_list, sh);
		free_strs(&assignment);
		env++;
	}
	return (env_list);
}

/**
 *	@brief add new env variable to the end of env list
 *	@warning name and value are duplicated
 */
void add_new_env_var(char *name, char *value, t_list **list, t_shell *sh)
{
	t_env_var *var;

	var = s_alloc(ft_calloc(1, sizeof(t_env_var)), ONLY_CHECK, sh);
	var->name = ft_strdup(name);
	if (value == NULL)
		var->value = NULL;
	else
		var->value = ft_strdup(value);
	ft_lstadd_back(list, s_alloc(ft_lstnew(var), ONLY_CHECK, sh));
}
