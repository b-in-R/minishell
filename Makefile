# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: albertooutumurobueno <albertooutumurobu    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/29 12:28:22 by rabiner           #+#    #+#              #
#    Updated: 2025/06/02 15:11:14 by albertooutu      ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CC = gcc
RM = rm -f
CFLAGS = -Wall -Wextra -Werror -Iincludes -ILibft

LIBFT_DIR = Libft
LIBFT_A = $(LIBFT_DIR)/libft.a
LIBS = -lreadline

SRCS = \
	srcs/main.c \
	srcs/utils/free.c \
	srcs/utils/exit.c \
	srcs/utils/error.c \
	srcs/signals/signals.c \
	srcs/lexer/lexer.c \
	srcs/lexer/create_token.c \
	srcs/lexer/handle_tokens.c \
	srcs/parser/parser.c \
	srcs/parser/create_cmd.c \
	srcs/expander/expander.c \
	srcs/executor/exec.c

OBJS = $(SRCS:.c=.o)

all: $(LIBFT_A) $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT_A) -o $(NAME) $(LIBS)

$(LIBFT_A):
	@$(MAKE) -C $(LIBFT_DIR)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)
	@$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	$(RM) $(NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re

