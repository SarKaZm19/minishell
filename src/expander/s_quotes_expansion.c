#include "minishell.h"

// Verify if it's a join or a dup depending on the existence of new_arg
// If new_arg exists, something has been read, so we need to add the s_quote expand to what's been read.
static char	*s_quote_return(t_expander *exp, t_shell *sh)
{
	char	*s_quote_expand;

	s_quote_expand = NULL;
	if (!exp->new_arg) // !arg_part is verified in s_alloc
	{
		s_quote_expand = ft_strdup(exp->arg_part);
		s_alloc(s_quote_expand, EXPANDER, sh);
	}
	else
	{
		s_quote_expand = ft_strjoin(exp->new_arg, exp->arg_part);
		s_alloc(s_quote_expand, EXPANDER, sh);
		// free(exp->new_arg);
		// exp->new_arg = NULL;
	}
	// free(exp->arg_part);
	// exp->arg_part = NULL;
	return (s_quote_expand);
}

/*
// Isolate the ' ' part of the arg, and treat it
// return it as a string, we join or dup depending on the value of new_arg
*/
char	*treat_s_quotes(t_expander *exp, t_shell *sh, int *start_i)
{
	int		j; // end_sequence

	*start_i += 1; // start_sequence
	j = 0;
	while (exp->arg[*start_i + j] && exp->arg[*start_i + j] != '\'')
		j++;
	exp->arg_part = ft_substr(exp->arg, *start_i, j);
	s_alloc(exp->arg_part, ONLY_CHECK, sh);
	*start_i += j + 1;
	return (s_quote_return(exp, sh));
}
