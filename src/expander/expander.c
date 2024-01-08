/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvastena <fvastena@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 23:58:31 by fvastena          #+#    #+#             */
/*   Updated: 2024/01/08 20:32:25 by fvastena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_var(char *tmp, int i)
{
	char	*var;
	int		j;

	i += 1;
	j = i;
	while (tmp[j] && tmp[j] != '\"' && tmp[j] != '\'' && !is_space(tmp[j]) && tmp[j] != '$' && tmp[j] != '*')
		j++;
	var = ft_substr(tmp, i, j - i - 1);
	return (var);
}

char	*get_var_sub(char *tmp, char *var, int *i)
{
	char	*var_sub;

	var_sub = ft_strdup(getenv(var));
	returm (var_sub);
}

char	*expand_var(char *new_cmd, char *tmp, int *tmp_i)
{
	char	*d_quote_expand;
	char	*var;
	char	*tmp_expand;

	var = get_var(tmp, tmp_i);
	if (!var)
		return (NULL);
	if (tmp[*tmp_i] == '$')
	{
		tmp_expand = get_var_sub(tmp, var, tmp_i);
		printf("var = %s, getenv(var) = %s\n", var, tmp_expand);
		if (tmp_expand)
		{
			d_quote_expand = ft_strjoin(new_cmd, tmp_expand);
			free(tmp_expand);
		}
		free(new_cmd);
	}
	else if (tmp[*tmp_i] == '*')
	{
		if (var)
			d_quote_expand = ft_strjoin(new_cmd, var);
	}
	else
	{
		if (var)
			d_quote_expand = ft_strjoin(new_cmd, var);
	}
	tmp_i += ft_strlen(var);
	if (var)
		free(var);
	else
		d_quote_expand = ft_strdup(new_cmd);
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
		cmds_tab[i] == ft_strdup(tmp);
		i++;
		j += k;
	}
	return (cmds_tab);
}

char	**expand_no_quotes(char *cmd, char *new_cmd, int *start_i)
{
	char	**sub_tab;
	char	*sub;
	char	*tmp;
	int		tmp_i;
	int		j;

	j = 0;
	while (cmd[*start_i + j] && cmd[*start_i + j] != '\'' && cmd[*start_i + j] != '\"' && !is_space(cmd[*start_i + j]) && cmd[*start_i + j] != '$' && cmd[*start_i + j])
		j++;
	tmp = ft_substr(cmd, *start_i, j);
	while (tmp[tmp_i])
	{
		if (tmp[tmp_i] == '$')
		{
			sub = expand_var(new_cmd, tmp, &tmp_i);
			sub_tab = get_cmds_tab(sub_tab, sub);
			// format sub to split sur espace ajout tab sur ligne en cours, derniere ligne du tab devient ligne en cours
			// ajout tableau
		}
		else if (tmp[tmp_i] == '*')
		{
			sub = expand_var(new_cmd, tmp, &tmp_i);
			sub_tab = get_cmds_tab(sub_tab, sub);
			// format sub to split sur espace
			// ajout tableau
		}
		else
		{
			sub = expand_var(new_cmd, tmp, &tmp_i);
			sub_tab = get_cmds_tab(sub_tab, sub);
			// ajout tab dans la ligne en cours
		}
	}
	// si * dans une var, pas de substitution du *, interprete litteralement
	// voir si on peut pas lire tout d un coup et tout mettre dans un tableau apres dans expand
}

char	*expand_d_quote(char *cmd, char *new_cmd, int *start_i)
{
	char	*tmp;
	int		j;
	int		i;

	*start_i += 1;
	j = 0;
	while (cmd[*start_i + j] && cmd[*start_i + j] != '\"')
		j++;
	tmp = ft_substr(cmd, *start_i, j);
	// prot tmp + free
	printf("quote tmp = .%s., start_i = %d, j = %d\n", tmp, *start_i, j);
	i = 0;
	while (tmp[i])
	{
		if (tmp[i] == '$')
			new_cmd = expand_var(new_cmd, tmp, i);
		else
			new_cmd = expand_var(new_cmd, tmp, i);
		printf("quote $ treatment starts\n");		
	}
	printf("new_cmd quote_expand = .%s.\n", new_cmd);
	return (new_cmd);
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

char	**expand(char *cmd, t_shell *sh, int *cmds_size)
{
	(void)sh;
	(void)cmds_size;
	int	i;
	int	quotes;
	char	*new_cmd;
	char	*cmd_tab;

	new_cmd = NULL;
	cmd_tab = NULL;
	i= 0;
	while (cmd[i])
	{
		printf("cmd[%d] check to expand = %c\n", i, cmd[i]);
		if (cmd[i] == '\'')
			new_cmd = expand_s_quote(cmd, new_cmd, &i);
		else if (cmd[i] == '\"')
		{
			new_cmd = expand_d_quote(cmd, new_cmd, &i);
			i += ft_strlen(new_cmd);
		}
		else
		{
			new_cmd = expand_no_quotes(cmd, new_cmd, &i);
			i += ft_strlen(new_cmd);
			// nouveau tableau + ajout new_cmd dans tab
		}
		// cmd_to_tabs
		printf("cmd after an expansion = %s\n", new_cmd);
	}
	return (NULL);
}

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