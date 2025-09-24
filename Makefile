# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rabiner <rabiner@student.42lausanne.ch>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/29 12:28:22 by rabiner           #+#    #+#              #
#    Updated: 2025/09/23 00:14:45 by rabiner          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CC = gcc
RM = rm -f
RMDIR = rm -rf
CFLAGS = -Wall -Wextra -Werror -g -fdiagnostics-color=always
LIBS = -lreadline
MAKEFLAGS += --no-print-directory


# Colors
GREEN	:= \033[0;32m
RED		:= \033[0;31m
BLUE	:= \033[0;34m
YELLOW	:= \033[0;33m
RST		:= \033[0m

#
#	DOSSIER_DIR = dossier
#	DOSSIER = fichier_1.c fichier_2.c ...
#

SRCS_DIR = srcs
SRCS =	main.c

LIBFT_DIR =  libft
LIBFT_A = $(LIBFT_DIR)/libft.a

EXEC_DIR = execution
EXEC =  execute.c \
		execute_utils.c \
		redirection.c \
		path.c \

MEM_DIR = mem_manager
MEM =	fd_manager.c \
		mem_manager.c \
		mem_manager_utils.c \
		pool_strings.c \

ENV_DIR = env
ENV = env.c \
		env_utils.c \
		env_utils_2.c \

BUILT_DIR = builtin
BUILT = builtin_1.c \
		builtin_2.c \
		check_builtin.c \

EXPAND_DIR = expander
EXPAND = expand_tokens.c \
		expand_utils.c \
		get_env.c \
		var.c \

LEXER_DIR = lexer
LEXER = create_token.c \
		handle_tokens.c \
		lexer.c \

PARSER_DIR = parser
PARSER = create_cmd.c \
		parser.c \
		syntax_checker.c \
		heredoc.c \

SIGNAL_DIR = signals
SIGNAL = signal.c \

UTILS_DIR = utils
UTILS = utils.c \
		free.c \

# Renvoi des dossiers et fichiers dans SRCS, mettre $(NOM_DIR)/, $(NOM)
SRCS := $(addprefix $(SRCS_DIR)/, $(SRCS)) \
		$(addprefix $(SRCS_DIR)/$(EXEC_DIR)/, $(EXEC)) \
		$(addprefix $(SRCS_DIR)/$(ENV_DIR)/, $(ENV)) \
		$(addprefix $(SRCS_DIR)/$(BUILT_DIR)/, $(BUILT)) \
		$(addprefix $(SRCS_DIR)/$(UTILS_DIR)/, $(UTILS)) \
		$(addprefix $(SRCS_DIR)/$(EXPAND_DIR)/, $(EXPAND)) \
		$(addprefix $(SRCS_DIR)/$(LEXER_DIR)/, $(LEXER)) \
		$(addprefix $(SRCS_DIR)/$(PARSER_DIR)/, $(PARSER)) \
		$(addprefix $(SRCS_DIR)/$(SIGNAL_DIR)/, $(SIGNAL)) \
		$(addprefix $(SRCS_DIR)/$(MEM_DIR)/, $(MEM)) \


OBJS_DIR = $(SRCS_DIR)/objs
OBJS = $(addprefix $(OBJS_DIR)/, $(notdir $(SRCS:.c=.o)))

INCLUDES = -I. -I$(LIBFT_DIR)

all: $(OBJS_DIR) $(NAME)

$(OBJS_DIR):
	@mkdir -p $(OBJS_DIR)

$(LIBFT_A):
	@$(MAKE) -C $(LIBFT_DIR)

$(NAME): $(OBJS) $(LIBFT_A)
	$(CC) $(CFLAGS) $(INCLUDES) $(OBJS) $(LIBFT_A) $(LIBS) -o $(NAME)
	@{ \
	$(CC) $(CFLAGS) $(INCLUDES) $(OBJS) $(LIBFT_A) $(LIBS) -o $(NAME); \
	}	2> .build_err && \
		(printf "$(GREEN)[compilation ok]$(RST)\n"; rm -f .build_err) || \
		(cat .build_err; rm -f .build_err; false)

$(OBJS_DIR)/%.o: $(SRCS_DIR)/*/%.c
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@ 2> .build_err \
		|| (cat .build_err && rm -f .build_err && false)


$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@ 2> .build_err \
		|| (cat .build_err && rm -f .build_err && false)

clean:
	@$(RM) $(OBJS)
	@$(MAKE) -C $(LIBFT_DIR) clean
	@printf "$(BLUE)[srcs clean]$(RST)\n"

fclean: clean
	@$(RM) $(NAME)
	@$(RMDIR) $(OBJS_DIR)
	@$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
