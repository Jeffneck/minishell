#include "../../../include/minishell.h"

static t_token	*return_end_sequence(t_token *begin_seq)
{
	t_token	*curr;

	curr = begin_seq;
	while (curr && !is_operator_tkn(curr->type))
	{
		curr = curr->next;
	}
	return (curr);
}

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

static t_token	*reorder_sequence(t_tknlist *tknlst, t_token *curr)
{
	t_tknlist	*reordered_lst;
	t_token		*before_seq;
	t_token		*after_seq;
	t_token		*prev;

	init_list(&reordered_lst);
	before_seq = curr->prev;
	after_seq = return_end_sequence(curr);
	while (curr->next != after_seq)
		curr = curr->next;
	while (curr != before_seq && !is_operator_tkn(curr->type))
	{
		prev = curr->prev;
		unbound_token_in_place(tknlst, curr);
		if (is_cmd_tkn(curr->type) || curr->type == PARENTHESIS)
			tknlst_addfront(curr, reordered_lst);
		if (is_redir_tkn(curr->type) || curr->type == HEREDOC)
			tknlst_addback(curr, reordered_lst);
		curr = prev;
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
