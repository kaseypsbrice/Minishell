# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kbrice <kbrice@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/04/19 13:25:35 by bburston          #+#    #+#              #
#    Updated: 2023/08/09 09:17:00 by kbrice           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

LIBFT = libft/libft.a

HEADER = ./includes/

CC = gcc

FLAGS = -Iincludes -Iusr/local/opt/readline/include/ -Wall -Werror -Wextra #-g -fsanitize=address

LINK_RL = /usr/local/opt/readline/lib
INCL_RL = /usr/local/opt/readline/include/

SRCS = srcs/*.c \

OBJECTS = $(SRCS:.c=.o)

GREEN	=	\e[0;32m
YELLOW	=	\e[0;33m
CYAN	=	\e[0;36m
RESET	=	\e[0m

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	@$(CC) $(FLAGS) $(SRCS) -lcurses -ltermcap $(LIBFT) -I $(INCL_RL) -I $(HEADER) -o $(NAME) -L $(LINK_RL) -lreadline
	@printf "$(GREEN)Executable created$(RESET)\n"

all: $(NAME)

$(LIBFT):
	@make -C ./libft
	@printf "$(GREEN)Compiling libraries...$(RESET)\n"

clean:
	@rm -f $(OBJECTS)
	@make clean -C ./libft
	@printf "$(CYAN)Cleaning object files...$(RESET)\n"

fclean: clean
	@rm -f $(NAME)
	@make fclean -C ./libft
	@printf "$(CYAN)Cleaning executables...$(RESET)\n"

re: fclean all

.PHONY: all clean fclean re
