#include "../../include/minishell.h"

void	verify_syntax(t_tknlist *list_tkn)
{
// ne pas commencer par un token impossible &&, || , |
// certains tokens doivent obligatoirement être suivis d’autre chose qu’un operateur <, >, >>, <<, |, (, ||

	t_token *curr;
	t_token *next;

	curr = list_tkn->head;
	if (curr->type == AND || curr->type == OR || curr->type == PIPE)
		exit(EXIT_FAILURE); //err management
	while (curr)
	{
		next = curr->next;
		if (is_token_operator(*curr))
		{
			if (!next || is_token_operator(*next))
				exit(EXIT_FAILURE); //err management
		}
		curr = curr->next;
	}
}