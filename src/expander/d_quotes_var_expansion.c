#include "minishell.h"

/*
// Get the index of the last char of the var 
*/
static int	get_end_of_var(t_expander *exp, int *part_i)
{
	int	end_var;

	end_var = 0;
	while (exp->arg_part[*part_i + end_var] && !is_space(exp->arg_part[*part_i + end_var])
		&& ft_isalnum(exp->arg_part[*part_i + end_var]))
		end_var++;
	return (end_var);
}

/*
// Isolate the $var and get value in env
// Returns the substituted var
*/
static char	*expand_var_in_d_quotes(t_expander *exp, t_shell *sh, int *part_i)
{
	int		j;
	char	*sub_env;

	sub_env = NULL;
	*part_i += 1;
	j = get_end_of_var(exp, part_i);
	exp->var_to_sub = ft_substr(exp->arg_part, *part_i, j);
	s_alloc(exp->var_to_sub, ONLY_CHECK, sh);
	exp->var_to_sub_len = j;
	*part_i += exp->var_to_sub_len;
	// Modif getenv and get the real var in the struct
	sub_env = getenv(exp->var_to_sub);
	if (sub_env)
		exp->subbed_var = ft_strdup(sub_env);
	exp->subbed_var_len = ft_strlen(exp->subbed_var);
	free(exp->var_to_sub);
	exp->var_to_sub = NULL;
	return (exp->subbed_var);
}

/*
// Expand env variable, handle some special cases like solo $, $? 
*/
char	*d_quote_dollar_expand (t_expander *exp, t_shell *sh, int *i)
{
	char *dq_dollar_expand;

	if (!exp->arg_part[*i + 1] ||
		(exp->arg_part[*i + 1] &&
		(exp->arg_part[*i + 1] == '\'' || is_space(exp->arg_part[*i + 1]))))
	{
		dq_dollar_expand = ft_strdup("$");
		*i += 1;
	}
	else if (exp->arg_part[*i + 1] == '?')
	{
		dq_dollar_expand = ft_itoa(sh->last_prompt_exit_status);
		*i += 2;
	}
	else
		dq_dollar_expand = expand_var_in_d_quotes(exp, sh, i);
	s_alloc(dq_dollar_expand, EXPANDER, sh);
	return (dq_dollar_expand);
}
