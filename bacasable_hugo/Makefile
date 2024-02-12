NAME = minishell

CC = cc

CFLAGS = -Werror -Wall -Wextra

RM = rm -rf

SRCS =  srcs/lexer/lexer.c \
		srcs/lexer/lexer_utils.c \
		srcs/lexer/list.c \
		srcs/lexer/token_handler.c \
		srcs/lexer/token_handler_suite.c \
		srcs/lexer/error_handler.c \
		\
		srcs/builtins/cd.c \
		srcs/builtins/echo.c \
		srcs/builtins/env.c \
		srcs/builtins/pwd.c \
		srcs/builtins/unset.c \
		srcs/builtins/export.c \
		\
		srcs/env/env_handler.c \
		srcs/env/env_utils.c \
		srcs/env/sort_env.c \
		\
		srcs/main.c\
		srcs/test_wildcard.c

OBJS    = ${SRCS:.c=.o}

LIBFT_DIR = libft
LIBFT_A = libft/libft.a
_CLEAR	= \033[0K\r\c
_OK	= [\033[32mOK\033[0m]

all : $(LIBFT_A) $(NAME)

$(LIBFT_A): libft/libft.h
	@echo "[..] libft... compiling \r\c"
	@$(MAKE) -C $(LIBFT_DIR) -s
	@echo "$(_CLEAR)"

${NAME}: ${OBJS} $(LIBFT_A)
	@${CC} ${CFLAGS} -g -I include -I libft/ -o $@ $^ -Llibft -lft -lreadline
	@echo "$(_OK) $(NAME) compiled"

%.o: %.c  include/fdf.h libft/libft.h
	@echo "[..] $(NAME_SHORT)... compiling $*.c\r\c"
	@${CC} ${CFLAGS} -g -Iinclude -Ilibft/ -c $< -o $@
	@echo "$(_CLEAR)"

clean :
	@$(MAKE) -C $(LIBFT_DIR) clean -s
	@$(RM) $(OBJS)
	@$(RM) $(OBJS_BONUS)

fclean : clean
	@$(MAKE) -C $(LIBFT_DIR) fclean -s
	@$(RM) $(NAME)
	@$(RM) $(NAME_BONUS)

re : fclean all

.PHONY: all clean fclean re libft

