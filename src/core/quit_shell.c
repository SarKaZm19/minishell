
#include "minishell.h"

void	quit_shell(int exit_status, t_shell *sh)
{
	// if (sh && sh->termios_backup)
	// 	tcsetattr(STDIN_FILENO, TCSANOW, sh->termios_backup);
	if (sh->in_main_process && isatty(STDIN_FILENO))
		s_write_fd("exit\n", STDERR_FILENO, sh);
	if (sh)
	{
		clear_prompt(sh);
		ft_lstclear(&sh->saved_bin_paths, free_env_var);
		ft_lstclear(&sh->env, free_env_var);
		ft_lstclear(&sh->allocated_pointers[SH], free);
	}
	// todo: enable when readline is in lib/
	// rl_clear_history();
	exit(exit_status);
}
