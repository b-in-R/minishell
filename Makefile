# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rabiner <rabiner@student.42lausanne.ch>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/29 12:28:22 by rabiner           #+#    #+#              #
#    Updated: 2025/06/17 16:37:32 by rabiner          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CC = gcc
RM = rm -f
RMDIR = rm -rf
CFLAGS = -Wall -Wextra -Werror -Iinclude
LIBS = -lreadline

#
#	DOSSIER_DIR = dossier
#	DOSSIER = fichier_1.c fichier_2.c ...
#

SRCS_DIR = srcs
SRCS = main.c \
#SRCS =	test_main.c \

LIBFT_DIR =  libft
LIBFT_A = $(LIBFT_DIR)/libft.a

EXEC_DIR = execution
EXEC = execute.c \
		redirection.c \
		check_builtin.c \
		builtin_1.c \
		builtin_2.c \
		builtin_utils.c \
		path.c \

EXPAND_DIR = expander
EXPAND = expander.c \

LEXER_DIR = lexer
LEXER = tokenization.c \

PARSER_DIR = parser
PARSER = parsing.c \

SIGNAL_DIR = signals
SIGNAL = signal.c \

UTILS_DIR = utils
UTILS = utils.c \
		env.c \
		
		
# Renvoi des dossiers et fichiers dans SRCS, mettre $(NOM_DIR)/, $(NOM)
SRCS := $(addprefix $(SRCS_DIR)/, $(SRCS)) \
		$(addprefix $(SRCS_DIR)/$(EXEC_DIR)/, $(EXEC)) \
		$(addprefix $(SRCS_DIR)/$(UTILS_DIR)/, $(UTILS)) \
		
#		$(addprefix $(SRCS_DIR)/$(EXPAND_DIR)/, $(EXPAND)) \
		$(addprefix $(SRCS_DIR)/$(LEXER_DIR)/, $(LEXER)) \
		$(addprefix $(SRCS_DIR)/$(PARSER_DIR)/, $(PARSER)) \
		$(addprefix $(SRCS_DIR)/$(SIGNAL_DIR)/, $(SIGNAL)) \

OBJS = $(SRCS:.c=.o)


OBJS_DIR = $(SRCS_DIR)/objs
OBJS = $(addprefix $(OBJS_DIR)/, $(notdir $(SRCS:.c=.o)))

INCLUDES = -I. -I$(LIBFT_DIR)

all: $(OBJS_DIR) $(NAME)

$(OBJS_DIR):
	@mkdir -p $(OBJS_DIR)

$(LIBFT_A):
	$(MAKE) -C $(LIBFT_DIR)

$(NAME): $(OBJS) $(LIBFT_A)
	$(CC) $(CFLAGS) $(INCLUDES) $(OBJS) $(LIBFT_A) $(LIBS) -o $(NAME)

$(OBJS_DIR)/%.o: $(SRCS_DIR)/*/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(LIBFT_A):
	@$(MAKE) -C $(LIBFT_DIR)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)
	@$(MAKE) -C $(LIBFT_DIR) clean
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	$(RM) $(NAME)
	$(RMDIR) $(OBJS_DIR)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re

