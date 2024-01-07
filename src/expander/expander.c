/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvastena <fvastena@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 23:58:31 by fvastena          #+#    #+#             */
/*   Updated: 2024/01/06 23:17:27 by fvastena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


char	*expand_d_quote(char *cmd, char *new_cmd, int *start_i)
{
	int		j;
	char	*d_quote_expand;
	char	*tmp;
	char	*tmp_expand;
	int		i;
	int		k;

	*start_i += 1;
	j = 0;
	while (cmd[*start_i + j] && cmd[*start_i + j] != '\"')
		j++;
	tmp = ft_substr(cmd, *start_i, j);
	// prot tmp + free
	printf("d_quote_expand = .%s., start_i = %d, j = %d\n", tmp, *start_i, j);
	i = 0;
	while (tmp[i])
	{
		if (tmp[i] == '$')
		{
			i++;
			k = i;
			while (cmd[k] && !is_space(tmp[k]))
				k++;
			tmp_expand = ft_substr(cmd, i, k - j);
			// tmp = getenv()
			// d_quote_expand = (((tmp + i) + tmp_expand) + (tmp i + expand_len))
			//d_quote_expand = 
		}
	}
	if (!new_cmd)
	{
		tab_cmds = malloc(sizeof(char *) * 2);
		if (!tab_cmds)
			return (NULL);
		tab_cmds[0] = ft_strdup(tmp);
		tab_cmds[1] = NULL;
	}
	if (!new_cmd)
		d_quote_expand = ft_strdup(tmp);	
	else
	{
		d_quote_expand = ft_strjoin(new_cmd, tmp);
		free(new_cmd);
		new_cmd = NULL;
	}
	if (tmp)
	{
		free(tmp);
		tmp = NULL;
	}
	*start_i += j + 1;
	printf("d_quote_expand = .%s.\n", d_quote_expand);
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

char	**expand(char *cmd, t_shell *sh, int *cmds_size)
{
	(void)sh;
	(void)cmds_size;
	int	i;
	char	*new_cmd;

	new_cmd = NULL;
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
			new_cmd = ft_strdup(cmd);
			i += ft_strlen(new_cmd);
		}
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