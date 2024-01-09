/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvastena <fvastena@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 23:58:31 by fvastena          #+#    #+#             */
/*   Updated: 2024/01/09 19:53:12 by fvastena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_var(char *tmp, int i, int *save_i)
{
	char	*var;
	int		j;

	j = i;
	while (tmp[i + j] && tmp[i + j] != '\"' && tmp[i + j] != '\'' && tmp[i + j] != '$' && tmp[i + j] != '*')
		j++;
	*save_i += j + 1;
	var = ft_substr(tmp, i, j);
	return (var);
}

char	*get_var_sub(char *var)
{
	char	*var_sub;

	var_sub = ft_strdup(getenv(var));
	return (var_sub);
}

char	*expand_var(char *new_cmd, char *tmp, int *tmp_i)
{
	char	*d_quote_expand;
	char	*var;
	char	*tmp_expand;
	int		save_i;

	printf("expand_var tmp = %s && tmp_i = %d\n", tmp, *tmp_i);
	save_i = 0;
	var = get_var(tmp, *tmp_i, &save_i);
	d_quote_expand = NULL;
	if (!var)
	{
		if (new_cmd)
			return (ft_strdup(new_cmd));
		else
			return (NULL);
	}
	printf("expand_var var = .%s.\n", var);
	if (tmp[save_i] == '$')
	{
		printf("$\n");
		tmp_expand = get_var_sub(var);
		printf("var = %s, getenv(var) = %s\n", var, tmp_expand);
		if (tmp_expand)
		{
			if (new_cmd)
				d_quote_expand = ft_strjoin(new_cmd, tmp_expand);
			else
				d_quote_expand = ft_strdup(tmp_expand);
			free(tmp_expand);
		}
		free(new_cmd);
		printf("tmp_expand var = %s\n", tmp_expand);
	}
	else if (tmp[*tmp_i] == '*')
	{
		if (var)
		{
			if (new_cmd)
				d_quote_expand = ft_strjoin(new_cmd, var);
			else
				d_quote_expand = ft_strdup(var);
			printf("1d_quote_expand = %s\n", d_quote_expand);
		}
	}
	else
	{
		if (var)
		{
			if (new_cmd)
				d_quote_expand = ft_strjoin(new_cmd, var);
			else
				d_quote_expand = ft_strdup(var);
			printf("2d_quote_expand = %s\n", d_quote_expand);
		}
	}
	*tmp_i += save_i;
	printf("new_tmp_i = %d\n", *tmp_i);
	printf("expand_var = %s\n", d_quote_expand);
	return (d_quote_expand);
}

char	**get_cmds_tab(char **sub_tab, char *sub)
{
	int		i;
	int		j;
	int		k;
	char	**cmds_tab;
	int		nb_words;
	int		sub_tab_size;
	int		mem;
	char	*tmp;

	i = 0;
	nb_words = 0;
	while (sub_tab[i])
		i++;
	sub_tab_size = i;
	i = 0;
	while (sub[i])
	{
		while (is_space(sub[i]))
			i++;
		if (sub[i])
			nb_words += 1;
		while (sub[i] && !is_space(sub[i]))
			i++;
	}
	cmds_tab = malloc(sizeof(char *) * (nb_words + i + 1));
	if (!cmds_tab)
		return (NULL);
	i = 0;
	while (sub_tab[i])
	{
		cmds_tab[i] = ft_strdup(sub_tab[i]);
		free(sub_tab[i]);
		i++;
	}
	mem = i - 1;
	k = 0;
	j = 0;
	cmds_tab[mem] = ft_strjoin(cmds_tab[mem], tmp);
	while (nb_words--)
	{
		while (is_space(sub[j]))
			j++;
		while (sub[j + k] && !is_space(sub[j + k]))
			k++;
		tmp = ft_substr(sub, j, k);
		cmds_tab[i] = ft_strdup(tmp);
		i++;
		j += k;
	}
	return (cmds_tab);
}

char	*expand_no_quotes(char *cmd, char *new_cmd, int *start_i, int *parts)
{
	char	*sub;
	int		j;
	int		k;

	j = 0;
	k = 0;
	sub = expand_var(new_cmd, cmd, start_i);
	if (sub)
	{
		if (cmd[*start_i + j] == '$')
		{
			if (sub[k] && !is_space(sub[k]))
				*parts += 1;
			while (sub[k] && !is_space(sub[k]))
				k++;
			while (sub[k] && is_space(sub[k]))
				k++;
		}
		else if (cmd[*start_i + j] == '*')
		{
			if (sub[k] && !is_space(sub[k]))
				*parts += 1;
			while (sub[k] && !is_space(sub[k]))
				k++;
			while (sub[k] && is_space(sub[k]))
				k++;
		}
		else
			*parts += 1;
	}

	*start_i += j;
	printf("start_i no quotes incr =  %d\n", *start_i);
	return (sub);
	// si * dans une var, pas de substitution du *, interprete litteralement
	// voir si on peut pas lire tout d un coup et tout mettre dans un tableau apres dans expand
}

char	*expand_d_quote(char *cmd, char *new_cmd, int *start_i)
{
	int		j;
	int		i;
	char	*tmp;
	char	*d_quote_expand;
	
	*start_i += 1;
	j = 0;
	while (cmd[*start_i + j] && cmd[*start_i + j] != '\"')
		j++;
	tmp = ft_substr(cmd, *start_i, j);
	// prot tmp + free
	printf("d_quote_substr = .%s., start_i = %d, j = %d\n", tmp, *start_i, j);
	i = 0;
	while (tmp[i])
	{
		if (tmp[i] == '$')
			d_quote_expand = expand_var(new_cmd, tmp, &i);
		else
			d_quote_expand = expand_var(new_cmd, tmp, &i);
	}
/* 	if (new_cmd)
	{
		printf("new_cmd exists --> join\n");
		free(new_cmd);
		new_cmd = ft_strjoin(new_cmd, tmp2);
	}
	else
	{
		printf("new_cmd doesnt exists --> dup\n");
		new_cmd = ft_strdup(tmp2);
	} */
	printf("new_cmd quote_expand = .%s.\n", new_cmd);
	*start_i += j + 1;
	return (d_quote_expand);
}

