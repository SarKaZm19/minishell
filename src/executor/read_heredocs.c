#include "minishell.h"

int	execute_heredocs(t_ast *node, t_shell *sh)
{
	char	*heredoc_file_name;
	int		status;

	status = EXIT_SUCCESS;
	if (!(node->type == AST_REDIRECTION
			&& node->data.redirection.direction == TK_HEREDOC)) // todo: sub function is_heredoc
		return (search_for_heredocs(node, sh));
	//
	heredoc_file_name = ft_strjoin("/tmp/.minishell_heredoc_",
			s_alloc(ft_itoa(ft_lstsize(sh->temporary_files)), PROMPT, sh)); // todo: check return itoa + strjoin
	s_alloc(heredoc_file_name, PROMPT, sh);
	lst_add_and_track(heredoc_file_name, &sh->temporary_files, PROMPT, sh);
	status = heredoc_listener(&node->data.redirection, heredoc_file_name, sh);
	if (status != EXIT_SUCCESS)
		return (status);
	node->data.redirection.direction = TK_REDIRECT_IN;
	// about REDIRECT_IN reconversion :
	// cannot work with expansion because redirect_in is not expanded but heredoc is
	// todo: can we expand at reading or it should also be done just on time during execution ?
	return (status);
}

int	search_for_heredocs(t_ast *node, t_shell *sh)
{
	if (!node)
		return (EXIT_SUCCESS);
	else if (node->type == AST_REDIRECTION)
	{
		if (node->data.redirection.direction == TK_HEREDOC)
			return (execute_heredocs(node, sh));
		else
			return (search_for_heredocs(node->data.redirection.child, sh));
	}
	else if (node->type == AST_LOGICAL)
		return (search_for_heredocs(node->data.logical.left, sh)
			|| search_for_heredocs(node->data.logical.right, sh));
	else if (node->type == AST_PIPELINE)
		return (search_for_heredocs(node->data.pipeline.left, sh)
			|| search_for_heredocs(node->data.pipeline.right, sh));
	else if (node->type == AST_GROUP)
		return (search_for_heredocs(node->data.group.child, sh));
	return (EXIT_SUCCESS);
}

int	heredoc_listener(t_ast_redirection *redir, char *tmp_file_name, t_shell *sh)
{
	int		tmp_fd;
	char	*line;
	char	*delimiter;
	int		gnl_status;

	tmp_fd = s_open(tmp_file_name, O_WRONLY | O_CREAT | O_TRUNC, 0600, sh);
	set_signal_heredoc();
	delimiter = redir->file;
	gnl_status = EXIT_SUCCESS;
	while (true)
	{
		// problem of multiple prompt if multiple lines
		// s_write_fd("heredoc> ", STDOUT_FILENO, sh);
		gnl_status = get_next_line(STDIN_FILENO, &line);
		if (gnl_status == EXIT_FAILURE)
		{
			if (errno == EINTR) // read stop by signal
				return (130);
			else
				error("heredoc", "fail to read heredoc", EXIT_FAILURE, sh);
		}
		if (ft_strlen(line) == 0) // for ^D
		{
			heredoc_warming_message(delimiter, sh);
			free(line);
			break ;
		}
		if (ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0
			&& line[ft_strlen(delimiter)] == '\n')
		{
			free(line);
			break ;
		}
		s_write_fd(line, tmp_fd, sh);
		free(line);
	}
	redir->file = tmp_file_name;
	set_signal_main_process();
	s_close(tmp_fd, sh);
	return (EXIT_SUCCESS);
}

void	heredoc_warming_message(char *delimiter, t_shell *sh)
{
	s_write_fd("\nminishell: warning: here-document delimited by end-of-file (wanted `",
		STDERR_FILENO, sh);
	s_write_fd(delimiter, STDERR_FILENO, sh);
	s_write_fd("')\n", STDERR_FILENO, sh);
}
