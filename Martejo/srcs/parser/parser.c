#include "../../include/minishell.h"
#include "parser.h"

t_btree	*parser(t_mini mini)
{
	verify_syntax(mini.tkn_lst);
	reducer(mini.tkn_lst->head); //gerer le cas ou la reduction entraine une chaine vide comme echo "" ...
	expander(mini);
	linker(mini.tkn_lst->head); //regroupe plusieurs nodes linkes en 1 seul, transformer les types quotes en word (puisque l' expamnsion est terminee)
	set_token_index(mini.tkn_lst->head);
	create_bin_tree(list_tkn->head);

}