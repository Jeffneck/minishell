/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hanglade <hanglade@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 09:44:39 by gemartel          #+#    #+#             */
/*   Updated: 2024/01/31 17:01:09 by hanglade         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
#define LEXER_H

#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include "libft/libft.h"

typedef enum
{
	WORD,
	IN,
	OUT,
	HEREDOC,
	APPEND,
	PIPE,
	AND,
	OR,
	LEFT_PAR,
	ONE_QUOTE,
	TWO_QUOTE,
	PARENTHESE,
	WILDCARD //retirer cette merde
} token_type;

typedef enum
{
	NOTHING = -1,
	MALLOC_E,
	FILE_E,
	TYPE_E
} e_error;

/**list**/
typedef struct	s_token
{
	token_type		type;
	char			*content;
	struct s_token	*next;
	struct s_token	*prev;
	int				index; //
	int				priority;
}	t_token;

typedef struct s_lister
{
	struct s_token	*head;
	struct s_token	*tail;
}	t_lister;

t_token *create_node(int type, char *content);
int add_node(t_lister *list, t_token *node);
void free_lister(t_lister *list);

/**lexer**/
void	lexer(char *buffer, t_lister *list);
char *ft_strndup(char *buffer, int len);

/**token_parse**/
int parenthese_handler(char *buffer, t_lister *list, e_error *error);
int	double_quote_handler(char *buffer, t_lister *list, e_error *error);
int	simple_quote_handler(char *buffer, t_lister *list, e_error *error);
int	pipe_handler(char *buffer, t_lister *list, e_error *error);
int file_handler(char *buffer, t_lister *list, token_type type, e_error *error);
int	operator_handler(char *buffer, t_lister *list, token_type type, e_error *error);
int	cmd_handler(char *buffer, t_lister *list, e_error *error);

/**error**/
void error_handler(t_lister *list, char *buffer, char *msg);

#endif