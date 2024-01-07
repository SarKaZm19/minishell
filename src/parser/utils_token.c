#include "minishell.h"

t_token_type	tk_type(t_list *token)
{
	return (((t_token *)token->content)->type);
}

char	*tk_value(t_list *token)
{
	if (token == NULL)
		return (NULL);
	return (((t_token *)token->content)->value);
}

bool	is_tk_type(t_list **token_list, int num_args, ...)
{
	va_list			args;
	t_token_type	type;
	t_token_type	current_type;

	if (token_list == NULL || *token_list == NULL)
		return (false);
	current_type = tk_type(*token_list);
	va_start(args, num_args);
	while (num_args--)
	{
		type = va_arg(args, t_token_type);
		if (current_type == type)
		{
			va_end(args);
			return (true);
		}
	}
	va_end(args);
	return (false);
}

char *tk_type_to_string(t_token_type type)
{
	if (type == TK_PIPE)
		return ("|");
	else if (type == TK_AND)
		return ("&&");
	else if (type == TK_OR)
		return ("||");
	else if (type == TK_GROUP_OPEN)
		return ("(");
	else if (type == TK_GROUP_CLOSE)
		return (")");
	else if (type == TK_WORD)
		return ("word");
	else
		return ("[unknown token]");
}
