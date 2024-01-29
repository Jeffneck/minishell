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

	node = (t_token *)malloc(1 * sizeof(t_token));
	if (!node)
		return (NULL);
	node->content = content;
	node->type = type;
	node->next = NULL;
	node->prev = NULL;
	return (node);
}


int add_node(t_list **list, t_token *node)
{
	if (!node)
		return (-1);
	if (!(*list)->head)
	{
		(*list)->head = node;
		(*list)->tail = node;
	}
	else
	{
		node->prev = (*list)->tail;
		(*list)->tail->next = node;
		(*list)->tail = node;
	}
	return (1);
}

int	detect_chr(const char c)
{
	if (c == '\'')
		return (ONE_QUOTE);
	else if (c == '\"')
		return (TWO_QUOTE);
	else if (c == '(')
		return (LEFT_PAR);
	else if (c == ')')
		return (RIGHT_PAR);
	else if (c == '|')
		return (PIPE);
	else if (c == '<')
		return (IN);
	else if (c == '>')
		return (OUT);
	else if (c == '>>')
		return (APPEND);
	else if (c == '<<')
		return (HEREDOC);
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

void	double_quote_handler(char *buffer)
{

}

void	simple_quote_handler(char *buffer)
{

}


void	process_token(char *buffer, t_list *list)
{
	int	i;
	int	flag;
	int type;

	i = 0;
	while (buffer[i])
	{
		type = detect_chr(buffer[i]);
		flag = i;
		if (type == 0)
		{
			while (ft_isspace(buffer[i]) == 0 && buffer[i])
				i++;
			if (!add_node(&list, create_node(type, ft_strndup(&buffer[flag], i - flag))))
				return (-1);
		}
		else
		{
			if (type == ONE_QUOTE)
				simple_quote_handler(&buffer[i]);
			else if (type == TWO_QUOTE)
				double_quote_handler(&buffer[i]);
			
		}	
	}	
}

int	main()
{
	char	*buffer;
	char	*pwd;
	t_list	list;

	list.head = NULL;
	list.tail = NULL;
	pwd = getcwd(NULL, 0);
	pwd = ft_strjoin(pwd, " $> ");
	while (1)
	{
		buffer = readline(pwd);
		
		if (buffer == NULL || strcmp(buffer, "exit") == 0)
			break;
		free(buffer);
	}
	if (buffer)
		free(buffer);
	if (pwd)
		free(pwd);
	rl_clear_history();
	if (cmd != NULL)
		free_array(&cmd);
	
}