#include "minishell.h"

int builtin_env(t_ast_command *cmd, t_shell *sh)
{
	(void)cmd;
	(void)sh;
	fprintf(stderr, "builtin_env not implemented\n");
	if (cmd->in_pipeline)
		exit(EXIT_SUCCESS);
	return (0);
}