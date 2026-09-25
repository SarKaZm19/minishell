# Minishell

A Unix shell written in **C**, developed as part of the 42 curriculum by **Florent & Ulysse**.

The project recreates a focused subset of Bash: interactive input, lexical analysis, syntax parsing, expansions, pipelines, redirections, built-ins, process execution and signal handling.

Rather than interpreting a command line directly from left to right, this implementation separates the work into a real processing pipeline:

```text
user input
    ↓
  lexer
    ↓
 token list
    ↓
  parser
    ↓
    AST
    ↓
 heredocs
    ↓
 expander / executor
    ↓
processes, pipes and redirections
```

## Highlights

- Interactive prompt built with GNU Readline
- Command history
- Dedicated lexer/tokenizer
- Recursive parser producing an Abstract Syntax Tree
- Pipelines
- Input/output redirections: `<`, `>`, `>>`
- Heredocs with `<<`
- Logical operators `&&` and `||`
- Parenthesized command groups / subshell-style execution
- Single- and double-quote handling
- Environment-variable expansion and word splitting
- Executable resolution through `PATH`
- Built-ins: `echo`, `cd`, `pwd`, `export`, `unset`, `env`, `exit`
- Signal handling adapted to prompt and child-process contexts
- Internal allocation tracking by execution scope
- Token and AST debug output for parser development
- Documentation and parser test fixtures included in the repository

## From text to execution

The core of the project is the separation between **lexing, parsing and execution**.

A command such as:

```bash
cat input.txt | grep hello && echo "found" > result.txt
```

is first split into typed tokens rather than executed immediately.

Conceptually:

```text
WORD(cat)
WORD(input.txt)
PIPE
WORD(grep)
WORD(hello)
AND
WORD(echo)
WORD("found")
REDIRECT_OUT
WORD(result.txt)
```

The parser then turns those tokens into an **Abstract Syntax Tree (AST)**. Different node types represent commands, pipelines, logical expressions, redirections and grouped expressions.

That structure gives the executor enough information to respect syntax and operator relationships instead of relying on fragile string manipulation.

## Lexer

The lexer scans the raw input and identifies shell tokens including:

```text
words
|
&&
||
<
>
>>
<<
(
)
```

Quoted regions are kept together so that spaces and shell metacharacters inside quotes are not treated as ordinary separators.

The repository includes debug helpers capable of serializing token streams, which made complex parsing cases easier to inspect during development.

## Parser and AST

The parser consumes the token list and recursively constructs the AST.

The main node categories implemented by the project are:

```text
AST_COMMAND
AST_PIPELINE
AST_LOGICAL
AST_REDIRECTION
AST_GROUP
```

For example:

```bash
echo hello | grep h && (pwd || echo error)
```

is represented as nested syntax nodes rather than as one flat command string.

This architecture was one of the most important design choices in the project: once syntax has been converted into a tree, the executor can dispatch behavior according to node type and naturally compose pipelines, logical operations and groups.

## Execution

The executor walks the AST and dispatches each node to the corresponding execution path.

Simple external commands are resolved against `PATH` and launched as child processes. Pipelines connect commands using Unix pipes and file-descriptor redirection, building on the same low-level concepts explored earlier in pipex.

Logical nodes implement short-circuit behavior for `&&` and `||`, while grouped nodes allow a nested expression to execute in its own context.

The shell also tracks child exit states and translates signal termination into shell-style exit statuses.

## Redirections and heredocs

Redirections are represented directly in the syntax tree instead of being handled as an afterthought.

Supported operators include:

```text
<   input
>   output
>>  append
<<  heredoc
```

Heredocs are collected before normal AST execution. This keeps input acquisition separate from the execution of the final command tree and makes pipeline/redirection combinations easier to reason about.

## Expansion and quoting

Before command execution, command nodes pass through an expansion stage.

The project distinguishes between:

- unquoted text;
- single-quoted text;
- double-quoted text;
- environment-variable substitution;
- word splitting where applicable.

This is one of the parts where shell behavior becomes deceptively complex: the same characters can have different meanings depending on quoting and expansion context.

## Built-ins

The following built-ins are implemented directly by minishell:

```text
echo
cd
pwd
export
unset
env
exit
```

Commands that must affect the shell's own state — such as `cd`, `export` or `unset` — cannot always be treated like ordinary external programs, because changes made only inside a child process would disappear when that child exits.

## Signals

Signal behavior is handled differently depending on whether minishell is waiting at the interactive prompt or executing child processes.

The project keeps the global signal state deliberately limited and converts interrupted child processes into the expected shell-style status codes.

## Build

Requirements:

- a C compiler;
- GNU Readline development files;
- `make`.

Compile:

```bash
make
```

Run:

```bash
./minishell
```

The Makefile contains a macOS-specific Readline path configuration when running on Darwin and links against the system Readline library on other supported Unix-like environments.

## Repository structure

```text
minishell/
├── include/              # Shared structures and declarations
├── src/
│   ├── builtins/         # Shell built-ins
│   ├── core/             # Prompt and shell lifecycle
│   ├── environment/      # Environment representation
│   ├── executor/         # AST execution, pipelines and redirections
│   ├── expander/         # Quotes and variable expansion
│   ├── lexer/            # Tokenization
│   ├── parser/           # AST construction
│   ├── signals/          # Signal behavior
│   └── errors/           # Error handling
├── lib/                  # Libft and get_next_line
├── docs/                 # Design notes and Bash references
└── tests/                # Lexer / AST test fixtures and tester
```

## Documentation and testing

The repository contains design notes covering the lexer, grammar, executor, expansion, signals and implementation choices.

A dedicated test directory also stores complex token/AST cases for pipelines, logical expressions, redirections and nested parentheses. These were useful for testing the parser independently from the interactive shell.

## What this project demonstrates

Minishell is less about reproducing every Bash feature than understanding what happens between pressing **Enter** and seeing a command execute.

It combines lexical analysis, parsing, tree structures, process management, Unix file descriptors, environment state, expansion rules and signals in one program.

The AST-based architecture is the central idea: shell syntax is first transformed into structured data, and only then executed.

---

Part of my developer portfolio: **[github.com/Overflow-ADW](https://github.com/Overflow-ADW)**  
Professional work: **[Avenue du Web](https://avenueduweb.be)**
