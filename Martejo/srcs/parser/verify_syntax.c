#include "../../include/minishell.h"

void	verify_syntax(t_tknlist *list_tkn) //rename verify last tkn
{
// ne pas commencer par un token impossible &&, || , |
// certains tokens doivent obligatoirement être suivis d’autre chose qu’un operateur <, >, >>, <<, |, (, ||

	ft_printf("verify_syntax\n");
	t_token *tail;

	tail = list_tkn->tail;
	if (tail->type == PIPE)
	{
		ft_printf_fd(2, "Minishell: open pipe are not interpreted.");
		free_and_exit(2);
	}
	if (tail->type == AND || tail->type == OR)
	{
		ft_printf_fd(2, "Minishell: uncomplete logical operators are not interpreted.");
		free_and_exit(2);
	}
}