
## External Function Categories
(gpt)

### Readline and History Functions:
- `readline`: Reads a line from the terminal and returns it.
- `rl_clear_history`: Clears the entire history list in readline.
- `rl_on_new_line`: Prepares readline to read a new line by moving to a new line.
- `rl_replace_line`: Replaces the current input line with a new one.
- `rl_redisplay`: Redraws the current input line.
- `add_history`: Adds a new line to the readline history.

### Process and Signal Handling:
- `wait3`: Waits for child processes to report status; also provides resource usage.
- `wait4`: Waits for specific child processes, similar to `wait3`.
- `signal`: Sets a function to handle a signal (somewhat outdated, `sigaction` is preferred).
- `sigaction`: Examines or modifies signal actions (more robust than `signal`).
- `sigemptyset`: Initializes a signal set to empty.
- `sigaddset`: Adds a signal to a signal set.
- `kill`: Sends a signal to a process.

### File and Directory Operations:
- `getcwd`: Gets the current working directory.
- `chdir`: Changes the current directory.
- `stat`: Gets file status.
- `lstat`: Like `stat`, but if the file is a symbolic link, it retrieves information about the link itself.
- `fstat`: Gets file status of an open file based on its file descriptor.
- `unlink`: Removes a file.
- `execve`: Executes a program.
- `dup`: Duplicates a file descriptor.
- `dup2`: Duplicates a file descriptor to a specified descriptor.
- `opendir`: Opens a directory for reading.
- `readdir`: Reads a directory entry.
- `closedir`: Closes a directory.

### Terminal and TTY Control:
- `isatty`: Determines if a file descriptor is associated with a terminal.
- `ttyname`: Returns name of terminal associated with the open file descriptor.
- `ttyslot`: Returns the terminal slot number.
- `ioctl`: Controls device.
- `tcsetattr`: Sets terminal attributes.
- `tcgetattr`: Gets terminal attributes.
- `tgetent`: Loads a terminal description into buffer.
- `tgetflag`: Returns the boolean value of a terminal capability.
- `tgetnum`: Returns the numeric value of a terminal capability.
- `tgetstr`: Returns the string value of a terminal capability.
- `tgoto`: Moves the cursor within the terminal display.
- `tputs`: Outputs a string to the terminal, with padding.

### Environment Control:
- `getenv`: Gets the value of an environment variable.
