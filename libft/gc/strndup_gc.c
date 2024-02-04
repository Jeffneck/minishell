/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strdup_gc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hanglade <hanglade@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 14:33:21 by hanglade          #+#    #+#             */
/*   Updated: 2024/01/29 14:47:36 by hanglade         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

char	*strndup_gc(const char *str, size_t size, int id_gc)
{
	char	*result;
	size_t	length;
	size_t	i;

	if (!str)
		return (NULL);
	length = ft_strlen(str);
	if(size < length)
		length = size;
	result = (char *)malloc_gc((length + 1) * sizeof(char), id_gc);
	if (!result)
		return (NULL);
	i = 0;
	while (i < length)
	{
		result[i] = str[i];
		i++;
	}
	result[i] = '\0';
	return (result);
}
