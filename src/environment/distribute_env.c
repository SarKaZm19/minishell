#include "minishell.h"

/**
 *	@brief Prints the environment variables list in STDOUT.
 *	@result "NAME=value\n" or "export NAME="value"\n"
 *	@param format options to print as env or export
 */
int print_env(bool export_format, t_list *env, t_shell *sh)
{
    while (env)
    {
		if (!export_format && !env_value(env))
		{
			env = env->next;
			continue;
		}
        if (export_format)
            s_write_fd("declare -x ", STDOUT_FILENO, sh);
        s_write_fd(env_name(env), STDOUT_FILENO, sh);
        if (env_value(env))
			s_write_fd("=", STDOUT_FILENO, sh);
        if (export_format && env_value(env))
            s_write_fd("\"", STDOUT_FILENO, sh);
		if (env_value(env))
        	s_write_fd(env_value(env), STDOUT_FILENO, sh);
        if (export_format && env_value(env))
            s_write_fd("\"", STDOUT_FILENO, sh);
        s_write_fd("\n", STDOUT_FILENO, sh);
        env = env->next;
    }
	return (EXIT_SUCCESS);
}

/**
 *	@brief convert the environment variables list to a char **
 *	@result array of strings with the format "NAME=value"
 *	@note this function is called for each command execution, it could be optimized by storing the array and reallocating it only when needed
 */
char **env_to_char_array(t_shell *sh)
{
	char **env_array;
	char *name_and_equal;
	int i;
	t_list *env;

	env = sh->env;
	env_array = s_alloc(ft_calloc(ft_lstsize(env) + 1, sizeof(char *)), PROMPT, sh);
	i = 0;
	while (env)
	{
		if (env_value(env))
		{
			name_and_equal = s_alloc(ft_strjoin(env_name(env), "="), ONLY_CHECK, sh);
			env_array[i] = s_alloc(ft_strjoin(name_and_equal, env_value(env)), PROMPT, sh);
			free(name_and_equal);
			i++;
		}
		env = env->next;
	}
	env_array[i] = NULL;
	return (env_array);
}
