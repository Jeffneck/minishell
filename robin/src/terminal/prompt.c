/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gemartel <gemartel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/19 12:04:29 by cberganz          #+#    #+#             */
/*   Updated: 2024/02/13 15:24:08 by gemartel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*create_prompt(char **envp[])
{
	char	*prompt;
	char	*home;
	int		home_len;

	home = getenv("HOME");
	prompt = get_env("PWD", envp);
	if (!prompt)
		prompt = "Minishell";
	if (home && ft_strstr(prompt, home))
	{
		home_len = ft_strlen(home);
		if (prompt[0] == '/' && prompt[1] == '/')
			home_len += 1;
		prompt[home_len - 1] = '~';
		prompt = &prompt[home_len - 1];
	}
	if (g_status)
		prompt = ft_strjoin("\001\e[31;1m\002> \001\e[35m\002", prompt, LOOP);
	else
		prompt = ft_strjoin("\001\e[32;1m\002> \001\e[35m\002", prompt, LOOP);
	if (!prompt)
		print_message("minishell: Allocation error.\n", RED, 1);
	return (prompt);
}
