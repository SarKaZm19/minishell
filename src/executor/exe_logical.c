#include "minishell.h"

int	execute_logical(t_ast_logical *logical, t_shell *sh)
{
	int left_status;
	int	right_status;

	left_status = execute(logical->left, O_RETURN, sh);
	sh->last_prompt_exit_status = left_status;
	if (logical->operator== TK_AND)
	{
		if (left_status == EXIT_SUCCESS)
		{
			right_status = execute(logical->right, O_RETURN, sh);
			return (right_status);
		}
		return (left_status);
	}
	else if (logical->operator== TK_OR)
	{
		if (left_status != EXIT_SUCCESS)
		{
			right_status = execute(logical->right, O_RETURN, sh);
			return (right_status);
		}
		return (left_status);
	}
	return (-1); // Return an error status if the operator is not recognized
}
