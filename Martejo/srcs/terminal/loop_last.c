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

void	free_all(char ***buffer, char **prompt)
{
	free_char2(buffer);
	free(*prompt);
	clear_garbage(ENV, free);
}

// uint8_t	check_eof_heredoc(char *input)
// {
// 	if (!input)
// 	{
// 		if (g_status == 130)
// 			return (1);
// 		else
// 		{
// 			ft_putendl_fd(EOF_HEREDOC, 2);
// 			return (1);
// 		}
// 	}
// 	return (0);
// }

void	sig_handler(int sigcode)
{
	ft_printf("SIGCODE = %d\n", sigcode);
	if (sigcode == SIGINT)
	{
		ft_printf("SIGINT");
		// close(STDIN_FILENO);
		// g_status = 130;
	}
	if (sigcode == SIGQUIT)
	{
		ft_printf("SIGINT");
		write(2, "\b\b  \033[2D", 8);
		exit(EXIT_FAILURE); //
	}
	if (sigcode == SIGTSTP)
	{
		ft_printf("SIGTSTP");
		write(2, "\b\b  \033[2D", 8);
	}
}

void	sig_void(int sigcode)
{
	(void)sigcode;
	if (sigcode == SIGQUIT)
		g_status = 131;
}


void	prompt_loop(t_mini *mini)
{
	char	*line_read;
	//char	**cmds;
	// char *buffer = "export >test1.txt";
	
	// signal(SIGINT, sig_handler);
	signal(SIGQUIT, sig_handler);
	signal(SIGTSTP, sig_handler);
	while(1)
	{
		line_read = readline(create_prompt(mini));
		if(line_read && *line_read) //eviter de stocker une line = NULL ou vide dans l' historique
			add_history(line_read);
	}
	// mini->tkn_lst = lexer(line_read);
	//reactiver plus tard
	// mini->b_tree = parser(mini->tkn_lst, mini->env);
	// ft_printf("BTREE TOKENS IN EXEC ORDER CREATION ///////////////////////////////////\n\n");
	// root_first_search(mini->b_tree, display_node);
	// browse_tree(mini, mini->b_tree, mini->io_global);
	//addhistory => voir quand ajouter dans l'historique
	//history clear == appeler quand on met fin au prog

	clear_garbage(TKN_LIST, free);
	clear_garbage(ENV, free);
	clear_garbage(TMP, free);
	clear_garbage(B_TREE, free);
}


	// display_tknlist(mini->tkn_lst);
		//cmds = ft_split(read_buffer, ' ');
		// if (prompt_processing(cmds, mini) == -1)
		// {
		// 	free(read_buffer);
		// 	free_all(&cmds, &prompt);
		// 	exit(EXIT_SUCCESS);
		// }
		//free(read_buffer);
		//free(prompt);
		//read_buffer = NULL;
		//prompt = NULL;
		//free_char2(&cmds);