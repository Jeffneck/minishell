#include "../../include/minishell.h"

int	update_oldpwd(t_env **env)
{
	char	cwd[PATH_MAX];
	char	*oldpwd;

	if (getcwd(cwd, PATH_MAX) == NULL)
	{
		ft_putstr_fd(strerror(errno), 2);
		return (1);
	}
	oldpwd = ft_strjoin("OLDPWD=", cwd);
	if (!oldpwd)
	{
		ft_putstr_fd("Minishell: Malloc error\n", 2);
		free_and_exit(1);
	}
	if (is_in_env(*env, oldpwd) == 0)
		env_add(oldpwd, env, 0);
	return (0);
}

int	update_pwd(t_env **env)
{
	char	cwd[PATH_MAX];
	char	*pwd;

	if (getcwd(cwd, PATH_MAX) == NULL)
	{
		ft_putstr_fd(strerror(errno), 2);
		return (1);
	}
	pwd = ft_strjoin("PWD=", cwd);
	if (!pwd)
	{
		ft_putstr_fd("Minishell: Malloc error\n", 2);
		free_and_exit(1);
	}
	if (is_in_env(*env, pwd) == 0)
		env_add(pwd, env, 0);
	return (0);
}

int	go_to_path(t_env **env)
{
	int		ret;
	char	*env_path;

	env_path = NULL;
	update_oldpwd(env);
	env_path = get_env_path(*env, "HOME=", 4);
	if (!env_path)
		free_and_exit(1);
	else if (ft_strcmp(env_path, "") == 0)
	{
		ft_putstr_fd("Minishell: cd: HOME not set\n", 2);
		return (1);
	}
	ret = chdir(env_path);
	if (ret)
	{
		ft_putstr_fd(strerror(errno), 2);
		return (free(env_path), 1);
	}
	update_pwd(env);
	return (free(env_path), ret);
}

int	cd(char **cmds, t_env **env)
{
	int	ret_cd;

	if (cmds[1] == NULL)
		return (go_to_path(env));
	else
	{
		if (update_oldpwd(env) != 0)
			return (1);
		ret_cd = chdir(cmds[1]);
		if (ret_cd < 0)
		{
			ft_putstr_fd(strerror(errno), 2);
			return (1);
		}
		if (update_pwd(env) != 0)
			return (1);
	}
	return (ret_cd);
}
