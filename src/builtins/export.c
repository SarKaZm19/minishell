#include "minishell.h"

int builtin_export(t_ast_command *cmd, t_shell *sh)
{
	(void)cmd;
	(void)sh;
	fprintf(stderr, "builtin_export not implemented\n");
	return (0);
}