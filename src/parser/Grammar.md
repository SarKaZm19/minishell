
# Grammar
## todo
sk gpt:
- Give a detailed explanation of how the Extended Backus–Naur form works. Give examples with the bash grammar.
- (understand all bash grammar by asking question)
- Adapt the bash grammar to the minishell subject by removing all the unnecessary statements. [give subject and full grammar]
- translate it in a pseudo-code for every major step of the parser

## Ressources
lot of information in this tread: https://42born2code.slack.com/archives/CN9RHKQHW/p1678624330556569
Make sure that you understand the shell syntax: https://pubs.opengroup.org/onlinepubs/009695399/utilities/xcu_chap02.html
- read: https://en.wikipedia.org/wiki/Recursive_descent_parser
- check that : https://pubs.opengroup.org/onlinepubs/009695399/utilities/xcu_chap02.html#tag_02_10
- "Recursive Descent Parsing" (https://stackoverflow.com/a/63858759)

https://repository.unikom.ac.id/48769/1/Compilers%20-%20Principles%2C%20Techniques%2C%20and%20Tools%20%282006%29.pdf
pages: 42, 60, 191

# Minishell Grammar
Here is a custom grammar for Minishell. It come from the [original bash grammar](https://pubs.opengroup.org/onlinepubs/009695399/utilities/xcu_chap02.html#tag_02_10) and has been adapted for the subject. Unused rules and tokens have been removed. A custom notation is used to represent optional arguments on a single line (`(`, `)` and `|` ), only for single alternatives a null option is supposed.

A second grammar much different from the original rename rules and tokens to match our Minishell implementation. It also mark a `%` before each token to make it easier to read.

## Trunked Bash Grammar
```
/* Grammar symbols */
%token WORD
%token  AND_IF    OR_IF
/*      '&&'      '||' */
%token  DLESS  DGREAT
/*      '<<'   '>>'  */

/* Grammar */
%start complete_command

complete_command : (complete_command) and_or
                 ;

and_or           : pipeline
                 | and_or (AND_IF | OR_IF) pipeline
                 ;

pipeline         : (pipeline '|') command
                 ;

command          : simple_command io_redirect
                 ;

simple_command   : (cmd_prefix) cmd_name (cmd_suffix)
                 | (cmd_prefix)
                 ;

subshell         : '(' complete_command ')'

cmd_name         : WORD /* Apply rule for command name */

cmd_prefix       : (cmd_prefix) io_redirect
                 ;

cmd_suffix       : (cmd_suffix) WORD /* for arguments */
                 | (cmd_suffix) io_redirect
                 ;

io_redirect      : '<' filename
                 | '>' filename
                 | DGREAT filename /* append */
                 | DLESS  here_end /* heredoc */
                 ;

here_end         : WORD  /* Delimiter for end of heredoc */
                 ;

filename         : WORD  /* Apply rule for filename */
                 ;
```

## Custom Minishell Grammar

```custom_notation
(* Grammar symbols *)
%token WORD
%token AND OR
%token PIPE
%token REDIRECT_IN REDIRECT_OUT APPEND HEREDOC
%token GROUP_OPEN GROUP_CLOSE

(* Grammar *)
%start sequence

sequence         = pipeline
                 | pipeline, %AND | %OR, sequence
                 ;

pipeline         = executable, {%PIPE executable}
                 ;

executable       = {redirect}, command, {redirect}
                 | {redirect}, group, {redirect}
				 | redirect, {redirect}
                 ;

command          = %WORD, {redirect}, {arguments}
                 ;

arguments        = %WORD
				 ;

group            = %GROUP_OPEN, sequence, %GROUP_CLOSE
                 ;

redirect      	= %REDIRECT_IN | %REDIRECT_OUT | %APPEND, filename
                 | %HEREDOC, here_end
                 ;

here_end         = %WORD
                 ;

filename         = %WORD
                 ;
```

https://en.wikipedia.org/wiki/Extended_Backus%E2%80%93Naur_form#Table_of_symbols
repetition  { ... }  none or more

```ebnf
(* Grammar *)
sequence         = pipeline, {"&&" | "|", pipeline}
                 ;

pipeline         = executable, {"|" executable}
                 ;

executable       = {redirect}, command, {redirect}
                 | {redirect}, group, {redirect}
				 | redirect, {redirect}
                 ;

group            = "(", sequence, ")"
                 ;

command          = WORD, {redirect}, {arguments}
                 ;

arguments        = WORD
				 ;

redirect      	= "<" | ">" | ">>", filename
                 | "<<", here_end
                 ;

here_end         = WORD
                 ;

filename         = WORD
                 ;
```