# Minishell Implementation Choices

## Lexer
### About `<&`, `>>&`, and `>&`
For now we just raising a parsing error.

## Parser / AST
### Arithmetic Expansion nested subshell, aka (()) problem
(()) are used for arithmetic expansion
((ls)) raised an error 
((ls) > out ) not 
(((ls) > out)) raised an error because `(ls) > out`  in arithmetic.
As we don't implement a.e., we cannot properly raise the error.
We could be tempted to simply check for pairs of double pa `(( ))`, but `((ls) | (cat > out ))` would fall into this filter despite being a valid command (not sure if valid bur it works in zsh).

Solution: ignore the existence of this notation and do not interpret it.
So, not as bash, our minishell interpret `((ls))` as `( (ls) )`.

### Redirections
The order of redirections matters!
Ok, but is the pre/post-fix order important? 
No bc all redirections are applied before the command is executed.
cf. https://www.gnu.org/software/bash/manual/html_node/Redirections.html