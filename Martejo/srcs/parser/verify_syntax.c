#include "../../include/minishell.h"

void	verify_last_tkn(t_token *tail)
{
	ft_printf("verify_last_tkn\n");

	if (tail->type == PIPE)
	{
		ft_printf_fd(2, "Minishell: open pipe are not interpreted.");
		g_status = 2;
	}
	else if (tail->type == AND || tail->type == OR)
	{
		ft_printf_fd(2, "Minishell: uncomplete logical operators are not interpreted.");
		g_status = 2;
	}
}