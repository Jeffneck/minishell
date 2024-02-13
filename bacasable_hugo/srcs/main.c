/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hanglade <hanglade@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 09:04:25 by gemartel          #+#    #+#             */
/*   Updated: 2024/02/13 15:08:54 by hanglade         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"


int main(int argc, char **argv) 
{
	t_tknlist *list;
	t_token	*curr;
	t_token	*original;
	t_tknlist *list_expnd;
	
	list = lexer(argv[1]);
	curr = list->head;
	original = curr;
	display_tknlist(list);
	list_expnd = expand_wildcard(curr, is_asterisk(curr->content[0]));
	ft_printf("1");
	// while(curr)
	// {
	// 	expand_wildcard(list, curr, is_asterisk(curr->content[0]));
	// 	curr = curr->next;	
	// }
	if (list_expnd->head)
	{
		add_tknlst_in_tknlst_after_target(list, original, list_expnd);
		pop_node_in_place(list, original);
	}
	display_tknlist(list);
	clear_garbage(EXPANDER, free);
	clear_garbage(LEXER, free);
    return argc;
}
