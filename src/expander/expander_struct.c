#include "minishell.h"

void	init_expander(t_expander *exp, char *cmd)
{
	exp->arg = cmd;
	exp->arg_part = NULL;
	exp->var_to_sub = NULL;
	exp->subbed_var = NULL;
	exp->new_arg = NULL;
	exp->nb_split_parts = 0;
	exp->arg_part_len = 0; 
	exp->var_to_sub_len = 0;
	exp->arg_len = ft_strlen(cmd);
	exp->subbed_var_len = 0;
	exp->nb_args = 1;
}

void	reset_exp(t_expander *exp)
{
	if (exp->arg_part)
	{
		free(exp->arg_part);
		exp->arg_part = NULL;
		exp->arg_part_len = 0;
	}
	if (exp->subbed_var)
	{
		free(exp->subbed_var);
		exp->subbed_var = NULL;
		exp->subbed_var_len = 0;
	}
	if (exp->var_to_sub)
	{
		free(exp->var_to_sub);
		exp->var_to_sub = NULL;
		exp->var_to_sub_len = 0;
	}
	exp->nb_split_parts = 0;
}
