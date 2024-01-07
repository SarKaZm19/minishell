/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvastena <fvastena@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/31 01:44:10 by fvastena          #+#    #+#             */
/*   Updated: 2023/12/31 01:44:10 by fvastena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// call in execute() for command and redirection
// expand(node, sh)

// return the expended node or NULL if there is no expension

// Need to remove all quotes
// If d_quotes --> interpret as one string
// if s_quotes --> values not modified ($var stays $var)
// if $ --> var substitution


int		calculate_lens(char *cmd, t_expander *exp)
{
	int	new_cmd_len;

	exp->cmd_len = ft_strlen(cmd);
	printf("calculate...\n");
	printf("cmd_len = %d\n", exp->cmd_len);
	printf("var_len = %d\n", exp->var_len);
	printf("var_sub_len = %d\n", exp->var_sub_len);
	new_cmd_len = exp->cmd_len - exp->var_len - 1 + exp->var_sub_len;
	if (exp->quote == '\"')
		new_cmd_len -= 2;
	return (new_cmd_len);
	
}

char	*get_new_cmd(char *cmd, t_expander *exp, int *i)
{
	int		j;
	char	*new_cmd;

	(void)cmd;
	new_cmd = malloc(sizeof(char) * (exp->var_sub_len + 1));
	if (!new_cmd)
		return (NULL);
	j = 0;
	printf("i = %d\n", *i);
	while (j < exp->var_sub_len)
	{
		new_cmd[j] = exp->var_sub[j];
		j++;
	}
	new_cmd[j] = '\0';
	*i += exp->var_len + 1;
	//free(cmd); //free_cmd
	return (new_cmd);
}

int		get_nb_words(t_expander *exp)
{
	int	i;

	i = 0;
	exp->nb_var_words = 0;
	if (!exp->var_sub)
		return (0);
	if (exp->quote == '\"')
		return (1);
	while (exp->var_sub[i])
	{
		if (is_space(exp->var_sub[i]))
			exp->nb_var_words++;
		while (is_space(exp->var_sub[i]))
			i++;
		i++;
	}
	exp->var_sub_len = i;
	printf("var_sub_len (get_nb_words)= %d\n", exp->var_sub_len);
	printf("nb_var_words (get_nb_words)= %d\n", exp->nb_var_words);
	return (exp->nb_var_words);
}

char	*get_var_sub(t_expander *exp)
{
	char	*var_sub;

	var_sub = getenv(exp->var_in_cmd);
	if (!var_sub)
	{
		
	}

	return (var_sub);
}

char	*extract_var_update_cmd(char *cmd, int *i, t_expander *exp, int *current_size)
{
	int		j;
	char	*new_cmd;

	j = *i;
	while (cmd[j])
	{
		if (!is_space(cmd[j]) && cmd[j] != '\"')
			break ;
		j++;
	}
	exp->var_len = j - *i - 1;
	printf("exp->var_len = %d\n", exp->var_len);
	exp->var_in_cmd = ft_substr(cmd, *i, exp->var_len);
	printf("var_in_cmd = %s, len = %d\n", exp->var_in_cmd, exp->var_len);
	exp->var_sub = getenv(exp->var_in_cmd);
	exp->var_sub_len = ft_strlen(exp->var_sub);
	printf("var sub = %s, var_sub_len = %d\n", exp->var_sub, exp->var_sub_len);
	exp->nb_var_words = get_nb_words(exp);
	printf("nb_var_words = %d\n", exp->nb_var_words);
	new_cmd = get_new_cmd(cmd, exp, i);
	*current_size += exp->nb_var_words;
	// gestion si !var_sub
	// new_cmd = nouvelle cmd avec la subsitution de la var
	//var_sub = getenv(cmd[i])
	if (exp->var_in_cmd)
	{
		exp->var_in_cmd = NULL;
		free(exp->var_in_cmd);
	}
	printf("new_cmd = %s\n", new_cmd);
	return (new_cmd);
}

t_expander	*init_exp_struct(t_expander *exp, char *cmd)
{
	exp->cmd = NULL;
	exp->cmd_len = ft_strlen(cmd);
	exp->nb_var_words = 0;
	exp->quote = 0;
	exp->var_in_cmd = NULL;
	exp->var_len = 0;
	exp->var_sub = NULL;
	exp->var_sub_len = 0;
	return (exp);
}

char	set_in_quote(char q, char c)
{
	if (q == 0)
		q = c;
	else if (c == q)
		q = 0;
	return (q);
}

char	*expand_cmd(char *exp_cmd, char *new_cmd)
{
	char	*tmp;

	if (exp_cmd)
	{
		if (!new_cmd)
			new_cmd = ft_strdup(exp_cmd);
		else
		{
			tmp = ft_strdup(new_cmd);
			free(new_cmd);
			new_cmd = ft_strjoin(tmp, exp_cmd);
			free(tmp);
		}
		free(exp_cmd);
	}
	return (new_cmd);
}

char	**exp_word_split(char **cmd_tab, char *cmd, int nb_cmds)
{
	char	**word_split;
	int		i;
	int		j;
	int		k;

	word_split = malloc(sizeof(char *) * (nb_cmds + 1));
	if (!word_split)
		return (NULL);
	i = 0;
	j = 0;
	k = 0;
	printf("start wordsplit\n");
	if (!cmd_tab)
	{
		while (cmd[i])
		{
			j = i;
			while (cmd[i] && !is_space(cmd[i]))
				i++;
			word_split[k] = ft_substr(cmd, j, i - j);
			printf("1word_split[%d] = .%s.\n", i, word_split[k]);
			if (cmd[i])
				i++;
			k++;
		}
	}
	else
	{
		while (cmd_tab[i])
		{
			word_split[k] = ft_strdup(cmd_tab[i]);
			printf("2word_split[%d] = .%s.\n", i, word_split[i]);
			k++;
			i++;
		}
		k--;
		i = 0;
		while (cmd[j])
		{
			j = i;
			while (cmd[j] && !is_space(cmd[j]))
				i++;
			word_split[k] = ft_substr(cmd, j, i - j);
			printf("2word_split[%d] = .%s.\n", i, word_split[k]);
			if (cmd[i])
				i++;
			k++;
		}
	}
	//free_tab(cmd_tab);
	j = 0;
	k = 0;
	
	word_split[i] = NULL;
	printf("end wordsplit\n");
	return (word_split);
}

// ls
// ls $test
// ls "$test"
// ls '$test'
// ls dir "$test" $test "$test$test" $test$test

char	**expand(char *cmd, t_shell *sh, int *current_size)
{
	// Faire sauter toutes les quotes et faire toutes les substitutions et tout recoller ensemble~
	// wildcard * --> Pas d'expand entre s_quote ou d_quote
	// $ --> Expand si d_quote, pas d'expand entre s_quote

	int		i;
	t_expander	exp;

	char	**cmd_tab;
	char	*tmp_cmd;
	int		j;
	(void)sh;
	// (void)current_size;
	// return(cmd);
	i = 0;
	init_exp_struct(&exp, cmd);
	cmd_tab = NULL;
	tmp_cmd = NULL;
	j = 0;
	int test;

	test = 0;
	while (cmd[i])
	{
		test++;
		if (test >= 10)
		{
			printf("TESTBREAK ON LOOP : cmd[%d] = .%c.", i, cmd[i]);
			break;
		}
		printf("index to check = cmd[%d]= .%c.\n", i, cmd[i]);
		if (cmd[i] == '\"' || cmd[i] == '\'')
		{
			exp.quote = set_in_quote(exp.quote, cmd[i]);
			printf("exp->quote = %d, %c\n", exp.quote, exp.quote);
			if (exp.quote != 0)
			{
				printf("-- \',\" case --\n");
				i++;
				j = i;
				while (cmd[j] != exp.quote)
					j++;
				printf("j size_quotes = %d\n", j);
				exp.cmd = ft_substr(cmd, i, j - i);
				printf("quote exp.cmd = %s\n", exp.cmd);
				tmp_cmd = expand_cmd(exp.cmd, tmp_cmd);
				printf("tmp_cmd \', \" = .%s.\n", tmp_cmd);
				i++;
				printf("test before next loop cmd[%d] = %c, %d\n", i, cmd[i], cmd[i]);
			}
		}
		else if (cmd[i] == '$')
		{
			printf("cmd start if $ = .%s.\n", cmd);
			printf("current_size before var_update = %d\n", *current_size);
			if (exp.quote == '\"')
				i++;
			exp.cmd = extract_var_update_cmd(cmd, &i, &exp, current_size);
			printf("current_size after var_update = %d\n", *current_size);
			printf("exp.cmd $ = .%s.\n", exp.cmd);
			tmp_cmd = expand_cmd(exp.cmd, tmp_cmd);
			printf("tmp_cmd $ = .%s.\n", tmp_cmd);
			printf("current_size = %d\n", *current_size);
			if (exp.quote == '\"')
			{
				printf("case %c", exp.quote);
				cmd_tab = exp_word_split(cmd_tab, tmp_cmd, exp.nb_var_words);
			}
			printf("end $\n");
		}
		else if (cmd[i] == '*')
		{
			printf("need extand *\n");
			while (cmd[i] || (!is_space(cmd[i]) && cmd[i] != '$' && cmd[i] != '\"' && cmd[i] != '\''))
				i++;
		}
		else
		{
			printf("else part\n");
			j = i;
			printf("i = %d, j = %d\n", i, j);
			while (cmd[i])
			{
				if (cmd[i] == '\"' || cmd[i] == '\'' || cmd[i] == '$' || cmd[i] == '*')
					break ;
				i++;
			}
			printf("len = %d\n", i - j);
			exp.cmd = ft_substr(cmd, j, i - j);
			printf("sub_str = .%s., len = %d\n", exp.cmd, i - j);
			printf("tmp_cmd \' = .%s.\n", tmp_cmd);
			if (cmd[i] == '\"')
				i++;
			if (exp.quote)
			{
				free(tmp_cmd);
				tmp_cmd = NULL;
			}
			tmp_cmd = expand_cmd(exp.cmd, tmp_cmd);
			printf("tmp_cmd \' = .%s.\n", tmp_cmd);
		}
		printf("i end of loop = %d\n", i);
	}
	if (!cmd_tab)
	{
		if (tmp_cmd)
		{
			cmd_tab = ft_split(tmp_cmd, ' ');
			if (exp.quote == 0)
				cmd_tab = ft_split(tmp_cmd, ' ');
			else
			{
				cmd_tab = malloc(sizeof(char *) * 2);
				cmd_tab[0] = ft_strdup(tmp_cmd);
				cmd_tab[1] = NULL;
			}
			//free(tmp_cmd);
		}
		else
		{
			cmd_tab = malloc(sizeof(char *) * 2);
			cmd_tab[0] = ft_strdup("");
			cmd_tab[1] = NULL;
		}
	}
	i = 0;
	while (cmd_tab[i])
	{
		printf("cmd_tab[%d] = .%s.\n", i, cmd_tab[i]);
		i++;
	}
	return (cmd_tab);
}

char	**add_cmds(char **cmds, int position, char **exp_cmds, int current_size)
{
	char	**new_cmds;
	int		i;
	int		k;

	if (!exp_cmds)
		return (cmds);
	new_cmds = malloc(sizeof(char *) * (current_size + 1));
	if (!new_cmds)
		return (NULL);
	i = -1;
	printf("position = %d, current_size = %d\n", position, current_size);
	while (++i < position)
	{
		new_cmds[i] = ft_strdup(cmds[i]);
		printf("1new_cmds[%d] = %s\n", i, new_cmds[i]);
	}
	k = 0;
	while (exp_cmds[k])
	{
		printf("2exp_cmds[%d] = %s\n", k, exp_cmds[k]);
		new_cmds[i + k] = ft_strdup(exp_cmds[k]);
		printf("2new_cmds[%d] = %s\n", i + k, new_cmds[i + k]);
		k++;
	}
	printf("...\n");
	printf("i + k = %d + %d = %d, position = %d current_size = %d\n", i, k, i + k, position, current_size);
	new_cmds[i + k] = 0;
	printf("end 1,2,3\n");
	if (cmds)
	{
		while (cmds[i])
		{
			free(cmds[i]);
			i++;
		}
		free(cmds);
		cmds = NULL;
	}
	printf("??\n");
	return (new_cmds);
}

t_AST	*expander(t_AST *node, t_shell *sh)
{
	(void)sh;
	int		i;
	int		j;
	//int		mem_size;
	int		current_size;
	char	**exp_word_split;
	char	**cmd_tab;

	if (node->type == AST_REDIRECTION)
		expand(node->data.redirection.file, sh, 0);
	else if (node->type == AST_COMMAND)
	{
		cmd_tab = NULL;
		i = 0;
		while (node->data.command.cmd_exe[i])
			i++;
		current_size = i;
		i = 0;
		while (node->data.command.cmd_exe[i])
		{
			printf("\n-------------------------------------------------------------------\n");
			printf("i = %d\n", i);
			j = current_size;
			exp_word_split = expand(node->data.command.cmd_exe[i], sh, &current_size);
			printf("exp_word_split = expand done\n");
			if (exp_word_split)
			{
				int k = -1;
				while (exp_word_split[++k])
					printf("exp_word_split[%d] = .%s.\n", k, exp_word_split[k]);
			}	
			printf("---------------------go add cmd-----------------------------------\n");
			printf("i node = %d, current_size = %d\n", i, current_size);
			cmd_tab = add_cmds(cmd_tab, i, exp_word_split, current_size);
			printf("cmd added\n");
			//node->data.command.cmd_exe[i] = add_cmds(&node->data.command.cmd_exe, i, exp_word_split, current_size);
			printf("node->data.cmd_exe[%d] = .%s.\n", i, node->data.command.cmd_exe[i]);
			i++;
			printf("i after incr = %d\n", i);
			j = 0;
			if (exp_word_split)
			{
				while (exp_word_split[j])
					free(exp_word_split[j++]);
				free(exp_word_split);
			}
			printf("------------------\n");
		}
		if (cmd_tab)
		{
			i = 0;
			while (cmd_tab[i])
			{
				printf("cmd_tab[%d] = %s\n", i, cmd_tab[i]);
				i++;
			}
			printf("\\\\\\\n");
			node->data.command.cmd_exe = cmd_tab;
		}
	}
	
	return (node);
}

t_AST	*expander(t_AST *node, t_shell *sh)
{
	(void)sh;
	int		i;
	int		j;
	//int		mem_size;
	int		current_size;
	char	**exp_word_split;
	char	**cmd_tab;

	if (node->type == AST_REDIRECTION)
		expand(node->data.redirection.file, sh, 0);
	else if (node->type == AST_COMMAND)
	{
		cmd_tab = NULL;
		i = 0;
		while (node->data.command.cmd_exe[i])
			i++;
		current_size = i;
		i = 0;
		while (node->data.command.cmd_exe[i])
		{
			printf("\n-------------------------------------------------------------------\n");
			printf("node->cmd_exe[%d] = %s\n", i, node->data.command.cmd_exe[i]);
			j = current_size;
			exp_word_split = expand(node->data.command.cmd_exe[i], sh, &current_size);
			printf("exp_word_split = expand done\n");
			if (exp_word_split)
			{
				int k = -1;
				while (exp_word_split[++k])
					printf("exp_word_split[%d] = .%s.\n", k, exp_word_split[k]);
			}	
			printf("---------------------go add cmd-----------------------------------\n");
			printf("i node = %d, current_size = %d\n", i, current_size);
			cmd_tab = add_cmds(cmd_tab, i, exp_word_split, current_size);
			printf("cmd added\n");
			//node->data.command.cmd_exe[i] = add_cmds(&node->data.command.cmd_exe, i, exp_word_split, current_size);
			printf("node->data.cmd_exe[%d] = .%s.\n", i, node->data.command.cmd_exe[i]);
			i++;
			printf("i after incr = %d\n", i);
			j = 0;
			if (exp_word_split)
			{
				while (exp_word_split[j])
					free(exp_word_split[j++]);
				free(exp_word_split);
			}
			printf("------------------\n");
		}
		if (cmd_tab)
		{
			i = 0;
			while (cmd_tab[i])
			{
				printf("cmd_tab[%d] = %s\n", i, cmd_tab[i]);
				i++;
			}
			printf("\\\\\\\n");
			node->data.command.cmd_exe = cmd_tab;
		}
	}
	
	return (node);
}