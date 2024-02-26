#include "../../include/minishell.h"


//Voir par rapport a la detection des singles et doubles quotes exemple :
// export sak='sa!lu^%*' ->> ca fonctionne car single quotes
//export sak=sa!lu^%*  ---> zsh: event not found: lu ca ne fonctionne oas

int	is_valid_env(const char *env)
{
	int	i;

	i = 0;
	if (ft_isdigit(env[i]) == 1)
		return (0); // message erreur avec export: not an identifier: nombre trouvé
	while (env[i] && env[i] != '=')
	{
		if (ft_isalnum(env[i]) == 0 && env[i] != '_') //verifier par rapport au underscore
			return (-1); //export: «l'argument d'export» : not a valid identifier
		//bash: !sa=df: event not found
		i++;
	}
	if (env[i] != '=')
		return (2);
	return (1);
}

int			env_add(char *value, t_env **env, int mod)
{
	t_env	*new;

	new = NULL;
	// if (env && env->value == NULL)
	// {
	// 	env->value = ft_strndup(value, ft_strlen(value), ENV);
	// 	return (1);
	// }
	new = malloc_gc(sizeof(t_env), ENV);
	if (!new) //gerer erreur malloc
		return (-1);
	new->value = ft_strndup(value, ft_strlen(value), ENV);
	new->secret = mod;
	new->next = NULL;
	env_add_back(env, new);
	return (1);
}


int	export_handler(char *args, t_env **env)
{
	int	new_env;
	int	error_ret;

	new_env = 0;
	error_ret = 0;
	error_ret = is_valid_env(args);
	if (args[0] == '=') // bash: export: `=': not a valid identifier
		error_ret = -3;
	if (error_ret <= 0)
		return (-1); //gerer erreur et afficher
	new_env = is_in_env((*env), args);
	if (new_env == 0)
	{
		if (error_ret == 1)
			env_add(args, env, 0);// 0 ou 1 pour secret export
		else
			env_add(args, env, 1);
	}
	return (0);
}
int	ft_export(char **args, t_env **envt, t_io fds)
{
	int	i;

	i = 1;
	if (!args[1])
	{
		print_sorted_env(*envt, fds);
		return (0);
	}
	else
	{
		while (args[i] != NULL)
		{
			if (export_handler(args[i], envt) != 0)
				return (1);
			i++;
		}	
	}
	return (0);
}