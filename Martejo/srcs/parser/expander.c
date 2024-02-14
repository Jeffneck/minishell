#include "parser.h"
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
	if (!ft_isalnum(c) && c != '_')
		return (0);
	return(1);
}

char	*expand_dollar(t_env *env, char *str, size_t start) //stopper a wildcard et espaces
{
	char	*var;
	char	*expansion;
	char	*new;
	size_t	len_var;
	
	if (str[start + 1] == '?')
		len_var = 2;
	else
		len_var = ft_strlen_until(&str[start], is_charset_env);
	var = calloc_gc(len_var + 1, sizeof(char), TMP);
	if (!var)
		exit(EXIT_FAILURE); //err man
	ft_memcpy(var, &str[start + 1], len_var - 1); //valider les + 1 - 1
	expansion = get_env_path(env, var, len_var - 1);//valider les + 1 - 1 et gestion du signe = (GEOFFREY comprend)
	if (!expansion)
		exit(EXIT_FAILURE); //err man
	// apply_expansion(str, expansion);
	new = replace_substr(str, expansion, start, len_var -1); //valider les + 1 - 1
	if (!new)
		exit(EXIT_FAILURE); //err man
	del_one_garbage(expansion, TKN_LIST); // on peut ne pas utiliser le gc dans getenv_mini
	del_one_garbage(str, TKN_LIST);
	return (new);
}

char	*expander_handler(t_env *env, t_token *tkn, t_tknlist *tkn_lst)
{
	char *new;
	ssize_t	i;

	new = NULL;
	if (!tkn || !tkn->content || !tkn_lst)
		exit(EXIT_FAILURE); //err man
	i = 0;
	while(tkn->content[i])
	{
		if (tkn->content[i] == '$')
			new = expand_dollar(env, tkn->content, i);
		i++;
	}
	if(tkn->type != TWO_QUOTE && char_is_in_str('*', tkn->content)) // garder apres la boucle
		new = expand_wildcard(tkn->content, tkn_lst);
	return (new);
}

void	expander(t_mini mini)
{
	t_tknlist *tkn_lst;
	t_token	*curr;
	char	*tmp;

	tkn_lst = mini.tkn_lst;
	curr = tkn_lst->head;
	while (curr)
	{
		//expansion heredoc
		if(curr->type == WORD || curr->type == IN || curr->type == OUT || curr->type == APPEND || curr->type == TWO_QUOTE)
		{
			tmp = expander_handler(curr, tkn_lst);
			if (!tmp)
				exit(EXIT_FAILURE); //err man
			del_one_garbage(curr->content, TKN_LIST);
			curr->content = tmp;
		}
		curr = curr->next;
	}
}