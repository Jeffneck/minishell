#include "../../include/minishell.h"

void	set_token_index(t_token	*curr)
{
	size_t	i;

	i = 0;
	while(curr)
	{
		curr->index = i;
		if(is_redir_tkn(curr->type) || curr->type == HEREDOC)
			curr->priority = 1;
		if(is_redir_tkn(curr->type) || curr->type == PARENTHESIS)
			curr->priority = 2;
		if(curr->type == PIPE)
			curr->priority = 3;
		if(is_logical_op_tkn(curr->type))
			curr->priority = 4;
		i++;
		curr = curr->next;
	}
}

size_t	nb_redirs_in_a_row(t_token *curr)
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

void	reverse_cmd_redirs(t_tknlist *tknlst, t_token *first, size_t nb_redirs)
{
	ft_printf("reverse_cmd_redirs\n");
	printf(" first debut = %s\n nb redir = %zu\n", first->content, nb_redirs);
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
	ft_printf("next = %s\n", first->next->content);
	// return (curr);
}

void	rearrange_cmd_redir_order(t_tknlist *tknlst)
{
	ft_printf("rearrange_cmd_redir_order\n");
	
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

t_btree	*parser(t_tknlist *tknlst, t_env *env)
{
	ft_printf("parser\n");
	
	ft_printf("TOKENS LEXED BEFORE PARSING  ///////////////////////////////////\n\n");
	display_tknlist(tknlst);//
	verify_syntax(tknlst);
	reducer(tknlst->head); //gerer le cas ou la reduction entraine une chaine vide comme echo "" ...
	expander(tknlst, env);
	rearrange_cmd_redir_order(tknlst);
	set_token_index(tknlst->head);
	ft_printf("TOKENS PARSED BEFORE BTREE CREATION ///////////////////////////////////\n\n");
	display_tknlist(tknlst);//
	return (create_bin_tree(tknlst, env));
}