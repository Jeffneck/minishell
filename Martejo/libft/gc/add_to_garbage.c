/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_to_garbage.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hanglade <hanglade@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 14:32:20 by hanglade          #+#    #+#             */
/*   Updated: 2024/01/29 14:39:54 by hanglade         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	*add_to_garbage(void *ptr, int id_gc)
{
	if (id_gc < 0 || id_gc >= GARBAGE_SIZE)
        return NULL;
	t_list *el = ft_lstnew(ptr);
    if (el == NULL)
        return (free(ptr),NULL);
    ft_lstadd_front(garbage_ptr(id_gc), el);
    return ptr;
}