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
void	prompt_loop(t_mini *mini)
{
	//char	*prompt;
	//char	*read_buffer;
	//char	**cmds;
	char *buffer = "c1 c1arg1 c1arg2 <in <in2 <in3 <in4 || >>app  c2 c2arg1 c2arg2 >out| c3 c3arg1 && c4 c4arg1";
	
	
	//prompt = create_prompt(mini);
	//read_buffer = readline(prompt);
	mini->tkn_lst = lexer(buffer);
	mini->b_tree = parser(mini->tkn_lst, mini->env);
	ft_printf("BTREE TOKENS IN EXEC ORDER CREATION ///////////////////////////////////\n\n");
	root_first_search(mini->b_tree, display_node);
		
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
	
	clear_garbage(TKN_LIST, free);
	clear_garbage(ENV, free);
	clear_garbage(TMP, free);
	clear_garbage(B_TREE, free);
}