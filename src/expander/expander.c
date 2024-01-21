#include "minishell.h"

char	*trim_white_spaces(t_expander *exp, char *sub_env)
{
	char	*res;
	int		i;
	int		size;
	int		j;

	i = 0;
	size = 0;
	while (is_space(sub_env[i]))
		i++;
	while (i < exp->subbed_var_len)
	{
		if (!is_space(sub_env[i]))
			size++;
		i++;
	}
	res = malloc(sizeof(char) * (size + exp->split_parts + 1));
	if (!res)
		return (NULL);
	i = 0;
	j = 0;
	while (sub_env[i] && is_space(sub_env[i]))
		i++;
	while (i < exp->subbed_var_len)
	{
		if (!is_space(sub_env[i]))
		{
			res[j] = sub_env[i];
			j++;
			i++;
		}
		else
		{
			while (i < exp->subbed_var_len && is_space(sub_env[i]))
				i++;
			if (i < exp->subbed_var_len)
			{
				res[j] = ' ';
				j++;
			}
		}
	}
	res[j] = '\0';
	return (res);
}

int		count_parts(char *subbed)
{
	int	i;
	int	nb_w;

	i = 0;
	nb_w = 0;
	while (is_space(subbed[i]))
		i++;
	while (subbed[i])
	{
		if (!is_space(subbed[i]))
		{
			nb_w++;
			while (subbed[i] && !is_space(subbed[i]))
				i++;
		}
		else
		{
			while (subbed[i] && is_space(subbed[i]))
			i++;
		}
		
	}
	return (nb_w - 1);
}

char	*expand_var(t_expander *exp, int *tmp_i)
{
	int		j;
	char	*sub_env;

	*tmp_i += 1;
	j = 0;
	while (exp->cmd_part[*tmp_i + j] && ft_isalnum(exp->cmd_part[*tmp_i + j]))
		j++;
	exp->var_to_sub = ft_substr(exp->cmd_part, *tmp_i, j);
	exp->var_to_sub_len = j;
	*tmp_i += exp->var_to_sub_len;
	sub_env = getenv(exp->var_to_sub);
	if (!sub_env)
	{
		exp->subbed_var = NULL;
		exp->subbed_var_len = 0;
		exp->split_parts = 0;
		return (NULL);
	}
	exp->subbed_var_len = ft_strlen(sub_env);
	exp->split_parts = count_parts(sub_env);
	exp->subbed_var = trim_white_spaces(exp, sub_env);
	return (exp->subbed_var);
}

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
		if (arg[i] == '\'')
			exp.new_cmd = treat_s_quotes(&exp, sh, &i);
		else if (arg[i] == '\"')
			exp.new_cmd = treat_d_quotes(&exp, sh, &i);
		else
			exp.new_cmd = treat_no_quotes(&exp, sh, &i);
		if (exp.split_parts)
		{
			exp.nb_cmds += exp.split_parts;
			cmd_tab = word_split_expander(&exp, cmd_tab, sh);
			free(exp.new_cmd);
			exp.new_cmd = NULL;
			reset_exp(&exp);
		}
	}
	if (!exp.split_parts && exp.new_cmd)
		cmd_tab = word_split_expander(&exp, cmd_tab, sh);
	*nb_cmds += exp.nb_cmds;
	if (!exp.new_cmd)
		*nb_cmds -= 1;
	exp.nb_cmds = 0;
	if (exp.new_cmd)
	{
		free(exp.new_cmd);
		exp.new_cmd = NULL;
	}
	return (cmd_tab);
}

/*
// If node->type is redirection or command, we proceed to expand if necessary
// expand_arg --> treat each command and expand them, return the tab of expanded arg
// cmd_tab --> New node->data.command.cmd_exe values
 */
 t_AST	*expander(t_AST *node, t_shell *sh)
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
		i = 0;;
		while (node->data.command.cmd_exe[i])
		{
			mem_nb_cmds = nb_cmds;
			exp_word_split = expand_arg(node->data.command.cmd_exe[i], sh, &nb_cmds);
			cmd_tab = add_cmds(cmd_tab, mem_nb_cmds, exp_word_split, nb_cmds, sh);
			i++;
		}
		node->data.command.cmd_exe = cmd_tab;
	} 
	return (node);
}
