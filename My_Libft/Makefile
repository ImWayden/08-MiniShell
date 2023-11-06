# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: therodri <therodri@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/08 20:56:08 by therodri          #+#    #+#              #
#    Updated: 2022/12/10 23:41:25 by therodri         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a

IS = norelink_is
MEM = norelink_mem
PRINT = norelink_print
STR = norelink_str
LST = norelink_lst
FTGNL = norelink_ftgnl
FTPRINTF = norelink_ftprintf

CC = cc
CFLAGS = -Wall -Wextra -Werror
AR = ar rcs

ISFILES = ft_atoi  ft_isalnum ft_isascii ft_isdigit ft_isprint\
	ft_isalpha ft_itoa	
MEMFILES = ft_bzero ft_memchr ft_memcpy ft_memmove ft_memset ft_memcmp ft_calloc
PRINTFILES = ft_putnbr_fd ft_putstr_fd ft_putchar_fd ft_putendl_fd
STRFILES = ft_split ft_strchr ft_strdup ft_striteri ft_strjoin ft_strlcat\
	ft_strlen ft_strmapi ft_strncmp ft_strnstr ft_strrchr ft_strtrim ft_substr\
	ft_tolower ft_toupper ft_strlcpy 	
LSTFILES = ft_lstlast ft_lstdelone ft_lstadd_back ft_lstsize ft_lstnew ft_lstadd_front\
	ft_lstclear ft_lstiter ft_lstmap
FTGNLFILES = get_next_line
FTPRINTF_FILES = ft_printf ft_pf_print ft_pf_count ft_pf_simpleft
 
	
	
OBJS_DIR = ./
ISFILES_DIR = ./src/is/
MEMFILES_DIR = ./src/mem/
PRINT_DIR = ./src/print/
STRFILES_DIR = ./src/str/
LSTFILES_DIR = ./src/lst/
FTGNL_DIR = ./src/ft_get_next_line/
FTPRINTF_DIR = ./src/ft_printf/
HEADER_DIR = ./header/

IS_SRCS	   = $(addprefix $(ISFILES_DIR), $(addsuffix .c, $(ISFILES)))
MEM_SRCS   = $(addprefix $(MEMFILES_DIR), $(addsuffix .c, $(MEMFILES)))
PRINT_SRCS = $(addprefix $(PRINT_DIR), $(addsuffix .c, $(PRINTFILES)))
STR_SRCS   = $(addprefix $(STRFILES_DIR), $(addsuffix .c, $(STRFILES)))
LST_SRCS   = $(addprefix $(LSTFILES_DIR), $(addsuffix .c, $(LSTFILES)))
FTGNL_SRCS = $(addprefix $(FTGNL_DIR), $(addsuffix .c, $(FTGNLFILES)))
FTPRINTF_SRCS   = $(addprefix $(FTPRINTF_DIR), $(addsuffix .c, $(FTPRINTF_FILES)))

IS_OBJS	   = $(addprefix $(ISFILES_DIR), $(addsuffix .o, $(ISFILES)))
MEM_OBJS   = $(addprefix $(MEMFILES_DIR), $(addsuffix .o, $(MEMFILES)))
PRINT_OBJS = $(addprefix $(PRINT_DIR), $(addsuffix .o, $(PRINTFILES)))
STR_OBJS   = $(addprefix $(STRFILES_DIR), $(addsuffix .o, $(STRFILES)))
LST_OBJS   = $(addprefix $(LSTFILES_DIR), $(addsuffix .o, $(LSTFILES)))
FTGNL_OBJS = $(addprefix $(FTGNL_DIR), $(addsuffix .o, $(FTGNLFILES)))
FTPRINTF_OBJS   = $(addprefix $(FTPRINTF_DIR), $(addsuffix .o, $(FTPRINTF_FILES)))

OBJS =$(IS_OBJS) $(MEM_OBJS) $(PRINT_OBJS) $(STR_OBJS) $(LST_OBJS) $(FTGNL_OBJS) $(FTPRINTF_OBJS)

RULES = is mem print str lst ftgnl ftprintf
all:$(NAME)
	   
$(NAME): $(RULES)
	@echo "\033[1;32mLibft Done!\n\033[0m"
		
is: $(IS)
$(IS) : $(IS_OBJS)
	@${AR} $(NAME) $^
	@echo "\033[1;32m\nft_is.. Done!\n\033[0m"
	
mem: $(MEM)
$(MEM) : $(MEM_OBJS)
	@${AR} $(NAME) $^
	@echo "\033[1;32m\nft_mem.. Done!\n\033[0m"

print: $(PRINT)
$(PRINT) : $(PRINT_OBJS)
	@${AR} $(NAME) $^
	@echo "\033[1;32m\nft_print.. Done!\n\033[0m"
	
str: $(STR)
$(STR) : $(STR_OBJS)
	@${AR} $(NAME) $^
	@echo "\033[1;32m\nft_str.. Done!\n\033[0m"
	
lst: $(LST)
$(LST) : $(LST_OBJS)
	@${AR} $(NAME) $^
	@echo "\033[1;32m\nft_lst.. Done!\n\033[0m"

ftgnl: mem $(FTGNL)
$(FTGNL) : $(FTGNL_OBJS)
	@${AR} $(NAME) $^
	@echo "\033[1;32m\nget_next_line Done!\n\033[0m"
	
ftprintf: is $(FTPRINTF) 

$(FTPRINTF) : $(FTPRINTF_OBJS)
	@${AR} $(NAME) $^
	@echo "\033[1;32m\nft_printf Done!\n\033[0m"

%.o: %.c
	@printf "\033[1;32mGenerating libftobject..." 
	@printf "\033[1;36m%-38.38s \r" $@
	@$(CC) $(CFLAGS) -c $< -o $@
	
clean: 
	@rm -f $(OBJS)
fclean: clean
	@rm -f $(NAME)
re: clean all

.PHONY:
