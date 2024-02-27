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

	if (!env)
		return (0);
	i = 0;
	while (env[i] && env[i] != '=')
		i++;
	return (i);
}

int	unset(char **value, t_env **env)
{
	t_env	*tmp;
	t_env	*curr;
	t_env	*node;

	ft_printf("UNSET\n");
	
	if (!(value[1]))
		return (0);
	// if (size_env(env) == 1)
	// 	env_add(NULL, &env, 0); // Peut etre creer un node vierge des le debut pour eviter conflit si liste vide par unset, a voir ce qui se passe dans la boucle
	// 	// Mofidier nom fonctions size_env et env_size, on comprends rien
	if (ft_strncmp(value[1], (*env)->value, (int)env_size((*env)->value)) == 0)
	{
		tmp = *env;
		if ((*env)->next)
			*env = (*env)->next;
		else
			*env = NULL;
		del_one_garbage(tmp, ENV);
		return (0);
	}
	curr = *env;
	while (curr && curr->next)
	{
		if (ft_strncmp(value[1], curr->next->value, env_size(curr->next->value)) == 0)
		{
			tmp = curr->next->next;
			node = curr->next;
			ft_printf("value = %s", curr->next->value);
			del_one_garbage(node->value, ENV);
			del_one_garbage(node, ENV);

			curr->next = tmp;
			ft_printf("value = %s", curr->next->value);
			return (0);
		}
		curr = curr->next;
	}
	return (0);
}