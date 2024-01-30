/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   char2dup_gc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hanglade <hanglade@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/12 13:47:49 by hanglade          #+#    #+#             */
/*   Updated: 2024/01/15 19:11:38 by hanglade         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

char	**char2dup_gc(char **char2, int id_gc)
{
	size_t	i;
	size_t	size;
	char	**char2dup;

	i = 0;
	size = char2len(char2) + 1;
	char2dup = (char **) calloc_gc(size, sizeof(char *), id_gc);
	if (!char2dup)
		return (NULL);
	while (i < size - 1)
	{
		char2dup[i] = strdup_gc(char2[i], id_gc);
		if (!char2dup[i])
			return (clear_garbage(id_gc, free), NULL);
		i++;
	}
	return (char2dup);
}
