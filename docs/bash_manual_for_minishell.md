# Bash Reference Manual for Minishell
This documents contains the [bash documentation](https://www.gnu.org/software/bash/) that is used in the minishell project with bonuses, only the relevant parts are kept here. </br> </br>

## 3 Basic Shell Features
### 3.2 Shell Commands
#### [3.2.3 Grouping Commands](https://www.gnu.org/software/bash/manual/html_node/Command-Grouping.html)
When commands are grouped, redirections may be applied to the entire command list. For example, the output of all the commands in the list may be redirected to a single stream. 
Format: `( list )`
Placing a list of commands between parentheses forces the shell to create a subshell (see Command Execution Environment), and each of the commands in list is executed in that subshell environment. Since the list is executed in a subshell, variable assignments do not remain in effect after the subshell completes. 
*Note for Minishell:* subshell for group are not asked by the subject. However, subshell are used for executing commands and builtins in pipelines. </br>
The exit status is the exit status of list. 

#### [3.2.3 Pipelines](https://www.gnu.org/software/bash/manual/html_node/Command-Grouping.html)
A pipeline is a sequence of one or more commands separated by the control operators `|`.
</br> The format for a pipeline is: `command1 [ | command2 ] …`. </br>
The output of each command in the pipeline is connected via a pipe to the input of the next command. That is, each command reads the previous command’s output. This connection is performed before any redirections specified by `command1`.
The shell waits for all commands in the pipeline to complete. 
Each command in a multi-command pipeline, where pipes are created, is executed in its own subshell, which is a separate process (see Command Execution Environment).
The exit status of a pipeline is the exit status of the last command in the pipeline.

## 3.5 Shell Expansions
https://www.gnu.org/software/bash/manual/html_node/Shell-Expansions.html

## 3.6 Redirections
Before a command is executed, its input and output may be redirected using a special notation interpreted by the shell. Redirection allows commands’ file handles to be duplicated, opened, closed, made to refer to different files, and can change the files the command reads from and writes to. Redirection may also be used to modify file handles in the current shell execution environment. The following redirection operators may precede or appear anywhere within a simple command or may follow a command. Redirections are processed in the order they appear, from left to right.

If the first character of the redirection operator is `<`, the redirection refers to the standard input (file descriptor 0). If the first character of the redirection operator is `>`, the redirection refers to the standard output (file descriptor 1).

The word following the redirection operator in the following descriptions, unless otherwise noted, is subjected parameter expansion, quote removal, filename expansion, and word splitting. If it expands to more than one word, Bash reports an error.

Note that the order of redirections is significant. For example, the command
```bash
ls > dirlist 2>&1
```
directs both standard output (file descriptor 1) and standard error (file descriptor 2) to the file dirlist, while the command directs only the standard output to file dirlist, because the standard error was made a copy of the standard output before the standard output was redirected to dirlist.

```bash
ls 2>&1 > dirlist
```

Bash handles several filenames specially when they are used in redirections, as described in the following table. 
A failure to open or create a file causes the redirection to fail.
Redirections using file descriptors greater than 9 should be used with care, as they may conflict with file descriptors the shell uses internally. 

## 3.7 Executing Commands
#### 3.7.1 Simple Command Expansion
When a simple command is executed, the shell performs the following expansions and redirections, from left to right, in the following order.

1. The words that the parser has marked as redirections are saved for later processing.
2. The words that are not redirections are expanded (see Shell Expansions). If any words remain after expansion, the first word is taken to be the name of the command and the remaining words are the arguments.
3. Redirections are performed as described above (see Redirections).

If no command name results, redirections are performed, but do not affect the current shell environment. A redirection error causes the command to exit with a non-zero status.
If there is a command name left after expansion, execution proceeds as described below. Otherwise, the command exits.

### 3.7.2 Command Search and Execution
After a command has been split into words, if it results in a simple command and an optional list of arguments, the following actions are taken.

1. If the command name contains no slashes, the shell attempts to locate it.
2. The shell searches for the name in the list of shell builtins. If a match is found, that builtin is invoked.
3. If the name is not a builtin, and contains no slashes, Bash searches each element of $PATH for a directory containing an executable file by that name. Bash uses a hash table to remember the full pathnames of executable files to avoid multiple PATH searches (see the description of hash in Bourne Shell Builtins). A full search of the directories in $PATH is performed only if the command is not found in the hash table. If the search is unsuccessful, the shell searches for a defined shell function named command_not_found_handle. If that function exists, it is invoked in a separate execution environment with the original command and the original command’s arguments as its arguments, and the function’s exit status becomes the exit status of that subshell. If that function is not defined, the shell prints an error message and returns an exit status of 127.
4. If the search is successful, or if the command name contains one or more slashes, the shell executes the named program in a separate execution environment. Argument 0 is set to the name given, and the remaining arguments to the command are set to the arguments supplied, if any.
5. If this execution fails because the file is not in executable format, and the file is not a directory, it is assumed to be a shell script and the shell executes it as described in Shell Scripts.
6. The shell waits for the command to complete and collects its exit status. 

### 3.7.3 Command Execution Environment 
The shell has an execution environment, which consists of the following:

- open files inherited by the shell at invocation, as modified by redirections supplied to the exec builtin
- the current working directory as set by cd or inherited by the shell at invocation
- the file creation mode mask inherited from the shell’s parent
- shell parameters that are set by variable assignment or with `set` or inherited from the shell’s parent in the environment
- shell functions defined during execution or inherited from the shell’s parent in the environment
- options enabled at invocation (either by default or with command-line arguments) or by `set` <!-- clarify -->
- shell aliases defined with alias (see Aliases) <!-- not for Minishell ? -->
- various process IDs, including those of background jobs (see Lists of Commands), the value of `$$`, and the value of `$PPID` <!-- not for Minishell ? -->

When a simple command other than a builtin is to be executed, it is invoked in a separate execution environment that consists of the following. Unless otherwise noted, the values are inherited from the shell.

- the shell’s open files, plus any modifications and additions specified by redirections to the command
- the current working directory
- the file creation mode mask <!-- not for Minishell, as always the same, as no unmask() builtin ? -->
- shell variables and functions marked for export, along with variables exported for the command, passed in the environment (see Environment)

A command invoked in this separate environment cannot affect the shell’s execution environment.

A subshell is a copy of the shell process.

Builtin commands that are invoked as part of a pipeline are executed in a subshell environment. Changes made to the subshell environment cannot affect the shell’s execution environment.

### 3.7.4 Environment
When a program is invoked it is given an array of strings called the environment. This is a list of name-value pairs, of the form name=value.

Bash provides several ways to manipulate the environment. On invocation, the shell scans its own environment and creates a parameter for each name found, automatically marking it for export to child processes. Executed commands inherit the environment. The export and ‘declare -x’ commands allow parameters and functions to be added to and deleted from the environment. If the value of a parameter in the environment is modified, the new value becomes part of the environment, replacing the old. The environment inherited by any executed command consists of the shell’s initial environment, whose values may be modified in the shell, less any pairs removed by the unset and ‘export -n’ commands, plus any additions via the export and ‘declare -x’ commands.

When Bash invokes an external command, the variable ‘$_’ is set to the full pathname of the command and passed to that command in its environment. 


### 3.7.5 Exit Status
The exit status of an executed command is the value returned by the waitpid system call or equivalent function. Exit statuses fall between 0 and 255, though, as explained below, the shell may use values above 125 specially. Exit statuses from shell builtins and compound commands are also limited to this range. Under certain circumstances, the shell will use special values to indicate specific failure modes.

For the shell’s purposes, a command which exits with a zero exit status has succeeded. A non-zero exit status indicates failure. This seemingly counter-intuitive scheme is used so there is one well-defined way to indicate success and a variety of ways to indicate various failure modes. When a command terminates on a fatal signal whose number is N, Bash uses the value 128+N as the exit status.

If a command is not found, the child process created to execute it returns a status of 127. If a command is found but is not executable, the return status is 126.

If a command fails because of an error during expansion or redirection, the exit status is greater than zero.

The exit status is used by the Bash conditional commands (see Conditional Constructs).

All of the Bash builtins return an exit status of zero if they succeed and a non-zero status on failure, so they may be used by the conditional and list constructs. All builtins return an exit status of 2 to indicate incorrect usage, generally invalid options or missing arguments.

The exit status of the last command is available in the special parameter $? (see Special Parameters).  <!-- remove ? -->

### 3.7.6 Signals
<!-- todo: this section is not fully cleared for minishell, cf remove comments -->
When Bash is interactive, it ignores SIGTERM (so that ‘kill 0’ does not kill an interactive shell), and SIGINT is caught and handled (so that the wait builtin is interruptible). When Bash receives a SIGINT, it breaks out of any executing loops. In all cases, Bash ignores SIGQUIT.

Non-builtin commands started by Bash have signal handlers set to the values inherited by the shell from its parent.

The shell exits by default upon receipt of a SIGHUP.

When Bash is waiting for a foreground command to complete, the shell receives keyboard-generated signals such as SIGINT (usually generated by ‘^C’) that users commonly intend to send to that command. This happens because the shell and the command are in the same process group as the terminal, and ‘^C’ sends SIGINT to all processes in that process group. See Job Control, for a more in-depth discussion of process groups.

When Bash receives SIGINT while waiting for a foreground command, it waits until that foreground command terminates and then decides what to do about the SIGINT:

1. If the command terminates due to the SIGINT, Bash concludes that the user meant to end the entire script, and acts on the SIGINT (e.g., by running a SIGINT trap or exiting itself);
2. If the pipeline does not terminate due to SIGINT, the program handled the SIGINT itself and did not treat it as a fatal signal. In that case, Bash does not treat SIGINT as a fatal signal, either, instead assuming that the SIGINT was used as part of the program’s normal operation (e.g., emacs uses it to abort editing commands) or deliberately discarded. However, Bash will run any trap set on SIGINT, as it does with any other trapped signal it receives while it is waiting for the foreground command to complete, for compatibility.  <!-- remove trap ? -->