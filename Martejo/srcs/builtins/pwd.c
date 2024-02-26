/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gemartel <gemartel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 14:52:45 by gemartel          #+#    #+#             */
/*   Updated: 2024/02/14 11:50:16 by gemartel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
//Pour la macro PATH_MAX représente la longueur maximale
//(en caractères) que peut avoir un chemin de fichier sur le système de fichiers

int	ft_pwd(t_io fds)
{
	char	*cwd;
	char	buffer[PATH_MAX];

	cwd = getcwd(buffer, sizeof(buffer));
	if (cwd == NULL)
	{
		perror("pwd");
		return (1); //gerer erreur
	}
	else
	{
		ft_putstr_fd(cwd, fds.fd_out);
		write(fds.fd_out, "\n", 1);
	}
	return (0);
}

