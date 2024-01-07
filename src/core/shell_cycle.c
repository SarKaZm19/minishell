
#include "minishell.h"

// lexer -> parser -> expander -> executor
int	interactive_shell(t_shell *sh)
{
	char	*input;
	int		status;

	while (true)
	{
		if (g_sigint_received) 
		{
            s_write_fd("\n> ", STDOUT_FILENO, sh);
            g_sigint_received = 0;
            continue;
        }

		input = readline("> ");
		if (!input) // Exit on ctrl-D
			exit(EXIT_SUCCESS);
		add_history(input);
		status = shell_process(input, sh);
		// fprintf(stderr, "STATUS: %d\n", status); // debug
		free(input);
	}
	return (status);
}

int	shell_process(char *input, t_shell *sh)
{
	t_list	*token_list;
	t_AST	*ast;
	int		status;

	status = EXIT_FAILURE;
	//
	token_list = lexer(input, sh);
	if (token_list)
	{
		print_tokens(token_list); // debug
		ast = parser(token_list, sh);
		if (ast)
		{
			write_ast_to_file(ast, "ast.json"); // debug
			status = execute(ast, sh);
		}
	}
	ft_lstclear(&(sh->allocated_pointers[PROMPT]), free);
	sh->parsing_error = NULL;
	return (status);
}
