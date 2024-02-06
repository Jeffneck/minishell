/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gemartel <gemartel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 09:04:25 by gemartel          #+#    #+#             */
/*   Updated: 2024/02/06 12:46:39 by gemartel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	main(int argc, char **argv, char **env)
{
	t_mini	mini;
	
	if (argc > 1)
	{
		ft_putstr_fd("Error, too many arguments\n", 2);
		return (1);
	}
	(void)argv;
	if (!env[0])
	{
		ft_putstr_fd("Minishell: Environment unavailable.\n", 2)
		return (1);
	}
	mini.env = init_env(env);
	
}