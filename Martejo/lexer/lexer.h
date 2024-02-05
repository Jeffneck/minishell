/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gemartel <gemartel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 09:44:39 by gemartel          #+#    #+#             */
/*   Updated: 2024/02/05 13:35:01 by gemartel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>
# include <string.h>
# include <stdio.h>
# include <unistd.h>
# include <sys/wait.h>
# include <signal.h>
# include "libft/libft.h"

typedef enum e_tkntype
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
}	t_tkntype;

typedef enum e_error
{
	NOTHING = -1,
	MALLOC_E,
	FILE_E,
	TYPE_E
}	t_error;

/**list**/
typedef struct s_token
{
	t_tkntype		type;
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

t_token		*create_node(t_tkntype type, char *content, int link);
int			add_node(t_tknlist *list, t_token *node);
void		init_list(t_tknlist **list);

/**lexer**/
t_tknlist	*lexer(char *buffer);
int			detect_type(const char c, const char c2);
void		handle_token(char *buffer, t_tknlist *list, t_tkntype type, int *i);

/**lexer_utils**/
char		*ft_strndup(char *buffer, int len);
int			is_operator(const char c, const char c2);
void		detect_error_type(const char c);

/**token_handler**/
int			simple_quote_handler(char *buffer, t_tknlist *list);
int			pipe_handler(char *buffer, t_tknlist *list);
int			file_handler(char *buffer, t_tknlist *list, t_tkntype type);
int			cmd_handler(char *buffer, t_tknlist *list);

/**token_handler_suite**/
int			parenthese_handler(char *buffer, t_tknlist *list);
int			double_quote_handler(char *buffer, t_tknlist *list);
int	operator_handler(char *buffer, t_tknlist *list, t_tkntype type);

/**error**/
void		error_handler_lexer(int id_gc, char *msg);

#endif