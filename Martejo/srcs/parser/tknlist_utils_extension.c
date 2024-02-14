#include "parser.h"
#include "../../include/minishell.h"

void	add_after_another(t_tknlist	*list, t_token *el_flag, t_token *el_toplace)
{
	el_toplace->next = el_flag->next;
	el_flag->next = el_toplace;
	el_toplace->prev = el_flag;
	if (el_flag == list->tail)
		list->tail = el_toplace;
}

//token_utils.c
int	is_token_operator(t_token tkn)
{
	if (tkn.type == AND || tkn.type == OR || tkn.type == PIPE || tkn.type == HEREDOC || tkn.type == IN || tkn.type == OUT || tkn.type == APPEND)
		return (1);
	return (0);
}

void	pop_node_in_place(t_tknlist *list_tkn, t_token *to_pop)
{
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
		del_one_garbage(to_pop->content, LEXER);
    del_one_garbage(to_pop, LEXER);
}

void	add_tknlst_in_tknlst_after_target(t_tknlist *list, t_token *target_tkn, t_tknlist *list_expnd)
{
	if (!list || !list->head || !target_tkn || !list_expnd || !list_expnd->head)
		return ;
	if (target_tkn == list->tail)
		list->tail = list_expnd->tail;
	(list_expnd->tail)->next = target_tkn->next;
	target_tkn->next = (list_expnd->head);
}

void	swap_tokens(t_tknlist	*lst, t_token *tkn1, t_token *tkn2)
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

void	display_tknlist(t_tknlist *list)
{
		t_token *curr;
		int	i;

		curr = list->head;
		i = 0;
		while (curr)
		{
			printf("--NODE (%d)--\n", i);
			printf("type = %d\n", curr->type);
			printf("content = %s\n\n", curr->content);
			curr = curr->next;
			i++;
		}
}