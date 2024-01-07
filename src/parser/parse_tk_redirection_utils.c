#include "minishell.h"

void	add_arg_to_command(t_AST *command, char *arg, t_shell *sh)
{
	char	**new_cmd_exe;
	int		i;

	i = 0;
	while (command->data.command.cmd_exe[i])
		i++;
	new_cmd_exe = calloc(i + 2, sizeof(char *));
	s_alloc(new_cmd_exe, PROMPT, sh);
	i = 0;
	while (command->data.command.cmd_exe[i])
	{
		new_cmd_exe[i] = command->data.command.cmd_exe[i];
		i++;
	}
	new_cmd_exe[i] = arg;
	new_cmd_exe[i + 1] = NULL;
	command->data.command.cmd_exe = new_cmd_exe;
	// here we could eventually free the old cmd_exe to optimize memory usage
	// requirer a new function free_a_pointer() 
	// that search in the allocated pointer list after a specific pointer to free
}

t_AST	*get_end_of_redirection_list(t_AST *node)
{
	// printf("get_end_of_redirection_list\n");
	while (node->data.redirection.child)
		node = node->data.redirection.child;
	return (node);
}
