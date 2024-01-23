#include "minishell.h"

// https://www.gnu.org/software/coreutils/manual/coreutils.html#Exit-status
// However, some of the programs documented here do produce other exit 
// status values and a few associate different meanings with the values 
// ‘0’ and ‘1’. Here are the exceptions: env. 

// https://www.gnu.org/software/coreutils/manual/coreutils.html#env-invocation
// If no command name is specified following the environment specifications, 
// the resulting environment is printed. 

// Is "env" command the built-in of the bash?: 
// https://stackoverflowteams.com/c/42network/questions/1686

// Write bellow canonical comment with brief, arg, ...
/**
 *	@brief Implementation of the env builtin without options and arguments.
 *	@result Prints the environment variables list in STDOUT.
 *	@param cmd command to execute with cmd->argv
 *	@param sh shell structure
 *	@return
 */
int env_builtin(t_ast_command *cmd, t_shell *sh)
{
	(void)cmd;
	print_env(false, sh->env, sh);
	return (EXIT_SUCCESS);
}
