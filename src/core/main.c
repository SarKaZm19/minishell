#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_shell	sh;
	int		status;

	// setup_signal_handling();
	set_signal_main_process();
	init_shell(&sh, envp);
	if (argc == 3 && strcmp(argv[1], "-c") == 0)
		status = prompt_execution(argv[2], &sh);
	else
		status = interactive_shell(&sh);
	quit_shell(status, &sh);
}
