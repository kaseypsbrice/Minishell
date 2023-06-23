# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bburston <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/02/14 14:42:06 by bburston          #+#    #+#              #
#    Updated: 2022/02/15 16:19:24 by bburston         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a

SRC = *.c

OBJS = $(SRC:%.c=%.o)

FLAGS = -Wall -Wextra -Werror

$(NAME):
	@gcc $(FLAGS) -c $(SRC) -I./
	@ar rc $(NAME) $(OBJS)

all: @$(NAME)

clean:
	@rm -f $(OBJS)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all, clean, fclean, re