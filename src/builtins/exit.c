#include "minishell.h"

int builtin_exit(t_ast_command *cmd, t_shell *sh)
{
	(void)cmd;
	(void)sh;
	fprintf(stderr, "builtin_exit not implemented\n");
	if (cmd->in_pipeline)
		exit(EXIT_SUCCESS);
	return (0);
}