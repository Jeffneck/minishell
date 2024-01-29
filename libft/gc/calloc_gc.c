/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calloc_gc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hanglade <hanglade@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 11:01:01 by hanglade          #+#    #+#             */
/*   Updated: 2024/01/29 11:05:43 by hanglade         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	*calloc_gc(size_t nmemb, size_t size, int id_gc)
{
	void	*ptr; 

	ptr = malloc_gc(nmemb * size, id_gc);
	if (!ptr)
		return (NULL);
	ft_bzero(ptr, nmemb *size);
	return (ptr);
}