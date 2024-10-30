# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jmouette <jmouette@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/03 11:23:04 by arissane          #+#    #+#              #
#    Updated: 2024/10/30 11:38:34 by arissane         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CC = cc

CFLAGS = -Wall -Werror -Wextra

SRCS = main.c signals.c parse.c parse_utils.c parse_helper.c split_input.c\
       execute.c split_tokens.c utils.c utils2.c utils3.c pipes.c free_shell.c\
       commands.c find_path.c tokens.c redirections.c check_chars.c redirect_fd.c\
       builtins/builtins_utils.c builtins/cd.c builtins/exit.c builtins/export.c\
       builtins/unset.c builtins/env.c builtins/echo.c builtins/pwd.c\
       builtins/heredoc.c

OFILES = $(SRCS:.c=.o)

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

all: LIBFT $(NAME)

LIBFT:
	@make -C $(LIBFT_DIR)

$(NAME): $(OFILES) $(LIBFT)
	@$(CC) $(CFLAGS) $(OFILES) libft/libft.a -lreadline -o $(NAME) $(LIBFT)
	@ echo "Minishell compiled"

clean:
	@rm -f $(OFILES)
	@make clean -C $(LIBFT_DIR)

fclean: clean
	@rm -f $(NAME) history.txt
	@make fclean -C $(LIBFT_DIR)
	@ echo "Minishell fclean"

re: fclean all

.PHONY: all clean fclean re libft
