#include "minishell.h"

void	*s_alloc(void *pointer, t_tracking_scope scope, t_shell *sh)
{
	if (!pointer)
		error("Memory allocation", strerror(errno), EXIT_FAILURE, sh);
	if (scope != ONLY_CHECK)
		track_alloc(pointer, scope, sh);
	return (pointer);
}

void	track_alloc(void *pointer, t_tracking_scope scope, t_shell *sh)
{
	t_list	*new_node;

	new_node = ft_lstnew(pointer);
	if (!new_node)
	{
		free(pointer);
		error("Memory allocation", strerror(errno), EXIT_FAILURE, sh);
	}
	ft_lstadd_front(&sh->allocated_pointers[scope], new_node);
}
