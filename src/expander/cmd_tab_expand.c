#include "minishell.h"

char	**word_split_expander(t_expander *exp, char **cmd_tab)
{
	char	**new_tab;

	new_tab = NULL;
	printf("exp.split_parts = %d\n", exp->split_parts);
	if (!cmd_tab)
	{
		printf("no existing tab\n");
		if (exp->split_parts == 0)
		{
			new_tab = malloc(sizeof(char *) * 2);
			if (!new_tab)
				return (NULL);
			new_tab[0] = ft_strdup(exp->new_cmd);
			new_tab[1] = NULL;
			printf("new_tab[0] = %s\n", cmd_tab[0]);
		}
		else
		{
			// a faire si cmd_tab existe pas mais cmd a split
			/* new_tab = malloc(sizeof(char *) * (exp->split_parts + 1));
			if (!new_tab)
				return (NULL);
			while (cmd_tab[i])
			{
				cmd_tab[i] = ft_strdup(new_tab[i]);

			} */
		}
	}
	else
	{
		printf("existing tab\n");
		//a faire si cmd_tab existe et split_part = 0 ou +
	}
	return (new_tab);
}

char	**add_cmds(char	**cmd_tab, int tab_i, char **cmd_to_add, int new_tab_size)
{
	char	**new_cmds;
	int		i;
	int		j;

	i = 0;
	if (!cmd_to_add)
	{
		printf("no cmds to add\n");
		new_cmds = malloc(sizeof(char *) * (tab_i + 1));
		if (!new_cmds)
			return (NULL);
		while (i < tab_i)
		{
			new_cmds[i] = ft_strdup(cmd_tab[i]);
			printf("new_cmds[%d] = %s\n", i, new_cmds[i]);
			i++;
		}
		return (new_cmds);
	}
	printf("cmds to add\n");
	printf("tab_i + new_tab_size = %d\n", tab_i + new_tab_size);
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
	return (new_cmds);
}

char	**split_cmds(t_expander *exp)
{
	char	**tmp_tab;
	int		i;
	int		j;
	int		k;

	tmp_tab = malloc(sizeof(char *) * (exp->split_parts + 1));
	if (!tmp_tab)
		return (NULL);
	i = 0;
	j = 0;
	k = 0;
	while (k < exp->split_parts)
	{
		while (exp->new_cmd[i] && is_space(exp->new_cmd[i]))
			i++;
		while (exp->new_cmd[i + j] && !is_space(exp->new_cmd[i + j]))
			j++; 
		tmp_tab[k] = ft_substr(exp->new_cmd, i, j);
		i++;
		k++;
	}
	tmp_tab[k] = NULL;
	return (tmp_tab);
}

char	**cmd_to_tab(t_expander *exp, char **cmd_tab)
{
	char	**tmp;
	char	**new_tab;
	int		cmd_tab_size;
	int		i;
	int		j;

	tmp = NULL;
	if (exp->split_parts > 0)
		tmp = split_cmds(exp);
	cmd_tab_size = 0;
	while (cmd_tab && cmd_tab[cmd_tab_size])
		cmd_tab_size++;
	i = 0;
	if (!exp->new_cmd)
		return (cmd_tab);
	if (!cmd_tab)
	{
		new_tab = malloc(sizeof(char *) * (exp->split_parts + 1));
		if (!new_tab)
			return (NULL);
		while (i < exp->split_parts)
		{
			new_tab[i] = ft_strdup(exp->new_cmd);
			i++;
		}
	}
	else
	{
		j = 0;
		new_tab = malloc(sizeof(char *) * (cmd_tab_size + exp->split_parts));
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
			new_tab[i] = ft_strjoin(cmd_tab[i], exp->new_cmd);
		printf("(after join new_tab[%d] = %s\n", i, new_tab[i]);
		while (++i < exp->split_parts)
		{
			new_tab[i] = ft_strdup(tmp[++j]);
		}
		printf("copied new\n");
	}
	new_tab[i] = NULL;
	if (exp->new_cmd)
		free(exp->new_cmd);
	return (new_tab);
}
