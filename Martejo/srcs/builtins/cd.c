/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gemartel <gemartel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 15:17:17 by gemartel          #+#    #+#             */
/*   Updated: 2024/02/06 12:37:10 by gemartel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"


//errno contient le code d'erreur de chdir en cas d'echec
//getcwd renvoie le working directory du processus actuel, utile pour le minishell


int			env_add(char *value, t_env *env)
{
	t_env	*new;

	if (env && env->value == NULL)
	{
		env->value = ft_strndup(value, ft_strlen(value), ENV);
		return (1);
	}
	new = malloc_gc(sizeof(t_env), ENV);
	if (!new) //gerer erreur malloc
		return (-1);
	new->value = ft_strndup(value, ft_strlen(value), ENV);
	env_add_back(&env, new);
	return (1);
}

void	get_env_name_var(char *dest, char *src)
{
	int	i;

	i = 0;
	while (src[i] && src[i] != '=')
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
}
int	is_in_env(t_env *env, char *args)
{
	char	var_name[1024];
	char	env_name[1024];

	get_env_name_var(var_name, args);
	while (env)
	{
		get_env_name_var(env_name, env->value);
		if (ft_strcmp(var_name, env_name) == 0)
		{
			del_one_garbage(env->value, ENV);
			env->value = ft_strndup(args, ft_strlen(args), ENV);
			return (1);
		}
		env = env->next;
	}
	return (0);
}

char	*get_env_path(t_env *env, const char *var, size_t len)
{
	char	*path;

	while (env != NULL)
	{
		if (ft_strncmp(env->value, var, len) == 0)
		{
			path = ft_strdup(&env->value[len]);
			if (!path)
				return (NULL); // gerer erreur malloc
			return (path);
		}
		env = env->next;
	}
	return (NULL);
}

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
	return (0);
}

int	update_pwd(t_env *env)
{
	char	cwd[PATH_MAX];
	char	*pwd;

	if (getcwd(cwd, PATH_MAX) == NULL)
		return (-1);
	pwd = ft_strjoin("PWD=", cwd);
	if (!pwd)
		return (-1);
	if (is_in_env(env, pwd) == 0) // Verifie et remplace par la new var si la variable est deja presente dans l'env
		env_add(pwd, env); // sinon si var n'est pas present dans env on rajoute 
	return (0);
}

int	go_to_path(t_env *env)
{
	int		ret;
	char	*env_path;

	env_path = NULL;
	update_oldpwd(env);
	env_path = get_env_path(env, "HOME=", 5); // remplacer par notre fonction
	if (!env_path)
		return (-1);
		//afficher erreur comme quoi HOME n'existe pas et return erreur
	ret = chdir(env_path); //recuperer erreur de chdir
	update_pwd(env);
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
		update_pwd(env);
		if (ret_cd < 0)
			return (-1); // gerer erreur retour chdir
	}
	return (ret_cd);
}