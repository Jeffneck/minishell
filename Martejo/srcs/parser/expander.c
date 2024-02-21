#include "../../include/minishell.h"

//Expander_utils.c
// char	*apply_expansion(char *str, char *expansion, size_t	len)
// {
// 	if (expansion)
// 		new = replace_substr(str, expansion, 0, len);
// 	else
// 		new = remove_substr(str, 0, len);
// }

int	is_charset_env(char c)
{
	if (ft_isalnum(c) || c == '_')
		return (1);
	return(0);
}

char	*expand_dollar(t_env *env, char *str, size_t start) //stopper a wildcard et espaces
{
	ft_printf(" expand_dollar\n");
	
	char	*var;
	char	*expansion;
	char	*new;
	size_t	len_var;
	
	if (str[start + 1] == '?')
	{
		expansion = ft_itoa(12); //remplacer par g_status
		if (!expansion)
			exit(EXIT_FAILURE); //err man
		add_to_garbage(expansion, TMP);
		new = replace_substr(str, expansion, start, 1); //valider le 0 ou 1 ou 2
		if (!new)
			exit(EXIT_FAILURE);		
		add_to_garbage(new, TKN_LIST);
	}

	else
	{
		len_var = ft_strlen_until(&str[start + 1], is_charset_env);
		var = calloc_gc(len_var + 1, sizeof(char), TMP);
		if (!var)
			exit(EXIT_FAILURE); //err man
		ft_printf("start = %c , len_var = %u\n", str[start], (unsigned int)len_var); //
		ft_memcpy(var, &str[start + 1], len_var); //valider les + 1 - 1
		ft_printf("var = %s\n", var);
		expansion = get_env_path(env, var, len_var );//valider les + 1 - 1 et gestion du signe = (GEOFFREY comprend)
		ft_printf("expansion = %s\n", expansion);
		if (!expansion)
			exit(EXIT_FAILURE); //err man
		// apply_expansion(str, expansion);
		new = replace_substr(str, expansion, start, len_var + 1); //valider les + 1 - 1
		
		ft_printf("new = %s\n", new);
		if (!new)
			exit(EXIT_FAILURE); //err man
		add_to_garbage(new, TKN_LIST);
	}
	if(expansion && expansion[0])
		free(expansion); // on peut ne pas utiliser le gc dans getenv_mini
	del_one_garbage(str, TKN_LIST);
	return (new);
}

char	*expander_handler(t_env *env, t_token *tkn, t_tknlist *tkn_lst) //cette commande ne sert pratiquement a rien, refactoriser
{
	// ft_printf(" expander_handler, token to expand = %s\n", tkn->content);
	char *new;
	ssize_t	i;

	new = NULL;
	if (!tkn || !tkn->content || !tkn_lst)
	{
		// ft_printf("1\n");
		exit(EXIT_FAILURE); //err man
	}
	i = 0;
	while(tkn->content[i])
	{
		if (tkn->content[i] == '$')
		{
			new = expand_dollar(env, tkn->content, i);
			break ;
			//tkn->content = new;
			//i--;
		}
		i++;
	}
	return (new);
}

void	expander(t_tknlist *tkn_lst, t_env *env)
{
	ft_printf(" expander\n");
	t_token	*curr;
	char	*tmp;

	curr = tkn_lst->head;
	while (curr)
	{
		//expansion heredoc
		if(curr->type == WORD || curr->type == IN || curr->type == OUT || curr->type == APPEND || curr->type == TWO_QUOTE)
		{
			tmp = expander_handler(env, curr, tkn_lst);
			if (tmp)
			{
				del_one_garbage(curr->content, TKN_LIST);
				curr->content = tmp;
				continue;
			}
			// ft_printf("node after expander handler => %s type : %d\n", curr->content, curr->type);
			if(curr->type != TWO_QUOTE && char_is_in_str('*', curr->content)) // garder apres la boucle
				expand_wildcard(&curr, tkn_lst);
		}
		curr = curr->next;
	}
}