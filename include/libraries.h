

// Standard libraries
# include <errno.h>

#include <stdio.h>          // printf, perror
#include <stdlib.h>         // malloc, free, exit, getenv
#include <unistd.h>         // access, chdir, close, dup, dup2, execve, fork, getcwd, isatty, 
                            // read, unlink, write, ttyname, ttyslot
#include <fcntl.h>          // open
#include <sys/types.h>      // pid_t type for fork, wait, waitpid
#include <sys/wait.h>       // wait, waitpid, wait3, wait4
#include <sys/stat.h>       // stat, lstat, fstat
#include <signal.h>         // signal, sigaction, sigemptyset, sigaddset, kill
#include <dirent.h>         // opendir, readdir, closedir
#include <string.h>         // strerror
#include <termios.h>        // tcsetattr, tcgetattr
#include <readline/readline.h>      // readline, rl_clear_history, rl_on_new_line, 
                                    // rl_replace_line, rl_redisplay, add_history
#include <readline/history.h>       // rl_clear_history, add_history
#include <sys/ioctl.h>      // ioctl

// The following includes are for the termcap library, which is not as standard as the others:
// Depending on your system, you might need to install the libncurses or libtermcap library.
#include <curses.h>         // tgetent, tgetflag, tgetnum, tgetstr, tgoto, tputs (part of ncurses)
#include <term.h>           // tgetent, tgetflag, tgetnum, tgetstr, tgoto, tputs (part of termcap)


// to-do : put specific libraries in files that use them
//  For complex projects, the decentralized approach is generally considered best practice. 
//  It leads to cleaner, more modular code, and can make the compilation process more efficient. 
//  Each file or module should include only what it needs to function independently.

// Our libraries
#include "libft.h"
#include "get_next_line.h"
