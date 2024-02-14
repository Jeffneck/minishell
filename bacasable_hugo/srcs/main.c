/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hanglade <hanglade@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 09:04:25 by gemartel          #+#    #+#             */
/*   Updated: 2024/02/14 12:07:42 by hanglade         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"


int main(int argc, char **argv) 
{
	t_tknlist *list;
	t_token	*curr;
	
	list = lexer(argv[1]);
	curr = list->head;
	ft_printf("BEFORE------------\n");
	display_tknlist(list);
	ft_printf("AFTER-------------\n");
	expand_wildcard(curr, list);
	clear_garbage(EXPANDER, free);
	clear_garbage(LEXER, free);
    return argc;
}
