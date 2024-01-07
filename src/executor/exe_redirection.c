#include "minishell.h"

int	execute_redirection(t_ast_redirection *redir, t_shell *sh)
{
	int	status;

	if (redir->direction == TK_REDIRECT_IN)
		status = redirect_input(redir, sh);
	else if (redir->direction == TK_REDIRECT_OUT)
		status = redirect_output(redir, sh);
	else if (redir->direction == TK_APPEND_OUT)
		status = append_output(redir, sh);
	else if (redir->direction == TK_HEREDOC)
		status = heredoc_listener(redir, sh);
	else
	{
		fprintf(stderr, "Invalid redirection type\n");
		status = EXIT_FAILURE;
	}
	return (status);
}

int	redirect_input(t_ast_redirection *redir, t_shell *sh)
{
	int	input_fd;
	int	original_stdin;
	int	status;
		
	input_fd = open(redir->file, O_RDONLY);
	if (input_fd == -1)
	{
		report_errno(redir->file, sh);
		input_fd = s_open("/dev/null", O_RDONLY, 0, sh);
	}
	original_stdin = dup(STDIN_FILENO);
	s_dup2(input_fd, STDIN_FILENO, sh);
	s_close(input_fd,sh);
	status = execute(redir->child, sh);
	dup2(original_stdin, STDIN_FILENO);
	close(original_stdin);
	return (status);
}

int	redirect_output(t_ast_redirection *redir, t_shell *sh)
{
	int	output_fd;
	int	original_stdout;
	int	status;

	output_fd = s_open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644, sh);
	original_stdout = dup(STDOUT_FILENO);
	s_dup2(output_fd, STDOUT_FILENO, sh);
	s_close(output_fd, sh);
	status = execute(redir->child, sh);
	s_dup2(original_stdout, STDOUT_FILENO, sh);
	s_close(original_stdout, sh);
	return (status);
}

int	append_output(t_ast_redirection *redir, t_shell *sh)
{
	int	output_fd;
	int	original_stdout;
	int	status;

	output_fd = s_open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644, sh);
	original_stdout = dup(STDOUT_FILENO);
	s_dup2(output_fd, STDOUT_FILENO, sh);
	s_close(output_fd, sh);
	status = execute(redir->child, sh);
	s_dup2(original_stdout, STDOUT_FILENO, sh);
	s_close(original_stdout, sh);
	return (status);
}

int	heredoc_listener(t_ast_redirection *redir, t_shell *sh)
{
	int		tmp_fd;
	char	*line;
	int		gnl_status;
	int		status;

	tmp_fd = s_open("/tmp/.pp_heredoc", O_WRONLY | O_CREAT | O_TRUNC, 0600, sh);
	gnl_status = EXIT_SUCCESS;
	while (gnl_status == EXIT_SUCCESS)
	{
		gnl_status = get_next_line(0, &line);
		if (gnl_status == EXIT_FAILURE)
			error("get_next_line", "fail to read heredoc", EXIT_FAILURE, sh);
		if (ft_strncmp(line, redir->file, ft_strlen(redir->file)) == 0
			&& line[ft_strlen(redir->file)] == '\n')
		{
			free(line);
			break ;
		}
		s_write_fd(line, tmp_fd, sh);
		free(line);
	}
	s_close(tmp_fd, sh);
	redir->file = "/tmp/.pp_heredoc";
	status = redirect_input(redir, sh); // check if its right with redirect_input errors management
	s_unlink("/tmp/.pp_heredoc", sh);
	return (status);
}
