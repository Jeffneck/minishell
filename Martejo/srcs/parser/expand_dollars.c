#include "../../include/minishell.h"

static int	is_charset_env(char c)
{
	if (ft_isalnum(c) || c == '_') //ajouter les whitespaces (a priori non)
		return (1);
	return(0);
}

//init type ser pour echo $STAR
static void	lstadd_dollar_expansions (t_mini *mini, t_tkntype init_type, char *to_expand, t_tknlist *dollar_lst)
{
	size_t	i;
	t_token	*new_tkn;
	size_t	len_sub;

	i = 0;
	while (to_expand[i])
	{
		new_tkn = NULL;
		len_sub = 1;
		if(to_expand[i] == '$')
		{
			if (to_expand[i + 1] == '?')
				new_tkn = create_node(init_type, itoa_gc(mini->last_gstatus, TKN_LIST), 1); //pas besoin de la fonction ?
			else if (ft_isdigit(to_expand[i + 1]))
				new_tkn = create_node(init_type, "", 1); //pas besoin de la fonction ?
			else if (!is_charset_env(to_expand[i + 1]))
				new_tkn = create_node(init_type, strndup_gc(&to_expand[i], len_sub + 1, TKN_LIST), 1);
			else
			{
				len_sub = ft_strlen_until_not(&to_expand[i + 1], is_charset_env);
				new_tkn = create_node(init_type, get_env_value(mini->env, &to_expand[i + 1], len_sub), 1);
			}
		}
		else
		{
			len_sub = ft_strlen_until_char(&to_expand[i], '$') - 1;
			//ft_printf("len_sub = %d", (int)len_sub);
			new_tkn = create_node(init_type, strndup_gc(&to_expand[i], len_sub + 1, TKN_LIST), 1);
		}
		//qd on aura modif create node il faudra surement tester si new node est NULL
		add_node(dollar_lst, new_tkn);
		//ft_printf("len sub = %d i = %d letter = %c\n",(int) len_sub, (int) i,  to_expand[i]);
		
		i += len_sub + 1;
	}
}

t_token	*expand_dollar(t_mini *mini, t_token *tkn_toexpand, t_tknlist *tkn_lst)
{
	// ft_printf(" expander_handler, token to expand = %s\n", tkn->content);
	t_tknlist	*dollar_lst;
	init_list(&dollar_lst);
	lstadd_dollar_expansions(mini, tkn_toexpand->type, tkn_toexpand->content, dollar_lst);
	// if(!dollar_lst->head)
	// 	return (tkn_toexpand);
	dollar_lst->tail->link = tkn_toexpand->link;
	add_tknlst_in_tknlst_after_target(tkn_lst, tkn_toexpand, dollar_lst);
	pop_token_in_place(tkn_lst, tkn_toexpand);
	// display_tknlist(tkn_lst);
	//exit(EXIT_FAILURE); //
	// tkn_toexpand = dollar_lst->head;
	return (dollar_lst->tail);
}

