#include "minishell.h"

volatile sig_atomic_t	g_signal_value = 0;
extern void rl_replace_line (const char *, int);

void	sigint_prompt(int signum)
{
	g_signal_value = signum;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	sigint_heredoc(int signum)
{
	g_signal_value = signum;
	s_write_fd("\n", 1, NULL);
}

// an other implementation, but I don't see particular advantages
// void	sigint_heredoc(int signum)
// {
// 	struct termios	original_termios;
// 	struct termios	new_termios;

// 	g_signal_value = signum;
// 	tcgetattr(STDIN_FILENO, &original_termios);
// 	tcgetattr(STDIN_FILENO, &new_termios);
// 	new_termios.c_lflag &= (tcflag_t)(~(ICANON | ECHO));
// 	tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);
// 	ioctl(STDIN_FILENO, TIOCSTI, "\n");
// 	tcsetattr(STDIN_FILENO, TCSANOW, &original_termios);
// }
