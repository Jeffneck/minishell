/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hanglade <hanglade@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 12:48:48 by gemartel          #+#    #+#             */
/*   Updated: 2024/02/15 11:55:13 by hanglade         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"


// Attention faire peux etre en sorte que get_env_path utilise garbage ainsi que le strjoin
// En cas d'erreur de malloc free et exit le programme
char	*create_prompt(t_mini *mini)
{
	char	*home;
	char	*prompt;
	int		home_len;

	home = getenv("HOME");
	prompt = get_env_path(mini->env, "PWD", 3);
	if (!prompt)
		prompt = "Minishell";
	if (home && ft_strnstr(prompt, home, ft_strlen(home)))  //si on est dans le home, on remplace le chemin par ~$
	{
		home_len = ft_strlen(home);
		if (prompt[0] == '/' && prompt[1] == '/')
			home_len += 1;
		prompt[home_len - 1] = '~';
		prompt = &prompt[home_len - 1];
	}
	// Rajouter en fonction du code d'erreur la couleur rouge ou vert
	//prompt = ft_strjoin("\001\e[31;1m\002> \001\e[35m\002", prompt);
	prompt = ft_strjoin("\001\e[32m\002>\001\e[37m\002", prompt);
	//prompt = ft_strjoin("\001\e[31;1m\002> \001\e[35m\002", prompt);
	prompt = ft_strjoin(prompt, "$ ");
	return (prompt);
}


