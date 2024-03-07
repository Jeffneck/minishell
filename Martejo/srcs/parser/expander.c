#include "../../include/minishell.h"

static bool	is_dollar_expansible(t_token *tkn)
{
	if(!char_is_in_str('$', tkn->content))
		return (false);
	if (tkn->prev && tkn->prev->type == HEREDOC)
		return (false);
	if (tkn->type != WORD && tkn->type != TWO_QUOTE)
		return (false);
	return (true);
}

static bool	is_wildcard_expansible(t_token *tkn)
{
	if(!char_is_in_str('*', tkn->content))
		return (false);
	if (tkn->prev && tkn->prev->type == HEREDOC)
		return (false);
	if (tkn->type != WORD)
		return (false);
	return (true);
}

void	expander(t_mini *mini,t_tknlist *tkn_lst)
{
	//ft_printf(" expander\n");
	t_token	*curr;

	if (g_status != 0)
		return;
	curr = tkn_lst->head;
	while (curr)
	{
		if(is_dollar_expansible(curr))
			curr = expand_dollar(mini, curr, tkn_lst);
		display_tknlist(tkn_lst);
		// ft_printf("curr = %s\n", curr->content);
		// if(curr->next)
		// 	ft_printf("next = %s\n", curr->next->content);
		// sleep(2);
		curr = curr->next;
	}
	// ft_printf("curr = %s\n", curr->content);
	curr = tkn_lst->head;
	while (curr)
	{
		if(is_wildcard_expansible(curr))
			curr = expand_wildcard(curr, tkn_lst);
		curr = curr->next;
	}
}