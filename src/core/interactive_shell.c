
#include "minishell.h"

// lexer -> parser -> expander -> executor
int	interactive_shell(t_shell *sh)
{
	char	*user_input;
	int		status;

	while (42)
	{
        g_signal_value = 0;
		set_signal_prompt();
		user_input = readline("\x1b[32mminishell$ \x1b[0m"); // replace by env PS1
		set_signal_main_process();
		if (!user_input)
			exit_builtin(NULL, sh);
		track_alloc(user_input, PROMPT, sh);
		if (ft_strlen(user_input) > 0)
		{
			add_history(user_input);
			status = prompt_execution(user_input, sh);
			sh->last_prompt_exit_status = status;
		}
		clear_prompt(sh);
	}
	return (status);
}

void clear_prompt(t_shell *sh)
{
	while (sh->temporary_files)
	{
		s_unlink(sh->temporary_files->content, sh);
		sh->temporary_files = sh->temporary_files->next;
	}
	ft_lstclear(&(sh->temporary_files), free);
	ft_lstclear(&(sh->allocated_pointers[PROMPT]), free);
	sh->parsing_error = NULL;
}
