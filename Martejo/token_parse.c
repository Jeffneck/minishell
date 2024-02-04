#include "lexer.h"

int parenthese_handler(char *buffer, t_tknlist *list)
{
	int	i;
	int	depth;

	i = 0;
	depth = 0;
	while (buffer[i])
	{
		if (buffer[i] == '(')
			depth++;
		else if (buffer[i] == ')')
		{
			depth--;
			if (depth == 0)
			{
				if (!add_node(list, create_node(PARENTHESIS, ft_strndup(buffer, i + 1))))
					error_handler_lexer(1, "Malloc error\n");
				return (i + 1);
			}
		}
		i++;
	}
	error_handler_lexer(1, "Parenthesis must be closed.\n");
}

int	double_quote_handler(char *buffer, t_tknlist *list)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (buffer[i])
	{
		if (buffer[i] == '\"')
			count += 1;
		if (count % 2 == 0)
		{
			if (ft_isspace(buffer[i + 1]) || buffer[i + 1] == '|' || buffer[i + 1] == '&' || buffer[i + 1] == '\0')
				break;
		}
		i++;
	}
	if (count % 2 != 0)
		error_handler_lexer(1, "Double quotes must be closed.\n");
	if (!add_node(list, create_node(TWO_QUOTE, ft_strndup(buffer, i + 1))))
		error_handler_lexer(1, "Malloc error\n");
	return (i + 1);
}

int	simple_quote_handler(char *buffer, t_tknlist *list)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (buffer[i])
	{
		if (buffer[i] == '\'')
			count += 1;
		if (count % 2 == 0)
		{
			if (ft_isspace(buffer[i + 1]) || buffer[i + 1] == '|' || buffer[i + 1] == '&' || buffer[i + 1] == '\0')
				break;
		}
		i++;
	}
	if (count % 2 != 0)
		error_handler_lexer(1, "Simple quotes must be closed.\n");
	if (!add_node(list, create_node(ONE_QUOTE, ft_strndup(buffer, i + 1))))
		error_handler_lexer(1, "Malloc error\n");
	return (i + 1);
}



int	pipe_handler(char *buffer, t_tknlist *list)
{
	if (!add_node(list, create_node(PIPE, ft_strndup(buffer, 1))))
		error_handler_lexer(1, "Malloc error\n");
	return (1);
}


int file_handler(char *buffer, t_tknlist *list, e_tkntype type)
{
	int	i;

	i = 1;
	while (ft_isspace(buffer[i]))
		i++;
	while (ft_isspace(buffer[i]) == 0)
		i++;
	if (!add_node(list, create_node(type, ft_strndup(buffer, i))))
		error_handler_lexer(1, "Malloc error\n");
	return (i);
}

int	operator_handler(char *buffer, t_tknlist *list, e_tkntype type)
{
	if (!add_node(list, create_node(type, ft_strndup(buffer, 2))))
		error_handler_lexer(1, "Malloc error\n");
	return (2);
}

int	cmd_handler(char *buffer, t_tknlist *list)
{
	int	i;

	i = 0;
	while (ft_isspace(buffer[i]) == 0 && buffer[i])
		i++;
	if (!add_node(list, create_node(WORD, ft_strndup(buffer, i))))
		error_handler_lexer(1, "Malloc error\n");
	return (i + 1);
}