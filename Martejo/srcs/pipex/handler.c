#include "../../include/minishell.h"

void	exec(char *cmd, int *pipe_fd, char **env)
{
	char	**s_cmd;
	char	*path;

	if (dup2(pipe_fd[IN], STDIN_FILENO) == -1)
		perror_msg("dup2 STDIN_FILENO");
	if (dup2(pipe_fd[OUT], STDOUT_FILENO) == -1)
		perror_msg("dup2 STDOUT_FILENO");
	close(pipe_fd[IN]);
	close(pipe_fd[OUT]);
	s_cmd = ft_split(cmd, ' ');
	if (!s_cmd)
		perror_msg("Malloc");
	path = get_path(s_cmd[0], env);
	if (!path)
	{
		ft_free_tab(s_cmd);
		ft_strerror(PATH_CMD, cmd);
	}
	if (execve(path, s_cmd, env) == -1)
	{
		free(path);
		ft_free_tab(s_cmd);
		perror_msg("pipex: failed to execute a command:");
	}
}

void	parent_process(int *fd_to_use, int *p)
{
	close(fd_to_use[IN]);
	close(fd_to_use[OUT]);
	fd_to_use[IN] = p[READ];
}

int	wait_pids(int *pid, int len)
{
	int	i;
	int	status;

	i = 0;
	while (i < len)
	{
		waitpid(pid[i], &status, 0);
		i++;
	}
	free(pid);
	return (status);
}

int	fork_exec(char *cmd, int *fds, char **env, int *pid_tab)
{
	int	pid;

	pid = fork();
	if (pid == -1)
		perror_msg("pid");
	if (pid == 0)
	{
		free(pid_tab);
		close(fds[READ]);
		exec(cmd, &fds[2], env);
	}
	return (pid);
}

int	command_receiver(int nb_cmd, char **cmds, char **env, int *fd_in_and_out)
{
	int	fds[4];
	int	*pid;
	int	i;

	i = -1;
	pid = (int *)malloc(sizeof(int) * nb_cmd);
	if (!pid)
		perror_msg("Malloc");
	while (++i < nb_cmd)
	{
		if (pipe(fds) == -1)
			perror_msg("pipe");
		if (i == 0)
			set_fd_to_use(fds, fd_in_and_out[IN], fds[WRITE]);
		else if (i == nb_cmd - 1)
			set_fd_to_use(fds, fds[2 + IN], fd_in_and_out[OUT]);
		else
			set_fd_to_use(fds, fds[2 + IN], fds[2 + OUT]);
		pid[i] = fork_exec(cmds[i], fds, env, pid);
		parent_process(&fds[2], fds);
	}
	i = wait_pids(pid, nb_cmd);
	return (i);
}