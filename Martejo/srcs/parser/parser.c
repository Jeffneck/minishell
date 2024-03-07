#include "../../include/minishell.h"

void	set_token_index(t_token	*curr)
{
	size_t	i;

	i = 0;
	while(curr)
	{
		curr->index = i;
		if(is_redir_tkn(curr->type) || curr->type == HEREDOC)
			curr->priority = 1;
		// else if(curr->type == PARENTHESIS) // 
		// 	curr->priority = 2;
		if(curr->type == PIPE)
			curr->priority = 3;
		else if(is_logical_op_tkn(curr->type))
			curr->priority = 4;
		i++;
		curr = curr->next;
	}
}

t_btree	*parser(t_mini *mini)
{
	//ft_printf("parser\n");
	if (g_status != 0)
	{
		clear_loop();
		return NULL;
	}
	//ft_printf("TOKENS LEXED BEFORE PARSING  ///////////////////////////////////\n\n");
	// display_tknlist(mini->tkn_lst);//
	verify_syntax_tknlist(mini->tkn_lst);
	if (g_status != 0)
	{
		clear_loop();
		return (NULL);
	}
	reducer(mini->tkn_lst->head); //gerer le cas ou la reduction entraine une chaine vide comme echo "" ...
	expander(mini, mini->tkn_lst);
	linker(mini->tkn_lst);
	rearrange_cmd_redir_order(mini->tkn_lst);
	set_token_index(mini->tkn_lst->head);
	//ft_printf("TOKENS PARSED BEFORE BTREE CREATION ///////////////////////////////////\n\n");
	display_tknlist(mini->tkn_lst);//
	return (create_bin_tree(mini->tkn_lst));
}