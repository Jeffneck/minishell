/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc_gc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hanglade <hanglade@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 14:25:00 by hanglade          #+#    #+#             */
/*   Updated: 2024/01/29 14:44:03 by hanglade         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	*malloc_gc(size_t size, int id_gc)
{
	void	*ptr;

	ptr = malloc(size);
	if (!ptr)
		return (NULL);
	if ((!add_to_garbage(ptr, id_gc)))
		return(free(ptr), NULL);
	return (ptr);
}