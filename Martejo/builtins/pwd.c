/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gemartel <gemartel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 14:52:45 by gemartel          #+#    #+#             */
/*   Updated: 2024/02/05 15:11:23 by gemartel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include <unistd.h>
#include <string.h>
//Pour la macro PATH_MAX représente la longueur maximale
//(en caractères) que peut avoir un chemin de fichier sur le système de fichiers

int	ft_pwd(void)
{
	char	*cwd;
	char	buffer[PATH_MAX];


	cwd = getcwd(buffer, sizeof(buffer));
	if (cwd == NULL)
	{
		//perror("getcwd() error");
		return (0); //gerer erreur
	}
	else
		write(1, cwd, strlen(cwd));
		//ft_printf("%s", cwd);
	return (1);
}

