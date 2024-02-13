/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gemartel <gemartel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 11:01:46 by gemartel          #+#    #+#             */
/*   Updated: 2024/02/13 10:47:52 by gemartel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	strlen_2d(char **tab)
{
	int		i;

	i = 0;
	while (tab[i])
		i++;
	return (i);
}

void	sort_env(char **tab, int len_env)
{
	int		i;
	int		flag;
	char	*tmp;

	flag = 0;
	while (tab && flag == 0)
	{
		flag = 1;
		i = 0;
		while (i < len_env - 1)
		{
			if (ft_strcmp(tab[i], tab[i + 1]) > 0)
			{
				tmp = tab[i];
				tab[i] = tab[i + 1];
				tab[i + 1] = tmp;
				flag = 0;
			}
			i++;
		}
		len_env--; // a voir si utile
	}
}

void	print_sorted_env(t_env *env)
{
	int		i;
	char	**tab;
	char	*str_env;

	i = 0;
	str_env = env_to_str(env);
	if (!str_env)
		return ; //gerer erreur
	tab = ft_split(str_env, '\n');
	if (!tab)
		return ; //gerer erreur et liberation
	free(str_env);
	sort_env(tab, strlen_2d(tab));
	while (tab[i])
		ft_printf("declare -x %s\n", tab[i++]);
	//liberer tableau 2d;
}