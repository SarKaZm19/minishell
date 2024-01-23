#include "minishell.h"

/**
 *	@brief Get the path of a binary
 *	@param bin binary name as `ls` or `./bin/ls`
 *	@result path to the binary, an error is raised if not found
 */
char	*get_bin_path(char *bin, t_shell *sh)
{
	if (ft_strchr(bin, '/'))
	{
		if (check_access(bin, sh))
			return (bin);
		else if (errno == EACCES)
			error("Permission denied", bin, 126, sh);
		else if (errno == ENOENT)
			error("No such file or directory", bin, 127, sh);
	}
	else
		return (find_bin_path(bin, sh));
	return (NULL);
}

/**
 *	@brief Find the path of a binary in the PATH environment variable
 *			Use saved_bin_paths and saved_paths_array to save time
 *	@param bin binary name as `ls`
 *	@result path to the binary as `/bin/ls`, an error is raised if not found
 *	@note search function return null if not found, find functions raise error if not found
 *	@todo: in export, if PATH is changed, clear saved_bin_paths list and saved_paths_array list with a new clear_path() function
 */
char	*find_bin_path(char *bin, t_shell *sh)
{
	char	*bin_path;

	if (sh->saved_bin_paths)
	{
		bin_path = search_in_saved_bins_path(bin, sh->saved_bin_paths, sh);
		if (bin_path)
			return (bin_path);
	}
	if (!sh->saved_paths_array)
		sh->saved_paths_array = build_paths_array(sh);
	bin_path = find_with_paths_array(bin, sh->saved_paths_array, sh);
	add_new_env_var(bin, bin_path, &sh->saved_bin_paths, sh);
	return (bin_path);
}

/**
 *	@brief Search the path of a binary in the saved_bin_paths list
 *	@param saved_bin_paths t_list of t_env_var with name = bin name and value = bin path
 *	@note check_access is important as the binary may have been deleted or moved since it was saved
 *	@result path of the binary or NULL if not found or access denied
 */
char	*search_in_saved_bins_path(char *bin, t_list *saved_bin_paths, t_shell *sh)
{
	t_env_var	*saved_path;

	while (saved_bin_paths)
	{
		saved_path = saved_bin_paths->content;
		if (ft_strcmp(bin, saved_path->name) == 0)
		{
			if (check_access(saved_path->value, sh))
				return (saved_path->value);
			else if (errno == EACCES || errno == ENOENT)
			{
				ft_lstclear(&sh->saved_bin_paths, free_env_var);
				errno = 0;
				return (NULL);
			}
		}
		saved_bin_paths = saved_bin_paths->next;
	}
	return (NULL);
}

/**
 *	@brief Search the path of a binary in paths
 *	@arg paths from sh->saved_paths_array
 *	@result (unsaved) path of the binary, raise error if not found or access denied
 */
char	*find_with_paths_array(char *bin_name, char **paths, t_shell *sh)
{
	char	*bin_path;
	bool	permission_denied;

	if (!paths)
		error("command not found", bin_name, 127, sh);
	permission_denied = false;
	while (*paths)
	{
		bin_path = join_path(*paths, bin_name, sh);
		if (check_access(bin_path, sh))
			return (bin_path);
		if (errno == EACCES)
			permission_denied = true;
		free(bin_path);
		paths++;
	}
	if (permission_denied)
		error("Permission denied", bin_name, 126, sh);
	else
		error("command not found", bin_name, 127, sh);
	return (NULL);
}

