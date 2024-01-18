
- read: https://www.gnu.org/software/bash/manual/html_node/Shell-Expansions.html

### Where to do do the expender?
"The WORD tokens shall have the word expansion rules applied to them immediately before the associated command is executed, not at the time the command is parsed." [https://pubs.opengroup.org/onlinepubs/009695399/utilities/xcu_chap02.html#tag_02_10]

More detailed in this thread : https://42born2code.slack.com/archives/CN9RHKQHW/p1678346990838969

Check relevant parts: https://aosabook.org/en/v1/bash.html

2 role : 
- expand the word
- remove quotes when needed


Note Ulysse:
`echo "'$myval'" && export myval=1 && echo "'$myval'"`
Ok ça je capte. Je me dis que ducoup il faut simplement avoir un token TK_EXPEND et un node AST_EXEPAND. Pendant l'execution, si un node AST_EXEPAND est rencontré, on execute la fonction d'expension dessus. Et on remplace le token TK_EXPEND par le résultat de l'expension. Et on continue l'execution normalement.
AUTRE STRATÉGIE: pas de token et de node spécial. On commence simplement dans l'exécution de AST_COMMAND à checker si il y a de l'expension à faire. Si oui on fait l'expension et on remplace le AST_COMMAND par le résultat.
Donc la fonction expender() prend un argument un AST_COMMAND et retourne un AST. Elle est exécuté dans execute si (node->type == AST_COMMAND), juste avant execute_command(), exécuté si il n'y a pas eu expension, si expension on rappelle execute() sur le nouveau node retourné par expender().


"Double quotes prevent word splitting during the expansion phase"
=> donc je suppose que l'expender doit avoir une notion du contexte dans lequel il est appelé.


TESTS :
"" --> exec_command à modif je pense
'ls'
'ls la'
'ls''la'
'ls $test la' / '$test ls la' / 'ls $test la'
"ls"
"ls la"
"ls""la"
"ls "" la"
"ls $test"
"ls$test"
"ls $test la" / "$test ls la" / "ls la $test"
"ls$test" -> 'cat'"ls$test" -> 'cat'"ls$test""lol" --> ast'cat'"ls$test""lol"ast 
ast -> astast -> ast$test"ast"
test $test = "hello world' --> test inside " and without
ast$test --> $test"hello" --> ast$test"hello" --> ast$test"hello"'hello'
"hello"'hello'$test"cat""oui" 
$test$test --> 
ast"hello"$test$test"hello""$test$test"