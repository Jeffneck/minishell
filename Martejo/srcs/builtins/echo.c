/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gemartel <gemartel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 14:40:47 by gemartel          #+#    #+#             */
/*   Updated: 2024/02/29 12:36:45 by gemartel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	size_cmds(char **cmds)
{
	int	size;

	size = 0;
	while (cmds[size])
		size++;
	return (size);
}

int	echo(char **cmds, t_io fds)
{
	int	i;
	int	n_option;

	i = 1;
	n_option = 0;
	if (size_cmds(cmds) > 1)
	{
		while (cmds[i] && ft_strcmp(cmds[i], "-n") == 0)
		{
			n_option = 1;
			i++;
		}
		while (cmds[i])
		{
			ft_putstr_fd(cmds[i], fds.fd_out);
			if (cmds[i + 1] && cmds[i][0] != '\0')
				write(fds.fd_out, " ", 1);
			i++;
		}
	}
	if (n_option == 0)
		write(fds.fd_out, "\n", 1);
	return (0);
}
