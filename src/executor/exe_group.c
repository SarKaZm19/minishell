#include "minishell.h"

int	execute_group(t_ast_group *group, t_shell *sh)
{
	pid_t pid;
	int status;

	pid = s_fork(sh);
	if (pid == 0)
	{
		sh->in_main_process = false;
		set_signal_child_process();
		execute(group->child, O_EXIT, sh);
	}
	s_wait(&status, sh);
	status = check_process_child_exit(status, NULL, sh);
	return (status);
}
