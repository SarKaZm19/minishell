#include "minishell.h"

char	*trim_white_spaces(t_expander *exp, char *sub_env)
{
	char	*res;
	int		i;
	int		size;
	int		j;

	i = 0;
	size = 0;
	printf("subbed_var_len = %d\n", exp->subbed_var_len);
	while (is_space(sub_env[i]))
		i++;
	while (i < exp->subbed_var_len)
	{
		if (!is_space(sub_env[i]))
			size++;
		i++;
	}
	printf("size = %d\n", size);
	res = malloc(sizeof(char) * (size + exp->split_parts + 1));
	if (!res)
		return (NULL);
	i = 0;
	j = 0;
	while (i < exp->subbed_var_len && is_space(sub_env[i]))
	{
		
	}
	while (i < exp->subbed_var_len)
	{
		if (!is_space(sub_env[i]))
		{
			res[j] = sub_env[i];
			j++;
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
		i++;
	}
	res[j] = '\0';
	printf("res = %s\n", res);
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
	printf("nb_w = %d\n", nb_w);
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
	printf("exp->var_to_sub d_quotes = .%s., len = %d\n", exp->var_to_sub, exp->var_to_sub_len);
	printf("new_tmp_i = %d\n", *tmp_i);
	sub_env = getenv(exp->var_to_sub);
	if (!sub_env)
	{
		exp->subbed_var = NULL;
		exp->subbed_var_len = 0;
		exp->split_parts = 0;
		return (NULL);
	}
	exp->subbed_var_len = ft_strlen(sub_env);
	printf("exp->subbed_var d_quotes = .%s., len = %d\n", sub_env, exp->subbed_var_len);
	exp->split_parts = count_parts(sub_env);
	exp->subbed_var = trim_white_spaces(exp, sub_env);
	printf("exp->split_parts = %d\n", exp->split_parts);
	return (exp->subbed_var);
}


// Expand function takes a cmd_tab index and check the expansion with $, ",' and *
char	**expand(char *cmd, t_shell *sh, int *nb_cmds)
{
	(void)sh;
	int	i;
	char	**cmd_tab;
	t_expander	exp;

	cmd_tab = NULL;
	i= 0;

	init_expander(&exp, cmd);
	while (cmd[i])
	{
		printf("to_test = cmd[%d] = %c\n", i, cmd[i]);
		if (cmd[i] == '\'')
		{
			printf("s_quote...\n");
			exp.new_cmd = expand_s_quote(&exp, sh, &i);
			printf("exp.new_cmd s_quote = %s\n", exp.new_cmd);
		}
		else if (cmd[i] == '\"')
		{
			printf("d_quote...\n");
			exp.new_cmd = expand_d_quote(&exp, &i);
			printf("exp.new_cmd d_quote = %s\n", exp.new_cmd);
		}
		else
		{
			printf("no_quote...\n");
			exp.new_cmd = expand_no_quotes(&exp, &i);
			printf("exp.new_cmd no_quote = %s\n", exp.new_cmd);
		}
		printf("new_cmd expand() = %s\n", exp.new_cmd);
		if (exp.split_parts)
			cmd_tab = word_split_expander(&exp, cmd_tab, sh);
		printf("i = %d\n", i);
		reset_exp(&exp);
	}
	*nb_cmds += exp.split_parts;
	cmd_tab = word_split_expander(&exp, cmd_tab, sh);
	if (exp.new_cmd)
	{
		free(exp.new_cmd);
		exp.new_cmd = NULL;
	}
	return (cmd_tab);
}


// Expander loop through the cmds table and try expand with expand() function
// expand() returns a tab of cmd that have to be added to the main cmd_tab

 t_AST	*expander(t_AST *node, t_shell *sh)
{
	(void)sh;
	int		i;
	int		j;
	//int		mem_size;
	int		nb_cmds;
	char	**exp_word_split;
	char	**cmd_tab;

	nb_cmds = 0;
	if (node->type == AST_REDIRECTION)
		expand(node->data.redirection.file, sh, 0);
	else if (node->type == AST_COMMAND)
	{
		cmd_tab = NULL;
		i = 0;
		while (node->data.command.cmd_exe[i])
			i++;
		i = 0;
		while (node->data.command.cmd_exe[i])
		{
			exp_word_split = expand(node->data.command.cmd_exe[i], sh, &nb_cmds);
			printf("------end exp------\n");
			printf("cmds size = %d\n", nb_cmds);
			cmd_tab = add_cmds(cmd_tab, i, exp_word_split, nb_cmds, sh);
			printf("??\n");
			//cmd_tab = add_cmds(cmd_tab, i, exp_word_split, nb_cmds);
			// ajout tableau recu dans expand dans le nouveau tab general.
			// --> Expand renverra le tableau pour la cmd en cours --> $var$var
			if (cmd_tab)
			{
				printf("printing cmd_tab_i...\n");
				j = 0;
				while (cmd_tab[j])
				{
					printf("cmd_tab[%d] = .%s.\n", j, cmd_tab[j]);
					j++;
				}
				printf("\\\\\\\n");
			}
			i++;
		}
		node->data.command.cmd_exe = cmd_tab;
	} 
	return (node);
}