#include "minishell.h"

// init a global shell variable and set its values
void	init_shell(t_shell *sh, char **env)
{
	errno = 0;
	//
	sh->env = init_env(env, sh);
	sh->saved_paths_array = NULL;
	sh->saved_bin_paths = NULL;
	//
	sh->allocated_pointers[ONLY_CHECK] = NULL;
	sh->allocated_pointers[SH] = NULL;
	sh->allocated_pointers[PROMPT] = NULL;
	sh->temporary_files = NULL;
	//
	sh->parsing_error = NULL;
	sh->in_main_process = true;
	sh->last_prompt_exit_status = EXIT_SUCCESS;
	// sh->interactive = isatty(STDIN_FILENO);
}
