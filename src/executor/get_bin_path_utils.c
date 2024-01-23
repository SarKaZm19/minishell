#include "minishell.h"

/**
 *	@brief Check if a binary is executable
 * 	@note as EACCES and ENOENT are handled by minishell, they are managed upper
*/
bool	check_access(char *bin_path, t_shell *sh)
{
	if (access(bin_path, X_OK) == EXIT_SUCCESS)
		return (true);
	if (errno != 0 && errno != EACCES && errno != ENOENT)
		syscall_error(bin_path, errno, sh);
	return (false);
}

/**
 *	@brief Set sh->saved_paths_array
 *		Take the PATH environment variable or default path and split it into an array of strings
 *	@note default paths is not describe by bash manual, but it is a common behavior
 *	@todo test: manual: A null directory name (= CURENT DIR) may appear as two adjacent colons, or as an initial or trailing colon. 
 */
char	**build_paths_array(t_shell *sh)
{
	char	**paths_array;
	char	*paths;
	int		i;
	
	const char *default_paths = "/bin:/sbin:/usr/bin:/usr/sbin:/usr/local/bin:\
		/usr/local/sbin:/opt/bin:/opt/sbin";
	paths_array = NULL;
	if (!get_env_var("PATH", sh->env))
		paths = s_alloc(ft_strdup(default_paths), PROMPT, sh);
	else
		paths = env_value(get_env_var("PATH", sh->env));
	paths_array = s_alloc(ft_split(paths, ':'), PROMPT, sh);
	if (check_current_dir(paths))
		add_arg_to_array(&paths_array, ".", sh);
	i = 0;
	while (paths_array[i])
		track_alloc(paths_array[i++], PROMPT, sh);
	return (paths_array);
}

bool	check_current_dir(char *paths)
{
	if (paths[0] == ':' || paths[ft_strlen(paths) - 1] == ':')
		return (true);
	if (ft_strnstr(paths, "::", ft_strlen(paths)) != NULL)
		return (true);
	return (false);
}

/**
 *	@brief Join a directory path and a binary name
 *	@result full path ready to be used in execve
 */
char	*join_path(char *path_to, char *file, t_shell *sh)
{
	char	*slash_path;
	char	*full_path;

	slash_path = ft_strjoin(path_to, "/");
	if (!slash_path)
		error("ft_strjoin", "fail to join file path", EXIT_FAILURE, sh);
	full_path = ft_strjoin(slash_path, file);
	if (!full_path)
		error("ft_strjoin", "fail to join file path", EXIT_FAILURE, sh);
	free(slash_path);
	return (full_path);
}
