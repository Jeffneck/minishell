#include "../../include/minishell.h"

void	add_after_another(t_tknlist	*list, t_token *tkn1, t_token *tkn2) //rename addtkn2_after_tkn1
{
	tkn2->next = tkn1->next;
	tkn1->next = tkn2;
	tkn2->prev = tkn1;
	if (tkn1 == list->tail)
		list->tail = tkn2;
}

void	pop_token_in_place(t_tknlist *list_tkn, t_token *to_pop) //rename pop_tkn_in_place
{
	//ft_printf("pop_token_in_place head %p tail %p\n", list_tkn->head, list_tkn->tail);
	if (to_pop == NULL)
		return;
    if (to_pop != list_tkn->head)
        (to_pop->prev)->next = to_pop->next;
	else
		list_tkn->head = to_pop->next;
    if (to_pop != list_tkn->tail)
        (to_pop->next)->prev = to_pop->prev;
	else 
		list_tkn->tail = to_pop->prev;
	if (to_pop->content)
		del_one_garbage(to_pop->content, TKN_LIST);
//	ft_printf("after head %p tail %p\n", list_tkn->head, list_tkn->tail);
    del_one_garbage(to_pop, TKN_LIST);
}

void	add_tknlst_in_tknlst_after_target(t_tknlist *lst1, t_token *tkn_flag, t_tknlist *lst2) // rename add_tknlst1_inside_tknlst2
{
	if (!lst1 || !lst1->head || !tkn_flag || !lst2 || !lst2->head)
		return ;
	if (tkn_flag == lst1->tail)
		lst1->tail = lst2->tail;
	(lst2->tail)->next = tkn_flag->next;
	tkn_flag->next = (lst2->head);
}

// void add_tknlst_in_tknlst_after_target(t_tknlist *lst1, t_token *tkn_flag, t_tknlist *lst2) {
//     // Vérification préalable des arguments pour s'assurer qu'ils ne sont pas NULL
//     // et que les listes ne sont pas vides.
//     if (!lst1 || !lst1->head || !tkn_flag || !lst2 || !lst2->head)
//         return;

//     // Vérification pour s'assurer que tkn_flag est dans lst1.
//     t_token *current = lst1->head;
//     int flagFound = 0;
//     while (current) {
//         if (current == tkn_flag) {
//             flagFound = 1;
//             break;
//         }
//         current = current->next;
//     }
//     if (!flagFound) // Si tkn_flag n'est pas trouvé dans lst1, on sort de la fonction.
//         return;

//     // Insertion de lst2 après tkn_flag
//     if (tkn_flag->next) { // Si tkn_flag n'est pas le dernier élément de lst1
//         (lst2->tail)->next = tkn_flag->next;
//     } else { // Si tkn_flag est le dernier élément, on met à jour lst1->tail
//         lst1->tail = lst2->tail;
//     }
//     tkn_flag->next = lst2->head;
// }

void	swap_tokens(t_tknlist	*lst, t_token *tkn1, t_token *tkn2) //rename swap_tkns_in_tknlist
{
    t_token *tmp_prev;

    if (tkn1 == NULL || tkn2 == NULL || lst == NULL || lst->head == NULL || lst->tail == NULL)
		return ;// error management
	tmp_prev = tkn1->prev;
    if (tkn1 == lst->head)
        lst->head = tkn2;
    else
        (tkn1->prev)->next = tkn2;
    if (tkn2 == lst->tail)
        lst->tail = tkn1;
    else
        (tkn2->next)->prev = tkn1;
    tkn1->prev = tkn2;
    tkn1->next = tkn2->next;
    tkn2->prev = tmp_prev;
    tkn2->next = tkn1;
}

size_t	tknlst_size(t_tknlist *tknlst)
{
	t_token	*curr;
	size_t	i;

	curr = tknlst->head;
	i = 0;
	while (curr)
	{
		curr = curr->next;
		i++;
	}
	return (i);
}