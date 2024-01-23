#include "minishell.h"

t_ast	*parse_redirection(t_list **token, t_shell *sh)
{
	t_ast	*prefix;
	t_ast	*suffix;
	t_ast	*command;

	prefix = parse_redirection_list(token, NULL, sh);
	if (sh->parsing_error)
		return (NULL);
	command = parse_group(token, sh);
	suffix = parse_redirection_list(token, command, sh);
	if (sh->parsing_error)
		return (NULL);
	return (build_redirected_command(prefix, suffix, command));
}

t_ast	*parse_redirection_list(t_list **token, t_ast *command, t_shell *sh)
{
	t_ast	*first;
	t_ast	*last_parent;
	t_ast	*new_parent;

	first = NULL;
	while (is_tk_type(token, 4, TK_REDIRECT_IN, TK_REDIRECT_OUT, TK_APPEND_OUT,
			TK_HEREDOC) || (is_tk_type(token, 1, TK_WORD) && command))
	{
		if (is_tk_type(token, 1, TK_WORD) && command)
		{
			add_arg_to_array(&command->data.command.cmd_exe,
					tk_value(*token), sh);
			*token = (*token)->next;
			continue ;
		}
		if (!first)
		{
			first = create_ast_redirection(tk_type(*token), (*token)->next,
					command, sh);
			last_parent = first;
		}
		else
		{
			new_parent = create_ast_redirection(tk_type(*token), (*token)->next,
					NULL, sh);
			last_parent->data.redirection.child = new_parent;
			last_parent = new_parent;
		}
		if (!first || !new_parent)
			return (NULL);
		*token = (*token)->next->next;
	}
	return (first);
}

t_ast	*build_redirected_command(t_ast *prefix, t_ast *suffix, t_ast *child)
{
	if (prefix && suffix)
	{
		get_last_of_redirection_list(prefix)->data.redirection.child = suffix;
		get_last_of_redirection_list(suffix)->data.redirection.child = child;
		return (prefix);
	}
	else if (prefix && !suffix)
	{
		get_last_of_redirection_list(prefix)->data.redirection.child = child;
		return (prefix);
	}
	else if (!prefix && suffix)
	{
		get_last_of_redirection_list(suffix)->data.redirection.child = child;
		return (suffix);
	}
	else
		return (child);
}
