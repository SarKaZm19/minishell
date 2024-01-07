#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_shell	sh;
	int status;
	
	setup_signal_handling();
	init_shell(&sh, envp);
    if (argc == 3 && strcmp(argv[1], "-c") == 0)
        status = shell_process(argv[2], &sh);
    else
		status = interactive_shell(&sh);
	ft_lstclear(&(sh.allocated_pointers[SH]), free);
	exit(status);
}

// parse_input(input);
// execute_command(input);
void	setup_signal_handlers(void)
{
	return ;
}
