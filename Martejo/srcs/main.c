/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hanglade <hanglade@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 09:04:25 by gemartel          #+#    #+#             */
/*   Updated: 2024/02/29 10:15:23 by hanglade         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	g_status = 0;

t_mini	*singleton_mini(t_mini *address_mini)
{
	static t_mini	*mini_todeliver;
	if(address_mini)
		mini_todeliver = address_mini;
	return (mini_todeliver);
}

int	main(int argc, char **argv, char **envp)
{
	t_mini	mini;
	(void)argc;
	(void)argv;
	
	if (argc > 1)
		exit(EXIT_FAILURE);//print_message(ARGS_ERR_MSG, RED, EXIT_FAILURE);
	(void) argv;
	if (!envp[0])
		exit(EXIT_FAILURE);//print_message(UNAVAILABLE_ENV, RED, EXIT_FAILURE);
	mini.env = init_env(envp);
	mini.io_global = (t_io){0, 1};
	mini.last_gstatus = 0;
	singleton_mini(&mini);//inutilise maintenant ?
	prompt_loop(&mini);
	return (0);
}