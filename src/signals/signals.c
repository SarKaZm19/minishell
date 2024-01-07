#include "minishell.h"

// check: https://www.gnu.org/software/libc/manual/html_node/Termination-Signals.html
// (done, said that the global var should be an exit status) https://github.com/madebypixel02/minishell#global-variable
// [Sending and Handling Signals in C (kill, signal, sigaction)](https://www.youtube.com/watch?v=83M5-NPDeWs)
// [099 Bash Jobs & Signals - Bringing Jobs Back To The Command Line](https://www.youtube.com/watch?v=xu2lGV-kISI)

// SA_RESTART flag
// SA_RESTART is used with sigaction() to automatically restart certain system calls that have been interrupted by a signal. Without this flag, system calls like read() or write() would fail with the error EINTR if a signal is caught during their operation. With SA_RESTART, the system call will be automatically retried instead of failing.
// This behavior is often desirable in interactive applications like shells, where you don't want the entire application to fail just because a signal was received while waiting for input or writing output.


# define SIGINT_EXIT 130
# define SIGQUIT_EXIT 131
// But chatGPT say SIGINT is 2, SIGQUIT is 3 and SIGTSTP is 20

volatile sig_atomic_t g_sigint_received = 0;

void sigint_handler(int signum) 
{
	(void)signum;
    g_sigint_received = 1;
}

void setup_signal_handling(void) 
{
    struct sigaction sa;

	g_sigint_received = 0;

    // Setup the SIGINT handler
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0; // or SA_RESTART to avoid system calls failing with EINTR
    sa.sa_handler = sigint_handler;
    sigaction(SIGINT, &sa, NULL);

    // Ignore SIGQUIT
    signal(SIGQUIT, SIG_IGN);
}
