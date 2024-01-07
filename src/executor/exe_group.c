#include "minishell.h"

int	execute_group(t_ast_group *group, t_shell *sh)
{
	pid_t	pid;
	int		status;

	pid = s_fork(sh);
	if (pid == 0)
	{
		// work if there is a forked command in the group
		// check if there isn't fork in he group (if grammar allows it)
		exit(execute(group->child, sh));
	}
	waitpid(pid, &status, 0);
	return (status);
}