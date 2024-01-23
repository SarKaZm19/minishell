
#include "minishell.h"

// add a option to execute_command to specify if has to return or exit
int	execute(t_ast *node, t_execute_end end, t_shell *sh)
{
	int status;

	if (!node)
		return (EXIT_SUCCESS);
	if (node->type == AST_REDIRECTION || node->type == AST_COMMAND)
		expander(node, sh);
	if (node->type == AST_LOGICAL)
		status = execute_logical(&node->data.logical, sh);
	else if (node->type == AST_PIPELINE)
		status = execute_pipeline(node, sh);
	else if (node->type == AST_REDIRECTION)
		status = execute_redirection(&node->data.redirection, sh);
	else if (node->type == AST_GROUP)
		status = execute_group(&node->data.group, sh);
	else if (node->type == AST_COMMAND)
		status = execute_command(&node->data.command, end, sh);
	else // debug
		error("execute", "illegal node type", EXIT_FAILURE, sh);
	if (end == O_EXIT)
		exit(status);
	return (status);
}

int check_process_child_exit(int status, bool *new_line, t_shell *sh)
{
	int signal;

	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
	{
		signal = WTERMSIG(status);
		if (signal == SIGQUIT)
			s_write_fd("Quit: 3", STDERR_FILENO, sh);
		if (signal == SIGQUIT || signal == SIGINT)
		{
			if (!new_line || (new_line && *new_line == false))
				printf("\n");
			if (new_line && *new_line == false)
				*new_line = true;
		}
		return (128 + signal);
	}
	else
		return (EXIT_FAILURE);
}
