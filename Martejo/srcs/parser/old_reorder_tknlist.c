#include "../../include/minishell.h"

static size_t	nb_redirs_in_a_row(t_token *curr)
{
	size_t	i;

	i = 0;
	while(curr && is_redir_tkn(curr->type))
	{
		curr = curr->next;
		i++;
	}
	return (i);
}

static void	reverse_cmd_redirs(t_tknlist *tknlst, t_token *first, size_t nb_redirs)
{
	//ft_printf("reverse_cmd_redirs\n");
	//printf(" first debut = %s\n nb redir = %zu\n", first->content, nb_redirs);
	size_t	i;
	size_t	j;
	t_token	*curr;
	t_token	*next;
	t_token *tmp;

	i = 0;
	tmp = first;
	while (i < nb_redirs - 1)
	{
		curr = tmp;
		tmp = curr->next;
		j = i;
		while(j < nb_redirs - 1)
		{
			next = curr->next;
			swap_tokens(tknlst, curr, next);
			j++;
		}
		i++;
	}
	// curr = first;
	// while (nb_redirs > 0)
	// {
	// 	curr = curr->next;
	// 	nb_redirs--;
	// }
	//ft_printf("next = %s\n", first->next->content);
	// return (curr);
}

void	rearrange_cmd_redir_order(t_tknlist *tknlst)
{
	//ft_printf("rearrange_cmd_redir_order\n");
	
	t_token *curr; 
	t_token *next; 

	curr = tknlst->head;
	while(curr && curr->next)
	{
		next = curr->next;
		if((is_redir_tkn(curr->type) || curr->type == HEREDOC) && is_cmd_tkn(next->type))
		{
			swap_tokens(tknlst, curr, next);
			continue;
		}
		// if((is_redir_tkn(curr->type) || curr->type == HEREDOC) && (is_redir_tkn(next->type) || next->type == HEREDOC) )
		// {
		// 	swap_tokens(tknlst, curr, next);
		// 	continue;
		// }
		curr = curr->next;
	}
	curr = tknlst->head;
	while(curr && curr->next)
	{
		next = curr->next;
		if((is_redir_tkn(curr->type) || curr->type == HEREDOC)
			&& (is_redir_tkn(next->type) || next->type == HEREDOC))
			reverse_cmd_redirs(tknlst, curr, nb_redirs_in_a_row(curr));
		curr = curr->next;
	}
	
}