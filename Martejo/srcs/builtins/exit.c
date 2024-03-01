/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gemartel <gemartel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 12:37:22 by gemartel          #+#    #+#             */
/*   Updated: 2024/02/29 12:41:08 by gemartel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	check_status_code(char *status_code)
{
	int	i;
	int	sign;

	i = 0;
	sign = 0;
	while (status_code[i])
	{
		if (status_code[i] == '-' || status_code[i] == '+')
			sign++;
		if (!(status_code[i] >= 48 && status_code[i] <= 57) && (status_code[i] != '-' && status_code[i] != '+'))
			return (1);
		i++;
	}
	if (sign > 1)
		return (1);
	return (0);
}

int	builtin_exit(t_mini *mini, char **cmds)
{
	int	exit_status;
	char	*tmp;
	exit_status = mini->last_gstatus;
	if ((cmds[1][0] == '-'  || cmds[1][0] == '+') && cmds[1][1] == 0 && cmds[2])
	{
		tmp = strjoin_gc(cmds[1], cmds[2], TMP);
		cmds[1] = tmp;
		cmds[2] = NULL;
	}
	write(1, "exit\n", 5);

	if (cmds[1])
	{
		if (cmds[2])
		{
			ft_putstr_fd("Minishell: exit: too many arguments", 2);
			return (1);
		}
		else if (check_status_code(cmds[1]) == 1)
		{
			ft_printf_fd(2, "Minishell: exit: %s : numeric argument required\n",
				cmds[1]);
			exit_status = 2;
		}
		
		else
		{
			exit_status = ft_atoi(cmds[1]);
			if (cmds[1][0] == '-' || exit_status > 255)
				exit_status = exit_status % 256;
		}
	}
	free_and_exit(exit_status);
	return (exit_status);
}
