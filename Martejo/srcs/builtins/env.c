/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gemartel <gemartel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 11:14:00 by gemartel          #+#    #+#             */
/*   Updated: 2024/02/12 15:36:37 by gemartel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	env(t_env *env)
{
	while (env)
	{
		if (env->secret == 0 && env->value != NULL)
			ft_printf("%s\n", env->value);
		env = env->next;
	}
	return (0);
}