char	*expand_s_quote(char *cmd, char *new_cmd, int *start_i)
{
	int		j;
	char	*s_quote_expand;
	char	*tmp;

	*start_i += 1;
	j = 0;
	while (cmd[*start_i + j] && cmd[*start_i + j] != '\'')
		j++;
	tmp = ft_substr(cmd, *start_i, j);
	printf("s_quote_substr = .%s., start_i = %d, j = %d\n", tmp, *start_i, j);
	*start_i += j + 1;
	if (!new_cmd)
		s_quote_expand = ft_strdup(tmp);	
	else
	{
		s_quote_expand = ft_strjoin(new_cmd, tmp);
		free(new_cmd);
		new_cmd = NULL;
	}
	if (tmp)
	{
		free(tmp);
		tmp = NULL;
	}
	printf("s_quote_expand = .%s.\n", s_quote_expand);
	return (s_quote_expand);
}


char	**split_cmds(char *new_cmd, int parts)
{
	char	**tmp_tab;
	int		i;
	int		j;
	int		k;

	tmp_tab = malloc(sizeof(char *) * (parts + 1));
	if (!tmp_tab)
		return (NULL);
	i = 0;
	j = 0;
	k = 0;
	while (k < parts)
	{
		while (new_cmd[i] && is_space(new_cmd[i]))
			i++;
		while (new_cmd[i + j] && !is_space(new_cmd[i + j]))
			j++; 
		tmp_tab[k] = ft_substr(new_cmd, i, j);
		i++;
		k++;
	}
	tmp_tab[k] = NULL;
	return (tmp_tab);
}

