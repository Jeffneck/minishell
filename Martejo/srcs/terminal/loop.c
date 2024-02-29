/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hanglade <hanglade@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 16:04:48 by gemartel          #+#    #+#             */
/*   Updated: 2024/02/15 14:21:45 by hanglade         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*A supprimer
void	free_all(char ***buffer, char **prompt)
{
	free_char2(buffer);
	free(*prompt);
	clear_garbage(ENV, free);
}*/

void	clear_loop(void)
{
	clear_garbage(TMP, free);
	clear_garbage(TKN_LIST, free);
	clear_garbage(B_TREE, free);
}

void	clear_ctrl_c(int *stdin_cpy, int *first_read, char **line_read)
{
	if (*line_read)
		free(*line_read);
	*line_read = NULL;
	dup2(*stdin_cpy, STDIN_FILENO);
	close(*stdin_cpy);
	if (*first_read == 1)
	{
		write(1, "\n", 1);
		*first_read = 0;
	}
	clear_loop();
}

void	sig_handler(int sigcode)
{
	if (sigcode == SIGINT)
	{
		close(STDIN_FILENO);
		g_status = 130;
	}
	if (sigcode == SIGQUIT)
	{
		write(2, "\b\b  \033[2D", 8);
		exit(EXIT_FAILURE);
	}
	if (sigcode == SIGTSTP)
	{
		write(2, "\b\b  \033[2D", 8);
	}
}

void	process_shell(t_mini *mini, char *line_read, int *stdin_cpy)
{
	add_history(line_read);
	verify_input_syntax(line_read);
	mini->tkn_lst = lexer(line_read);
	mini->b_tree = parser(mini);
	browse_tree(mini, mini->b_tree, mini->io_global);
	dup2(*stdin_cpy, STDIN_FILENO);
	close(*stdin_cpy);
	clear_loop();
}

void	prompt_loop(t_mini *mini)
{
	int		stdin_cpy;
	char	*line_read;
	int		first_read;

	first_read = 1;
	signal(SIGINT, sig_handler);
	signal(SIGQUIT, sig_handler);
	signal(SIGTSTP, sig_handler);
	while (1)
	{
		stdin_cpy = dup(STDIN_FILENO);
		mini->last_gstatus = g_status;
		g_status = 0;
		line_read = readline(create_prompt(mini));
		if (g_status == 130)
		{
			clear_ctrl_c(&stdin_cpy, &first_read, &line_read);
			continue ;
		}
		if (!line_read)
			free_and_exit(0);
		if (line_read && *line_read)
			process_shell(mini, line_read, &stdin_cpy);
		free(line_read);
	}
}
