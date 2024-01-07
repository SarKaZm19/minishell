#include "minishell.h"

// I decided to adopt a simpler approach than recursive execution
// before any execution, we will build a linked list of the whole pipeline
// then we will execute it as in Pipex

int	execute_pipeline(t_AST *node, t_shell *sh)
{
	t_list	*cmd_list;

	cmd_list = build_cmd_list(node, sh);
	return (execute_pipeline_list(cmd_list, sh));
}

t_list	*build_cmd_list(t_AST *node, t_shell *sh)
{
	t_list	*cmd_list;
	t_list	*cmd_right;
	t_list	*cmd_left;

	cmd_list = NULL;
	while (node->type == AST_PIPELINE)
	{
		cmd_right = ft_lstnew(node->data.pipeline.right);
		s_alloc(cmd_right, PROMPT, sh);
		ft_lstadd_front(&cmd_list, cmd_right);
		if (node->data.pipeline.left->type == AST_PIPELINE)
			node = node->data.pipeline.left;
		else
		{
			cmd_left = ft_lstnew(node->data.pipeline.left);
			s_alloc(cmd_left, PROMPT, sh);
			ft_lstadd_front(&cmd_list, cmd_left);
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
	// setup_input(d->input_file, d->limiter, sh);
	// setup_output(d->output_file, sh);
	last_pid = 0; // ca c'est bizare a checker
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
	if (prev_read_end != -1)
	{
		s_dup2(prev_read_end, STDIN_FILENO, sh);
		s_close(prev_read_end, sh);
	}
	if (cmd_list->next != NULL)
		s_dup2(p[WRITE_END], STDOUT_FILENO, sh);
	s_close(p[READ_END], sh);
	s_close(p[WRITE_END], sh);

	// char	** cmd_exe = ((t_AST *)cmd_list->content)->data.command.cmd_exe;
	// s_execve(get_bin_path(cmd_exe[0], sh->env_paths, sh), cmd_exe, sh->env, sh);
	get_ast_command(cmd_list->content)->in_pipeline = true;
	execute((t_AST *)cmd_list->content, sh);

	return (pid); // unreachable except in case of execute error
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
	pid_t child_pid;
	int status;
	int last_cmd_exit_status;

	last_cmd_exit_status = 0;
	while (n_cmd--)
	{
		child_pid = s_wait(&status, sh);
		if (child_pid == last_pid && WIFEXITED(status))
			last_cmd_exit_status = WEXITSTATUS(status);
	}
	return (last_cmd_exit_status);
}

t_ast_command *get_ast_command(t_AST *node)
{
	while (node->type == AST_REDIRECTION)
		node = node->data.redirection.child;
	return (&node->data.command);
}