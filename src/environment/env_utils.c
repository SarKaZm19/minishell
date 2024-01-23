#include "minishell.h"

t_list*	get_env_var(char *name, t_list *env)
{
	t_env_var *var;

	while (env)
	{
		var = (t_env_var *)(env->content);
		if (ft_strcmp(name, var->name) == 0)
			return (env);
		env = env->next;
	}
	return (NULL);
}

char *env_name(t_list *env)
{
	// if (!env)
	// 	return (NULL);
	return (((t_env_var *)(env->content))->name);
}

char *env_value(t_list *env)
{
	if (!env)
		return (NULL);
	return (((t_env_var *)(env->content))->value);
}

/**
 *  @brief If PATH is modified, delete caches to rebuild them at next execution
 */
void	check_for_modified_path(char *modified_var, t_shell *sh)
{
	if (ft_strcmp(modified_var, "PATH") == 0)
	{
		sh->saved_paths_array = NULL;
		ft_lstclear(&sh->saved_bin_paths, free_env_var);
	}
}
