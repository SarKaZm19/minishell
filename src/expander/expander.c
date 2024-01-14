/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvastena <fvastena@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/12 02:39:07 by fvastena          #+#    #+#             */
/*   Updated: 2024/01/15 00:10:19 by fvastena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*expand_var(char *new_cmd, char *tmp, int *tmp_i)
{
	char	*d_quote_expand;
	char	*var;
	char	*tmp_expand;
	int		save_i;

	save_i = *tmp_i;
	d_quote_expand = NULL;
	var = get_var(tmp, tmp_i);
	printf("var = %s\n", var);
	if (!var)
	{
		if (new_cmd)
			return (new_cmd);
		else
			return (NULL);
	}
	if (tmp[save_i] == '$')
	{
		tmp_expand = get_var_sub(var + 1, tmp_i);
		printf("new_cmd in $ = %s\n", new_cmd);
		if (tmp_expand)
		{
			if (new_cmd)
				d_quote_expand = ft_strjoin(new_cmd, tmp_expand);
			else
				d_quote_expand = ft_strdup(tmp_expand);
			free(tmp_expand);
			printf("d_quote_expand = %s\n", d_quote_expand);
		}
		else
		{
			if (new_cmd)
				d_quote_expand = ft_strdup(new_cmd);
		}
		*tmp_i += ft_strlen(var) + 1;
	}
	else if (tmp[*tmp_i] == '*')
	{
		if (var)
		{
			if (new_cmd)
				d_quote_expand = ft_strjoin(new_cmd, var);
			else
				d_quote_expand = ft_strdup(var);
		}
		*tmp_i += ft_strlen(var) + 1;
	}
	else
	{
		if (var)
		{
			if (new_cmd)
				d_quote_expand = ft_strjoin(new_cmd, var);
			else
				d_quote_expand = ft_strdup(var);
		}
		*tmp_i += ft_strlen(var);
	}
	return (d_quote_expand);
}


// Expand function takes a cmd_tab index and check the expansion with $, ",' and *
char	**expand(char *cmd, t_shell *sh, int *cmds_size)
{
	(void)sh;
	int	i;
	char	*new_cmd;
	char	**cmd_tab;
	int		parts;

	new_cmd = NULL;
	cmd_tab = NULL;
	i= 0;
	while (cmd[i])
	{
		parts = 0;
		if (cmd[i] == '\'')
		{
			new_cmd = expand_s_quote(cmd, new_cmd, &i);
			parts++;
		}
		else if (cmd[i] == '\"')
		{
			new_cmd = expand_d_quote(cmd, new_cmd, &i);
			parts++;
		}
		else
		{
			new_cmd = expand_no_quotes(cmd, new_cmd, &i, &parts);
		}
		printf("new_cmd expand() = %s\n", new_cmd);
		cmd_tab = cmd_to_tab(cmd_tab, new_cmd, parts);
		if (!cmd_tab)
			return (NULL);
		*cmds_size = parts;
		printf("i = %d\n", i);
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
		i = 0;
		while (node->data.command.cmd_exe[i])
		{
			exp_word_split = expand(node->data.command.cmd_exe[i], sh, &cmds_size);
			printf("------end exp------\n");
			printf("cmds size = %d\n", cmds_size);
			cmd_tab = add_cmds(cmd_tab, i, exp_word_split, cmds_size);
			printf("cmd_tab[0] = %s\n", cmd_tab[0]);
			printf("??\n");
			//cmd_tab = add_cmds(cmd_tab, i, exp_word_split, cmds_size);
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