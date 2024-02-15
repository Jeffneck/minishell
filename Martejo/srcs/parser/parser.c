#include "../../include/minishell.h"

void	set_token_index(t_token	*curr)
{
	size_t	i;

	i = 0;
	while(curr)
	{
		curr->index = i;
		if(curr->type == PARENTHESIS)
			curr->priority = 1;
		if(curr->type == AND)
			curr->priority = 2;
		if(curr->type == OR)
			curr->priority = 3;
		if(curr->type == PIPE)
			curr->priority = 4;
		i++;
		curr = curr->next;
	}
}


t_btree	*parser(t_mini mini)
{
	ft_printf("parser\n");
	verify_syntax(mini.tkn_lst);
	// display_tknlist(mini.tkn_lst);//
	reducer(mini.tkn_lst->head); //gerer le cas ou la reduction entraine une chaine vide comme echo "" ...
	expander(mini.tkn_lst, mini.env);
	linker(mini.tkn_lst); //regroupe plusieurs nodes linkes en 1 seul, transformer les types quotes en word (puisque l' expamnsion est terminee
	set_token_index(mini.tkn_lst->head);
	// create_bin_tree(list_tkn->head);
	return (NULL);
}