#include "minishell.h"

char	**add_exp_to_cmd_tab(char **cmd_tab, int cmd_tab_size, char **exp_tab, int new_tab_size, t_shell *sh)
{
	char	**new_cmds;
	int		i;
	int		j;

	i = 0;
	//printf("cmd_tab_size = %d, new_tab_size = %d\n", cmd_tab_size, new_tab_size);
	if (cmd_tab_size != 0 && new_tab_size > 1)
		new_tab_size--;
	if (!exp_tab)
	{
		if (!cmd_tab)   // Si envoi une seule commande genre $test mais $test existe pas dans les var d'env, getenv renvoi null pas bon résultat en envoyant une string vide (prob pipex execve) ou un espace (considéré comme une commande)
			return (cmd_tab); // Renvoyer NUll fonctionne bien, juste une condition a rajouter dans l'executor à mon avis
		new_cmds = malloc(sizeof(char *) * (cmd_tab_size + 1));
		s_alloc(new_cmds, PROMPT, sh);
		while (i < cmd_tab_size)
		{
			new_cmds[i] = ft_strdup(cmd_tab[i]);
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
		i++;
	}
	j = 0;
	while (exp_tab && exp_tab[j] && j < new_tab_size)
	{
		new_cmds[i] = ft_strdup(exp_tab[j]);
		s_alloc(new_cmds[i], PROMPT, sh);
		i++;
		j++;
	}
	new_cmds[i] = NULL;
	return (new_cmds);
}
