/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc_gc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hanglade <hanglade@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 10:55:12 by hanglade          #+#    #+#             */
/*   Updated: 2024/01/29 14:44:26 by hanglade         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	*realloc_gc(void *ptr, size_t prev_size, size_t new_size, int id_gc)
{
	size_t	size;
	void	*new;

	if (new_size == 0)
		return (del_one_garbage(ptr, id_gc), NULL);
	size = (size_t) max_value(prev_size, new_size, sizeof(size_t));
	new = malloc_gc(size, id_gc);
	if(ptr)
		ft_memcpy(new, ptr, size);
	del_one_garbage(ptr, id_gc);
	return (new);
}