/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_handler_suite.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hanglade <hanglade@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 13:20:40 by gemartel          #+#    #+#             */
/*   Updated: 2024/02/15 11:19:53 by hanglade         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	parenthese_handler(char *buffer, t_tknlist *list)
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
				if (!add_node(list,
						create_node(PARENTHESIS, ft_strndup(buffer, i + 1, TKN_LIST), 0)))
					error_handler_lexer(1, "Malloc error\n");
				return (i + 1);
			}
		}
		i++;
	}
	error_handler_lexer(1, "Parenthesis must be closed.\n");
	return (0);
}

int	double_quote_handler(char *buffer, t_tknlist *list)
{
	int	i;
	int	link;

	i = 1;
	link = 0;
	while (buffer[i])
	{
		if (buffer[i] == '\"')
			break ;
		i++;
	}
	if (buffer[i] == '\0')
		error_handler_lexer(1, "Double quotes must be closed.\n");
	if (ft_isspace(buffer[i + 1]) == 0)
		link = 1;
	if (!add_node(list,
			create_node(TWO_QUOTE, ft_strndup(buffer, i + 1, TKN_LIST), link)))
		error_handler_lexer(1, "Malloc error\n");
	return (i + 1);
}

int	operator_handler(char *buffer, t_tknlist *list, t_tkntype type)
{
	if (!add_node(list, create_node(type, ft_strndup(buffer, 2, TKN_LIST), 0)))
		error_handler_lexer(1, "Malloc error\n");
	return (2);
}
