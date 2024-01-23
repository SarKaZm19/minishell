#include "minishell.h"

void	set_signal_handler(int signum, void (*handler)(int))
{
	struct sigaction	sa;

	sa.sa_handler = handler;
	sa.sa_flags = 0; // or SA_RESTART
	sigemptyset(&sa.sa_mask);
	sigaction(signum, &sa, NULL);
}

void	set_signal_prompt(void)
{
	set_signal_handler(SIGINT, sigint_prompt);
	set_signal_handler(SIGQUIT, SIG_IGN);
}

void	set_signal_main_process(void)
{
	set_signal_handler(SIGINT, SIG_IGN);
	set_signal_handler(SIGQUIT, SIG_IGN);
	// EOF is not a signal, so it's not handled here.
}

void	set_signal_child_process(void)
{
	set_signal_handler(SIGINT, SIG_DFL);
	set_signal_handler(SIGQUIT, SIG_DFL);
	// EOF is handled by the command's standard input mechanisms.
}

void	set_signal_heredoc(void)
{
	set_signal_handler(SIGINT, sigint_heredoc);
	set_signal_handler(SIGQUIT, SIG_IGN);
}
