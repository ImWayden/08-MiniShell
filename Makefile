# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: wayden <wayden@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/10/30 16:41:34 by wayden            #+#    #+#              #
#    Updated: 2023/11/07 23:23:42 by wayden           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
DEBUGNAME = debug_minishell

CC = cc
CFLAGS = -g3 

FILES = main

FILES_DEBUG = env_setup builtins cleaning utils lst_to_tab lexer singletons

DIR = ./src/

SRCS   = $(addprefix $(DIR), $(addsuffix .c, $(FILES)))
OBJ	   = $(addprefix $(DIR), $(addsuffix .o, $(FILES)))
OBJ_DEBUG = $(addprefix $(DIR), $(addsuffix .o, $(FILES_DEBUG)))

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME) -lreadline

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ -lreadline

debug : $(NAME)
	valgrind ./minishell

$(DEBUGNAME): $(OBJ_DEBUG)
	$(CC) $(CFLAGS) $(OBJ_DEBUG) -o $(DEBUGNAME) -lreadline -L. -lft

clean:
	rm -rf ${OBJ} ${OBJ_DEBUG}
	
fclean: clean
	rm -rf $(NAME) $(DEBUGNAME)
	
re: fclean all
.PHONY: