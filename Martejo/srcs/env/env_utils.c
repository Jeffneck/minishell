/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gemartel <gemartel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 11:06:36 by gemartel          #+#    #+#             */
/*   Updated: 2024/02/13 15:04:38 by gemartel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*get_env_path(t_env *env, const char *var, size_t len) //changer nom par get_env_value
{
	char	*path;

	while (env != NULL)
	{
		if (ft_strncmp(env->value, var, len) == 0)
		{
			path = ft_strdup(&env->value[len]);
			if (!path)
				return (NULL); // gerer erreur malloc
			return (path);
		}
		env = env->next;
	}
	return ("");
}

size_t	size_env(t_env *lst)
{
	size_t	len;

	len = 0;
	while (lst)
	{
		if (lst->value != NULL)
		{
			len += ft_strlen(lst->value);
			len += 1;
		}
		lst = lst->next;
	}
	return (len);
}

char	*env_to_str(t_env *lst)
{
	char	*env;
	int		i;
	int		j;

	env = (char *)malloc((size_env(lst) + 1) * sizeof(char));
	if (!env)
		return (NULL); //gerer erreur
	i = 0;
	while (lst)
	{
		if (lst->value != NULL)
		{
			j = 0;
			while (lst->value[j])
			{
				env[i] = lst->value[j]; // a voir pour remplacer par strjoin_gc ou autre
				i++;
				j++;
			}
		}
		if (lst->next)
			env[i++] = '\n';
		lst=lst->next;
	}
	env[i] = 0;
	return (env);
}

