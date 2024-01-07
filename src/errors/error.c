#include "minishell.h"

void	error(const char *context, char *description, int exit_status,
		t_shell *sh)
{
	ft_write_fd("minishell: ", 2);
	ft_write_fd(description, 2);
	ft_write_fd(": ", 2);
	ft_write_fd(context, 2);
	ft_write_fd("\n", 2);
	if (sh)
	{
		ft_lstclear(&sh->allocated_pointers[PROMPT], free);
		ft_lstclear(&sh->allocated_pointers[SH], free);
	}
	exit(exit_status);
}

void	syscall_error(const char *context, int errnum, t_shell *sh)
{
	ft_write_fd("pipex syscall_error: ", 2);
	error(context, strerror(errnum), EXIT_FAILURE, sh);
}

// cf specific case in setup_input pipex
void	report_errno(char *context, t_shell *sh)
{
	s_write_fd("pipex report_errno: ", 2, sh);
	perror(context);
}

void *report_syntax_error(t_shell *sh)
{
	s_write_fd("syntax error near unexpected token `", STDERR_FILENO, sh);
	s_write_fd(sh->parsing_error, STDERR_FILENO, sh);
	s_write_fd("'\n", STDERR_FILENO, sh);
	return (NULL);
}

ssize_t	s_write_fd(const char *s, int fd, t_shell *sh)
{
	ssize_t	ret;

	ret = ft_write_fd(s, fd);
	if (ret == -1)
		syscall_error("ft_write_fd", errno, sh);
	return (ret);
}