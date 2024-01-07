#include "minishell.h"

int builtin_unset(t_ast_command *cmd, t_shell *sh)
{
	(void)cmd;
	(void)sh;
	fprintf(stderr, "builtin_unset not implemented\n");
	return (0);
}