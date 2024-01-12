/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvastena <fvastena@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 23:58:31 by fvastena          #+#    #+#             */
/*   Updated: 2024/01/12 13:57:21 by fvastena         ###   ########.fr       */
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
	var = get_var(tmp, tmp_i);
	d_quote_expand = NULL;
	if (!var)
	{
		if (new_cmd)
			return (ft_strdup(new_cmd));
		else
			return (NULL);
	}
	if (tmp[save_i] == '$')
	{
		tmp_expand = get_var_sub(var + 1, tmp_i);
		if (tmp_expand)
		{
			if (new_cmd)
				d_quote_expand = ft_strjoin(new_cmd, tmp_expand);
			else
				d_quote_expand = ft_strdup(tmp_expand);
			free(tmp_expand);
			*tmp_i += ft_strlen(var);
		}
		else
			return (NULL);
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
		cmd_tab = cmd_to_tab(cmd_tab, new_cmd, parts);
		if (!cmd_tab)
			return (NULL);
		*cmds_size = parts;
	}
	return (cmd_tab);
}


// Expander loop through the cmds table and try expand with expand() function
// expand() returns a tab of cmd that have to be added to the main cmd_tab

 t_AST	*expander(t_AST *node, t_shell *sh)
{
	// (void)sh;
	// (void)node;
	int		i;
	int		j;
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
			cmd_tab = add_cmds(cmd_tab, i, exp_word_split, cmds_size);
		}
		node->data.command.cmd_exe = cmd_tab;
	}
	return (node);
}