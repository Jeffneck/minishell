/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gemartel <gemartel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 14:52:45 by gemartel          #+#    #+#             */
/*   Updated: 2024/02/29 12:42:56 by gemartel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_pwd(t_io fds)
{
	char	*cwd;
	char	buffer[PATH_MAX];

	cwd = getcwd(buffer, sizeof(buffer));
	if (cwd == NULL)
	{
		ft_putstr_fd(strerror(errno), 2);
		return (1);
	}
	else
	{
		ft_putstr_fd(cwd, fds.fd_out);
		write(fds.fd_out, "\n", 1);
	}
	return (0);
}
