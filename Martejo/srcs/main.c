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
	
	
	ft_putstr_fd("LESS_TERMCAP_mb=$\'\\E[1;31m'", open("test2", O_WRONLY | O_CREAT | O_TRUNC, 0644));
	mini.env = init_env(envp);
	mini.io_global = (t_io){0, 1};
	prompt_loop(&mini);
	return (0);
}