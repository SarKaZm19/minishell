#include "minishell.h"

char	**word_split_expander(t_expander *exp, char **cmd_tab, t_shell *sh)
{
	char	**new_tab;
	char	**sub_split;

	int		i;
	int		j;

	new_tab = NULL;
	sub_split = NULL;
	i = 0;
	if (!exp->new_arg)
	{
		if (!cmd_tab)
			return (NULL);
		return (cmd_tab);
	}
	if (!cmd_tab)
	{
		if (exp->nb_split_parts == 0)
		{
			new_tab = s_alloc(malloc(sizeof(char *) * 2), PROMPT, sh);
			new_tab[0] = ft_strdup(exp->new_arg);
			s_alloc(new_tab[0], PROMPT, sh);
			new_tab[1] = NULL;
			return (new_tab);
		}
		new_tab = malloc(sizeof(char *) * (exp->nb_split_parts + 2));
		s_alloc(new_tab, PROMPT, sh);
		sub_split = ft_split(exp->new_arg, ' ');
		while (sub_split[i])
		{
			new_tab[i] = ft_strdup(sub_split[i]);
			s_alloc(new_tab[i], PROMPT, sh);
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
		if (exp->nb_split_parts == 0)
		{
			cmd_tab[exp->nb_args - 1] = ft_strjoin(cmd_tab[exp->nb_args - 1], exp->new_arg);
			s_alloc(cmd_tab[exp->nb_args - 1], PROMPT, sh);
			return(cmd_tab);
		}
		new_tab = malloc(sizeof(char *) * (exp->nb_args + 1));
		s_alloc(new_tab, PROMPT, sh);
		sub_split = ft_split(exp->new_arg, ' ');
		if (!sub_split)
			return (NULL);
		i = 0;
		while (cmd_tab[i])
		{
			if (cmd_tab[i + 1] == NULL)
				new_tab[i] = ft_strjoin(cmd_tab[i], sub_split[0]);
			else
				new_tab[i] = ft_strdup(cmd_tab[i]);
			i++;
		}
		j = 1;
		while (sub_split[j])
		{
			new_tab[i + j - 1] = sub_split[j];
			j++;
		}
		new_tab[i + j - 1] = NULL;
	}
	return (new_tab);
}
