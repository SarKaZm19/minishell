#include "minishell.h"

// todo: quote handling
int	builtin_echo(t_ast_command *cmd, t_shell *sh)
{
	(void)sh;
	bool new_line = true;
	int i = 1;

	if (cmd->cmd_exe[i] && strcmp(cmd->cmd_exe[i], "-n") == 0)
	{
		new_line = false;
		i++;
	}
	while (cmd->cmd_exe[i])
	{
		s_write_fd(cmd->cmd_exe[i], STDOUT_FILENO, sh);
		if (cmd->cmd_exe[i + 1])
			s_write_fd(" ", STDOUT_FILENO, sh);
		i++;
	}
	if (new_line)
		printf("\n");
	return (EXIT_SUCCESS);
}