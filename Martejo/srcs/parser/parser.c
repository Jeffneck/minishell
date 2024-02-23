#include "../../include/minishell.h"

void	set_token_index(t_token	*curr)
{
	size_t	i;

	i = 0;
	while(curr)
	{
		curr->index = i;
		if(is_redir_tkn(curr->type))
			curr->priority = 1;
		if(is_redir_tkn(curr->type) || curr->type == PARENTHESIS)
			curr->priority = 2;
		if(curr->type == PIPE)
			curr->priority = 3;
		if(is_logical_op_tkn(curr->type))
			curr->priority = 4;
		i++;
		curr = curr->next;
	}
}

void	rearrange_cmd_redir_order(t_tknlist *tknlst)
{
	t_token *curr; 
	t_token *next; 

	curr = tknlst->head;
	while(curr && curr->next)
	{
		next = curr->next;
		if(is_redir_related_tkn(curr->type) && is_cmd_related_tkn(next->type))
			swap_tokens(tknlst, curr, next);
		curr = curr->next;
	}

}

t_btree	*parser(t_tknlist *tknlst, t_env *env)
{
	ft_printf("parser\n");
	
	ft_printf("TOKENS LEXED BEFORE PARSING  ///////////////////////////////////\n\n");
	display_tknlist(tknlst);//
	verify_syntax(tknlst);
	reducer(tknlst->head); //gerer le cas ou la reduction entraine une chaine vide comme echo "" ...
	expander(tknlst, env);
	rearrange_cmd_redir_order(tknlst);
	set_token_index(tknlst->head);
	ft_printf("TOKENS PARSED BEFORE BTREE CREATION ///////////////////////////////////\n\n");
	display_tknlist(tknlst);//
	return (create_bin_tree(tknlst, env));
}