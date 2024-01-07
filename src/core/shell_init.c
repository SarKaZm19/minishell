#include "minishell.h"

// init a global shell variable and set its values
void	init_shell(t_shell *sh, char **env)
{
	errno = 0;

	sh->allocated_pointers[ONLY_CHECK] = NULL;
	sh->allocated_pointers[SH] = NULL;
	sh->allocated_pointers[PROMPT] = NULL;
	sh->parsing_error = NULL;
	sh->env = env;
	sh->env_paths = init_env_paths(env, sh);
	// sh->interactive = isatty(STDIN_FILENO);
}

char	**init_env_paths(char **env, t_shell *sh)
{
	char	**paths;
	char	*path;
	int		i;

	paths = NULL;
	i = 0;
	while (env[i] && ft_strncmp(env[i], "PATH=", 5))
		i++;
	if (!env[i])
		path = s_alloc(ft_strdup("/bin:/sbin:/usr/bin:/usr/sbin:/usr/local/bin:\
			/usr/local/sbin:/opt/bin:/opt/sbin"), SH, sh);
	else
		path = env[i] + 5;
	paths = ft_split(path, ':');
	if (!paths)
		error("ft_split", "fail to split env path", EXIT_FAILURE, sh);
	track_alloc(paths, SH, sh);
	i = 0;
	while (paths[i])
		track_alloc(paths[i++], SH, sh);
	return (paths);
}
