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

// void	sig_void(int sigcode)
// {
// 	(void)sigcode;
// 	if (sigcode == SIGQUIT)
// 		g_status = 131;
// }
void	sig_handler(int sigcode)
{
	// ft_printf("SIGCODE = %d\n", sigcode);
	if (sigcode == SIGINT)
	{
		// Nettoie la ligne courante sur laquelle readline attendait une entrée
		rl_replace_line("", 0); //
		// Déplace le curseur à une nouvelle ligne
		rl_on_new_line();
		// Redessine le prompt
		rl_redisplay();

		// Afficher un nouveau prompt (le `\n` garantit que le buffer est vidé si en mode tampon ligne)
		ft_printf("\n%s", create_prompt(keeper_mini(NULL)));
		// write(STDOUT_FILENO, "\nNouveau prompt> ", 16);
		// write(STDOUT_FILENO, "\n", 1);
		g_status = 130;
		// exit(EXIT_FAILURE); //retirer plus tard
	}
	if (sigcode == SIGQUIT)
	{
		ft_printf("SIGQUIT\n");
		write(2, "\b\b  \033[2D", 8);
		exit(EXIT_FAILURE); //
	}
	if (sigcode == SIGTSTP)
	{
		ft_printf("SIGTSTP\n");
		write(2, "\b\b  \033[2D", 8);
	}
}

void	process_shell(t_mini *mini, char *line_read)
{
	mini->tkn_lst = lexer(line_read);
	mini->b_tree = parser(mini->tkn_lst, mini->env);
	browse_tree(mini, mini->b_tree, mini->io_global);
	clear_garbage(TMP, free); //retirer qd tout sera proprement retire
	clear_garbage(TKN_LIST, free);//retirer qd tout sera proprement retire
	clear_garbage(B_TREE, free);

}

void	prompt_loop(t_mini *mini)
{
	// int		stdin_keeper;
	char	*line_read;

	// stdin_keeper = dup(STDIN_FILENO);
	// mini->io_global.fd_in = stdin_keeper;
	signal(SIGINT, sig_handler);
	signal(SIGQUIT, sig_handler);
	signal(SIGTSTP, sig_handler);
	while(1)
	{
		line_read = readline(create_prompt(mini));
		if (!line_read)
			exit(EXIT_FAILURE);// ctrl+D
		if(*line_read)//g_status pour eviter d' enregistrer une ligne qui contient ^C 
		{
			add_history(line_read);
			process_shell(mini, line_read);
		}	
		if (g_status == 130)
		{
        	ft_printf("\n");
        	// rl_on_new_line();
        	// ft_printf("\naaaa");
        	// rl_redisplay();
        	// ft_printf("\naaaa");
			
			g_status = 0;
    	}
	}
}
//ctrl d => readline
	// met fin au minishell si capte par readline
	// stoppe le mode lecture entree std dans heredoc et cat
//ctrl + c => stoppe toute la partie exec et le
// si ecrit au moment de l' ecriture du prompt et que l' on a deja ecrit qq chose => affiche un nouveau prompt et g_status = 130
// si premier char => affiche juste un nouveau prompt sans toucher a g_status(cas particulier osef)

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