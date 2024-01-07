
#include "minishell.h"

char	*ast_type_to_str(t_ast_type type)
{
	if (type == AST_LOGICAL)
		return ("AST_LOGICAL");
	else if (type == AST_PIPELINE)
		return ("AST_PIPELINE");
	else if (type == AST_REDIRECTION)
		return ("AST_REDIRECTION");
	else if (type == AST_GROUP)
		return ("AST_GROUP");
	else if (type == AST_COMMAND)
		return ("AST_COMMAND");
	else
		return ("UNKNOWN");
}

int	execute(t_AST *node, t_shell *sh)
{
	int status;

	if (node->type == AST_REDIRECTION || node->type == AST_COMMAND)
		expander(node, sh);
	if (node->type == AST_LOGICAL)
		status = execute_logical(&node->data.logical, sh);
	else if (node->type == AST_PIPELINE)
		status = execute_pipeline(node, sh);
	else if (node->type == AST_REDIRECTION)
		status = execute_redirection(&node->data.redirection, sh); // --> Expand filename *
	else if (node->type == AST_GROUP)
		status = execute_group(&node->data.group, sh);
	else if (node->type == AST_COMMAND)
		status = execute_command(&node->data.command, sh);
	else
		status = printf("Executor: a feature in the command is not implemented yet\n");
	return (status);
}