char	**cmd_to_tab(char **cmd_tab, char *new_cmd, int parts)
{
	char	**tmp;
	char	**new_tab;
	int		cmd_tab_size;
	int		i;
	int		j;

	if (parts > 1)
		tmp = split_cmds(new_cmd, parts);
	cmd_tab_size = 0;
	while (cmd_tab && cmd_tab[cmd_tab_size])
		cmd_tab_size++;
	printf("parts = %d, cmd_tab_size = %d\n", parts, cmd_tab_size);
	i = 0;
	if (!cmd_tab)
	{
		printf("cmd_tab not exists\n");
		new_tab = malloc(sizeof(char *) * (parts + 1));
		if (new_tab)
			return (NULL);
		while (i < parts + 1)
		{
			new_tab[i] = ft_strdup(new_cmd);
			printf("new_tab[%d] = %s\n", i, new_tab[i]);
			i++;
		}
	}
	else
	{
		printf("cmd_tab_exists\n");
		j = 0;
		new_tab = malloc(sizeof(char *) * (cmd_tab_size + parts));
		if (!new_tab)
			return (NULL);
		while (i < cmd_tab_size)
		{
			new_tab[i] = ft_strdup(cmd_tab[i]);
			printf("new_tab[%d] = %s\n", i, new_tab[i]);
			i++;
		}
		if (i != 0)
			i--;
		free(new_tab[i]);
		new_tab[i] = ft_strjoin(cmd_tab[i], tmp[j]);
		while (++i < parts)
		{
			new_tab[i] = ft_strdup(tmp[++j]);
			printf("new_tab[%d] = %s\n", i, new_tab[i]);
		}
	}
	if (new_cmd)
		free(new_cmd);
	new_tab[i] = NULL;
	return (new_tab);
}

// Expand function takes a cmd_tab index and check the expansion with $, ",' and *
char	**expand(char *cmd, t_shell *sh, int *cmds_size)
{
	(void)sh;
	(void)cmds_size;
	int	i;
	char	*new_cmd;
	char	**cmd_tab;
	int		parts;
	int		test;

	new_cmd = NULL;
	cmd_tab = NULL;
	test = -1;
	i= 0;
	while (cmd[i])
	{
		test++;
		if (test == 10)
		{
			printf("limit reached\n");
			exit(0);
		}
		parts = 0;
		printf("cmd[%d] check to expand = %c\n", i, cmd[i]);
		if (cmd[i] == '\'')
			new_cmd = expand_s_quote(cmd, new_cmd, &i);
		else if (cmd[i] == '\"')
			new_cmd = expand_d_quote(cmd, new_cmd, &i);
		else
		{
			new_cmd = expand_no_quotes(cmd, new_cmd, &i, &parts);
			//tmp_tab = expand_to_tab
			// cmd_tab = join_tabs
			//i += ft_strlen(new_cmd);
			// nouveau tableau + ajout new_cmd dans tab
		}
		cmd_tab = cmd_to_tab(cmd_tab, new_cmd, parts);
		printf("cmd after an expansion = %s\n", new_cmd);
	}
	return (NULL);
}
// Expander loop through the cmds table and try expand with expand() function
// expand() returns a tab of cmd that have to be added to the main cmd_tab

 t_AST	*expander(t_AST *node, t_shell *sh)
{
	(void)sh;
	int		i;
	int		j;
	//int		mem_size;
	int		cmds_size;
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
		cmds_size = i;
		i = 0;
		while (node->data.command.cmd_exe[i])
		{
			j = cmds_size;
			exp_word_split = expand(node->data.command.cmd_exe[i], sh, &cmds_size);
			printf("------end exp------\n");
			//cmd_tab = add_cmds(cmd_tab, i, exp_word_split, cmds_size);
			// ajout tableau recu dans expand dans le nouveau tab general.
			// --> Expand renverra le tableau pour la cmd en cours --> $var$var
			i++;
			j = 0;
			if (exp_word_split)
			{
				while (exp_word_split[j])
					free(exp_word_split[j++]);
				free(exp_word_split);
			}
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