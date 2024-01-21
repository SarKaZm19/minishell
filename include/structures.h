
#include "libraries.h"

extern volatile sig_atomic_t	g_signal_value;

#define SCOPES 3
typedef enum t_tracking_scope
{
	ONLY_CHECK,
	SH,
	PROMPT
}								t_tracking_scope;

typedef enum t_execute_end
{
	O_RETURN,
	O_EXIT
}								t_execute_end;

// Main structure ?
typedef struct s_shell
{
	t_list						*env;
	char						**env_paths;

	t_list						*allocated_pointers[SCOPES];
	t_list						*temporary_files;
	// allocated_pointers are freed after each prompt execution
	char						*parsing_error;
	bool						in_main_process;
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
	char						*cmd;
	char						*cmd_part;
	char						*var_to_sub;
	char						*subbed_var;
	char						*new_cmd;
	int							cmd_len;
	int							cmd_part_len;
	int							var_to_sub_len;
	int							subbed_var_len;
	int							split_parts;
	int							nb_cmds;
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
	struct s_AST				*left;
	struct s_AST				*right;
}								t_ast_pipeline;

typedef struct s_ast_logical
{
	t_token_type				operator;
	struct s_AST				*left;
	struct s_AST				*right;
}								t_ast_logical;

typedef struct s_ast_redirection
{
	t_token_type				direction;
	// TK_REDIRECT_IN, TK_REDIRECT_OUT, TK_APPEND_OUT,TK_HEREDOC
	struct s_AST				*child;
	char						*file;
}								t_ast_redirection;

typedef struct s_ast_group
{
	struct s_AST				*child;
}								t_ast_group;

typedef struct s_ast_syntax_error
{
	char						*unexpected_token;
}								t_ast_syntax_error;

typedef struct s_AST
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
}								t_AST;

typedef int						(*builtin_func)(t_ast_command *, t_shell *);

typedef struct s_builtin
{
	char						*name;
	builtin_func				func;
}								t_builtin;