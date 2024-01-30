#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include "libft/libft.h"
#include "lexer.h"

t_token *create_node(int type, char *content)
{
	t_token	*node;

	if (!content)
		return (NULL);
	node = (t_token *)malloc(1 * sizeof(t_token));
	if (!node)
		return (NULL);
	node->content = content;
	node->type = type;
	node->next = NULL;
	node->prev = NULL;
	return (node);
}


int add_node(t_lister *list, t_token *node)
{
	if (!node)
		return (-1);
	if (!list->head)
	{
		list->head = node;
		list->tail = node;
	}
	else
	{
		node->prev = list->tail;
		list->tail->next = node;
		list->tail = node;
	}
	return (1);
}

int	detect_chr(const char c, const char c2)
{
	if (c == '\'')
		return (ONE_QUOTE);
	else if (c == '\"')
		return (TWO_QUOTE);
	else if (c == '(')
		return (LEFT_PAR);
	else if (c == ')')
		return (RIGHT_PAR);
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
	else if (ft_isspace(c))
		return (-1);
	return (0);
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

int parenthese_handler(char *buffer, t_lister *list)
{
	int i = 0;
	int depth = 0;
	int start = -1;

	while (buffer[i])
	{
		if (buffer[i] == '(')
		{
			if (depth == 0)
				start = i;
			depth++;
		}
		else if (buffer[i] == ')')
		{
			depth--;
			if (depth == 0)
			{
				if (!add_node(list, create_node(PARENTHESE, ft_strndup(buffer, i + 1))))
				{
					printf("malloc_error\n");
					exit(EXIT_FAILURE);
				}
				return (i + 1);
			}
		}
		i++;
	}
	if (depth != 0)
	{
		printf("Unbalanced parentheses\n");
		exit(EXIT_FAILURE);
	}
	return i;
}

int double_quote_handler(char *buffer, t_lister *list)
{
	int i = 0;
	int count = 0;
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
		exit(EXIT_FAILURE);
	else
	{
		if (!add_node(list, create_node(TWO_QUOTE, ft_strndup(buffer, i + 1))))
		{
			printf("malloc_error\n");
			exit (EXIT_FAILURE);
		}
		
	}
	return (i + 1);
}


int	simple_quote_handler(char *buffer, t_lister *list)
{
	int i = 0;
	int count = 0;
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
		exit(EXIT_FAILURE);
	else
	{
		if (!add_node(list, create_node(TWO_QUOTE, ft_strndup(buffer, i + 1))))
		{
			printf("malloc_error\n");
			exit (EXIT_FAILURE);
		}
		
	}
	return (i + 1);
}

int	pipe_handler(char *buffer, t_lister *list)
{
	if (!add_node(list, create_node(PIPE, ft_strndup(buffer, 1))))
	{
		printf("malloc_error\n");
		exit (EXIT_FAILURE);
	}
	return (1);
}

int and_handler(char *buffer, t_lister *list)
{
	if (!add_node(list, create_node(AND, ft_strndup(buffer, 2))))
	{
		printf("malloc_error\n");
		exit (EXIT_FAILURE);
	}
	return (2);
}


int or_handler(char *buffer, t_lister *list)
{
	if (!add_node(list, create_node(OR, ft_strndup(buffer, 2))))
	{
		printf("malloc_error\n");
		exit (EXIT_FAILURE);
	}
	return (2);
}

int heredoc_handler(char *buffer, t_lister *list)
{
	int	i;

	i = 2;
	while (ft_isspace(buffer[i]))
		i++;
	while (ft_isspace(buffer[i]) == 0)
		i++;
	if (!add_node(list, create_node(HEREDOC, ft_strndup(buffer, i))))
	{
		printf("malloc_error\n");
		exit (EXIT_FAILURE);
	}
	return (i);
}

int in_handler(char *buffer, t_lister *list)
{
	int	i;

	i = 1;
	while (ft_isspace(buffer[i]))
		i++;
	while (ft_isspace(buffer[i]) == 0)
		i++;
	if (!add_node(list, create_node(IN, ft_strndup(buffer, i))))
	{
		printf("malloc_error\n");
		exit (EXIT_FAILURE);
	}
	return (i);
}

int out_handler(char *buffer, t_lister *list)
{
	int	i;

	i = 1;
	while (ft_isspace(buffer[i]))
		i++;
	while (ft_isspace(buffer[i]) == 0)
		i++;
	if (!add_node(list, create_node(OUT, ft_strndup(buffer, i))))
	{
		printf("malloc_error\n");
		exit (EXIT_FAILURE);
	}
	return (i);
}

int append_handler(char *buffer, t_lister *list)
{
	int	i;

	i = 2;
	while (ft_isspace(buffer[i]))
		i++;
	while (ft_isspace(buffer[i]) == 0)
		i++;
	if (!add_node(list, create_node(APPEND, ft_strndup(buffer, i))))
	{
		printf("malloc_error\n");
		exit (EXIT_FAILURE);
	}
	return (i);
}

int	cmd_handler(char *buffer, t_lister *list)
{
	int	i;

	i = 0;
	while (ft_isspace(buffer[i]) == 0 && buffer[i])
		i++;
	if (!add_node(list, create_node(WORD, ft_strndup(buffer, i))))
	{
		printf("malloc_error\n");
		exit (EXIT_FAILURE);
	}
	return (i + 1);
	
}

//buffer = prompt

// void	process_token(char *buffer, t_lister *list)
// {
// 	int	i;
// 	token_type type;

// 	i = 0;
// 	while (buffer[i])
// 	{
// 		type = detect_chr(buffer[i], buffer[i + 1]);
// 		if (type == TWO_QUOTE){
// 			i += double_quote_handler(&buffer[i], list);
// 			continue;
// 		}
// 		else if (type == ONE_QUOTE){
// 			i += simple_quote_handler(&buffer[i], list);
// 			continue;
// 		}
// 		if (type == PIPE){
// 			i += pipe_handler(&buffer[i], list);
// 			continue ;
// 		}
// 		if (type == AND){
// 			i += and_handler(&buffer[i], list);
// 			continue ;
// 		}
// 		if (type == OR){
// 			i += or_handler(&buffer[i], list);
// 			continue ;
// 		}
// 		if (type == HEREDOC){
// 			i += heredoc_handler(&buffer[i], list);
// 			continue ;
// 		}
// 		if (type == IN){
// 			i += in_handler(&buffer[i], list);
// 			continue ;
// 		}
// 		if (type == OUT){
// 			i += out_handler(&buffer[i], list);
// 			continue ;
// 		}
// 		if (type == APPEND){
// 			i += append_handler(&buffer[i], list);
// 			continue ;
// 		}
// 		if (type == LEFT_PAR){
// 			i += parenthese_handler(&buffer[i], list);
// 			continue ;
// 		}
// 		if (type == CMD){
// 			i += cmd_handler(&buffer[i], list);
// 			continue;
// 		}
//  		i++;
// 	}
// 	return ;
// }

void	handle_token(char *buffer, t_lister *list, token_type type, int *i)
{
	if (type == TWO_QUOTE)
			double_quote_handler(&buffer[*i], list, i);
	else if (type == ONE_QUOTE)
		i += simple_quote_handler(&buffer[*i], list, i);
	if (type == PIPE)
		i += pipe_handler(&buffer[*i], list, i);
	if (type == AND)
		i += and_handler(&buffer[*i], list, i);
	if (type == OR)
		i += or_handler(&buffer[*i], list, i);
	if (type == HEREDOC){
		i += heredoc_handler(&buffer[*i], list, i);
	if (type == IN){
		i += in_handler(&buffer[*i], list, i);
	if (type == OUT)
		out_handler(&buffer[*i], list, i);
	if (type == APPEND)
		append_handler(&buffer[*i], list, i);
	if (type == LEFT_PAR)
		parenthese_handler(&buffer[*i], list, i);
	if (type == CMD)
		cmd_handler(&buffer[*i], list, i);
	if (type == CMD){
		i += cmd_handler(&buffer[*i], list);
	if (ft_isspace(buffer[*i]))
		space_handler(&buffer[*i], i);
}

void	process_token(char *buffer, t_lister *list)
{
	int	i;
	token_type type;

	i = 0;
	while (buffer[i])
	{
		type = detect_chr(buffer[i], buffer[i + 1]);
		
	}
	return ;
}

int	main()
{
	char	*buffer;
	char	*pwd;
	t_lister	list;

	list.head = NULL;
	list.tail = NULL;
	pwd = getcwd(NULL, 0);
	pwd = ft_strjoin(pwd, " $> ");
	while (1)
	{
		buffer = readline(pwd);
		
		if (buffer == NULL || strcmp(buffer, "exit") == 0)
			break;
		printf("%s\n", buffer);
		process_token(buffer, &list);
		t_token *node;
		node = list.head;
		while (node)
		{
			printf("type = %d\n", node->type);
			printf("content = %s\n", node->content);
			node = node->next;
		}
		
		free(buffer);
	}
	if (buffer)
		free(buffer);
	if (pwd)
		free(pwd);
	rl_clear_history();	
}