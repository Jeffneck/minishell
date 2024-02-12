/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gemartel <gemartel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 13:32:40 by gemartel          #+#    #+#             */
/*   Updated: 2024/02/12 15:27:13 by gemartel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

size_t	env_size(char *env)
{
	size_t		i;

	i = 0;
	while (env[i] && env[i] != '=')
		i++;
	return (i);
}

int	unset(char **value, t_env *env)
{
	t_env	*tmp;
	
	if (!(value[1]))
	{
		ft_printf("unset: not enough arguments\n");
		return (0);
	}
	if (ft_strncmp(value[1], env->value, env_size(env->value)) == 0)
	{
		tmp = env;
		if (env->next)
			env = env->next;
		else
			env = NULL;
		del_one_garbage(tmp, ENV);
		return (1);
	}
	while (env && env->next)
	{
		if (ft_strncmp(value[1], env->next->value, env_size(env->next->value)) == 0)
		{
			tmp = env->next->next;
			del_one_garbage(env->next, ENV);
			env->next = tmp;
			return (1);
		}
		env = env->next;
	}
	return (0);
}