/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gemartel <gemartel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 13:13:08 by gemartel          #+#    #+#             */
/*   Updated: 2024/02/06 12:34:33 by gemartel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

//A rajouter dans libft
char	*ft_strndup(char *buffer, int len, t_id_gc id)
{
	char	*new;
	int		i;

	i = 0;
	while (buffer[i] && i < len)
		i++;
	new = malloc_gc((i + 1) * sizeof(char), id);
	if (!new)
		return (NULL);
		//error_handler_lexer(ENV, "Malloc error\n");
	i = 0;
	while (buffer[i] && i < len)
	{
		new[i] = buffer[i];
		i++;
	}
	new[i] = '\0';
	return (new);
}

int	is_operator(const char c, const char c2)
{
	if (c == '|' && c2 != '|')
		return (1);
	else if (c == '&' && c2 == '&')
		return (1);
	else if (c == '|' && c2 == '|')
		return (1);
	return (0);
}

void	detect_error_type(const char c)
{
	if (c == ')' || c == ';' || c == '\\')
		error_handler_lexer(1, "Error of type.\n");
}
