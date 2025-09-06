# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rabiner <rabiner@student.42lausanne.ch>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/01 14:56:11 by rabiner           #+#    #+#              #
#    Updated: 2025/09/06 08:31:37 by rabiner          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a

CC = cc
RM = rm -f
RMDIR = rm -rf
CFLAGS = -Wall -Wextra -Werror -I. -Iinclude

# Colors
GREEN	:= \033[0;32m
RED		:= \033[0;31m
BLUE	:= \033[0;34m
YELLOW	:= \033[0;33m
RST		:= \033[0m


SRCS = ft_bzero.c \
	ft_isalnum.c ft_isalpha.c ft_isascii.c ft_isdigit.c ft_isprint.c \
	ft_memcpy.c ft_memmove.c ft_memset.c ft_memchr.c ft_memcmp.c \
	ft_strlen.c ft_strlcpy.c ft_strlcat.c ft_strnstr.c ft_strncmp.c \
	ft_strcmp.c \
	ft_toupper.c ft_tolower.c \
	ft_strchr.c ft_strrchr.c ft_strdup.c \
	ft_atoi.c ft_itoa.c \
	ft_calloc.c \
	ft_substr.c ft_strjoin.c ft_strtrim.c \
	ft_split.c ft_split_free.c ft_split_quote.c \
	ft_strmapi.c ft_striteri.c \
	ft_putchar_fd.c ft_putstr_fd.c ft_putendl_fd.c ft_putnbr_fd.c \
	ft_printf.c ft_printf_write.c ft_printf_nbrs.c \
	ft_lstnew.c ft_lstadd_front.c ft_lstsize.c \
	ft_lstlast.c ft_lstadd_back.c ft_lstdelone.c \
	ft_lstclear.c ft_lstiter.c ft_lstmap.c \
	ft_get_next_line.c


OBJS_DIR =  objs
OBJS = $(SRCS:.c=.o)
OBJS := $(addprefix $(OBJS_DIR)/, $(OBJS))

all: $(NAME)

$(NAME): $(OBJS)
	@ar rcs $(NAME) $(OBJS) \
		2> .build_err && (printf "$(GREEN)[libft ok]$(RST)\n"; \
		rm -f .build_err) || (cat .build_err && rm -f .build_err)

$(OBJS_DIR)/%.o: %.c
	@mkdir -p $(OBJS_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@  2> .build_err \
		|| (cat .build_err && rm -f .build_err && false)

clean:
	@$(RM) $(OBJS)
	@printf "$(BLUE)[libft clean]$(RST)\n"
	
fclean: clean
	@$(RM) $(NAME)
	@$(RMDIR) $(OBJS_DIR)
	
re: fclean all

.PHONY: all clean fclean re
