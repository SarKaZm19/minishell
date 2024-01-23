
# Detailed process
https://pubs.opengroup.org/onlinepubs/7908799/xcu/chap2.html#tag_001_003

# What does the lexer?

performing the following tasks:

- Tokenization: The primary function of the lexer is to read the input stream (in this case, the command line input) and convert it into a sequence of tokens. Tokens are meaningful sequences of characters such as keywords, identifiers, literals, and operators. In a shell, these might be command names, arguments, file paths, or operators like |, >, <, &&, ||, etc.

- Removing Whitespace and Comments: The lexer typically discards irrelevant characters such as whitespace, tabs, or comments (if applicable in the shell or programming language) to simplify further processing.

- Handling Quotes and Escapes: In a shell context, the lexer must correctly handle quoted strings, where single (') or double (") quotes may be used to group arguments together as single tokens or to allow special characters to be treated as literals.

- Recognizing Syntax Errors: The lexer can also identify basic syntax errors related to the lexical structure of the input, such as unbalanced or misplaced quotes.

- Generating Lexemes: Each token is associated with a lexeme, which is the actual string of characters that the token represents. For example, the token type might be COMMAND and the lexeme could be ls.

- Maintaining Source Location Information: For more sophisticated systems, the lexer might also keep track of the line numbers and character positions within lines to report errors accurately.

The output of the lexer—a stream of tokens—is then typically passed to the parser, which will perform syntax analysis to determine if the token sequence is well-formed according to the grammar of the language and to build a data structure (often a parse tree or abstract syntax tree) representing the input.