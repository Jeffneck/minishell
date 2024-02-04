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
	return (WORD);
}

int	is_operator(const char c, const char c2)
{
	if (c == '|' && c2 != '|')
		return (1);
	else if (c == '&' && c2 == '&')
		return (1);
	else if (c == '|' && c2 == '|')
		return (1);
	return (0);

}

void	detect_error_type(const char c)
{
	if (c == ')' || c == ';' || c == '\\')
		error_handler_lexer(1, "Error of type.\n");
}

char *ft_strndup(char *buffer, int len)
{
	char	*new;
	int		i;

	i = 0;
	while (buffer[i] && i < len)
		i++;
	new = malloc_gc((i + 1) * sizeof(char), 1);
	if (!new)
		error_handler_lexer(1, "Malloc error\n");
	i = 0;
	while (buffer[i] && i < len)
	{
		new[i] = buffer[i];
		i++;
	}
	new[i] = '\0';
	return (new);
}

void	handle_token(char *buffer, t_tknlist *list, e_tkntype type, int *i)
{
	if (type == TWO_QUOTE)
		*i += double_quote_handler(buffer, list);
	else if (type == ONE_QUOTE)
		*i += simple_quote_handler(buffer, list);
	else if (type == PIPE)
		*i += pipe_handler(buffer, list);
	else if (type == AND || type == OR)
		*i += operator_handler(buffer, list, type);
	else if (type == HEREDOC || type == IN || type == OUT || type == APPEND)
		*i += file_handler(buffer, list, type);
	else if (type == LEFT_PAR)
		*i += parenthese_handler(buffer, list);
	else if (type == WORD)
		*i += cmd_handler(buffer, list);
}

t_tknlist	*lexer(char *buffer)
{
	int			i;
	int			flag;
	e_tkntype	type;
	e_tkntype	type_flag;
	t_tknlist	*list;
	
	if (!buffer)
		return (NULL);//close
	init_list(&list);
	i = 0;
	flag = 0;
	while (buffer[i])
	{
		if (!ft_isspace(buffer[i]))
		{
			detect_error_type(buffer[i]);
			type = detect_type(buffer[i], buffer[i + 1]);
			handle_token(&buffer[i], list, type, &i);
			
		}
		else
			i++;
	}
	return (list);
}