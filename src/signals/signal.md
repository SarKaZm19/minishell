
# Signals

## Each signal in different context

| Signal | Open Prompt (readline) | Main Process | Child Process (exe) | Heredoc |
|--------|----------------------|-----------------------------------|-------------------------|-------------------------|
| SIGINT (ctrl-C) | Interrupt current prompt + new prompt ✅ | Ignored  ✅ | Default handling (usually terminate) ✅| Interrupt current prompt + new prompt ✅ |
| SIGQUIT (ctrl-\\) | Ignored ✅ | Ignored  ✅ | Default  handling (usually terminate)  ✅ | Ignored ✅ |
| EOF (ctrl-D) | Exit shell if line is empty, else do nothing. ✅ | Ignored  ✅ | End of input for the process ✅ | Interrupt heredoc input + display warning ✅ but heredoc is still taken into account |

Actually , EOF (ctrl-D) is not a signal, it's a character.

## Resources
- https://www.gnu.org/software/libc/manual/html_node/Termination-Signals.html : done, said that the global var should be an exit status 
- https://github.com/madebypixel02/minishell#global-variable
- [Sending and Handling Signals in C (kill, signal, sigaction)](https://www.youtube.com/watch?v=83M5-NPDeWs)
- [099 Bash Jobs & Signals - Bringing Jobs Back To The Command Line](https://www.youtube.com/watch?v=xu2lGV-kISI)


## SA_RESTART flag

SA_RESTART is used with sigaction() to automatically restart certain system calls that have been interrupted by a signal. Without this flag, system calls like read() or write() would fail with the error EINTR if a signal is caught during their operation. With SA_RESTART, the system call will be automatically retried instead of failing.
This behavior is often desirable in interactive applications like shells, where you don't want the entire application to fail just because a signal was received while waiting for input or writing output.

## Exit status
```c
# define SIGINT_EXIT 130
# define SIGQUIT_EXIT 131
```
But chatGPT say SIGINT is 2, SIGQUIT is 3 and SIGTSTP is 20
