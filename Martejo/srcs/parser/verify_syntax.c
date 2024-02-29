#include "../../include/minishell.h"

void	verify_syntax(t_tknlist *list_tkn)
{
// ne pas commencer par un token impossible &&, || , |
// certains tokens doivent obligatoirement être suivis d’autre chose qu’un operateur <, >, >>, <<, |, (, ||

	ft_printf("verify_syntax\n");
	t_token *curr;
	t_token *next;

	curr = list_tkn->head;
	if (curr->type == AND || curr->type == OR || curr->type == PIPE)
	{
		ft_printf_fd(2, "bash: syntax error near unexpected token %s", curr->content);
		free_and_exit(2);
	}
//Normalement l'erreur qui suit et geree en amont dans le lexer

	while (curr)
	{
		next = curr->next;
		if (is_operator_tkn(curr->type))
		{
			if (!next || is_operator_tkn(next->type)) //bash: syntax error near unexpected token `next->content[0]'  // $? = 2 //err avant exec
				exit(EXIT_FAILURE); //err management
		}
		curr = curr->next;
	}
}