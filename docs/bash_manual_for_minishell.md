# Bash Reference Manual for Minishell
a.k.a. the subject details that 42 doesn't want to give you. </br>
This documents contains the [bash documentation](https://www.gnu.org/software/bash/) that is used in the minishell project with bonuses, only the relevant parts are kept here. </br>
It also contains the [IEEE Std page about env](https://pubs.opengroup.org/onlinepubs/9699919799/utilities/env.html). As `env` is not mentioned in the bash documentation as a builtin, I take POSIX as a reference. </br>

## 2 Definitions

control operator
    A token that performs a control function. It is a newline or one of the following: ‘||’, ‘&&’, ‘&’, ‘;’, ‘;;’, ‘;&’, ‘;;&’, ‘|’, ‘|&’, ‘(’, or ‘)’.

token
    A sequence of characters considered a single unit by the shell. It is either a word or an operator

word
    A sequence of characters treated as a unit by the shell. Words may not include unquoted metacharacters. 

metacharacter
    A character that, when unquoted, separates words. A metacharacter is a space, tab, newline, or one of the following characters: ‘|’, ‘&’, ‘;’, ‘(’, ‘)’, ‘<’, or ‘>’.

builtin
    A command that is implemented internally by the shell itself, rather than by an executable program somewhere in the file system.

name <!-- matter for the export builtin -->
    A word consisting solely of letters, numbers, and underscores, and beginning with a letter or underscore. Names are used as shell variable and function names. Also referred to as an identifier.

signal
    A mechanism by which a process may be notified by the kernel of an event occurring in the system.

exit status
    The value returned by a command to its caller. The value is restricted to eight bits, so the maximum value is 255.

return status
    A synonym for exit status.


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
The output of each command in the pipeline is connected via a pipe to the input of the next command. That is, each command reads the previous command's output. This connection is performed before any redirections specified by `command1`.
The shell waits for all commands in the pipeline to complete. 
Each command in a multi-command pipeline, where pipes are created, is executed in its own subshell, which is a separate process (see Command Execution Environment).
The exit status of a pipeline is the exit status of the last command in the pipeline.

## 3.5 Shell Expansions
https://www.gnu.org/software/bash/manual/html_node/Shell-Expansions.html

## 3.6 Redirections
Before a command is executed, its input and output may be redirected using a special notation interpreted by the shell. Redirection allows commands` file handles to be duplicated, opened, closed, made to refer to different files, and can change the files the command reads from and writes to. Redirection may also be used to modify file handles in the current shell execution environment. The following redirection operators may precede or appear anywhere within a simple command or may follow a command. Redirections are processed in the order they appear, from left to right.

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

### 3.6.6 Here Documents
This type of redirection instructs the shell to read input from the current source until a line containing only word (with no trailing blanks) is seen. All of the lines read up to that point are then used as the standard input for a command.

The format of here-documents is:
```
<<[-]word
        here-document
delimiter
```
No parameter and variable expansion or filename expansion is performed on word. If any part of word is quoted, the delimiter is the result of quote removal on word, and the lines in the here-document are not expanded. If word is unquoted, all lines of the here-document are subjected to parameter expansion, the character sequence \newline is ignored, and `\` must be used to quote the characters `\`, `$`, and ```.

## 3.7 Executing Commands
### 3.7.1 Simple Command Expansion
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
3. If the name is not a builtin, and contains no slashes, Bash searches each element of $PATH for a directory containing an executable file by that name. Bash uses a hash table to remember the full pathnames of executable files to avoid multiple PATH searches (see the description of hash in Bourne Shell Builtins). A full search of the directories in $PATH is performed only if the command is not found in the hash table. If the search is unsuccessful, the shell prints an error message and returns an exit status of 127.
4. If the search is successful, or if the command name contains one or more slashes, the shell executes the named program in a separate execution environment. Argument 0 is set to the name given, and the remaining arguments to the command are set to the arguments supplied, if any.
5. If this execution fails because the file is not in executable format, and the file is not a directory, it is assumed to be a shell script and the shell executes it as described in Shell Scripts.
6. The shell waits for the command to complete and collects its exit status. 

### 3.7.3 Command Execution Environment 
The shell has an execution environment, which consists of the following:
- open files inherited by the shell at invocation, as modified by redirections supplied to the exec builtin
- the current working directory as set by cd or inherited by the shell at invocation
- the file creation mode mask inherited from the shell`s parent
- shell parameters inherited from the shell`s parent in the environment
- shell functions defined during execution or inherited from the shell`s parent in the environment
- various process IDs, including those of background jobs (see Lists of Commands), the value of `$$`, and the value of `$PPID` <!-- not for Minishell ? -->

When a simple command other than a builtin is to be executed, it is invoked in a separate execution environment that consists of the following. Unless otherwise noted, the values are inherited from the shell.
- the shell`s open files, plus any modifications and additions specified by redirections to the command
- the current working directory
- the file creation mode mask <!-- not for Minishell, as always the same, as no unmask() builtin ? -->
- shell variables and functions marked for export, along with variables exported for the command, passed in the environment (see Environment)

A command invoked in this separate environment cannot affect the shell's execution environment.
A subshell is a copy of the shell process.
Builtin commands that are invoked as part of a pipeline are executed in a subshell environment. Changes made to the subshell environment cannot affect the shell`s execution environment.

### 3.7.4 Environment

When a program is invoked it is given an array of strings called the environment. This is a list of name-value pairs, of the form name=value.

Bash provides several ways to manipulate the environment. On invocation, the shell scans its own environment and creates a parameter for each name found, automatically marking it for export to child processes. Executed commands inherit the environment. The export command allow parameters and functions to be added to the environment. If the value of a parameter in the environment is modified, the new value becomes part of the environment, replacing the old. The environment inherited by any executed command consists of the shell’s initial environment, whose values may be modified in the shell, less any pairs removed by the unset command, plus any additions via the export command.

When Bash invokes an external command, the variable ‘$_’ is set to the full pathname of the command and passed to that command in its environment. 

### 3.7.5 Exit Status
The exit status of an executed command is the value returned by the waitpid system call or equivalent function. Exit statuses fall between 0 and 255, though, as explained below, the shell may use values above 125 specially. Exit statuses from shell builtins and compound commands are also limited to this range. Under certain circumstances, the shell will use special values to indicate specific failure modes.

For the shell`s purposes, a command which exits with a zero exit status has succeeded. A non-zero exit status indicates failure. This seemingly counter-intuitive scheme is used so there is one well-defined way to indicate success and a variety of ways to indicate various failure modes. When a command terminates on a fatal signal whose number is N, Bash uses the value 128+N as the exit status.

If a command is not found, the child process created to execute it returns a status of 127. If a command is found but is not executable, the return status is 126.

If a command fails because of an error during expansion or redirection, the exit status is greater than zero.

The exit status is used by the Bash conditional commands (see Conditional Constructs).

All of the Bash builtins return an exit status of zero if they succeed and a non-zero status on failure, so they may be used by the conditional and list constructs. All builtins return an exit status of 2 to indicate incorrect usage, generally invalid options or missing arguments.

The exit status of the last command is available in the special parameter $? (see Special Parameters).  <!-- remove ? -->

### 3.7.6 Signals
Also check: https://www.gnu.org/software/libc/manual/html_node/Termination-Signals.html

When Bash is interactive, in the absence of any traps, it ignores SIGTERM (so that `kill 0` does not kill an interactive shell), and SIGINT is caught and handled. When Bash receives a SIGINT, it breaks out of any executing loops. In all cases, Bash ignores SIGQUIT.

Non-builtin commands started by Bash have signal handlers set to the values inherited by the shell from its parent.

When Bash is running without job control enabled and receives SIGINT while waiting for a foreground command, it waits until that foreground command terminates and then decides what to do about the SIGINT:

<!-- todo: this section is not fully cleared for minishell, cf remove comments -->
When Bash is running and receives SIGINT while waiting for a foreground command, it waits until that foreground command terminates and then decides what to do about the SIGINT:
- If the command terminates due to the SIGINT, Bash concludes that the user meant to end the entire script, and acts on the SIGINT (e.g., by running a SIGINT trap or exiting itself);
- If the pipeline does not terminate due to SIGINT, the program handled the SIGINT itself and did not treat it as a fatal signal. In that case, Bash does not treat SIGINT as a fatal signal, either, instead assuming that the SIGINT was used as part of the program`s normal operation (e.g., emacs uses it to abort editing commands) or deliberately discarded. However, Bash will run any trap set on SIGINT, as it does with any other trapped signal it receives while it is waiting for the foreground command to complete, for compatibility.


## 4.1 Bourne Shell Builtins
The following shell builtin commands are inherited from the Bourne Shell. These commands are implemented as specified by the POSIX standard. 
<!-- NB: order has been changed for convenience -->
### exit
`exit [n]`
Exit the shell, returning a status of n to the shell’s parent. If n is omitted, the exit status is that of the last command executed. Any trap on EXIT is executed before the shell terminates.

### pwd
Print the absolute pathname of the current working directory. The return status is zero unless an error is encountered while determining the name of the current directory or an invalid option is supplied.

### cd
Change the current working directory to directory. If directory is not supplied, the value of the HOME shell variable is used. If the shell variable CDPATH exists, it is used as a search path: each directory name in CDPATH is searched for directory, with alternative directory names in CDPATH separated by a colon (‘:’). If directory begins with a slash, CDPATH is not used.

By default, symbolic links in directory are resolved after cd processes an instance of ‘..’ in directory.

If ‘..’ appears in directory, it is processed by removing the immediately preceding pathname component, back to a slash or the beginning of directory.

If directory is ‘-’, it is converted to $OLDPWD before the directory change is attempted.

If a non-empty directory name from CDPATH is used, or if ‘-’ is the first argument, and the directory change is successful, the absolute pathname of the new working directory is written to the standard output.

If the directory change is successful, cd sets the value of the PWD environment variable to the new directory name, and sets the OLDPWD environment variable to the value of the current working directory before the change.

The return status is zero if the directory is successfully changed, non-zero otherwise. 

### export
Mark each name to be passed to child processes in the environment. The names refer to shell variables. If no names are supplied, a list of names of all exported variables is displayed. If a variable name is followed by =value, the value of the variable is set to value.
The return status is zero unless an invalid option is supplied, one of the names is not a valid shell variable name.

### unset
Remove each variable. Each name refers to a variable; if there is no variable by that name is unset. Readonly variables may not be unset. Some shell variables lose their special behavior if they are unset; such behavior is noted in the description of the individual variables. The return status is zero unless a name is readonly or may not be unset. 

---

### IEEE Std 1003.1-2017: env
DESCRIPTION
The environment shall be written to the standard output, with one name= value pair per line.

STDOUT
Each name=value pair in the resulting environment shall be written in the form: `"%s=%s\n", <name>, <value>`

---

## 4.2 Bash Builtin Commands

### echo
`echo [-n] [arg …]`
Output the args, separated by spaces, terminated with a newline. The return status is 0 unless a write error occurs. If -n is specified, the trailing newline is suppressed. echo does not interpret -- to mean the end of options. 

<!-- TODO: TO CLEAN UNDER THIS LINE -->
## 5.1 Bourne Shell Variables <!-- TODO ? -->
Bash uses certain shell variables in the same way as the Bourne shell. In some cases, Bash assigns a default value to the variable. 

### HOME
The current user’s home directory; the default for the cd builtin command. The value of this variable is also used by tilde expansion (see Tilde Expansion).

### IFS
A list of characters that separate fields; used when the shell splits words as part of expansion.

### PATH
A colon-separated list of directories in which the shell looks for commands. A zero-length (null) directory name in the value of PATH indicates the current directory. A null directory name may appear as two adjacent colons, or as an initial or trailing colon.

### PS1
The primary prompt string. The default value is ‘\s-\v\$ ’. See Controlling the Prompt, for the complete list of escape sequences that are expanded before PS1 is displayed.

### PS2
The secondary prompt string. The default value is ‘> ’. PS2 is expanded in the same way as PS1 before being displayed.


## 5.2 Bash Variables <!-- TODO ? -->

### BASH
The full pathname used to execute the current instance of Bash.

### BASH_SUBSHELL
Incremented by one within each subshell or subshell environment when the shell begins executing in that environment. The initial value is 0. If BASH_SUBSHELL is unset, it loses its special properties, even if it is subsequently reset.

### HISTFILE, HISTFILESIZE, HISTIGNORE, HISTSIZE

### PWD
The current working directory as set by the cd builtin.

### PPID
The process ID of the shell’s parent process. This variable is readonly.

### SHLVL
Incremented by one each time a new instance of Bash is started. This is intended to be a count of how deeply your Bash shells are nested.


# 6 Bash Features
## 6.3 Interactive Shells
### 6.3.3 Interactive Shell Behavior

When the shell is running interactively, it changes its behavior in several ways.

1. Startup files are read and executed as described in Bash Startup Files.
3. Bash expands and displays PS1 before reading the first line of a command, and expands and displays PS2 before reading the second and subsequent lines of a multi-line command. Bash expands and displays PS0 after it reads a command but before executing it. See Controlling the Prompt, for a complete list of prompt string escape sequences. <!-- TODO ? -->
5. Readline (see Command Line Editing) is used to read commands from the user’s terminal. 
7.  Command history (see Bash History Facilities) and history expansion (see History Expansion) are enabled by default. Bash will save the command history to the file named by $HISTFILE when a shell with history enabled exits. <!-- TODO ? -->
10.  In the absence of any traps, SIGINT is caught and handled (see Signals). SIGINT will interrupt some shell builtins. 
15. The shell will not exit on expansion errors caused by var being unset or null in ${var:?word} expansions (see Shell Parameter Expansion). <!-- TODO: CHECK WHEN EXPANDER IS IMPLEMENTED -->
16. Redirection errors encountered by shell builtins will not cause the shell to exit. 
18. A failed exec will not cause the shell to exit (see Bourne Shell Builtins). 
19. Parser syntax errors will not cause the shell to exit. 

