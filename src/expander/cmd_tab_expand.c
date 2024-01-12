/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_tab_expand.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvastena <fvastena@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/12 02:46:13 by fvastena          #+#    #+#             */
/*   Updated: 2024/01/12 02:48:08 by fvastena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**add_cmds(char	**cmd_tab, int tab_i, char **cmd_to_add, int new_tab_size)
{
	char	**new_cmds;
	int		i;
	int		j;

	if (!cmd_to_add)
		return (cmd_tab);
	printf("tab_i = %d, new_tab_size = %d\n", tab_i, new_tab_size);
	new_cmds = malloc(sizeof(char *) * (tab_i + new_tab_size + 1));
	if (!new_cmds)
		return (NULL);
	i = 0;
	printf("malloc done\n");
	while (i < tab_i)
	{
		new_cmds[i] = ft_strdup(cmd_tab[i]);
		printf("print existing tab index\n");
		i++;
	}
	j = 0;
	printf("existing tab index copied\n");
	printf("cmd_to_add[j] = %s\n", cmd_to_add[j]);
	printf("i = %d, j = %d, new_tab_size = %d\n", i, j, new_tab_size);
	/* if (cmd_tab)
		new_cmds[i] = ft_strjoin(cmd_tab[i], cmd_to_add[j]);
	else
		new_cmds[i] = ft_strdup(cmd_to_add[j]); */
	//printf("copied on same index = %s\n", new_cmds[i]);
	while (cmd_to_add && j < new_tab_size)
	{
		new_cmds[i] = ft_strdup(cmd_to_add[j]);
		printf("add cmds to cmd_tab\n");
		i++;
		j++;
	}
	printf("cmd_to_add added to cmd_tab\n");
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
	printf("after free\n");
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
	printf("parts = %d, cmd_tab_size = %d\n", parts, cmd_tab_size);
	i = 0;
	if (!cmd_tab)
	{
		printf("cmd_tab not exists\n");
		new_tab = malloc(sizeof(char *) * (parts + 1));
		if (!new_tab)
			return (NULL);
		while (i < parts)
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
