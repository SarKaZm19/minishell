#include "minishell.h"

static char	*update_arg(t_shell *sh, char *var1, char *var2)
{
	char	*updated;

	updated = NULL;
	if (!var1)
		updated = ft_strdup(var2);
	else
		updated = ft_strjoin(var1, var2);
	s_alloc(updated, EXPANDER, sh);
	return (updated);
}

// Expand the part in d quotes
// check if $ --> expand env var if not create string and add the content until a $ or the end of the string is met
// then verify if something precedes this part of the arg and cat d_quote_return at the end of it.
static char	*d_quotes_expand(t_expander *exp, t_shell *sh)
{
	char	*d_quote_return;
	char	*expand_part;
	int		i;
	int		j;

	i = 0;
	expand_part = NULL;
	d_quote_return = NULL;
	while (exp->arg_part[i])
	{
		j = 0;
		if (exp->arg_part[i] == '$')
			expand_part = d_quote_dollar_expand(exp, sh, &i);
		else
		{
			while (exp->arg_part[i + j] && exp->arg_part[i + j] != '$')
				j++;
			expand_part = ft_substr(exp->arg_part, i, j);
			printf("expand_part = %s\n", expand_part);
			s_alloc(expand_part, EXPANDER, sh);
			printf("....\n");
		}
		//printf("expand_part = %s\n", expand_part);
		d_quote_return = update_arg(sh, d_quote_return, expand_part);
		//printf("d_quote return = %s\n", d_quote_return);
		i += j;
	}
	return (update_arg(sh, exp->new_arg, d_quote_return));
}

/*
// Isolate the double quotes part inside the command
// treat it and return it as a string
// if the " contains a solo $ or $ is followed by ?, we execute special functions
// if "normal" dollar case, expand the var and add it to the " treated string
*/
char	*treat_d_quotes(t_expander *exp, t_shell *sh, int *start_i) // !!! "$test" --> Command '' not found donc $test sans var est une string vide
{
	int		i;
	char	*ret;

	*start_i += 1;
	i = 0;
	while (exp->arg[*start_i + i] && exp->arg[*start_i + i] != '\"')
		i++;
	//printf("i == %d\n", i);
	//printf("exp->new_arg = .%s.\n", exp->new_arg);
	if (i == 0 && !exp->new_arg)
	{
		ret = ft_strdup("");
		*start_i += 1;
		return (ret);
	}
	exp->arg_part = ft_substr(exp->arg, *start_i, i);
	s_alloc(exp->arg_part, ONLY_CHECK, sh);
	//printf("d_quote arg_part = %s\n", exp->arg_part);
	exp->arg_part_len = i;
	ret = d_quotes_expand(exp, sh);
	*start_i += i + 1;
	free(exp->arg_part);
	exp->arg_part = NULL;
	return (ret);
}
