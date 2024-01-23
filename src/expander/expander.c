#include "minishell.h"

/*
// exp struct is used to store all data and send them through the program
// treat_* --> Different cases that need to be handled '," or nothing
// word_split --> create the tab that will be returned to expander()
// reset_exp --> reset some var of exp struct for next iteration
*/
char	**expand_arg(char *arg, t_shell *sh, int *nb_cmds)
{
	int	i;
	char	**cmd_tab;
	t_expander	exp;

	cmd_tab = NULL;
	i= 0;

	init_expander(&exp, arg);
	while (arg[i])
	{
		printf("...\n");
		if (arg[i] == '\'')
			exp.new_arg = treat_s_quotes(&exp, sh, &i);
		else if (arg[i] == '\"')
			exp.new_arg = treat_d_quotes(&exp, sh, &i);
		else
			exp.new_arg = treat_no_quotes(&exp, sh, &i);
		if (exp.nb_split_parts)
		{
			exp.nb_args += exp.nb_split_parts;
			cmd_tab = word_split_expander(&exp, cmd_tab, sh);
			free(exp.new_arg);
			exp.new_arg = NULL;
			reset_exp(&exp);
		}
		printf("exp.new_arg = %s\n", exp.new_arg);
	}
	if (!exp.nb_split_parts && exp.new_arg)
		cmd_tab = word_split_expander(&exp, cmd_tab, sh);
	*nb_cmds += exp.nb_args;
	if (!exp.new_arg)
		*nb_cmds -= 1;
	exp.nb_args = 0;
	if (exp.new_arg)
	{
		free(exp.new_arg);
		exp.new_arg = NULL;
	}
	return (cmd_tab);
}

void	print_tab(char **str)
{
	int i = 0;
	while (str[i])
	{
		printf("str[%d] = %s\n", i, str[i]);
		i++;
	}
}

/*
// If node->type is redirection or command, we proceed to expand if necessary
// expand_arg --> treat each command and expand them, return the tab of expanded arg
// cmd_tab --> New node->data.command.cmd_exe values
 */
 t_ast	*expander(t_ast *node, t_shell *sh)
{
	int		i;
	int		nb_cmds;
	int		mem_nb_cmds;
	char	**exp_word_split;
	char	**cmd_tab;

	nb_cmds = 0;
	/*if (node->type == AST_REDIRECTION)
		expand(node->data.redirection.file, sh, 0);*/
	if (node->type == AST_COMMAND)
	{
		cmd_tab = NULL;
		i = 0;
		while (node->data.command.cmd_exe[i])
		{
			mem_nb_cmds = nb_cmds;
			exp_word_split = expand_arg(node->data.command.cmd_exe[i], sh, &nb_cmds);
			cmd_tab = add_exp_to_cmd_tab(cmd_tab, mem_nb_cmds, exp_word_split, nb_cmds - mem_nb_cmds, sh);
			i++;
		}
		node->data.command.cmd_exe = cmd_tab;
		//print_tab(cmd_tab);
	} 
	return (node);
}
