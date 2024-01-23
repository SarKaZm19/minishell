
# Minishell
Made by [**`Florent`**]() & [**`Ulysse`**]().

## 🧰 General information
Version: 6. Bonus included.
Respect the new rules in version 6.1. about using global variables only for signals.

## 🚀 Set-up
To keep the GitHub repository tidy and to avoid displaying library code, the lib directory is archived. Follow the steps below to set up and run the project:
1. Decompress the Libraries: `tar xzf lib.tar.gz && rm -rf lib.tar.gz`
	If needed, to re-archive : `tar czf lib.tar.gz lib/ && rm -rf lib/`
2. Compile the Project: `make`
3. Run the Project: `./minishell`

## ✨ What makes this minishell special?
- Fidelity: build on the bash manual and POSIX standard, in the limits of the subject (see `/docs/bash_manual_for_minishell.md`).
- Security: robust design and all system calls checked.
- Performance: to avoid unnecessary search, binaries paths are cached in memory (see `/src/get_bin_path.c`).
- Extensibility: thanks to the Abstract Syntax Tree, it is easy to add new features (in executors, the parser could be a bit more tricky to extend).
- Debugging: tokens from the lexer and ast nodes from the parser can be printed in json format (see `/src/debug.c`).
- Documentation: parts of the code is documented with doxygen style.
- Extra-feature: parenthesis are implemented as subshells for easier testing.

## 📋 Testing
- tested with [**`/mpanic`**](https://github.com/ChewyToast/mpanic) : [results]