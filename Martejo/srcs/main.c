/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gemartel <gemartel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 09:04:25 by gemartel          #+#    #+#             */
/*   Updated: 2024/02/13 16:23:02 by gemartel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	g_status = 0;

t_mini	*keeper_mini(t_mini *address_mini)
{
	static t_mini	*mini_kept;
	if(address_mini)
		mini_kept = address_mini;
	return (mini_kept);
}

int	main(int argc, char **argv, char **envp)
{
	t_mini	mini;
	(void)argc;
	(void)argv;
	
	mini.env = init_env(envp);
	keeper_mini(&mini);
	mini.io_global = (t_io){0, 1};
	prompt_loop(&mini);
	return (0);
}