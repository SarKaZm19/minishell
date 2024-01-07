#include "minishell.h"

t_AST	*parse_redirection(t_list **token_list, t_shell *sh)
{
	t_AST	*prefix;
	t_AST	*suffix;
	t_AST	*command;

	prefix = parse_redirection_list(token_list, NULL, sh);
	command = parse_group(token_list, sh);
	suffix = parse_redirection_list(token_list, command, sh);
	return (build_redirected_command(prefix, suffix, command));
}

t_AST	*parse_redirection_list(t_list **token_list, t_AST *command, t_shell *sh)
{
	t_AST	*first;
	t_AST	*last_parent;
	t_AST	*new_parent;

	first = NULL;
	while (is_tk_type(token_list, 4, TK_REDIRECT_IN, TK_REDIRECT_OUT,
			TK_APPEND_OUT, TK_HEREDOC) || (is_tk_type(token_list, 1, TK_WORD)
			&& command))
	{
		if (is_tk_type(token_list, 1, TK_WORD) && command)
		{
			add_arg_to_command(command, tk_value(*token_list), sh);
			*token_list = (*token_list)->next;
			continue ;
		}
		if (!first)
		{
			first = create_ast_redirection(tk_type(*token_list),
					(*token_list)->next, command, sh);
			last_parent = first;
		}
		else
		{
			new_parent = create_ast_redirection(tk_type(*token_list),
					(*token_list)->next, NULL, sh);
			last_parent->data.redirection.child = new_parent;
			last_parent = new_parent;
		}
		*token_list = (*token_list)->next->next;
	}
	return (first);
}

t_AST	*build_redirected_command(t_AST *prefix, t_AST *suffix, t_AST *child)
{
	if (prefix && suffix)
	{
		get_end_of_redirection_list(prefix)->data.redirection.child = suffix;
		get_end_of_redirection_list(suffix)->data.redirection.child = child;
		return (prefix);
	}
	else if (prefix && !suffix)
	{
		get_end_of_redirection_list(prefix)->data.redirection.child = child;
		return (prefix);
	}
	else if (!prefix && suffix)
	{
		get_end_of_redirection_list(suffix)->data.redirection.child = child;
		return (suffix);
	}
	else
		return (child);
}
