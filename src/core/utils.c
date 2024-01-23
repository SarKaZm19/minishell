#include "minishell.h"

/**
 *	@file src/core/utils.c
 *	@brief General utils functions used across the project
 */

/**
 *	@brief Add a string to the end of an array of strings
 *	@warning to use only for PROMPT operations (allocation scope)
 *	@warning dont free the old array
 */
void	add_arg_to_array(char ***array, char *new_arg, t_shell *sh)
{
	char	**new_array;
	int		i;

	i = 0;
	while ((*array)[i])
		i++;
	new_array = s_alloc(ft_calloc(i + 2, sizeof(char *)), PROMPT, sh);
	i = 0;
	while ((*array)[i])
	{
		new_array[i] = (*array)[i];
		i++;
	}
	new_array[i] = new_arg;
	new_array[i + 1] = NULL;
	*array = new_array;
}

void	free_strs(char ***strs)
{
	int	i;

	i = 0;
	while ((*strs)[i])
		free((*strs)[i++]);
	free(*strs);
	*strs = NULL;
}
