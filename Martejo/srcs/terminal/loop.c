/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hanglade <hanglade@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 16:04:48 by gemartel          #+#    #+#             */
/*   Updated: 2024/02/15 13:06:56 by hanglade         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	prompt_processing(char **input, t_mini *mini)
{
	if (ft_strcmp(input[0], "export") == 0)
		ft_export(input, &mini->env);
	else if (ft_strcmp(input[0], "unset") == 0)
		unset(input, mini->env);
	else if (ft_strcmp(input[0], "cd") == 0)
		cd(input, &mini->env);
	else if (ft_strcmp(input[0], "env") == 0)
		env(mini->env);
	else if (ft_strcmp(input[0], "echo") == 0)
		echo(input);
	else if (ft_strcmp(input[0], "pwd") == 0)
		ft_pwd();
	else if (ft_strcmp(input[0], "exit") == 0)
		return(-1);
	return (0);
}

void	free_all(char ***buffer, char **prompt)
{
	free_char2(buffer);
	free(*prompt);
	clear_garbage(ENV, free);
}
void	prompt_loop(t_mini *mini)
{
	char	*prompt;
	char	*read_buffer;
	//char	**cmds;
	
	while (1)
	{
		prompt = create_prompt(mini);
		read_buffer = readline(prompt);
		mini->tkn_lst = lexer(read_buffer);
		
		parser(*mini);
		
		display_tknlist(mini->tkn_lst);
		//cmds = ft_split(read_buffer, ' ');
		// if (prompt_processing(cmds, mini) == -1)
		// {
		// 	free(read_buffer);
		// 	free_all(&cmds, &prompt);
		// 	exit(EXIT_SUCCESS);
		// }
		free(read_buffer);
		free(prompt);
		read_buffer = NULL;
		prompt = NULL;
		//free_char2(&cmds);
	}
	
}