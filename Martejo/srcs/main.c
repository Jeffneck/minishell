/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hanglade <hanglade@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 09:04:25 by gemartel          #+#    #+#             */
/*   Updated: 2024/02/29 11:25:14 by hanglade         ###   ########.fr       */
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

void	handle_shlvl(t_env *env, int nb)
{
	int		lvl;
	char	*shlvl;

	lvl = ft_atoi(get_env_path(env, "SHLVL", 5));
	if (lvl >= 999)
	{
		ft_printf_fd(2, "minishell: warning: shell level (%d) too high, resetting to 1", lvl + 1);
		// ft_putstr_fd("minishell: warning: shell level (", 2);
		// ft_putnbr_fd(lvl, 2);
		// ft_putendl_fd(") too high, resetting to 1", 2);
		lvl = 1;
	}
	else if (lvl < 0)
		lvl = 0;
	else
		lvl += nb;
	shlvl = itoa_gc(lvl, ENV);
	if (!shlvl)
		exit(EXIT_FAILURE);// print_message(MALLOC_ERR_MSG, RED, 1);
	shlvl = ft_strjoin("SHLVL=", shlvl);
	if (!shlvl)
		exit(EXIT_FAILURE);// print_message(MALLOC_ERR_MSG, RED, 1);
	is_in_env(env, shlvl);
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
	handle_shlvl(mini.env, 1);
	prompt_loop(&mini);
	return (0);
}