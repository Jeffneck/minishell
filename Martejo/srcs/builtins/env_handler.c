/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_handler.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gemartel <gemartel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 09:10:47 by gemartel          #+#    #+#             */
/*   Updated: 2024/02/06 12:34:15 by gemartel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Bien gerer les erreurs et liberation en cas d'echec

 void env_add_back(t_env **env, t_env *new)
{
	t_env *tmp;

	if (*env == NULL)
		*env = new;
	else
	{
		tmp = *env;
		while (tmp->next != NULL)
			tmp = tmp->next;
		tmp->next = new;
	}
}


t_env	*init_env(char **env_array)
{
	t_env	*env;
	t_env	*new;
	int		i;

	if (!env_array)
		return (NULL); // gerer erreur
	i = 0;
	env = NULL;
	while (env_array[i] != NULL)
	{
		new = malloc_gc(sizeof(env), ENV);
		if (!new)
			return (NULL); //gerer erreur
		new->value = ft_strndup(env_array[i], ft_strlen(env_array[i]), ENV);
		new->next = NULL;
		env_add_back(&env, new);
		i++;
	}
	return (env);
}