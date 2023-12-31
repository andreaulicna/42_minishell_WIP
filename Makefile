# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aulicna <aulicna@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/23 14:30:35 by aulicna           #+#    #+#              #
#    Updated: 2023/12/18 14:35:55 by aulicna          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

SRC = src/debug/main_a.c\
		src/debug/print.c\
		src/utils/utils.c\
		src/utils/env.c\
		src/builtins/ft_echo.c\
		src/builtins/ft_pwd.c\
		src/builtins/ft_env.c\
		src/builtins/ft_cd.c\
		src/builtins/ft_export.c\
		src/builtins/ft_unset.c\
		src/builtins/ft_exit.c\
		src/error/error_token.c\
		src/exit/exit.c\
		src/exit/free.c\
		src/exit/free_helpers.c\
		src/expander/expander.c\
		src/expander/expander_dollar.c\
		src/heredoc/heredoc.c\
		src/lexer/ft_split_minishell.c\
		src/lexer/lexer.c\
		src/parser/parser_redirects.c\
		src/parser/parser.c\
		src/init.c\
		src/prompt.c\
		src/quotes.c

OBJ = $(SRC:.c=.o)

HEADER = incl/minishell.h

CFLAGS = -Wall -Werror -Wextra -g

GCC = gcc

LIBFTPRINTF = libftprintf

all: libs $(NAME)
	@echo "minishell executable ready ✅"

.c.o:
	$(GCC) $(CFLAGS) -c $< -o $@

libs:
	@make -C $(LIBFTPRINTF)
	@echo "libprintf library ready ✅"

$(NAME): $(OBJ) $(HEADER)
	$(GCC) $(CFLAGS) -L $(LIBFTPRINTF) -o $@ $^ -lreadline -lftprintf

clean:
	@rm -f $(OBJ)
	@rm -f src/heredoc/.tmp_files/.tmp*
	@make clean -C $(LIBFTPRINTF)

fclean: clean
	@rm -f $(NAME)
	@make fclean -C $(LIBFTPRINTF)

re: fclean all

.PHONY: all clean fclean re