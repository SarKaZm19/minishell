# *The Outstanding Minishell*

## TODO FLORENT
- expand() function
- set custom test for the expander?
- wildcard * for current directory
- Fix Makefile to not execute "mkdir -p" each time a .c is substituted by a .o (Makefile:56)
- Do hard tests for the lexer
- Resource a lire et check si tout est implémenté : https://pubs.opengroup.org/onlinepubs/009695399/utilities/xcu_chap02.html#tag_02_10
- test `ls -l | grep "^d" | awk '{print $NF}' | xargs -I{} sh -c 'echo "Files in {}: "; ls {}'` fix infinite loop

#### Done Florent
- lexer()

---

## TODO ULYSSE
- fix test with redirections + group: `(cat) < infile` cat open a prompt instead of reading the file
- fix status new errors : link to redirection + group issue 
- remove test with selected fd as `cat 2> out`
---
- use PS1 for prompt, and PS2 for heredoc
- implement `$SHLVL`
- check how the "is directory" error is handled or not
- check bug : cf screenshot
- `env_builtin()`, `export_builtin()` and `unset_builtin()` : detect errors
- review  2.8.1 Consequences of Shell Errors : https://pubs.opengroup.org/onlinepubs/009604499/utilities/xcu_chap02.html#tag_02_08_01
- history : `rl_clear_history()` before exiting and check that empty input as space characters are not saved (https://42born2code.slack.com/archives/CN9RHKQHW/p1654165861713979)
- heredoc still display `^D`, understand why and how to disable it
- use wait3, wait4 ?
- save history? 
  - https://www.gnu.org/software/bash/manual/bash.html#Bash-History-Facilities
  - https://www.gnu.org/software/bash/manual/bash.html#Using-History-Interactively
- What if here is no tmp/ directory for the heredoc? test it
- more facade for cleaner code: 
  - `s_alloc(ft_calloc(i + 2, sizeof(char *)), PROMPT, sh)` -> `alloc_s(i + 2, sizeof(char *), PROMPT, sh)`;
  - `s_alloc(ft_lstnew(var), ONLY_CHECK, sh)` -> `lstnew_s(var, ONLY_CHECK, sh)`;
    + unprotected `ft_lstnew`
    + OU `lst_add_new_back()` ?
  - `s_alloc(ft_split(path, ':'), PROMPT, sh);` -> `split_s(path, ':', PROMPT, sh);`
- `_s` in postfix notation

#### Done Ulysse
- export: manage export but unset variable, added value
- create a cache system for path resolution in execve
- update execve to use dynamic env variable
- execution of multiple pipeline is very slow: I think is due to useless refork (one for pipeline, one for each command). Check by conting the fork number in other project and here. If it is the case, fix it.
- check stackoverflow-teams about minishell
- `env_builtin()`, `export_builtin()` and `unset_builtin()` : implement v1
- the /lib/ still compile with obj/ directory, rename it build/
- setup tester: mpanic
- track_temporary_file
- signal handling: `ctrl-C`, `ctrl-D`, `ctrl-\`
- HEREDOC: signal, heredocs should be done before any execution
- fix bugs: in_pipeline flag setup + get end of redirection list
- print "exit" whn the main shell is exited
- `exit_builtin()` : implement, robust to long long arg and over
- `echo_builtin()`
- check `/ms_compare/leaks-labs/src/exec/wait.c` and `errno != ECHILD`
- Memory: 0 leaks strategy + import allocation handling from pipex + split allocation for whole minishell and for each command
- Error handling: import from pipex
- `parser()`: handle syntax/parsing errors
- review all slack thread about minishell
- `parser()`: understand the EBNF-like grammar notation + write custom Minishell Grammar
- `parser()`: parse_redirection() -> pre/post-fix
- `parser()`: in postfix, redirections and argument can be mixed
- `executor()`: command grouping: `()`
- `executor()`: redirection: `<`, `>`, `<<`, `>>` #PIPEX
- `executor()`: pipeline: `|` #PIPEX
- `executor()`: Executing Commands (`fork()` and `execve()`) #PIPEX
- `executor()`: logical operators: `&&` and `||`
- review all external functions
- read https://www.gnu.org/software/bash/manual/html_node/Interactive-Shells.html

---

## To Discuss
- Redirection errors encountered by shell builtins will not cause the shell to exit. 
  For now, open() error are not fatal, but dup2() or close() error are fatal.
- do we use `ft_strtrim` to trim quotes and spaces in the lexer or in the expander?

## Future (in logical order)
- check this about ~ expand: https://stackoverflowteams.com/c/42network/questions/466
- add readline lib in /lib/ folder and modify the Makefile as: https://github.com/ChewyToast/03_minishell/blob/main/Makefile : TAKE IT FROM HERE ?: https://git.savannah.gnu.org/cgit/bash.git/tree/lib OR https://github.com/bminor/bash/tree/4b0f8ba28449c60d6b3436d0c98b14e016b232c2/lib/readline
- heredoc expansion: check if has to be done at heredoc reading or just before execution + implement it
- `pwd_builtin()` and `cd_builtin()`
- use IFS env variable to split input ?
- Makefile: check this one: [minishell/Makefile at main · leaks-labs/minishell](https://github.com/leaks-labs/minishell/blob/main/Makefile)
- secure all ft_strjoin, and other syscall function if needed
- for each syscall facade, rename with de s_ as postfix instead of prefix, for readability.
- security: add const arg for char* if it is (as argv).
- security: make sub-function static ? to discuss

---

## Done milestones
- Project structure
- Compilation with libraries
- Basic Shell Loop: display prompt + read input from the user
- History: add_history(user_input).
- lexer(): split input into tokens
- parser(): parse tokens into ast