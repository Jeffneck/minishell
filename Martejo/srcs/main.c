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

int	main(int argc, char **argv, char **envp)
{
	t_mini	mini;
	(void)argc;
	(void)argv;

	mini.env = init_env(envp);
	prompt_loop(&mini);
	return (0);
}