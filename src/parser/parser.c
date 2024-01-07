#include "minishell.h"

/**
 * @brief	Syntax analysis (cf abstract_syntax_tree.md).
 *			Converts the tokens list into an abstract syntax tree.
 * @return	The abstract syntax tree. NULL if there is a syntax error.
 */
t_AST	*parser(t_list *token_list, t_shell *sh)
{
	t_AST	*ast;
	ast = parse_logical(&token_list, sh);
	if(sh->parsing_error)
		return (report_syntax_error(sh));
	return (ast);
}

//  As we have sh struct, we can write in it a parsing error flag with the error message
// after all parsing, we can check if the flag is set and print the error message

// it mean that everything that could raise an parsing error could return NULL or NULL AST