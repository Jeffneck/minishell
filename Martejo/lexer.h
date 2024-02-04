/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gemartel <gemartel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 09:44:39 by gemartel          #+#    #+#             */
/*   Updated: 2024/01/29 09:57:39 by gemartel         ###   ########.fr       */
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
	PARENTHESIS,
} e_tkntype;

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
	e_tkntype		type;
	char			*content;
	int				link;
	struct s_token	*next;
	struct s_token	*prev;
}	t_token;

typedef struct s__tknlist
{
	struct s_token	*head;
	struct s_token	*tail;
}	t_tknlist;

t_token *create_node(e_tkntype type, char *content, int link);
int add_node(t_tknlist *list, t_token *node);
void free_lister(t_tknlist *list);
t_tknlist	*init_list(t_tknlist **list);

/**lexer**/
t_tknlist	*lexer(char *buffer);
char *ft_strndup(char *buffer, int len);

/**token_parse**/
int parenthese_handler(char *buffer, t_tknlist *list);
int	double_quote_handler(char *buffer, t_tknlist *list);
int	simple_quote_handler(char *buffer, t_tknlist *list);
int	pipe_handler(char *buffer, t_tknlist *list);
int file_handler(char *buffer, t_tknlist *list, e_tkntype type);
int	operator_handler(char *buffer, t_tknlist *list, e_tkntype type);
int	cmd_handler(char *buffer, t_tknlist *list);

/**error**/
void error_handler_lexer(int id_gc, char *msg);

#endif