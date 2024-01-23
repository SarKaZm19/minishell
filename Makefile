
################################################################################

# Compiler and flags

CC				=	gcc
CFLAGS			=	-Wall -Wextra -Werror -fsanitize=address
CFLAGS			+=	-O3 -g
CFLAGS			+=	-Iinclude -Isrc
RM				=	rm -rf
MSHELL			=	$(NAME)
NAME			=	minishell

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
    READLINE_PATH = $(HOME)/.brew/opt/readline
    CFLAGS += -I$(READLINE_PATH)/include/readline
    READLINE_LIB = -L$(READLINE_PATH)/lib
endif

# for Ulysse laptop:
    # READLINE_PATH = /opt/homebrew/opt/readline
    # CFLAGS += -I$(READLINE_PATH)/include

################################################################################

# Libraries

LIBFT			=	libft.a
LIBFT_DIR		=	lib/libft
LIBFT_FILE		=	$(LIBFT_DIR)/$(LIBFT)
CFLAGS			+=	-I $(LIBFT_DIR)/include

GET_NEXT_LINE	=	get_next_line.a
GNL_DIR			=	lib/get_next_line
GNL_FILE		=	$(GNL_DIR)/$(GET_NEXT_LINE)
CFLAGS			+=	-I $(GNL_DIR)/include

LIB				=	$(LIBFT_FILE) $(GNL_FILE)
MAKE_LIB		=	make --no-print-directory -C

################################################################################

# Source and Object Files

VPATH			=	include:src:src/builtins:src/environment:src/executor:src/input:src/parser:src/redirections:src/core:src/errors:src/expander:src/lexer:src/pipes:src/signals

MSHELL_INC		=	libraries.h	structures.h minishell.h
MSHELL_SRC		=	main.c init_shell.c interactive_shell.c prompt_execution.c quit_shell.c utils.c \
					init_env.c distribute_env.c env_utils.c \
					executor.c \
					read_heredocs.c \
					exe_command.c exe_pipeline.c exe_redirection.c \
					exe_logical.c exe_group.c \
					get_bin_path.c get_bin_path_utils.c get_builtin_fct.c \
					expander.c expander_struct.c \
					d_quotes_expansion.c d_quotes_var_expansion.c \
					expander_word_splitting.c new_cmd_tab.c \
					no_quotes_expansion.c s_quotes_expansion.c variable_expansion.c \
					lexer.c lexer_utils.c scan_next_token.c \
					parser.c parse_token.c parse_tk_redirection.c parse_tk_redirection_utils.c \
					utils_token.c create_ast_node.c \
					set_signals.c handle_signals.c \
					error.c allocation.c syscall_file.c syscall_process.c\
					cd.c echo.c env.c exit.c export.c pwd.c unset.c  \
					debug.c 
MSHELL_OBJ		=	$(MSHELL_SRC:%.c=build/%.o)

################################################################################

# Rules

all:			$(MSHELL)

bonus:			all

O_OBJS			=	$(MSHELL_OBJ)

$(MSHELL_OBJ):	build/%.o: %.c $(MSHELL_INC)
				mkdir -p $(dir $@)
				$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT_FILE):
				$(MAKE_LIB) $(LIBFT_DIR)

$(GNL_FILE):
				$(MAKE_LIB) $(GNL_DIR)

$(MSHELL):		$(LIB) $(O_OBJS)
				$(CC) $(CFLAGS) $(O_OBJS) -L$(LIBFT_DIR) -lft $(GNL_FILE) $(READLINE_LIB) -lreadline -o $@

lib_clean:
				$(MAKE_LIB) $(LIBFT_DIR) clean
				$(MAKE_LIB) $(GNL_DIR) clean

lib_fclean:
				$(MAKE_LIB) $(LIBFT_DIR) fclean
				$(MAKE_LIB) $(GNL_DIR) fclean

clean:			lib_clean
				$(RM) build

fclean:			clean lib_fclean
				$(RM) $(MSHELL)

re:				fclean all

.SILENT:

.PHONY:			all bonus lib_clean lib_fclean clean fclean re

################################################################################
