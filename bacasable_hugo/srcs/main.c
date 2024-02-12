/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hanglade <hanglade@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 09:04:25 by gemartel          #+#    #+#             */
/*   Updated: 2024/02/12 18:31:50 by hanglade         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"


int main(int argc, char **argv) 
{
	t_tknlist *list;
	t_token	*curr;
	t_token	*original;
	
	list = lexer(argv[1]);
	curr = list->head;
	original = curr;
	display_tknlist(list);
	expand_wildcard(list, curr, is_asterisk(curr->content[0]));
	
	// while(curr)
	// {
	// 	expand_wildcard(list, curr, is_asterisk(curr->content[0]));
	// 	curr = curr->next;	
	// }
	// pop_node_in_place(list, original);
	display_tknlist(list);
	clear_garbage(EXPANDER, free);
	clear_garbage(LEXER, free);
    return argc;
}
