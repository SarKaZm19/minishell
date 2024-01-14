/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_tab_expand.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvastena <fvastena@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/12 02:46:13 by fvastena          #+#    #+#             */
<<<<<<< Updated upstream
/*   Updated: 2024/01/12 13:58:37 by fvastena         ###   ########.fr       */
=======
/*   Updated: 2024/01/14 21:33:08 by fvastena         ###   ########.fr       */
>>>>>>> Stashed changes
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**add_cmds(char	**cmd_tab, int tab_i, char **cmd_to_add, int new_tab_size)
{
	char	**new_cmds;
	int		i;
	int		j;

	i = 0;
	if (!cmd_to_add)
	{
		new_cmds = malloc(sizeof(char *) * (tab_i + 1));
		if (!new_cmds)
			return (NULL);
		while (i < tab_i)
		{
			new_cmds[i] = ft_strdup(cmd_tab[i]);
			i++;
		}
		return (NULL);
	}
	new_cmds = malloc(sizeof(char *) * (tab_i + new_tab_size + 1));
	if (!new_cmds)
		return (NULL);
	i = 0;
	while (i < tab_i)
	{
		new_cmds[i] = ft_strdup(cmd_tab[i]);
		i++;
	}
	j = 0;
	while (cmd_to_add && j < new_tab_size)
	{
		new_cmds[i] = ft_strdup(cmd_to_add[j]);
		i++;
		j++;
	}
	new_cmds[i] = NULL;
	i = 0;
	if (cmd_tab)
	{
		while (cmd_tab[i])
		{
			free(cmd_tab[i]);
			cmd_tab[i] = NULL;
		}
		free(cmd_tab);
		cmd_tab = NULL;
	}
	i = 0;
	if (cmd_to_add)
	{
		while (cmd_to_add[i])
		{
			free(cmd_to_add[i]);
			cmd_to_add[i] = NULL;
		}
		free(cmd_to_add);
		cmd_to_add = NULL;
	};
	return (new_cmds);
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

	tmp = NULL;
	if (parts > 1)
		tmp = split_cmds(new_cmd, parts);
	cmd_tab_size = 0;
	while (cmd_tab && cmd_tab[cmd_tab_size])
		cmd_tab_size++;
	i = 0;
	if (!new_cmd)
		return (cmd_tab);
	if (!cmd_tab)
	{
		new_tab = malloc(sizeof(char *) * (parts + 1));
		if (!new_tab)
			return (NULL);
		while (i < parts)
		{
			new_tab[i] = ft_strdup(new_cmd);
			i++;
		}
		new_tab[i] = NULL;
	}
	else
	{
		j = 0;
		new_tab = malloc(sizeof(char *) * (cmd_tab_size + parts));
		if (!new_tab)
			return (NULL);
		while (i < cmd_tab_size)
		{
			new_tab[i] = ft_strdup(cmd_tab[i]);
			i++;
		}
		printf("copied existing\n");
		if (i != 0)
			i--;
		printf("i = %d\n", i);
		free(new_tab[i]);
		if (tmp)
			new_tab[i] = ft_strjoin(cmd_tab[i], tmp[j]);
		else
			new_tab[i] = ft_strjoin(cmd_tab[i], new_cmd);
		printf("(after join new_tab[%d] = %s\n", i, new_tab[i]);
		while (++i < parts)
		{
			new_tab[i] = ft_strdup(tmp[++j]);
		}
<<<<<<< Updated upstream
		new_tab[i] = NULL;
=======
		printf("copied new\n");
>>>>>>> Stashed changes
	}
	new_tab[i] = NULL;
	if (new_cmd)
		free(new_cmd);
	return (new_tab);
}
