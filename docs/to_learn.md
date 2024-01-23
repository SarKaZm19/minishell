
## Minishell things to learn 
(gpt)

### 1. I/O Redirection
- Learn about **file descriptors** and their role in redirection.
- Understand the functionality and implementation of `<`, `>`, `<<`, and `>>`.

### 2. Signals
- Familiarize yourself with Unix/Linux signals, especially `ctrl-C` (`SIGINT`), `ctrl-D` (EOF), and `ctrl-\` (`SIGQUIT`).
- Learn how to handle signals using functions like `signal()` and `sigaction()`.
- Understand **signal masking** and how to block/unblock specific signals.

### 3. Environment Variables
- Understand the concept and purpose of **environment variables** in Unix/Linux.
- Learn to access and modify environment variables using functions like `getenv()`.
- Understand the significance of the `$PATH` variable for command execution.

### 4. Built-in Commands
- Grasp the idea behind shell **built-in commands**.
- Learn how to implement the specified built-ins such as `echo`, `cd`, `pwd`, `export`, `unset`, `env`, and `exit`.

### 5. Terminal Control
- Understand what a **TTY** is and its role.
- Familiarize yourself with functions like `isatty`, `ttyname`, and the terminal control functions `tcsetattr` and `tcgetattr`.
- Delve into the **terminfo database** and understand how to use functions like `tgetent`, `tgetflag`, and others.

### 6. Readline Library
- Learn how to use the `readline()` function to read input and manage command history.
- Understand the nuances of the **readline library**, such as handling potential memory leaks.

### 7. Bonus Concepts
- Get a grasp on command chaining using logical operators like `&&` and `||`.
- Understand and implement filename **wildcards**, especially `*`.

### 8. File and Directory Operations
- Learn to use system calls like `stat`, `lstat`, and `fstat` to obtain information about files and directories.
- Familiarize yourself with directory-related operations using `opendir`, `readdir`, and `closedir`.
- Understand **file permissions**, ownership, and how they can affect command execution and redirection.
