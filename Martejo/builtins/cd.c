/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gemartel <gemartel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 15:17:17 by gemartel          #+#    #+#             */
/*   Updated: 2024/02/05 16:27:56 by gemartel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

// Extraire l'environnement dans une liste peu etre tres utile
//errno contient le code d'erreur de chdir en cas d'echec

typedef struct	s_env
{
	char			*value;
	struct s_env	*next;
}				t_env;

int	update_oldpwd(t_env *env)
{
	char	cwd[PATH_MAX];
	char	*oldpwd;

	if (getcwd(cwd, PATH_MAX) == NULL)
		return (-1);
	oldpwd = ft_strjoin("OLDPWD=", cwd);
	if (!oldpwd)
		return (-1);
	if (is_in_env(env, oldpwd) == 0) // Verifie et remplace par la new var si la variable est deja presente dans l'env
		env_add(oldpwd, env); // sinon si var n'est pas present dans env on rajoute 
}

int	go_to_path(t_env *env)
{
	int		ret;
	char	*env_path;

	env_path = NULL;
	update_oldpwd(env);
	env_path = get_env("HOME"); // remplacer par notre fonction
	if (!env_path)
		//afficher erreur comme quoi HOME n'existe pas et return erreur
	ret = chdir(env_path); //recuperer erreur de chdir
	free(env_path); //liberer ce buffer;
	return (ret);
}
int	cd(char **cmds, t_env *env)
{
	int	ret_cd;

	if (cmds[1] == NULL)
		return (go_to_path(env));
	else
	{
		update_oldpwd(env);
		ret_cd = chdir(cmds[1]);
		if (ret_cd < 0)
			// gerer erreur
	}
	return (ret_cd);
}