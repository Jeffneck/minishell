#include "../../include/minishell.h"

int	check_args(int argc, char **argv, char **envp)
{
	if (envp == NULL)
	{
		ft_putstr_fd("Error: Environment Path missing\n", 2);
		exit(EXIT_FAILURE);
	}
	if (argc < 5)
	{
		ft_putstr_fd("pipex: invalid number of arguments.\n", 2);
		exit(EXIT_FAILURE);
	}
	if (ft_strcmp(argv[1], "here_doc") == 0)
	{
		if (argc < 6)
		{
			ft_putstr_fd("pipex: invalid number of arguments.\n", 2);
			exit(EXIT_FAILURE);
		}
		return (1);
	}
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	int	status;
	int	fd[2];

	if (check_args(argc, argv, envp) == 1)
	{
		setting_here_doc(fd, argv[2], argv[argc - 1]);
		status = command_receiver(argc - 4, &argv[3], envp, fd);
		return (status);
	}
	set_fd(fd, argv[1], argv[argc - 1]);
	status = command_receiver(argc - 3, &argv[2], envp, fd);
	status = WEXITSTATUS(status);
	return (status);
}