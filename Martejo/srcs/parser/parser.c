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

t_btree	*parser(t_tknlist *tknlst, t_env *env)
{
	ft_printf("parser\n");
	verify_syntax(tknlst);
	reducer(tknlst->head); //gerer le cas ou la reduction entraine une chaine vide comme echo "" ...
	expander(tknlst, env);
	linker(tknlst); //regroupe plusieurs nodes linkes en 1 seul, transformer les types quotes en word (puisque l' expamnsion est terminee
	set_token_index(tknlst->head);
	ft_printf("TOKENS PARSED BEFORE BTREE CREATION ///////////////////////////////////\n\n");
	display_tknlist(tknlst);//
	return (create_bin_tree(tknlst, env));
}