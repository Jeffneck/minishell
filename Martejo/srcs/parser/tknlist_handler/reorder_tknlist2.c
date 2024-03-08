#include "../../../include/minishell.h"

t_token	*return_end_sequence(t_token *begin_seq)
{
	t_token	*curr;

	curr = begin_seq;
	while (curr && !is_operator_tkn(curr->type))
	{
		curr = curr->next;
	}
	return (curr);
}

size_t	size_sequence(t_token *begin_seq)
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