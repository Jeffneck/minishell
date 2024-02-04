#include "lexer.h"

void error_handler_lexer(int id_gc, char *msg)
{
	clear_garbage(id_gc, free);
	ft_putstr_fd(msg, 2);
	exit(EXIT_FAILURE);
}