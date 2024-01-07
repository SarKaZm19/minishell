#include "minishell.h"

int	s_open(const char *pathname, int flags, mode_t mode, t_shell *sh)
{
	int	ret;

	ret = open(pathname, flags, mode);
	if (ret == -1)
		syscall_error(pathname, errno, sh);
	return (ret);
}

int	s_close(int fd, t_shell *sh)
{
	int	ret;

	ret = close(fd);
	if (ret == -1)
		syscall_error("close", errno, sh);
	return (ret);
}

int	s_dup2(int oldfd, int newfd, t_shell *sh)
{
	int	ret;

	ret = dup2(oldfd, newfd);
	if (ret == -1)
		syscall_error("dup2", errno, sh);
	return (ret);
}

int	s_unlink(const char *pathname, t_shell *sh)
{
	int	ret;

	ret = unlink(pathname);
	if (ret == -1)
		syscall_error("unlink", errno, sh);
	return (ret);
}
