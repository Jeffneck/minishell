/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_addptr.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hanglade <hanglade@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/19 12:24:27 by cberganz          #+#    #+#             */
/*   Updated: 2024/01/26 17:31:05 by hanglade         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"


//ne malloc une liste[id] que lorsque l' id est recu en argument
void	*garbage_addptr(void *ptr, int id)
{
	t_list	*el[10]; //tableau de 10 inutile

	el[id] = garbage_lstnew(ptr);
	if (el[id] == NULL)
		return (NULL);
	ft_lstadd_front(garbage(id), el[id]);
	return (ptr);
}
