#include "../../include/minishell.h"

int		is_builtin(char *command)
{
	if (ft_strcmp(command, "echo") == 0)
		return (1);
	if (ft_strcmp(command, "cd") == 0)
		return (1);
	if (ft_strcmp(command, "pwd") == 0)
		return (1);
	if (ft_strcmp(command, "env") == 0)
		return (1);
	if (ft_strcmp(command, "export") == 0)
		return (1);
	if (ft_strcmp(command, "unset") == 0)
		return (1);
	return (0);
}

// Voir pour le fd avec les pipes notamment pour echo et PWD
//est ce fds 0 ou fds 1
// Bien verifier les status code

int		exec_builtin(t_env **envt, t_btree *tree_el, t_io fds)
{
	int		status;

	status = 0;
	if (ft_strcmp(tree_el->cmds[0], "echo") == 0)
		status = echo(tree_el->cmds, fds);
	if (ft_strcmp(tree_el->cmds[0], "cd") == 0)
		status = cd(tree_el->cmds, envt);
	if (ft_strcmp(tree_el->cmds[0], "pwd") == 0)
		status = ft_pwd(fds);
	if (ft_strcmp(tree_el->cmds[0], "env") == 0)
		env(*envt, fds);
	if (ft_strcmp(tree_el->cmds[0], "export") == 0)
		status = ft_export(tree_el->cmds, envt, fds);
	if (ft_strcmp(tree_el->cmds[0], "unset") == 0)
		status = unset(tree_el->cmds, *envt); // Modifier unset avec double pointeur
	return (status);
}


 //Les builtins ne doivent que toucher a la l' ecriture du pipe 
int	fork_builtin(t_env **envt, t_btree *tree_el, t_io fds)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
		exit(EXIT_FAILURE); // Voir pour l'erreur en cas de fork
	if(pid == 0)
	{
		status = exec_builtin(envt, tree_el, fds);
		free_and_exit(status);
	}
	waitpid(pid, &status, 0);
	return (status);
}