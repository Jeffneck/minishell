#include "../../include/minishell.h"

// static bool	is_cmd_redir_sequence(t_token *curr)
// {
// 	if (!is)
// }

// static bool	is_cmd_redir_sequence(t_token *curr)
// {
// 	if((!is_operator_tkn(curr->type)) && (!is_operator_tkn(curr->next->type)))
// 		return (1);
// 	return (0);
// }
static void	unbound_token_in_place(t_tknlist *list_tkn, t_token *to_pop) //rename pop_tkn_in_place
{
	ft_printf("unbound_token_in_place head %p tail %p\n", list_tkn->head, list_tkn->tail);
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
}

static void	tknlst_addfront(t_token *curr, t_tknlist *tknlst)
{
	ft_printf("tknlst_addfront\n");
	if (!curr || !tknlst)
		return ;
	curr->prev = NULL;
	curr->next = tknlst->head;
	if (tknlst->head)
		tknlst->head->prev = curr;
	if (!tknlst->tail)
		tknlst->tail = curr;
	tknlst->head = curr;
}

static void	tknlst_addback(t_token *curr, t_tknlist *tknlst)
{
	ft_printf("tknlst_addback\n");
	if (!curr || !tknlst)
		return ;
	curr->next = NULL;
	if (!tknlst->head)
	{
		tknlst->head = curr;
		tknlst->tail = curr;
	}
	else
	{
		curr->prev = tknlst->tail;
		tknlst->tail->next = curr;
		tknlst->tail = curr;
	}
}

static t_token	*return_end_sequence(t_token *begin_seq)
{
	ft_printf("return_end_sequence\n");
	t_token *curr;

	curr = begin_seq;
	while (curr && !is_operator_tkn(curr->type))
	{
		curr = curr->next;
	}
	return (curr);
}

static size_t	size_sequence(t_token *begin_seq)
{
	ft_printf("size_sequence\n");
	t_token *curr;
	size_t	i;

	curr = begin_seq;
	i = 0;
	while (curr && !is_operator_tkn(curr->type))
	{
		i++;
		curr = curr->next;
	}
	return (i);
}

static t_token	*reorder_sequence(t_tknlist *tknlst, t_token *curr)
{
	ft_printf("reorder_sequence curr->content = %s\n", curr->content);
	t_tknlist	*reordered_lst;
	t_token		*before_seq;
	t_token		*after_seq;
	t_token		*prev;

	init_list(&reordered_lst);
	before_seq = curr->prev;
	after_seq = return_end_sequence(curr);
	ft_printf("before_seq = %p after_seq = %p\n", before_seq, after_seq);
	// ft_printf("before_seq = %p after_seq = %p\n", before_seq, after_seq);
	while (curr->next != after_seq)
		curr = curr->next;
	while(curr != before_seq && !is_operator_tkn(curr->type))
	{
		prev = curr->prev;
		unbound_token_in_place(tknlst, curr);
		if(is_cmd_tkn(curr->type) || curr->type == PARENTHESIS)
			tknlst_addfront(curr, reordered_lst);
		if(is_redir_tkn(curr->type))
			tknlst_addback(curr, reordered_lst);
		ft_printf("curr = %p\n", curr);
		curr = prev;
		// ft_printf("reorder_sequence curr->content = %s\n", curr->content);

	}
	ft_printf("reordered_lst->head = %p reordered_lst->tail = %p\n", reordered_lst->head, reordered_lst->tail);
	// display_tknlist(reordered_lst);
	if (before_seq == NULL)
	{
		tknlst->head = reordered_lst->head;
		reordered_lst->tail->next = after_seq;
		if (after_seq == NULL)
			tknlst->tail = reordered_lst->tail;
		ft_printf("2\n");
	}
	else
	{
		add_tknlst_in_tknlst_after_target(tknlst, before_seq, reordered_lst);
		ft_printf("3\n");

	}
	return (after_seq);
}

void	rearrange_cmd_redir_order(t_tknlist *tknlst)
{
	ft_printf("rearrange_cmd_redir_order\n");
	t_token *curr; 

	curr = tknlst->head;
	while(curr && curr->next)
	{
		if(size_sequence(curr) > 2)
			curr = reorder_sequence(tknlst, curr);
		if(curr)
			curr = curr->next;
	}
}