#include "minishell.h"

void	init_expander(t_expander *exp, char *cmd)
{
	exp->cmd = cmd;
	exp->cmd_part = NULL;
	exp->var_to_sub = NULL;
	exp->subbed_var = NULL;
	exp->new_cmd = NULL;
	exp->split_parts = 0;
	exp->op_index = 0;
	exp->cmd_part_len = 0; 
	exp->var_to_sub_len = 0;
	exp->cmd_len = ft_strlen(cmd);
	exp->subbed_var_len = 0;
	exp->nb_cmds = 1;
}

void	reset_exp(t_expander *exp)
{
	if (exp->cmd_part)
	{
		free(exp->cmd_part);
		exp->cmd_part = NULL;
		exp->cmd_part_len = 0;
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
	exp->split_parts = 0;
}