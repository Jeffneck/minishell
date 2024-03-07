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

static void	tknlst_addfront(t_token *curr, t_tknlist *tknlst)
{
	if (!curr || !tknlst)
		return ;
	curr->next = tknlst->head;
	if (tknlst->head)
		tknlst->head->prev = curr;
	if (!tknlst->tail)
		tknlst->tail = curr;
}

static void	tknlst_addback(t_token *curr, t_tknlist *tknlst)
{
	if (!curr || !tknlst)
		return ;
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
	t_tknlist	*reordered_lst;
	t_token		*before_seq;
	t_token		*after_seq;

	
	init_list(&reordered_lst);
	before_seq = curr->prev;
	after_seq = return_end_sequence(curr);
	while(curr->next && !is_operator_tkn(curr->type))
	{
		if(is_cmd_tkn(curr->type) || curr->type == PARENTHESIS)
			tknlst_addfront(curr, reordered_lst);
		if(is_redir_tkn(curr->type))
			tknlst_addback(curr, reordered_lst);
		curr = curr->next;
	}
	
	if (before_seq == NULL)
	{
		tknlst->head = reordered_lst->head;
		reordered_lst->tail->next = after_seq;
		if (after_seq == NULL)
			tknlst->tail = reordered_lst->tail;
	}
	else
		add_tknlst_in_tknlst_after_target(tknlst, before_seq, reordered_lst);
	return (after_seq);
}

void	rearrange_cmd_redir_order(t_tknlist *tknlst)
{
	//ft_printf("rearrange_cmd_redir_order\n");
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