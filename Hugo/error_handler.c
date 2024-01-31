#include "lexer.h"

void error_handler(t_lister *list, char *buffer, char *msg)
{
	
	free(buffer);
	free_lister(list);
	ft_putstr_fd(msg, 2);
	exit(EXIT_FAILURE);
}