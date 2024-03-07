#include "../../include/minishell.h"

static int	is_empty_parenthis(char *content)
{
	size_t	i;

	i = 1;
	while (content[i] && content[i] != ')')
	{
		if (ft_isspace(content[i]) == 0)
			return (0);
		i++;
	}
	return (1);
}

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
	if (curr->type == PARENTHESIS && is_empty_parenthis(curr->content) == 1)
	{
		ft_printf_fd(2,"%s%s `%c'%s\n", RED, NEAR_TOKEN_ERR_MSG, ')', RESET);
		g_status = 2;
		return ;
	}
	while (curr && curr->next && g_status == 0)
	{
		if ((is_operator_tkn(curr->type) || is_redir_tkn(curr->type)) && is_operator_tkn(curr->next->type))
		{
			ft_printf_fd(2,"%s%s `%s'%s\n", RED, NEAR_TOKEN_ERR_MSG, curr->next->content, RESET);
			g_status = 2;
			return ;
		}
		if (is_redir_tkn(curr->type) && curr->next->type == WORD && char_is_in_str('*', curr->next->content)) //type heredoc retire car <<* fonctionne (pas d'expansion)
		{
			ft_printf_fd(2,"%s%s%s", RED, STAR_TOKEN_ERR_MSG, RESET);
			g_status = 1;
			return ;
		}
		if (curr->type == PARENTHESIS && is_empty_parenthis(curr->content) == 1)
		{
			ft_printf_fd(2,"%s%s `%c'%s\n", RED, NEAR_TOKEN_ERR_MSG, '(', RESET);
			g_status = 2;
			return ;
		}
		curr = curr->next;
	}
	if (curr->type == PARENTHESIS && is_empty_parenthis(curr->content) == 1)
	{
		ft_printf_fd(2,"%s%s `%c'%s\n", RED, NEAR_TOKEN_ERR_MSG, '(', RESET);
		g_status = 2;
		return ;
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
	else if (is_redir_tkn(tail->type) || tail->type == HEREDOC)
	{
			ft_printf_fd(2,"%s%s `%s'%s\n", RED, NEAR_TOKEN_ERR_MSG, "newline", RESET);
		g_status = 2;
	}
}