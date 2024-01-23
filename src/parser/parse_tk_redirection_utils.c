#include "minishell.h"

t_ast	*get_last_of_redirection_list(t_ast *node)
{
	while (node && node->type == AST_REDIRECTION && node->data.redirection.child
		&& node->data.redirection.child->type == AST_REDIRECTION)
		node = node->data.redirection.child;
	return (node);
}
