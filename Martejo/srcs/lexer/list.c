/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gemartel <gemartel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 13:15:16 by gemartel          #+#    #+#             */
/*   Updated: 2024/02/13 10:01:43 by gemartel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_token	*create_node(t_tkntype type, char *content, int link)
{
	t_token	*node;

	if (!content)
		return (NULL);
	node = malloc_gc(sizeof(t_token), LEXER);
	if (!node)
		error_handler_lexer(1, "Malloc error\n");
	node->content = content;
	node->type = type;
	node->link = link;
	node->next = NULL;
	node->prev = NULL;
	return (node);
}

int	add_node(t_tknlist *list, t_token *node)
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

void	init_list(t_tknlist **list)
{
	(*list) = malloc_gc(sizeof(t_tknlist), LEXER);
	if (!(*list))
		error_handler_lexer(1, "Malloc error\n");
	(*list)->head = NULL;
	(*list)->tail = NULL;
}
