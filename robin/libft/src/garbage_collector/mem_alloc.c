/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mem_alloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hanglade <hanglade@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/18 14:13:57 by cberganz          #+#    #+#             */
/*   Updated: 2024/01/26 16:28:38 by hanglade         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*case1(unsigned long size, void **ptr, int id)
{
	t_list	*el;

	*ptr = malloc(size);
	if (*ptr == NULL)
		return ((void *)1);
	el = garbage_lstnew(*ptr);
	if (el == NULL)
	{
		free(*ptr);
		return ((void *)1);
	}
	ft_lstadd_front(garbage(id), el);
	return (NULL);
}

void	*mem_alloc(unsigned long size, void **ptr, int id)
{
	t_list	*el;
	void	*ptr2;

	if (ptr != NULL)
		return (case1(size, ptr, id));
	else
	{
		ptr2 = malloc(size);
		if (ptr2 == NULL)
			return (NULL);
		el = garbage_lstnew(ptr2);
		if (el == NULL)
		{
			free(ptr2);
			return (NULL);
		}
		ft_lstadd_front(garbage(id), el);
		return (ptr2);
	}
}
