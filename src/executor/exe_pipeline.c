#include "minishell.h"

// I decided to adopt a simpler approach than recursive execution
// before any execution, we will build a linked list of the whole pipeline
// then we will execute it as in Pipex

int	execute_pipeline(t_ast *node, t_shell *sh)
{
	t_list	*cmd_list;

	cmd_list = build_cmd_list(node, sh);
	return (execute_pipeline_list(cmd_list, sh));
}

t_list	*build_cmd_list(t_ast *node, t_shell *sh)
{
	t_list	*cmd_list;

	cmd_list = NULL;
	while (node->type == AST_PIPELINE)
	{
		lst_add_and_track(node->data.pipeline.right, &cmd_list, PROMPT, sh);
		if (node->data.pipeline.left->type == AST_PIPELINE)
			node = node->data.pipeline.left;
		else
		{
			lst_add_and_track(node->data.pipeline.left, &cmd_list, PROMPT, sh);
			break ;
		}
	}
	return (cmd_list);
}

int	execute_pipeline_list(t_list *cmd_list, t_shell *sh)
{
	int		p[2];
	int		prev_read_end;
	pid_t	last_pid;
	int		n_cmd;
	int		last_cmd_exit_status;

	n_cmd = ft_lstsize(cmd_list);
	prev_read_end = -1;
	last_pid = 0;
	while (cmd_list)
	{
		s_pipe(p, sh);
		last_pid = execute_pipeline_command(cmd_list, sh, prev_read_end, p);
		setup_for_next_command(&prev_read_end, p, sh);
		cmd_list = cmd_list->next;
	}
	last_cmd_exit_status = wait_for_children(sh, last_pid, n_cmd);
	return (last_cmd_exit_status);
}

pid_t	execute_pipeline_command(t_list *cmd_list, t_shell *sh,
		int prev_read_end, int p[2])
{
	pid_t	pid;

	pid = s_fork(sh);
	if (pid != 0)
		return (pid);
	sh->in_main_process = false;
	set_signal_child_process();
	if (prev_read_end != -1)
	{
		s_dup2(prev_read_end, STDIN_FILENO, sh);
		s_close(prev_read_end, sh);
	}
	if (cmd_list->next != NULL)
		s_dup2(p[WRITE_END], STDOUT_FILENO, sh);
	s_close(p[READ_END], sh);
	s_close(p[WRITE_END], sh);
	//
	execute((t_ast *)cmd_list->content, O_EXIT, sh);
	return (pid);
}

void	setup_for_next_command(int *prev_read_end, int p[2], t_shell *sh)
{
	if (*prev_read_end != -1)
		s_close(*prev_read_end, sh);
	s_close(p[WRITE_END], sh);
	*prev_read_end = p[READ_END];
}

int	wait_for_children(t_shell *sh, pid_t last_pid, int n_cmd)
{
	pid_t	child_pid;
	int		status;
	int		last_cmd_exit_status;
	bool	new_line;

	new_line = false;
	last_cmd_exit_status = 0;
	while (n_cmd--)
	{
		child_pid = s_wait(&status, sh);
		if (child_pid == last_pid)
			last_cmd_exit_status = check_process_child_exit(status, &new_line,
					sh);
		else
			check_process_child_exit(status, &new_line, sh);
	}
	return (last_cmd_exit_status);
}
