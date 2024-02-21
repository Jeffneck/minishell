/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   del_one_garbage.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hanglade <hanglade@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 14:25:41 by hanglade          #+#    #+#             */
/*   Updated: 2024/01/29 14:37:12 by hanglade         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	del_one_garbage(void *ptr_to_free, int id_gc)
{
	t_list	**alst;
	t_list	*tmp;
	t_list	*prev_el;

	alst = garbage_ptr(id_gc);
	tmp = *alst;
	if (tmp && tmp->content == ptr_to_free)
	{
		*alst = tmp->next;
		ft_lstdelone(tmp, free);
		tmp = NULL;
	}
	while (tmp && tmp->content != ptr_to_free)
	{
		prev_el = tmp;
		tmp = tmp->next;
	}
	if (tmp && tmp->content == ptr_to_free)
	{
		prev_el->next = tmp->next;
		ft_lstdelone(tmp, free);
		tmp = NULL;
	}
}