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

enum token_type
{
	CMD,
	ARG,
	IN,
	OUT,
	HEREDOC,
	APPEND,
	PIPE,
	SEMICOLON,
	AND,
	OR,
	LEFT_PAR,
	RIGHT_PAR,
	ONE_QUOTE,
	TWO_QUOTE
};

typedef struct	s_token
{
	int				type;
	char			*content;
	struct s_token	*next;
	struct s_token	*prev;
}	t_token;

typedef struct s_list
{
	struct s_token	*head;
	struct s_token	*tail;
}	t_list;

#endif