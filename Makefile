# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: wayden <wayden@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/10/30 16:41:34 by wayden            #+#    #+#              #
#    Updated: 2023/12/28 01:48:24 by wayden           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CC = cc
CFLAGS = -Wall -Werror -Werror -g3

FILES = env_setup builtins cleaning utils lst_to_tab lexer singletons \
				parser parser_handlers signals_handlers expander env2 \
				error_handler protected_functions main cmd_verifier \
				executor executor2 serialize token_corrector lexer2\
				builtin_export debug\
				garbage_collector/add_garbage \
				garbage_collector/free_garbage \
				garbage_collector/get_garbage \
				garbage_collector/libft_split_gc \
				garbage_collector/libft_str_gc \
				garbage_collector/libft_gc \
				

DIR = ./src/

INCLUDE = -I .

SRCS   = $(addprefix $(DIR), $(addsuffix .c, $(FILES)))
OBJ = $(addprefix $(DIR), $(addsuffix .o, $(FILES)))

all: $(NAME)


$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(INCLUDE) -lreadline -L. -lft

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDE)

valdebug : $(NAME)
	valgrind ./minishell

clean:
	rm -rf $(OBJ) $(OBJ_DEBUG)
	
fclean: clean
	rm -rf $(NAME)
	
re: fclean all
.PHONY: all clean fclean re
