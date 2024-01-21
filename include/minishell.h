
#include "structures.h"

// ----- CORE ----- //
int				main(int argc, char **argv, char **envp);
// Init shell and env
void			init_shell(t_shell *sh, char **env);
// Prompt
int				interactive_shell(t_shell *sh);
int				prompt_execution(char *input, t_shell *sh);
void			clear_prompt(t_shell *sh);
// General utils
void			add_arg_to_array(char ***char_array, char *new_arg,
					t_shell *sh);

// ----- SIGNAL ----- //
// Set signal
void			set_signal_prompt(void);
void			set_signal_main_process(void);
void			set_signal_child_process(void);
void			set_signal_heredoc(void);
void			set_signal_handler(int signum, void (*handler)(int));
// Handle signal
void			sigint_prompt(int signum);
void			sigint_heredoc(int signum);

// ----- LEXER ----- //
int				lexer(char *input, t_list **token_list, t_shell *sh);
t_token			*create_token(t_token_type type, char *value, size_t len,
					t_shell *sh);
// Lexer: Scan next token
t_token_type	scan_next_token(char *word, size_t *len, t_shell *sh);
t_token_type	scan_tk_group(char *word, size_t *len);
t_token_type	scan_tk_logic_and_pipe(char *word, char c, size_t *len,
					t_shell *sh);
t_token_type	scan_tk_redirect(char *word, char c, size_t *len, t_shell *sh);
t_token_type	scan_tk_word(char *word, size_t *len, t_shell *sh);
// Lexer: Utils
// int					check_lexer_error(char *word, int c, size_t i);
int				is_space(int c);
int				is_token(int c);

// ----- PARSER ----- //
int				parser(t_list *token_list, t_AST **ast, t_shell *sh);
// Syntax Three
t_AST			*parse_logical(t_list **token_list, t_shell *sh);
t_AST			*parse_pipeline(t_list **token_list, t_shell *sh);
t_AST			*parse_group(t_list **token_list, t_shell *sh);
t_AST			*parse_command(t_list **token_list, t_shell *sh);
// Parse redirection
t_AST			*parse_redirection(t_list **token_list, t_shell *sh);
t_AST			*parse_redirection_list(t_list **token_list, t_AST *command,
					t_shell *sh);
t_AST			*build_redirected_command(t_AST *prefix, t_AST *suffix,
					t_AST *child);
// Parse redirection utils
t_AST			*get_last_of_redirection_list(t_AST *node);
// Create AST Nodes
void			*set_syntax_error(char *unexpected_token, t_shell *sh);
void			init_ast_node(t_AST **node, t_ast_type type, t_shell *sh);
t_AST			*create_ast_command(char **argv, t_shell *sh);
t_AST			*create_ast_group(t_AST *child, t_shell *sh);
t_AST	*create_ast_redirection(t_token_type direction,
								t_list *tk_filename,
								t_AST *child,
								t_shell *sh);
t_AST			*create_ast_pipeline(t_AST *left, t_AST *right, t_shell *sh);
t_AST			*create_ast_logical(t_AST *left, t_token_type operator, t_AST
					* right, t_shell *sh);

// utils_token.c
t_token_type	tk_type(t_list *token);
char			*tk_value(t_list *token);
bool			is_tk_type(t_list **token_list, int num_args, ...);
char			*tk_type_to_string(t_token_type type);

// ----- EXPANSION ----- //
// ----- Expander.c ---- //
t_AST			*expander(t_AST *node, t_shell *sh);
char			**expand_arg(char *str, t_shell *sh, int *current_size);
char			*expand_var(t_expander *exp, int *tmp_i);
// ----- exp_struct.c ----- //
void			init_expander(t_expander *exp, char *cmd);
void			reset_exp(t_expander *exp);
// ----- var_substitution.c ---- //
char			*get_var(char *tmp, int *tmp_i);
char			*get_var_sub(char *var, int *tmp_i);
// ----- expander_case.c ----- //
char			*treat_no_quotes(t_expander *exp, t_shell *sh, int *start_i);
char			*treat_d_quotes(t_expander *exp, t_shell *sh, int *start_i);
char			*treat_s_quotes(t_expander *exp, t_shell *sh, int *start_i);
char			*expand_var_in_d_quotes(t_expander *exp, int *start_i);
// ----- cmd_tab_expansion.c ----- //
char			**word_split_expander(t_expander *exp, char **cmd_tab,
					t_shell *sh);
char			**add_cmds(char **cmd_tab, int cmd_tab_size, char **cmd_to_add,
					int new_tab_size, t_shell *sh);
char			**split_cmds(t_expander *exp);
char			**cmd_to_tab(t_expander *exp, char **cmd_tab);

// ----- EXECUTION ----- //
int				execute(t_AST *node, t_execute_end end, t_shell *sh);
int				check_process_child_exit(int status, bool *new_line,
					t_shell *sh);

// Execute Group
int				execute_group(t_ast_group *group, t_shell *sh);

// Execute Logical
int				execute_logical(t_ast_logical *logical, t_shell *sh);

// Execute Redirection
int	execute_redirection(t_ast_redirection *redirection,
						t_shell *sh);
int				redirect_input(t_ast_redirection *redir, t_shell *sh);
int				redirect_output(t_ast_redirection *redir, t_shell *sh);
int				append_output(t_ast_redirection *redir, t_shell *sh);
// Execute Redirection: Heredoc
int				execute_heredocs(t_AST *node, t_shell *sh);
int				search_for_heredocs(t_AST *node, t_shell *sh);
int				heredoc_listener(t_ast_redirection *redir, char *tmp_file_name,
					t_shell *sh);
void			heredoc_warming_message(char *delimiter, t_shell *sh);

// Execute Pipeline
int				execute_pipeline(t_AST *node, t_shell *sh);
t_list			*build_cmd_list(t_AST *node, t_shell *sh);
int				execute_pipeline_list(t_list *cmd_list, t_shell *sh);
pid_t			execute_pipeline_command(t_list *cmd_list, t_shell *sh,
					int prev_read_end, int p[2]);
void			setup_for_next_command(int *prev_read_end, int p[2],
					t_shell *sh);
int				wait_for_children(t_shell *sh, pid_t last_pid, int n_cmd);

// Execute Command
int				execute_command(t_ast_command *cmd, t_shell *sh);
// Select the builtin function
builtin_func	get_builtin_function(char *cmd_name);
// Seach binary path for execution
char			*get_bin_path(char *bin, t_shell *sh);
char			**get_env_paths(t_shell *sh);
char			*find_path(char *bin, t_shell *sh);
char			*join_path(char *dir, char *file, t_shell *sh);
bool			check_access(char *bin, t_shell *sh);

// ----- BUILTINS ----- //
// Echo
int				echo_builtin(t_ast_command *cmd, t_shell *sh);
bool			option_check(char **args, int *i, char c);
//
int				cd_builtin(t_ast_command *cmd, t_shell *sh);
int				pwd_builtin(t_ast_command *cmd, t_shell *sh);
// Export
int				export_builtin(t_ast_command *cmd, t_shell *sh);
void			export_one(char *assignment, bool *name_error, t_shell *sh);
bool			is_valid_name(char *name);
// Unset
int				unset_builtin(t_ast_command *cmd, t_shell *sh);
t_list			*get_env_var(char *name, t_list *env);
void			remove_env_var(t_list *env, t_shell *sh);
void			free_env_var(void *env);
// Env
int				env_builtin(t_ast_command *cmd, t_shell *sh);
// Exit
int				exit_builtin(t_ast_command *cmd, t_shell *sh);
void			quit_shell(int exit_status, t_shell *sh);
int				count_args(char **argv);
bool			is_valid_64bit_signed_integer(const char *str);
long long		ft_atoi_long_long(const char *str);

// ----- ENVIRONMENT ----- //
// Init Environment
t_env_var		*init_env_var(char *name, char *value, t_shell *sh);
t_list			*init_env(char **env, t_shell *sh);
// Distribute Environment
int				print_env(bool export_format, t_list *env, t_shell *sh);
char			**env_to_char_array(t_shell *sh);
// Environment Utilities
char			*env_name(t_list *env);
char			*env_value(t_list *env);

// ----- UTILS ----- //

// Error (pipex import)
void			error(const char *context, char *description, int exit_status,
					t_shell *sh);
void			quit_shell(int exit_status, t_shell *sh);
void			syscall_error(const char *context, int errnum, t_shell *sh);
int				report_errno(char *context, t_shell *sh);
int				report_syntax_error(t_shell *sh);
ssize_t			s_write_fd(const char *s, int fd, t_shell *sh);

// Allocation
void			*s_alloc(void *pointer, t_tracking_scope scope, t_shell *sh);
void			track_alloc(void *pointer, t_tracking_scope scope, t_shell *sh);
void			lst_add_and_track(void *content, t_list **lst,
					t_tracking_scope scope, t_shell *sh);
void			free_data(t_shell *sh);

// Facade for system call
// Syscall: File descriptor
int				s_open(const char *pathname, int flags, mode_t mode,
					t_shell *sh);
int				s_close(int fd, t_shell *sh);
int				s_dup2(int oldfd, int newfd, t_shell *sh);
int				s_unlink(const char *pathname, t_shell *sh);
// Syscall: Process
int				s_execve(const char *pathname, char *const argv[],
					char *const envp[], t_shell *sh);
pid_t			s_fork(t_shell *sh);
int				s_pipe(int pipefd[2], t_shell *sh);
pid_t			s_wait(int *wstatus, t_shell *sh);

// ----- DEBUG ----- //
void			print_tokens(t_list *tokens);
void			print_ast(t_AST *ast);
void			write_ast_to_file(t_AST *root, const char *filename);
const char		*token_type_to_string(t_token_type type);