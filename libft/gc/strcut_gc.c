/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strcut_gc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hanglade <hanglade@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 14:34:51 by hanglade          #+#    #+#             */
/*   Updated: 2024/02/05 13:35:19 by hanglade         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

char	*strcut_gc(char const *str, size_t cut_begin, size_t cut_end, int id_gc)
{
	char	*dest;
	size_t	i;
	int		len;

	if (!str)
		return (NULL);
	len = ft_strlen(str) - cut_begin - cut_end;
	if (len <= 0)
		return (strdup_gc("", id_gc));
	dest = (char *)calloc_gc((len + 1), sizeof(char), id_gc); //utile pour le \0 automatique ?
	if (!dest)
		return (NULL);
	ft_strlcpy(dest, &str[cut_begin], len); //les char end sont bien retires sans compter le char de fin ?
	dest[len] = '\0'; //utile
	return (dest);
}
