# *The Outstanding Minishell*

### TODO FLORENT
- expand() function
- wildcard * for current directory
- Fix Makefile to not execute "mkdir -p" each time a .c is substituted by a .o (Makefile:56)
- Do hard tests for the lexer
- Resource a lire et check si tout est implémenté : https://pubs.opengroup.org/onlinepubs/009695399/utilities/xcu_chap02.html#tag_02_10
- test `ls -l | grep "^d" | awk '{print $NF}' | xargs -I{} sh -c 'echo "Files in {}: "; ls {}'` fix infinite loop

#### Done Florent
- lexer()

---
### TODO ULYSSE
- signal handling: `ctrl-C`, `ctrl-D`, `ctrl-\`
- check stackoverflow-teams about minishell
- use wait3, wait4 ?

#### Done Ulysse
- Memory: `s_alloc()` and `track_alloc()`: split allocation for whole minishell and for each command
- parser(): handle syntax/parsing errors
- review slack thread about minishell
- parser(): understand the EBNF-like grammar notation
- parser(): write Minishell Grammar
- parser(): parse_redirection() -> pre/post-fix
- import error handling from pipex
- import allocation handling from pipex
- executor(): command grouping: `()`
- executor(): redirection: `<`, `>`, `<<`, `>>` #PIPEX
- executor(): pipeline: `|` #PIPEX
- executor(): logical operators: `&&` and `||`
- executor(): Executing Commands (`fork()` and `execve()`) #PIPEX
- review all external functions
- read https://www.gnu.org/software/bash/manual/html_node/Interactive-Shells.html
- parser(): in postfix, redirections and argument can be mixed

---

### TO DISCUSS
- for now lexer manage some syntax error, but as there is EXPENDER after LEXER, it doesnt make sense. So we should completely remove syntax error from lexer and put it in parser.

## Future (in logical order)
- in heredoc() i think if we copy-paste multiple line, it display multiple prompt. Maybe use `rl_on_new_line()`, `rl_replace_line()`, and `rl_redisplay()` could avoid that.
- use IFS env variable to split input ?
- History : clear history and check that empty input as space characters are not saved (explanations here: https://42born2code.slack.com/archives/CN9RHKQHW/p1654165861713979)
- Built-in commands (`echo`, `cd`, `pwd`, `export`, `unset`, `env`, `exit`).
- Environment Variables and Exit Status (`$VAR`, `$?`)
- check this https://42born2code.slack.com/archives/CMX2R5JSW/p1702220631357929
- Makefile: check this one: [minishell/Makefile at main · leaks-labs/minishell](https://github.com/leaks-labs/minishell/blob/main/Makefile)

---

## Done milestones
- Project structure
- Compilation with libraries
- Basic Shell Loop: display prompt + read input from the user
- History: add_history(user_input).
- lexer(): split input into tokens
- parser(): parse tokens into ast