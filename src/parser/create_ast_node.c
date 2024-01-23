#include "minishell.h"

void	init_ast_node(t_ast **node, t_ast_type type, t_shell *sh)
{
	*node = ft_calloc(1, sizeof(t_ast));
	s_alloc(*node, PROMPT, sh);
	(*node)->type = type;
}

/**
 * @brief	Set the syntax error message only if there is no previous 
 * 			error as only the first error will be reported.
 * @return	NULL to be able to return NULL in the calling function.
 */
// is it the right place for this function ?
void	*set_syntax_error(char *unexpected_token, t_shell *sh)
{
	if (!sh->parsing_error)
		sh->parsing_error = unexpected_token;
	return (NULL);
}

t_ast	*create_ast_command(char **argv, t_shell *sh)
{
	t_ast	*node;

	init_ast_node(&node, AST_COMMAND, sh);
	node->data.command.cmd_exe = argv;
	return (node);
}

t_ast	*create_ast_group(t_ast *child, t_shell *sh)
{
	t_ast	*node;

	if(!child)
		return(set_syntax_error("(", sh));
	init_ast_node(&node, AST_GROUP, sh);
	node->data.group.child = child;
	return (node);
}

t_ast	*create_ast_redirection(t_token_type direction, t_list *tk_filename,
		t_ast *child, t_shell *sh)
{
	t_ast	*node;

	if (!tk_filename || tk_type(tk_filename) != TK_WORD)
		return(set_syntax_error(tk_type_to_string(direction), sh));
	init_ast_node(&node, AST_REDIRECTION, sh);
	node->data.redirection.direction = direction;
	node->data.redirection.file = tk_value(tk_filename);
	node->data.redirection.child = child;
	return (node);
}

t_ast	*create_ast_pipeline(t_ast *left, t_ast *right, t_shell *sh)
{
	t_ast	*node;

	if(!left || !right)
		return(set_syntax_error("|", sh));
	init_ast_node(&node, AST_PIPELINE, sh);
	node->data.pipeline.left = left;
	node->data.pipeline.right = right;
	return (node);
}

t_ast	*create_ast_logical(t_ast *left, t_token_type operator, t_ast * right, t_shell *sh)
{
	t_ast *node;

	if(!left || !right)
		return(set_syntax_error(tk_type_to_string(operator), sh));
	init_ast_node(&node, AST_LOGICAL, sh);
	node->data.logical.operator= operator;
	node->data.logical.left = left;
	node->data.logical.right = right;
	return (node);
}
