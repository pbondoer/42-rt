#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hmarot <marvin@42.fr>                      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2016/03/02 01:40:50 by hmarot            #+#    #+#              #
#    Updated: 2016/11/17 01:33:06 by hmarot           ###   ########.fr        #
#                                                                              #
#******************************************************************************#

FLAG_MLX = -framework OpenGL -framework AppKit

FLAG_GCC = -Werror -Wall -Wextra

MLX_PATH = minilibx_macos/

MLX = -lmlx

LIBFT = ~/Proget/libft/libftprintf.a

PTHREAD = -lpthread

NAME = test

INC_PATH = -I includes/ -I ~/Proget/libft/includes/ -I minilibx_macos/

OBJS =	test.o

OBJ = $(addprefix objs/, $(OBJS))

all : $(NAME)

libs:
	@make -C ~/Proget/libft/ all

$(NAME) : libs $(OBJ)
	@gcc $(FLAG_GCC) -o $(NAME) $(INC_PATH) $(PTHREAD) $(MLX) $(FLAG_MLX) $(OBJ) $(LIBFT)
	@echo "$(NAME) create.\n"

objs/%.o : srcs/%.c
	@gcc $(FLAG_GCC) -c $< -o $@ $(INC_PATH)

re : fclean
	@echo "$(NAME) deleted.\n"
	@make $(NAME)

clean :
	@rm -f $(OBJ)
	@make -C ~/Proget/libft/ clean

fclean : clean
	@rm -f $(NAME)
	@rm -f $(LIBFT)