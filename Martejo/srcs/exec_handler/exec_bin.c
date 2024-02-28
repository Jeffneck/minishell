#include "../../include/minishell.h"
# include <sys/stat.h>

// Trouver un moyen de discerner les differents status code d'erreur en cas d' echec d'execve

//Trier ce merdier

void	free_and_exit(int exit_code)
{
	//rl_clear_history(); A voir
	clear_garbage(TMP, free);
	clear_garbage(ENV, free);
	clear_garbage(B_TREE, free);
	exit(exit_code);
}

char	**env_to_char2(t_env *env)
{
	size_t	len;
	size_t	i;
	char	**env_2d;
	t_env	*tmp;

	if (!env)
		return (NULL);
	tmp = env;
	i = 0;
	len = 0;
	// Remplacer boucle par fonction qui renvoie taille
	while (tmp)
	{
		len++;
		tmp = tmp->next;
	}
	env_2d = (char **)malloc((len + 1) * sizeof(char *));
	if (!env_2d)
		return (NULL); // Gestion Malloc
	add_to_garbage(env_2d, TMP);
	tmp = env;
	while (i < len)
	{
		env_2d[i] = strdup_gc(tmp->value, TMP);
		if (!env_2d[i])
			exit(EXIT_FAILURE); // Erreur de malloc
		i++;
		tmp = tmp->next;
	}
	env_2d[i] = NULL;
	return (env_2d);
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


static void	print_path_error(char *arg, int exit_status, int error)
{
	ft_putstr_fd("Minishell: ", 2);
	ft_putstr_fd(arg, 2);
	if (error == 1)
		ft_putendl_fd(": command not found", 2);
	if (error == 2)
		ft_putendl_fd(": permission denied", 2);
	if (error == 3)
		ft_putendl_fd(": No such file or directory", 2);
	if (error == 4)
		ft_putendl_fd(": Is a directory", 2);
	// if (error == 5)
	// 	ft_putendl_fd(DOT_ERR_MSG, 2); // Messageur pour le ' . '
	ft_printf("exit_status = %d\n", exit_status);
	free_and_exit(exit_status);
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

static char	*path_handler(t_btree *tree_el, t_env *env)
{
	char	*cmdpath;
	char	*cmd;

	cmd = tree_el->cmds[0];
	cmdpath = NULL;
	if (!cmd)
		exit(0); //Voir pour liberation
	else if (cmd && (cmd[0] == '.' || cmd[0] == '\\'))
	{
		if (ft_strcmp(cmd, ".") == 0 && !tree_el->cmds[1])
			print_path_error(cmd, 2, 5);
		else if (ft_strcmp(cmd, "..") == 0)
			print_path_error(cmd, 127, 1);
		else
			cmdpath = cmd;
	}
	else if (cmd && cmd[0] != '\0')
		cmdpath = get_cmd_path(cmd, env);
	check_path(cmd, cmdpath);
	return (cmdpath);
}


void	exec_process(t_btree *tree_el, t_env *env, t_io fds)
{
	char *cmdpath;
	struct stat	stats;

	ft_printf("exec_process\n");
	cmdpath = path_handler(tree_el, env);
	ft_printf("cmdpath exec = %s\n", cmdpath);
	if (dup2(fds.fd_in, STDIN_FILENO) == -1)
	{
		ft_printf("erreur dup2 stdin\n");
		exit (EXIT_FAILURE); //perror_msg("dup2 STDIN_FILENO");
	}
	if (dup2(fds.fd_out, STDOUT_FILENO) == -1)
	{
		ft_printf("erreur dup2 stdout\n");
		exit (EXIT_FAILURE); //perror_msg("dup2 STDIN_FILENO");
	}
	if (fds.fd_out != 1)
		close(fds.fd_out);
	if (fds.fd_in != 0)
		close(fds.fd_in);
	ft_printf("avant lstat\n");
	if (lstat(cmdpath, &stats) != -1)
	{
		//ft_printf("premier lstats\n");
		if ((stats.st_mode & S_IXUSR) && (stats.st_mode & S_IFREG))
		{
			//ft_printf("on rentre dans exec\n");
			execve(cmdpath, tree_el->cmds, env_to_char2(env));
			perror("minishell: ");
			free_and_exit(1); // Voir pour la liberatoon
		}
		else
			print_path_error(tree_el->cmds[0], 126, 2);
	}
	else
	{
		ft_printf("pas de lstat\n");
		print_path_error(tree_el->cmds[0], 127, 3);
	}
}

int	exec_bin(t_env *env, t_btree *tree_el, t_io fds)
{
	pid_t	pid;
	int		status;

	printf("out = %d \n in = %d\n", fds.fd_out, fds.fd_in);

	status = 0;
	pid = 0;
	int exit_status;
	pid = fork();
	if (pid == -1) // Voir pour l'erreur de fork quel code retourner
		exit(EXIT_FAILURE);
	if(pid == 0)
	{
		printf("children\n");
		exec_process(tree_el, env, fds);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))  // Vérifie si le processus s'est terminé normalement
    	exit_status = WEXITSTATUS(status);
	else
	{
		ft_printf("pas de retour status\n"); // voir si on peut creer ce cas
		return (130);
	}
	return (exit_status);
}