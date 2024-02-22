#include "../include/minishell.h"
# include <sys/stat.h>

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

char	**find_path(t_env *env)
{
	char	*path;
	char	**path_split;

	path = get_env_path(env, "PATH", 4);
	if (!path)
		perror_msg("MALLOC ERROR\n");
	if (path && path[0] == 0)
		return (NULL);
	path_split = ft_split(path, ':');
	if (!path_split)
		perror_msg("MALLOC ERROR\n");
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
		// ERROR PATH 
	while (path_env[i])
	{
		exec = check_command_path(cmd, path_env[i]);
		if (exec)
		{
			free_char2(path_env);
			return (exec);
		}
		i++;
	}
	free_char2(path_env);
	if (ft_strchr(cmd, '/') && access(cmd, F_OK) == 0)
		return (ft_strdup(cmd));
	return (NULL);
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
	free_and_exit(exit_status);
}

static void	check_path(char  *command, char *path)
{
	DIR		*dir;

	dir = NULL;
	dir = opendir(command[0]);
	if (dir)
	{
		closedir(dir);
		print_path_error(command[0], 126, 4);
	}
	if (!path)
		print_path_error(command[0], 127, 1);
}

static char	*path_handler(t_btree *tree_el, t_env *env)
{
	char	*cmdpath;
	char	*cmd;

	cmd = tree_el->content;
	cmdpath = NULL;
	if (!cmd)
		exit(0); //Voir pour liberation
	else if (cmd && cmd[0] == '.' || cmd[0] == '\\')
	{
		if (ft_strcmp(cmd, ".") == 0 && !tree_el->args)
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


void	exec_bin(t_btree *tree_el, t_env *env)
{
	char *cmdpath;
	struct stat	stats;

	cmdpath = path_handler(tree_el, env);
	if (dup2(tree_el->io[IN], STDIN_FILENO) == -1)
		perror_msg("dup2 STDIN_FILENO");
	if (dup2(tree_el->io[OUT], STDOUT_FILENO) == -1)
		perror_msg("dup2 STDIN_FILENO");
	close(tree_el->io[IN]);
	close(tree_el->io[OUT]);
	if (lstat(cmdpath, &stat) != -1)
	{
		if ((stats.st_mode & S_IXUSR) && (stats.st_mode & S_IFREG))
		{
			execve(cmdpath, tree_el->args, env_to_char2(env));
			perror("minishell: ");
			free_and_exit(1); // Voir pour la liberatoon
		}
		else
			print_path_error(tree_el->content, 126, 2);
	}
	else
		print_path_error(tree_el->content, 127, 3);
}

pid_t	fork_exec(t_env *env, t_btree *tree_el)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		exit(EXIT_FAILURE);
	if(pid == 0)
		exec_process(tree_el);
	return (pid);
}

// A quel moment on attend le pid ?? 
int	wait_pids(int *pid, int len)
{
	int	i;
	int	status;

	i = 0;
	
	waitpid(pid, &status, 0);
	return (status);
}