# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: raveriss <raveriss@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/10/02 04:47:01 by raveriss          #+#    #+#              #
#    Updated: 2023/10/05 15:59:12 by raveriss         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo

SRC_DIR = src

OBJ_DIR = objs

INC_DIR = inc

SRCS =	check_death.c \
		clean.c	\
		ft_atol.c \
		init.c \
		life.c \
		philo_utils.c \
		philo.c \

SRCS := $(SRCS:%=$(SRC_DIR)/%)

OBJS := $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

DEPS := $(OBJS:.o=.d)

CC = gcc
C_FLAGS = -Wall -Wextra -Werror -MMD
DIR_DUP = mkdir -p $(@D)

all : $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(DIR_DUP)
	$(CC) $(C_FLAGS) -I $(INC_DIR) -c $< -o $@

$(NAME) : $(OBJS) Makefile
	$(CC) $(C_FLAGS) $(OBJS) -o $@

-include $(DEPS)

clean :
	rm -f $(OBJS)
	rm -f $(DEPS)
	rm -rf $(OBJ_DIR)

fclean : clean
	rm -f $(NAME)

re : fclean
	make all

.PHONY : all clean fclean re
