#include "minishell.h"

int builtin_pwd(t_ast_command *cmd, t_shell *sh)
{
	(void)cmd;
	(void)sh;
	fprintf(stderr, "builtin_pwd not implemented\n");
	return (0);
}