#include "minishell.h"

int	prompt_execution(char *user_input, t_shell *sh)
{
	t_list	*token_list;
	t_ast	*ast;
	int		status;

	status = lexer(user_input, &token_list, sh);
	if (status == EXIT_SUCCESS && token_list)
	{
		// print_tokens(token_list); // debug
		status = parser(token_list, &ast, sh);
		if (status == EXIT_SUCCESS && ast)
		{
			// write_ast_to_file(ast, "ast.json"); // debug
			status = execute_heredocs(ast, sh);
			if (status == EXIT_SUCCESS)
				status = execute(ast, O_RETURN, sh);
		}
	}
	return (status);
}
