/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gemartel <gemartel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 14:24:59 by gemartel          #+#    #+#             */
/*   Updated: 2024/02/29 14:33:03 by gemartel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*ft_strjoin_pipex(char *s1, char *s2, char *sep)
{
	char	*dest;

	if (s1 == NULL || s2 == NULL)
		return (NULL);
	dest = malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2)
				+ ft_strlen(sep) + 1));
	if (dest == NULL)
		return (NULL);
	dest[0] = '\0';
	dest = ft_strcat(dest, s1);
	dest = ft_strcat(dest, sep);
	dest = ft_strcat(dest, s2);
	return (dest);
}

static void	check_path(char  *command, char *path)
{
	DIR		*dir;

	dir = NULL;
	dir = opendir(command);
	if (dir)
	{
		closedir(dir);
		print_path_error(command, 126, 4);
	}
	if (!path)
		print_path_error(command, 127, 1);
}


char	*check_command_path(char *cmd, char *path)
{
	char		*path_cmd;
	struct stat	stats;

	if (!cmd || !path)
		return (NULL);
	path_cmd = ft_strjoin_pipex(path, cmd, "/");
	if (!path_cmd)
		exit (EXIT_FAILURE); // perror_msg("Malloc error");
	if (lstat(path_cmd, &stats) == 0)
	{
		add_to_garbage(path_cmd, TMP);
		return (path_cmd);
	}
	else
		free(path_cmd);
	return (NULL);
}

char	**find_path(t_env *env)
{
	char	*path;
	char	**path_split;

	path = get_env_path(env, "PATH", 4);
	if (!path)
		exit (EXIT_FAILURE); // perror_msg("Malloc error");
	if (path && path[0] == 0)
		return (NULL);
	path_split = split_gc(path, ':', TMP);
	if (!path_split)
		exit (EXIT_FAILURE); // perror_msg("Malloc error");
	free(path);
	return (path_split);
}


char	*get_cmd_path(char *cmd, t_env *env)
{
	char	*exec;
	char	**path_env;
	int		i;

	i = 0;
	path_env = find_path(env);
	if (!path_env)
		print_path_error(cmd, 127, 3); // Path inexistant
	while (path_env[i])
	{
		exec = check_command_path(cmd, path_env[i]);
		if (exec)
			return (exec);
		i++;
	}
	if (ft_strchr(cmd, '/') && access(cmd, F_OK) == 0)
		return (strdup_gc(cmd, TMP));
	return (NULL);
}
