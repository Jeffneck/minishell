/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gemartel <gemartel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 09:04:25 by gemartel          #+#    #+#             */
/*   Updated: 2024/02/12 16:47:31 by gemartel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_mini	mini;
	
	// if (argc > 1)
	// {
	// 	ft_putstr_fd("Error, too many arguments\n", 2);
	// 	return (1);
	// }
	(void)argc;
	(void)argv;
	if (!envp[0])
	{
		ft_putstr_fd("Minishell: Environment unavailable.\n", 2);
		return (1);
	}
	mini.env = init_env(envp);
	
	ft_export(argv + 1, mini.env);
	//unset(argv + 1, mini.env);
	//env(mini.env);
	env(mini.env);
	printf("\n\n\n\n");
	print_sorted_env(mini.env);
	clear_garbage(ENV, free);
	return (0);
}