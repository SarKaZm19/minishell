#include "minishell.h"

// check: https://www.gnu.org/savannah-checkouts/gnu/bash/manual/bash.html#Command-Search-and-Execution
// and https://www.gnu.org/savannah-checkouts/gnu/bash/manual/bash.html#Executing-Commands
int	execute_command(t_ast_command *cmd, t_execute_end end, t_shell *sh)
{
	pid_t			pid;
	int				status;
	builtin_func	builtin;

	status = EXIT_SUCCESS;
	builtin = get_builtin_function(cmd->cmd_exe[0]);
	if (builtin)
		return(builtin(cmd, sh));
	else if (end == O_EXIT)
		s_execve(get_bin_path(cmd->cmd_exe[0], sh), cmd->cmd_exe, env_to_char_array(sh), sh);
	else
	{
		pid = s_fork(sh);
		if (pid == 0)
		{
			sh->in_main_process = false;
			set_signal_child_process();
			s_execve(get_bin_path(cmd->cmd_exe[0], sh), cmd->cmd_exe, env_to_char_array(sh), sh);
		}
		s_wait(&status, sh);
		status = check_process_child_exit(status, NULL, sh);
	}
	return (status);
}
