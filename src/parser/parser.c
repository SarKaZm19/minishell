#include "minishell.h"

/**
 * @brief	Syntax analysis (cf abstract_syntax_tree.md).
 *			Converts the tokens list into an abstract syntax tree.
 * @return	The abstract syntax tree. NULL if there is a syntax error.
 */
int	parser(t_list *token_list, t_ast **ast, t_shell *sh)
{
	*ast = parse_logical(&token_list, sh);
	if(sh->parsing_error)
		return (report_syntax_error(sh));
	return (EXIT_SUCCESS);
}
