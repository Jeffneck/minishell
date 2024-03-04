#include "../../include/minishell.h"

void	verify_syntax_tknlist(t_tknlist *lst) //verify syntax
{
	//ft_printf("verify_last_tkn\n");
	t_token	*curr;
	t_token	*tail;

	curr = lst->head;
	if (is_operator_tkn(curr->type))
	{
		ft_printf_fd(2,"%s%s `%s'%s\n", RED, NEAR_TOKEN_ERR_MSG, curr->content, RESET);
		g_status = 2;
		return ;
	}
	while (curr && curr->next && g_status == 0)
	{
		if (is_operator_tkn(curr->type) && is_operator_tkn(curr->next->type))
		{
			ft_printf_fd(2,"%s%s `%s'%s\n", RED, NEAR_TOKEN_ERR_MSG, curr->next->content, RESET);
			g_status = 2;
			return ;
		}
		curr = curr->next;
	}
	tail = lst->tail;
	if (tail->type == PIPE)
	{
		ft_printf_fd(2,"%s%s%s\n", RED, OPEN_PIPE_ERR_MSG, RESET);
		g_status = 2;
		return ;
	}
	else if (tail->type == AND || tail->type == OR)
	{
		ft_printf_fd(2,"%s%s%s\n", RED, OPEN_LOGICAL_OP_ERR_MSG, RESET);
		g_status = 2;
	}
}