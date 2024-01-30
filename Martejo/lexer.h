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

typedef enum
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
	TWO_QUOTE,
	PARENTHESE,
	WORD
} token_type;

typedef struct	s_token
{
	token_type		type;
	char			*content;
	struct s_token	*next;
	struct s_token	*prev;
}	t_token;

typedef struct s_lister
{
	struct s_token	*head;
	struct s_token	*tail;
}	t_lister;

#endif