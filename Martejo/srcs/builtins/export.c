#include "../../include/minishell.h"


int		is_valid_env(const char *env)
{
	int		i;

	i = 0;
	if (ft_isdigit(env[i]) == 1)
		return (0);
	while (env[i] && env[i] != '=')
	{
		if (ft_isalnum(env[i]) == 0) //verifier par rapport au underscore
			return (-1);
		i++;
	}
	if (env[i] != '=')
		return (2);
	return (1);
}

int			env_add(char *value, t_env *env, int mod)
{
	t_env	*new;

	new = NULL;
	if (env && env->value == NULL)
	{
		env->value = ft_strndup(value, ft_strlen(value), ENV);
		return (1);
	}
	new = malloc_gc(sizeof(t_env), ENV);
	if (!new) //gerer erreur malloc
		return (-1);
	new->value = ft_strndup(value, ft_strlen(value), ENV);
	new->secret = mod;
	new->next = NULL;
	env_add_back(&env, new);
	return (1);
}

int	ft_export(char **args, t_env *env)
{
	int	new_env;
	int	error_ret;

	new_env = 0;
	if (!args[1])
	{
		print_sorted_env(env);
		return (1);
	}
	else
	{
		error_ret = is_valid_env(args[1]);
		if (args[1][0] == '=')
			error_ret = -3;
		if (error_ret <= 0)
			return (-1); //gerer erreur et afficher
		if (error_ret == 2) 
			new_env = 1;
		else 
			new_env = is_in_env(env, args[1]);
		if (new_env == 0)
		{
			if (error_ret == 1)
				env_add(args[1], env, 0);
			else
				env_add(args[1], env, 1);
		}
	}
	return (0);
}