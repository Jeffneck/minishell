#include "lexer.h"

int	detect_type(const char c, const char c2)
{
	if (c == '\'')
		return (ONE_QUOTE);
	else if (c == '\"')
		return (TWO_QUOTE);
	else if (c == '(')
		return (LEFT_PAR);
	else if (c == '|' && c2 != '|')
		return (PIPE);
	else if (c == '<' && c2 != '<')
		return (IN);
	else if (c == '>' && c2 != '>')
		return (OUT);
	else if (c == '>' && c2 == '>')
		return (APPEND);
	else if (c == '<' && c2 == '<')
		return (HEREDOC);
	else if (c == '&' && c2 == '&')
		return (AND);
	else if (c == '|' && c2 == '|')
		return (OR);
	else if (c == '*')
		return (WILDCARD);
	return (WORD);
}

int	detect_error_type(const char c)
{
	if (c == ')' || c == ';')
		return (-1);
	return (1);
}

char *ft_strndup(char *buffer, int len)
{
	char *new;
	int i;

	i = 0;
	while (buffer[i] && i < len)
		i++;
	new = (char *)malloc((i + 1) * sizeof(char));
	if (!new)
		return (NULL);
	i = 0;
	while (buffer[i] && i < len)
	{
		new[i] = buffer[i];
		i++;
	}
	new[i] = '\0';
	return (new);
}

void	handle_token(char *buffer, t_lister *list, token_type type, int *i, e_error *error)
{
	if (type == TWO_QUOTE)
		*i += double_quote_handler(buffer, list, error);
	else if (type == ONE_QUOTE)
		*i += simple_quote_handler(buffer, list, error);
	else if (type == PIPE)
		*i += pipe_handler(buffer, list, error);
	else if (type == AND || type == OR)
		*i += operator_handler(buffer, list, type, error);
	else if (type == HEREDOC || type == IN || type == OUT || type == APPEND)
		*i += file_handler(buffer, list, type, error);
	else if (type == LEFT_PAR)
		*i += parenthese_handler(buffer, list, error);
	else if (type == WORD)
		*i += cmd_handler(buffer, list, error);
}

void	lexer(char *buffer, t_lister *list)
{
	int			i;
	token_type	type;
	e_error		error;

	if (!buffer)
		return ;
	i = 0;
	error = -1;
	while (buffer[i])
	{
		if (!ft_isspace(buffer[i]))
		{
			if (!detect_error_type(buffer[i]))
				error_handler(list, buffer, "Error type\n");
			type = detect_type(buffer[i], buffer[i + 1]);
			handle_token(&buffer[i], list, type, &i, &error);
			if (error >= 0)
			{
				printf("%d\n", error);
				error_handler(list, buffer, "Error\n");
			}
		}
		else
			i++;
	}
	return ;
}