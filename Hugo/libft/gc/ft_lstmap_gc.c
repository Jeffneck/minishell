/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_gc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hanglade <hanglade@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 14:12:31 by hanglade          #+#    #+#             */
/*   Updated: 2024/01/29 14:38:21 by hanglade         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap_gc(t_list *lst, void *(*f)(void *), void (*del)(void *),
	int id_gc)
{
	t_list	*nlst;
	t_list	*elem;

	if (!lst || !f)
		return (NULL);
	nlst = NULL;
	while (lst)
	{
		elem = ft_lstnew(f(lst->content), garbage);
		if (!elem)
		{
			clear_garbage(del, id_gc);
			return (NULL);
		}
		ft_lstadd_back(&nlst, elem);
		lst = lst->next;
	}
	return (nlst);
}