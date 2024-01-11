
################################################################################

# Compiler and flags

CC				=	gcc
CFLAGS			=	-Wall -Wextra -Werror -fsanitize=address
CFLAGS			+=	-g -Iinclude -Isrc -Ofast
RM				=	rm -rf
MSHELL			=	$(NAME)
NAME			=	minishell

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

MSHELL_INC		=	libraries.h structures.h minishell.h
MSHELL_SRC		=	main.c shell_init.c shell_cycle.c \
					env_vars.c env_utils.c \
					executor.c \
					exe_command.c exe_pipeline.c exe_redirection.c \
					exe_logical.c exe_group.c \
					get_bin_path.c \
					expander.c \
					variable_expansion.c wildcard_expansion.c \
					prompt.c read_input.c \
					lexer.c lexer_utils.c scan_next_token.c \
					parser.c parse_token.c parse_tk_redirection.c parse_tk_redirection_utils.c \
					utils_token.c create_ast_node.c \
					signals.c \
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
				$(CC) $(CFLAGS) $(O_OBJS) -L$(LIBFT_DIR) -lft $(GNL_FILE) -lreadline -o $@

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
