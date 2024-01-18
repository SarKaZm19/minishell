#include "minishell.h"

char	**word_split_expander(t_expander *exp, char **cmd_tab, t_shell *sh)
{
	char	**new_tab;
	char	**sub_split;

	int		i;
	int		j;

	printf("----- START word split expander -----\n");
	new_tab = NULL;
	sub_split = NULL;
	i = 0;
	printf("exp->new_cmd = .%s.\n", exp->new_cmd);
	printf("exp->split_parts = %d\n", exp->split_parts);
	printf("exp->nb_cmds = %d\n", exp->nb_cmds);
	if (!exp->new_cmd)
	{
		if (!cmd_tab)
			return (NULL);
		return (cmd_tab);
	}
	if (!cmd_tab)
	{
		if (exp->split_parts == 0)
		{
			printf("no_split...\n");
			new_tab = malloc(sizeof(char *) * 2);
			s_alloc(new_tab, PROMPT, sh);
			new_tab[0] = ft_strdup(exp->new_cmd);
			s_alloc(new_tab[0], PROMPT, sh);
			if (new_tab && new_tab[0])
				printf("new_tab[0] = %s\n", new_tab[0]);
			new_tab[1] = NULL;
			return (new_tab);
		}
		printf("no tab (word split expander)\n");
		printf("split_parts...\n");
		new_tab = malloc(sizeof(char *) * (exp->split_parts + 2));
		s_alloc(new_tab, PROMPT, sh);
		sub_split = ft_split(exp->new_cmd, ' ');
		while (sub_split[i])
		{
			new_tab[i] = ft_strdup(sub_split[i]);
			s_alloc(new_tab[i], PROMPT, sh);
			printf("sub_split[%d], %s\n", i, sub_split[i]);
			printf("new_tab[%d] = %s\n", i, new_tab[i]);
			i++;
		}
		new_tab[i] = NULL;
		i = 0;
		if (sub_split)
		{
			while (sub_split[i])
			{
				free(sub_split[i]);
				sub_split[i] = NULL;
				i++;
			}
			free(sub_split);
			sub_split = NULL;
		}
	}
	else
	{
		if (exp->split_parts == 0)
		{
			printf("split_parts 0 & cmd-exists\n");
			cmd_tab[exp->nb_cmds - 1] = ft_strjoin(cmd_tab[exp->nb_cmds - 1], exp->new_cmd);
			s_alloc(cmd_tab[exp->nb_cmds - 1], PROMPT, sh);
			return(cmd_tab);
		}
		printf("existing tab\n");
		new_tab = malloc(sizeof(char *) * (exp->nb_cmds + 1));
		s_alloc(new_tab, PROMPT, sh);
		sub_split = ft_split(exp->new_cmd, ' ');
		if (!sub_split)
			return (NULL);
		i = 0;
		while (cmd_tab[i])
		{
			if (cmd_tab[i + 1] == NULL)
				new_tab[i] = ft_strjoin(cmd_tab[i], sub_split[0]);
			else
				new_tab[i] = ft_strdup(cmd_tab[i]);
			printf("1ew_tab[%d] = %s\n", i, new_tab[i]);
			i++;
		}
		j = 1;
		while (sub_split[j])
		{
			new_tab[i + j - 1] = sub_split[j];
			printf("2new_tab[%d] = %s\n", i + j - 1, new_tab[i + j - 1]);
			j++;
		}
		printf("i + j - 1 = %d + %d - 1 = %d\n", i, j, i + j - 1);
		new_tab[i + j - 1] = NULL;
	}
	printf("----- END word split expander -----\n");
	return (new_tab);
}

char	**add_cmds(char	**cmd_tab, int cmd_tab_size, char **cmd_to_add, int new_tab_size, t_shell *sh)
{
	char	**new_cmds;
	int		i;
	int		j;

	i = 0;
	printf("cmd_tab_size = %d, new_tab_size = %d\n", cmd_tab_size, new_tab_size);
	if (!cmd_to_add)
	{
		printf("no cmd to add\n");
		if (!cmd_tab)   // Si envoi une seule commande genre $test mais $test existe pas dans les var d'env, getenv renvoi null pas bon résultat en envoyant une string vide (prob pipex execve) ou un espace (considéré comme une commande)
			return (cmd_tab); // Renvoyer NUll fonctionne bien, juste une condition a rajouter dans l'executor à mon avis
		new_cmds = malloc(sizeof(char *) * (cmd_tab_size + 1));
		s_alloc(new_cmds, PROMPT, sh);
		while (i < cmd_tab_size)
		{
			new_cmds[i] = ft_strdup(cmd_tab[i]);
			printf("new_cmds[%d] = %s", i, new_cmds[i]);
			s_alloc(new_cmds[i], PROMPT, sh);
			i++;
		}
		new_cmds[i] = NULL;
		return (new_cmds);
	}
	new_cmds = malloc(sizeof(char *) * (cmd_tab_size + new_tab_size + 1));
	s_alloc(new_cmds, PROMPT, sh);
	i = 0;
	while (i < cmd_tab_size)
	{
		new_cmds[i] = ft_strdup(cmd_tab[i]);
		s_alloc(new_cmds[i], PROMPT, sh);
		printf("new_cmds[%d] %s = %s cmd_tab[%d]\n", i, new_cmds[i],  cmd_tab[i], i);
		i++;
	}
	j = 0;
	while (cmd_to_add && cmd_to_add[j] && j < new_tab_size)
	{
		new_cmds[i] = ft_strdup(cmd_to_add[j]);
		s_alloc(new_cmds[i], PROMPT, sh);
		printf("new_cmds[%d] %s = %s cmd_to_add[%d]\n", i, new_cmds[i], cmd_to_add[j], j);
		i++;
		j++;
	}
	new_cmds[i] = NULL;
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
