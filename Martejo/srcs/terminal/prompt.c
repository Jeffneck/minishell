/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gemartel <gemartel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 12:48:48 by gemartel          #+#    #+#             */
/*   Updated: 2024/02/06 12:55:23 by gemartel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*create_prompt(void)
{
	char	buffer[PATH_MAX];
	char	*prompt;

	if (getcwd(buffer, PATH_MAX) == NULL)
		return (NULL);
	prompt = ft_strjoin(buffer, "$");
	return (prompt);
}
