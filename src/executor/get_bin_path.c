#include "minishell.h"

char	*get_bin_path(char *bin, char **env_path, t_shell *sh)
{
	if (ft_strchr(bin, '/'))
		return (check_bin_with_path(bin, sh));
	return (find_path(bin, env_path, sh));
}

char	*check_bin_with_path(char *bin, t_shell *sh)
{
	if (check_access(bin, sh))
		return (bin);
	if (errno == EACCES)
		error("permission denied", bin, 126, sh);
	else if (errno != ENOENT)
		error("command not found", bin, 127, sh);
	return (NULL);
}

char	*find_path(char *bin, char **env_path, t_shell *sh)
{
	int		i;
	bool	permission_denied;
	char	*full_path;

	i = 0;
	permission_denied = false;
	if (!env_path)
		error("command not found", bin, 127, sh);
	while (env_path[i])
	{
		full_path = join_path(env_path[i], bin, sh);
		if (check_access(full_path, sh))
			return (full_path);
		if (errno == EACCES)
			permission_denied = true;
		free(full_path);
		i++;
	}
	if (permission_denied)
		error("permission denied", bin, 126, sh);
	else
		error("command not found", bin, 127, sh);
	return (NULL);
}

char	*join_path(char *dir, char *file, t_shell *sh)
{
	char	*slash_path;
	char	*full_path;

	slash_path = ft_strjoin(dir, "/");
	if (!slash_path)
		error("ft_strjoin", "fail to join file path", EXIT_FAILURE, sh);
	full_path = ft_strjoin(slash_path, file);
	if (!full_path)
		error("ft_strjoin", "fail to join file path", EXIT_FAILURE, sh);
	free(slash_path);
	return (full_path);
}

bool	check_access(char *bin, t_shell *sh)
{
	if (access(bin, X_OK) == EXIT_SUCCESS)
		return (true);
	if (errno != 0 && errno != EACCES && errno != ENOENT)
		syscall_error(bin, errno, sh);
	return (false);
}
