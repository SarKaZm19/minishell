#include "minishell.h"

int	echo_builtin(t_ast_command *cmd, t_shell *sh)
{
	bool	new_line;
	int		i;

	(void)sh;
	i = 1;
	new_line = !(option_check(cmd->cmd_exe, &i, 'n'));
	while (cmd->cmd_exe[i])
	{
		// While waiting for variable expansion
		if (strcmp(cmd->cmd_exe[i], "$?") == 0)
		{
			s_write_fd(ft_itoa(sh->last_prompt_exit_status), STDOUT_FILENO, sh);
			i++;
			continue ;
		}
		s_write_fd(cmd->cmd_exe[i], STDOUT_FILENO, sh);
		if (cmd->cmd_exe[i + 1])
			s_write_fd(" ", STDOUT_FILENO, sh);
		i++;
	}
	if (new_line)
		s_write_fd("\n", STDOUT_FILENO, sh);
	return (EXIT_SUCCESS);
}

bool	option_check(char **args, int *i, char c)
{
	int		j;
	bool	detected;

	detected = false;
	while (args[*i] && args[*i][0] == '-')
	{
		j = 1;
		while (args[*i][j] == c)
			j++;
		if (args[*i][j - 1] == c && args[*i][j] == '\0')
		{
			detected = true;
			(*i)++;
		}
		else if (!detected)
			return (false);
	}
	return (detected);
}
