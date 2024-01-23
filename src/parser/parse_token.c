#include "minishell.h"

// rename file: ast_builders.c or parsing.c

t_ast	*parse_command(t_list **token, t_shell *sh)
{
	t_list	*current;
	int		argc;
	char	**argv;

	current = *token;
	argc = 0;
	while (is_tk_type(&current, 1, TK_WORD))
	{
		argc++;
		current = current->next;
	}
	if (argc == 0)
		return (NULL);
	argv = ft_calloc((argc + 1), sizeof(char *));
	s_alloc(argv, PROMPT, sh);
	for (int i = 0; i < argc; i++)
	{
		argv[i] = tk_value(*token);
		*token = (*token)->next;
	}
	argv[argc] = NULL;
	return (create_ast_command(argv, sh));
}

t_ast	*parse_group(t_list **token, t_shell *sh)
{
	t_ast	*content;

	if (is_tk_type(token, 1, TK_GROUP_OPEN))
	{
		*token = (*token)->next;
		content = parse_logical(token, sh);
		if (*token && tk_type(*token) == TK_GROUP_CLOSE)
		{
			*token = (*token)->next;
			if ((*token) && is_tk_type(token, 1, TK_WORD))
				return (set_syntax_error(tk_value(*token), sh));
			return (create_ast_group(content, sh));
		}
		else if (*token == NULL)
			return (set_syntax_error("\\n", sh));
	}
	else if (is_tk_type(token, 1, TK_GROUP_CLOSE))
		return (set_syntax_error(")", sh));
	return (parse_command(token, sh));
}

// here cf src/parser/parse_tk_redirection.c

t_ast	*parse_pipeline(t_list **token, t_shell *sh)
{
	t_ast	*left;
	t_ast	*right;

	left = parse_redirection(token, sh); // check if this is correct
	while (*token && tk_type(*token) == TK_PIPE)
	{
		*token = (*token)->next;
		right = parse_redirection(token, sh); // bc no logic after pipe
		left = create_ast_pipeline(left, right, sh);
	}
	return (left);
}

t_ast	*parse_logical(t_list **token, t_shell *sh)
{
	t_ast			*left;
	t_token_type	logical_operator;
	t_ast			*right;

	left = parse_pipeline(token, sh);
	while (is_tk_type(token, 2, TK_AND, TK_OR))
	{
		logical_operator = tk_type(*token);
		*token = (*token)->next;
		right = parse_pipeline(token, sh);
		left = create_ast_logical(left, logical_operator, right, sh);
	}
	return (left);
}
