#include "minishell.h"

// check: https://www.gnu.org/savannah-checkouts/gnu/bash/manual/bash.html#Command-Search-and-Execution
// and https://www.gnu.org/savannah-checkouts/gnu/bash/manual/bash.html#Executing-Commands

// todo: detect if the command is a builtin
// todo: fork option (the command process could be already forked if it come from pipeline)
// Is `(ls)` doubly forked?
int	execute_command(t_ast_command *cmd, t_shell *sh)
{
	pid_t			pid;
	int				status;
	builtin_func	builtin;

	builtin = get_builtin_function(cmd->cmd_exe[0]);
	status = EXIT_SUCCESS;
	if (builtin && cmd->in_pipeline) // builtin in pipeline is executed in a subshell
		exit(builtin(cmd, sh));
	else if (builtin) // simple builtin is executed in the same process
		return(builtin(cmd, sh));
	else if (cmd->in_pipeline) // already forked by execute_pipeline
		s_execve(get_bin_path(cmd->cmd_exe[0], sh->env_paths, sh), cmd->cmd_exe, sh->env, sh);
	else // command is executed subshell
	{
		pid = s_fork(sh);
		if (pid == 0)
			s_execve(get_bin_path(cmd->cmd_exe[0], sh->env_paths, sh), cmd->cmd_exe, sh->env, sh);
		waitpid(pid, &status, 0);
	}
	return (status);
}

builtin_func	get_builtin_function(char *cmd_name)
{
	if (ft_strcmp(cmd_name, "echo") == 0)
		return (builtin_echo);
	if (ft_strcmp(cmd_name, "cd") == 0)
		return (builtin_cd);
	if (ft_strcmp(cmd_name, "pwd") == 0)
		return (builtin_pwd);
	if (ft_strcmp(cmd_name, "export") == 0)
		return (builtin_export);
	if (ft_strcmp(cmd_name, "unset") == 0)
		return (builtin_unset);
	if (ft_strcmp(cmd_name, "env") == 0)
		return (builtin_env);
	if (ft_strcmp(cmd_name, "exit") == 0)
		return (builtin_exit);
	return (NULL);
}


// more elegant approach, but I didn't find how to avoid the builtins[] array
// builtin_func	*get_builtin_function(char *cmd_name)
// {
// 	int	i;
// 	const static t_builtin builtins[] = {
// 			{"echo", builtin_echo},
// 			{"cd", builtin_cd},
// 			{"pwd", builtin_pwd},
// 			{"export", builtin_export},
// 			{"unset", builtin_unset},
// 			{"env", builtin_env},
// 			{"exit", builtin_exit},
// 			{NULL, NULL}
// 	};

// 	i = 0;
// 	while (builtins[i].name)
// 	{
// 		if (ft_strcmp(cmd_name, builtins[i].name) == 0)
// 			return (builtins[i].func);
// 		i++;
// 	}
// 	return (NULL);
// }