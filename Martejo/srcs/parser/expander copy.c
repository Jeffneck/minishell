#include "../../include/minishell.h"


static int	is_charset_env(char c)
{
	if (ft_isalnum(c) || c == '_') //ajouter les whitespaces (a priori non)
		return (1);
	return(0);
}

static char	*expand_g_status_dollar(t_mini *mini, t_env *env, char *str, size_t start)
{
	char	*expansion;
	char	*new;
	
	expansion = ft_itoa(mini->last_gstatus); //remplacer par g_status
	if (!expansion)
		exit(EXIT_FAILURE); //err man
	add_to_garbage(expansion, TMP);
	new = replace_substr(str, expansion, start, 2); //valider le 0 ou 1 ou 2
	if (!new)
		exit(EXIT_FAILURE); //err man
	add_to_garbage(new, TKN_LIST);
}

static char	*expand_env_dollar(t_mini *mini, t_env *env, char *str, size_t start) //stopper a wildcard et espaces
{
	//ft_printf(" expand_dollar\n");
	
	char	*var;
	char	*expansion;
	char	*new;
	size_t	len_var;

	var = calloc_gc(len_var + 1, sizeof(char), TMP);
	if (!var)
		exit(EXIT_FAILURE); //err man
	ft_printf("start = %c , len_var = %u\n", str[start], (unsigned int)len_var); //
	ft_memcpy(var, &str[start + 1], len_var); //valider les + 1 - 1
	//ft_printf("var = %s\n", var);
	expansion = get_env_value(env, var, len_var );//valider les + 1 - 1 et gestion du signe = (GEOFFREY comprend)
	ft_printf("expansion = %s\n", expansion);
	if (!expansion)
		exit(EXIT_FAILURE); //err man
	new = replace_substr(str, expansion, start, len_var + 1); //valider les + 1 - 1
	// ft_printf("new = %s\n", new);
	if (!new)
		exit(EXIT_FAILURE); //err man
	add_to_garbage(new, TKN_LIST);
	del_one_garbage(str, TKN_LIST);
	if(char_is_in_str('$', new))
		expander_handler
	return (new);
}

char	*expander_handler(t_mini *mini, t_env *env, t_token *tkn, t_tknlist *tkn_lst) //cette commande ne sert pratiquement a rien, refactoriser
{
	// ft_printf(" expander_handler, token to expand = %s\n", tkn->content);
	char *new;
	ssize_t	i;

	new = strdup_gc(tkn->content, TKN_LIST);
	i = 0;
	while(tkn->content[i])
	{
		if (tkn->content[i] == '$')
		{
			if (tkn->content[i + 1] == '?')
			{
				replace_substr()
				new = strjoin_gc(new, expand_g_status_dollar(mini, env, tkn->content, i));
				
			}
			else if (ft_isdigit(tkn->content[i + 1]))
				new = strdup_gc(tkn->content[i + 2], TKN_LIST);
			else if (!is_charset_env(tkn->content[i + 1])) // add
				new = strdup_gc(tkn->content[i], TKN_LIST);
			else
			{
				new = expand_env_dollar(mini, env, tkn->content, i);

			}
		}
		if (tkn->content[i] == '$' && is_charset_env(tkn->content[i + 1])) // add
		{
			new = expand_dollar(mini, env, tkn->content, i);
			break ;
			//tkn->content = new;
			//i--;
		}
		i++;
	}
	return (new);
}

void	expander(t_mini *mini,t_tknlist *tkn_lst, t_env *env)
{
	//ft_printf(" expander\n");
	t_token	*curr;
	char	*tmp;

	if (g_status != 0)
		return;
	curr = tkn_lst->head;
	while (curr)
	{
		//expansion heredoc
		if(curr->type == WORD || curr->type == IN || curr->type == OUT || curr->type == APPEND || curr->type == TWO_QUOTE)
		{
			tmp = expander_handler(mini, env, curr, tkn_lst);
			if (tmp)
			{
				del_one_garbage(curr->content, TKN_LIST);
				curr->content = tmp;
				continue;
			}
			// ft_printf("node after expander handler => %s type : %d\n", curr->content, curr->type);
			if(curr->type == WORD && char_is_in_str('*', curr->content)) // garder apres la boucle
				expand_wildcard(curr, tkn_lst);
		}
		curr = curr->next;
	}
}