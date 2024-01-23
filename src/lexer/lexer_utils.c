
#include "minishell.h"

int	is_space(int c)
{
	if (c == ' ' || c == '\t' || c == '\v' || c == '\n'
		|| c == '\r' || c == '\f')
		return (1);
	return (0);
}

int	is_token(int c)
{
	if (c == '(' || c == ')' || c == '<' || c == '>' || c == '&'
		|| c == '|')
		return (1);
	return (0);
}
