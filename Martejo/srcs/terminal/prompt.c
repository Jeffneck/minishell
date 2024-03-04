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

/*Attention faire peux etre en sorte que get_env_path utilise 
garbage ainsi que le strjoin
En cas d'erreur de malloc free et exit le programme*/
char	*create_prompt(t_mini *mini)
{
	char	*home;
	char	*prompt;
	int		home_len;

	home = getenv("HOME");
	prompt = get_env_value(mini->env, "PWD", 3);
	if (!prompt)
		prompt = "Minishell";
	if (home && ft_strnstr(prompt, home, ft_strlen(home)))
	{
		home_len = ft_strlen(home);
		if (prompt[0] == '/' && prompt[1] == '/')
			home_len += 1;
		prompt[home_len - 1] = '~';
		prompt = &prompt[home_len - 1];
	}
	if (g_status == 0)
		prompt = ft_strjoin("\001\e[32;1m\002> \001\e[37m\002", prompt);
	else
		prompt = ft_strjoin("\001\e[31;1m\002> \001\e[37m\002", prompt);
	prompt = ft_strjoin(prompt, "$ \x1b[0m");
	if (!prompt)
		print_and_exit(MALLOC_ERR_MSG, RED, 1);
	return (prompt);
}
