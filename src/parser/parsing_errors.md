
Unclosed Quotes are managed by the lexer.

---

1. **Missing Command Error**: A pipeline, group or Logical operators need associated command(s) (sequence in grammar). Warning: a its not the case for redirections.

2. **Unmatched Group Symbols Error**: If `%GROUP_OPEN` does not have a corresponding `%GROUP_CLOSE`, and vice versa. E.g., `( ls -l`.

3. **Incomplete Redirection Error**: A redirection token (`%REDIRECT_IN`, `%REDIRECT_OUT`, `%APPEND`, `%HEREDOC`) should be followed by a filename or a heredoc end identifier.

---

To do in expansions ? reason why it could be the first step of parsing and not a pre-parsing step ? :
1.  **Command Substitution Errors**: `echo $(ls`.

Check that with env part:
2.  **Environment Variable Errors**: Handle errors related to environment variables, such as unset or malformed variables.
    - Example: `echo $`.

Check that for bonus:
- Incorrect Use of Wildcards: `cat *.txt *`.

