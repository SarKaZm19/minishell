#include "minishell.h"

void	error(const char *context, char *description, int exit_status,
		t_shell *sh)
{
	ft_write_fd("minishell: ", 2);
	ft_write_fd(description, 2);
	ft_write_fd(": ", 2);
	ft_write_fd(context, 2);
	ft_write_fd("\n", 2);
	quit_shell(exit_status, sh);
}

void	syscall_error(const char *context, int errnum, t_shell *sh)
{
	error(context, strerror(errnum), EXIT_FAILURE, sh);
}

int	report_errno(char *context, t_shell *sh)
{
	int errnum;

	errnum = errno;
	s_write_fd("minishell: ", 2, sh);
	perror(context);
	return (EXIT_FAILURE);
}

int	report_syntax_error(t_shell *sh)
{
	s_write_fd("syntax error near unexpected token `", STDERR_FILENO, sh);
	s_write_fd(sh->parsing_error, STDERR_FILENO, sh);
	s_write_fd("'\n", STDERR_FILENO, sh);
	sh->parsing_error = NULL;
	return (2);
}

ssize_t	s_write_fd(const char *s, int fd, t_shell *sh)
{
	ssize_t	ret;

	ret = ft_write_fd(s, fd);
	if (ret == -1)
		syscall_error("ft_write_fd", errno, sh);
	return (ret);
}