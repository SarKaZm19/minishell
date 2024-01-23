- Check relevant parts: https://aosabook.org/en/v1/bash.html
- interesting discution about wait() and why not using `WNOHANG`
https://42born2code.slack.com/archives/CMX2R5JSW/p1688403957757249
- read: https://www.gnu.org/software/bash/manual/html_node/Executing-Commands.html

## Redirections
- read: https://www.gnu.org/software/bash/manual/html_node/Redirections.html
- check these cases : https://42born2code.slack.com/archives/CN9RHKQHW/p1678624330556569

[https://42born2code.slack.com/archives/CN9RHKQHW/p1678625021714629?thread_ts=1678624330.556569&cid=CN9RHKQHW]
- redirection can appear anywhere in the line.
- output ones have preference over pipes (in case of pipe and redirection, stdout goes to the redirection)
- you can have multiple redirections, even of the same type, (test a little how bash handles that)
- and of course, after a redirection there must be a file (delimiter in case of <<), a syntax error occurs if there's no file :)

Multiple redirections of the same kind in the same simple command are all opened (and truncated if using >, no truncation for >> ), but only the last file is written to or read from.

Solution describe by other students:
The simplest way to do it is to just go over stuff from left to right. Everything will be in the right order because your redirected to the last file last.
We loop trough all the files, open to write or append, but only last one of the array takes the output, that way you create all the files and only write in the last one.

// check: https://www.gnu.org/savannah-checkouts/gnu/bash/manual/bash.html#Redirections