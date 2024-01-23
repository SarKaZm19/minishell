#include "minishell.h"

// https://www.gnu.org/software/coreutils/manual/coreutils.html#pwd-invocation

int pwd_builtin(t_ast_command *cmd, t_shell *sh)
{
	(void)cmd;
	(void)sh;
	fprintf(stderr, "pwd_builtin not implemented\n");
	return (0);
}