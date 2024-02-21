
#include "../../include/minishell.h"

void	ft_free_tab(char **tab)
{
	size_t	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

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

char	**find_path(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp("PATH", envp[i], 4) == 0)
			return (ft_split(&envp[i][5], ':'));
		i++;
	}
	return (NULL);
}

char	*check_command_path(char *cmd, char *path)
{
	char	*path_cmd;

	if (!cmd || !path)
		return (NULL);
	path_cmd = ft_strjoin_pipex(path, cmd, "/");
	if (!path_cmd)
		perror_msg("Malloc error");
	if (access(path_cmd, F_OK) == 0)
		return (path_cmd);
	else
		free(path_cmd);
	return (NULL);
}

char	*get_path(char *cmd, char **envp)
{
	char	**path_envp;
	char	*exec;
	int		i;

	i = 0;
	path_envp = find_path(envp);
	if (!path_envp)
		return (NULL);
	while (path_envp[i])
	{
		exec = check_command_path(cmd, path_envp[i]);
		if (exec)
		{
			ft_free_tab(path_envp);
			return (exec);
		}
		i++;
	}
	ft_free_tab(path_envp);
	if (ft_strchr(cmd, '/') && access(cmd, F_OK) == 0)
		return (ft_strdup(cmd));
	return (NULL);
}