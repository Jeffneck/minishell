#include "../../../include/minishell.h"

// static t_token	*return_end_sequence(t_token *begin_seq)
// {
// 	t_token	*curr;

// 	curr = begin_seq;
// 	while (curr && !is_operator_tkn(curr->type))
// 	{
// 		curr = curr->next;
// 	}
// 	return (curr);
// }

static size_t	size_sequence(t_token *begin_seq)
{
	t_token	*curr;
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

static	void	extract_tkn_in_another_tknlist(t_token *tkn, \
		t_tknlist *tknlst_src, t_tknlist *tknlst_dest)
{
	unbound_token_in_place(tknlst_src, tkn);
	if (is_cmd_tkn(tkn->type) || tkn->type == PARENTHESIS)
		tknlst_addfront(tkn, tknlst_dest);
	if (is_redir_tkn(tkn->type) || tkn->type == HEREDOC)
		tknlst_addback(tkn, tknlst_dest);
}

static void	reinsert_ordered_sequence(t_tknlist *tknlst, \
		t_token *tkn_before, t_token *tkn_after, t_tknlist *reordered_lst)
{
	if (tkn_before == NULL)
	{
		tknlst->head = reordered_lst->head;
		reordered_lst->tail->next = tkn_after;
		if (tkn_after == NULL)
			tknlst->tail = reordered_lst->tail;
	}
	else
		add_tknlst_in_tknlst_after_target(tknlst, tkn_before, reordered_lst);
}

static t_token	*reorder_sequence(t_tknlist *tknlst, t_token *curr)
{
	t_tknlist	*reordered_lst;
	t_token		*tkn_before_seq;
	t_token		*tkn_after_seq;

	init_list(&reordered_lst);
	tkn_before_seq = curr->prev;
	while (curr && !is_operator_tkn(curr->type))
		curr = curr->next;
	tkn_after_seq = curr;
	curr = curr->prev;
	while (curr != tkn_before_seq && !is_operator_tkn(curr->type))
	{
		extract_tkn_in_another_tknlist(curr, tknlst, reordered_lst);
		curr = curr->prev;
	}
	reinsert_ordered_sequence(tknlst, tkn_before_seq, \
		tkn_after_seq, reordered_lst);
	return (tkn_after_seq);
}

void	rearrange_cmd_redir_order(t_tknlist *tknlst)
{
	t_token	*curr;

	curr = tknlst->head;
	while (curr && curr->next)
	{
		if (size_sequence(curr) > 1)
			curr = reorder_sequence(tknlst, curr);
		if (curr)
			curr = curr->next;
	}
}
