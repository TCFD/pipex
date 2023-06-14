# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rciaze <rciaze@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/03 10:50:48 by zbp15             #+#    #+#              #
#    Updated: 2023/02/02 11:06:24 by rciaze           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = pipex
CFLAGS = -Wall -Wextra -Werror -c -g3
COMP = cc $(FLAGS)
all : $(NAME)
CLEANF = rm -f *.o

SRCS =	main.c \
pipex.c \
childs.c

SRCS_B = main_bonus.c \
pipex_bonus.c \
childs_bonus.c \
utils_bonus.c

OBJ = ${SRCS:.c=.o}

OBJ_B = ${SRCS_B:.c=.o}

clean :
	make clean -C libft
	$(CLEANF)

$(NAME) : $(OBJ)
	make -C libft
	mv libft/libft.a .
	$(COMP) $(OBJ) libft.a -o $(NAME)

bonus : $(OBJ_B)
	make -C libft
	mv libft/libft.a .
	$(COMP) $(OBJ_B) libft.a -o $(NAME)

fclean : clean
	make fclean -C libft
	rm -f $(NAME)
	rm -f libft.a
re : fclean all


.PHONY : all re clean fclean 
