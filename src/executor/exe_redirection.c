#include "minishell.h"

int	execute_redirection(t_ast_redirection *redir, t_shell *sh)
{
	int	status;

	status = EXIT_FAILURE;
	if (redir->direction == TK_REDIRECT_IN)
		status = redirect_input(redir, sh);
	else if (redir->direction == TK_REDIRECT_OUT)
		status = redirect_output(redir, sh);
	else if (redir->direction == TK_APPEND_OUT)
		status = append_output(redir, sh);
	else
		error("execute_redirection", "redirection type not recognized", EXIT_FAILURE, sh);
	return (status);
}

int	redirect_input(t_ast_redirection *redir, t_shell *sh)
{
	int	input_fd;
	int	original_stdin;
	int	status;
	
	input_fd = open(redir->file, O_RDONLY);
	if (input_fd == -1)
		return (report_errno(redir->file, sh));
	original_stdin = dup(STDIN_FILENO);
	s_dup2(input_fd, STDIN_FILENO, sh);
	s_close(input_fd, sh);
	status = execute(redir->child, O_RETURN, sh);
	dup2(original_stdin, STDIN_FILENO);
	s_close(original_stdin, sh);
	return (status);
}

int	redirect_output(t_ast_redirection *redir, t_shell *sh)
{
	int	output_fd;
	int	original_stdout;
	int	status;

	output_fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (output_fd == -1)
		return (report_errno(redir->file, sh));
	original_stdout = dup(STDOUT_FILENO);
	s_dup2(output_fd, STDOUT_FILENO, sh);
	s_close(output_fd, sh);
	status = execute(redir->child, O_RETURN, sh);
	s_dup2(original_stdout, STDOUT_FILENO, sh);
	s_close(original_stdout, sh);
	return (status);
}

int	append_output(t_ast_redirection *redir, t_shell *sh)
{
	int	output_fd;
	int	original_stdout;
	int	status;

	output_fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (output_fd == -1)
		return (report_errno(redir->file, sh));
	original_stdout = dup(STDOUT_FILENO);
	s_dup2(output_fd, STDOUT_FILENO, sh);
	s_close(output_fd, sh);
	status = execute(redir->child, O_RETURN, sh);
	s_dup2(original_stdout, STDOUT_FILENO, sh);
	s_close(original_stdout, sh);
	return (status);
}
