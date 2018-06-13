# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bhamidi <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/05/20 19:05:42 by bhamidi           #+#    #+#              #
#    Updated: 2018/06/13 18:48:45 by bhamidi          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC		= gcc
NAME	= ft_nm
CFLAGS	= -Werror -Wextra -Wall -O2

## lib ##
LIBNAME = libft.a
LIBPATH = libft/
#######

SRC_PATH		= srcs/
INCLUDE_PATH	= includes/

OBJ		= main.o list_func.o object.o fat.o

INCLUDE	= ft_nm.h

OBJS=$(addprefix $(SRC_PATH), $(OBJ))
INCLUDES=$(addprefix $(INCLUDE_PATH), $(INCLUDE))

all: $(LIBNAME) $(NAME)

$(NAME): $(OBJS) $(LIBPATH)$(LIBNAME)
	$(CC) $(OBJS) $(LIBPATH)$(LIBNAME) $(CFLAGS) -o $@
	@echo "\033[32m$(NAME) linked\033[0m"

$(SRC_PATH)%.o: $(SRC_PATH)%.c $(INCLUDES) Makefile
	$(CC) $(CFLAGS) -I $(INCLUDE_PATH) -I $(LIBPATH) -c $< -o $@

$(LIBNAME):
	@make -C $(LIBPATH)

clean:
	make clean -C $(LIBPATH)
	rm -f $(OBJS)

fclean : clean
	make fclean -C $(LIBPATH)
	rm -f $(NAME)

re : fclean all

.PHONY: all fclean clean re
