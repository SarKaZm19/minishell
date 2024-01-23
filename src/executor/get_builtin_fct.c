#include "minishell.h"

builtin_func	get_builtin_function(char *cmd_name)
{
	int						i;
	const t_builtin	builtins[] = {{"echo", echo_builtin}, {"cd",
			cd_builtin}, {"pwd", pwd_builtin}, {"export", export_builtin},
			{"unset", unset_builtin}, {"env", env_builtin}, {"exit",
			exit_builtin}, {NULL, NULL}};

	i = 0;
	while (builtins[i].name)
	{
		if (ft_strcmp(cmd_name, builtins[i].name) == 0)
			return (builtins[i].func);
		i++;
	}
	return (NULL);
}

// builtin_func	get_builtin_function(char *cmd_name)
// {
// 	if (ft_strcmp(cmd_name, "echo") == 0)
// 		return (echo_builtin);
// 	if (ft_strcmp(cmd_name, "cd") == 0)
// 		return (cd_builtin);
// 	if (ft_strcmp(cmd_name, "pwd") == 0)
// 		return (pwd_builtin);
// 	if (ft_strcmp(cmd_name, "export") == 0)
// 		return (export_builtin);
// 	if (ft_strcmp(cmd_name, "unset") == 0)
// 		return (unset_builtin);
// 	if (ft_strcmp(cmd_name, "env") == 0)
// 		return (env_builtin);
// 	if (ft_strcmp(cmd_name, "exit") == 0)
// 		return (exit_builtin);
// 	return (NULL);
// }
