
#include "libraries.h"

extern volatile sig_atomic_t	g_signal_value;

// todo: check the new norm, III.4 Typedef, struct, enum and union
// can enum be in CAPITAL letter?: You must indent all structures’ names on the same column for their scope.

// todo: slipt stucture.h into shell.h and token.h, ast.h

#define SCOPES 4
typedef enum t_tracking_scope
{
	ONLY_CHECK,
	SH,
	PROMPT,
	EXPANDER
}								t_tracking_scope;

typedef enum t_execute_end
{
	O_RETURN,
	O_EXIT
}								t_execute_end;

typedef struct s_shell
{
	t_list						*env; // t_list of t_env_var
	char						**saved_paths_array; // manage by PROMPT scope
	t_list						*saved_bin_paths; // t_list of t_env_var
	// todo: free saved_paths_array and saved_bin_paths at quit_minishell
	//
	t_list						*allocated_pointers[SCOPES];
	t_list						*temporary_files;
	//
	char						*parsing_error;
	bool						in_main_process; // todo: could be replace by SHVL? not sure
	int							last_prompt_exit_status;
}								t_shell;

typedef struct s_env_var
{
	char						*name;
	char						*value;
}								t_env_var;

// Pipex program structure
typedef struct s_pipe
{
	char						*input_file;
	bool						here_doc;
	char						*limiter;
	char						*output_file;

	int							n_cmd;
	char						***commands;
	int							last_cmd_exit_status;

	t_list						*allocated_pointers;
}								t_pipe;

typedef struct s_expander
{
	char						*arg;
	int							arg_len;

	char						*arg_part;
	int							arg_part_len;

	char						*var_to_sub;
	int							var_to_sub_len;

	char						*subbed_var;
	int							subbed_var_len;

	char						*new_arg;
	int							nb_split_parts;
	int							nb_args;
}								t_expander;

#define READ_END 0
#define WRITE_END 1

// Lexical analysis / Tokenizer
typedef enum e_token_type
{
	TK_ERROR,        // err token
	TK_WORD,         // "word"
	TK_PIPE,         // |
	TK_AND,          // &&
	TK_OR,           // ||
	TK_REDIRECT_IN,  // <
	TK_REDIRECT_OUT, // >
	TK_APPEND_OUT,   // >>
	TK_HEREDOC,      // <<
	TK_GROUP_OPEN,   // (
	TK_GROUP_CLOSE,  // )
}								t_token_type;

typedef struct s_token
{
	t_token_type				type;
	char						*value;
}								t_token;

// Syntax analysis / Abstract Syntax Tree
typedef enum e_ast_type
{
	AST_COMMAND,
	AST_GROUP,
	AST_LOGICAL,
	AST_PIPELINE,
	AST_REDIRECTION,
	AST_SYNTAX_ERROR
}								t_ast_type;

// rename cmd_exe to argv or args ?
typedef struct s_ast_command
{
	char						**cmd_exe;
}								t_ast_command;

typedef struct s_ast_pipeline
{
	struct s_ast				*left;
	struct s_ast				*right;
}								t_ast_pipeline;

typedef struct s_ast_logical
{
	t_token_type				operator;
	struct s_ast				*left;
	struct s_ast				*right;
}								t_ast_logical;

typedef struct s_ast_redirection
{
	t_token_type				direction;
	// TK_REDIRECT_IN, TK_REDIRECT_OUT, TK_APPEND_OUT,TK_HEREDOC
	struct s_ast				*child;
	char						*file;
}								t_ast_redirection;

typedef struct s_ast_group
{
	struct s_ast				*child;
}								t_ast_group;

typedef struct s_ast_syntax_error
{
	char						*unexpected_token;
}								t_ast_syntax_error;

typedef struct s_ast
{
	t_ast_type					type;
	union
	{
		t_ast_command			command;
		t_ast_pipeline			pipeline;
		t_ast_logical			logical;
		t_ast_redirection		redirection;
		t_ast_group				group;
		t_ast_syntax_error		s_error;
	} data;
}								t_ast;

typedef int						(*builtin_func)(t_ast_command *, t_shell *);

typedef struct s_builtin
{
	char						*name;
	builtin_func				func;
}								t_builtin;