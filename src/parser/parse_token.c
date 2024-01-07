#include "minishell.h"

// rename file: ast_builders.c or parsing.c

t_AST	*parse_command(t_list **token_list, t_shell *sh)
{
	t_list	*current;
	int		argc;
	char	**argv;

	current = *token_list;
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
		argv[i] = tk_value(*token_list);
		*token_list = (*token_list)->next;
	}
	argv[argc] = NULL;
	return (create_ast_command(argv, sh));
}

t_AST	*parse_group(t_list **token_list, t_shell *sh)
{
	t_AST	*content;

	if (is_tk_type(token_list, 1, TK_GROUP_OPEN))
	{
		*token_list = (*token_list)->next;
		content = parse_logical(token_list, sh);
		if (*token_list && tk_type(*token_list) == TK_GROUP_CLOSE)
		{
			*token_list = (*token_list)->next;
			return (create_ast_group(content, sh));
		}
		else if (*token_list == NULL)
		{
			printf("syntax error: missing closing parenthesis\n");
			return (NULL);
		}
	}
	return (parse_command(token_list, sh));
}

// here cf src/parser/parse_tk_redirection.c

t_AST	*parse_pipeline(t_list **token_list, t_shell *sh)
{
	t_AST	*left;
	t_AST	*right;

	left = parse_redirection(token_list, sh); // check if this is correct
	while (*token_list && tk_type(*token_list) == TK_PIPE)
	{
		*token_list = (*token_list)->next;
		right = parse_redirection(token_list, sh); // bc no logic after pipe
		left = create_ast_pipeline(left, right, sh);
	}
	return (left);
}

t_AST	*parse_logical(t_list **token_list, t_shell *sh)
{
	t_AST			*left;
	t_token_type	logical_operator;
	t_AST			*right;

	left = parse_pipeline(token_list, sh);
	while (is_tk_type(token_list, 2, TK_AND, TK_OR))
	{
		logical_operator = tk_type(*token_list);
		*token_list = (*token_list)->next;
		right = parse_pipeline(token_list, sh);
		left = create_ast_logical(left, logical_operator, right, sh);
	}
	return (left);
}
