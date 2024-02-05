/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gemartel <gemartel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 13:19:28 by gemartel          #+#    #+#             */
/*   Updated: 2024/02/05 14:21:01 by gemartel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

int	simple_quote_handler(char *buffer, t_tknlist *list)
{
	int	i;
	int	link;

	i = 1;
	link = 0;
	while (buffer[i])
	{
		if (buffer[i] == '\'')
			break ;
		i++;
	}
	if (buffer[i] == '\0')
		error_handler_lexer(1, "Simple quotes must be closed.\n");
	if (ft_isspace(buffer[i + 1]) == 0)
		link = 1;
	if (!add_node(list,
			create_node(ONE_QUOTE, ft_strndup(buffer, i + 1), link)))
		error_handler_lexer(1, "Malloc error\n");
	return (i + 1);
}

int	pipe_handler(char *buffer, t_tknlist *list)
{
	if (!add_node(list, create_node(PIPE, ft_strndup(buffer, 1), 0)))
		error_handler_lexer(1, "Malloc error\n");
	return (1);
}

int	file_handler(char *buffer, t_tknlist *list, t_tkntype type)
{
	int	i;

	if (type == APPEND || type == HEREDOC)
		i = 2;
	else
		i = 1;
	if (!add_node(list, create_node(type, ft_strndup(buffer, i), 0)))
		error_handler_lexer(1, "Malloc error\n");
	return (i);
}

int	cmd_handler(char *buffer, t_tknlist *list)
{
	int	i;
	int	link;

	i = 0;
	link = 0;
	while (buffer[i] != '\0' && ft_isspace(buffer[i]) == 0
		&& is_operator(buffer[i], buffer[i + 1]) == 0)
	{
		if (buffer[i] == '\'' || buffer[i] == '\"')
			break ;
		i++;
	}
	if (buffer[i] != '\0' && ft_isspace(buffer[i]) == 0)
		link = 1;
	if (!add_node(list, create_node(WORD, ft_strndup(buffer, i), link)))
		error_handler_lexer(1, "Malloc error\n");
	return (i);
}
