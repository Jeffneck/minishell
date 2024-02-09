# include "lexer.h"

typedef	struct s_btree
{
	char **content;
	int	io[2]; //in out (peut etre compose de pipe)
	int	branch;
	token_type	type;
	struct s_btree	*left;
	struct s_btree	*right;
}	t_btree;

//token_utils.c
t_token	*find_prior_token(t_lister *list_tkn)
{
	t_token	*curr;
	t_token	*target;

	curr = list_tkn->head;
	target = curr;
	while (curr)
	{
		if (curr->priority > target->priority)
			target = curr;
		curr = curr->next;
	}
	return (target);
}

//syntaxic_tree.c
t_btree	*place_in_tree(t_btree *root, t_btree *toplace, int index)
{
	if (root == NULL)
		return (toplace);
	// if (root->type == PARENTHESE)//
	// 	root->left = toplace;//
	if (index < root->branch)
		root->left = place_in_tree(root->left, toplace, index);
	else if (index > root->branch)
		root->right = place_in_tree(root->right, toplace, index);
	return (root);
}

//syntaxic_tree.c
t_btree	*create_bin_tree(t_lister *list_tkn)
{
	t_token	*target;
	t_btree	*root;

	while(list_tkn->head)
	{
		target = find_prior_token(list_tkn);
		if (!root)
		 	root =  _in_tree(root, btree_new(target), target->index);
		else if (target->type == PARENTHESE)
		{
			// verify_syntax_inside_parenthesis
			place_in_tree(root, parser(lexer(target->content)), target->index);
		}
		else
			place_in_tree(root, btree_new(target), target->index);
	}
	return root;
}


//token_utils.c
int	is_token_operator(token_type type)
{
	if (type == AND || type == OR || type == PIPE || type == HEREDOC || type == IN || type == OUT || type == APPEND)
		return (1);
	return (0);
}

void	verify_syntax(t_lister *list_tkn)
{
// ne pas commencer par un token impossible &&, || , |
// certains tokens doivent obligatoirement être suivis d’autre chose qu’un operateur <, >, >>, <<, |, (, ||

	t_token *curr;

	curr = list_tkn->head;
	if (curr->type == AND || curr->type == OR || curr->type == PIPE)
		close_error(); //
	while (curr)
	{
		if (!curr->next && is_token_operator(curr->type))
			close_error(); //
		if (is_token_operator(curr->next) && is_token_operator(curr->type))
			close_error(); //
		curr = curr->next;
	}
}

//expander_utils.c
//probleme, on ne peut pas gerer id_gc de l' exterieur (idee : creer une variable char ** qui contient str et replacement ou add to gc en dehors de la fonction ou toutjours utiliser le m^eme id ?
char	*replace_substr(char *str, char *replacement, size_t start, size_t len)
{
	char	*new;
	size_t	size_new;

	size_new = (ft_strlen(str) - len + ft_strlen(replacement)) + 1;
	new = (char *) calloc_gc(size_new, ID);
	if (new == NULL)
		return (NULL);// appliquer gestion d' erreur
	ft_memcpy(new, str, start); //ou start + 1 ou start - 1
	ft_strcat(new, replacement);
	ft_strcat(new, &str[start + len]); //valider
	return (new);
}

//expander_utils.c
//probleme, on ne peut pas gerer id_gc de l' exterieur (idee : creer une variable char ** qui contient str et replacement ou add to gc en dehors de la fonction ou toutjours utiliser le m^eme id ?
char	*remove_substr(char *str, size_t start, size_t len)
{
	char	*new;
	size_t	size_new;

	size_new = ft_strlen(str) - len + 1;
	new = (char *) calloc_gc(size_new, ID);
	if (new == NULL)
		return (NULL);// appliquer gestion d' erreur
	ft_memcpy(new, str, start); //ou start + 1 ou start - 1
	ft_strcat(new, &str[start + len]); //valider
	return (new);
}

//avance tant que la fonction ne detecte pas un char cible
//libft
size_t	ft_strlen_until(const char *str, int(*f)(char))
{
	size_t	len;

	len = 0;
	while (str[len] && !f(str[len]))
		len++;
	return (len);
}

//avance tant que la fonction detecte un char cible
//libft
size_t	ft_strlen_until_not(const char *str, int(*f)(char))
{
	size_t	len;

	len = 0;
	while (str[len] && f(str[len]))
		len++;
	return (len);
}

//Expander_utils.c
char	*apply_expansion(char *str, char *expansion, size_t	len)
{
	if (expansion)
		new = replace_substr(str, expansion, 0, len);
	else
		new = remove_substr(str, 0, len);
}

//apply_expansion.c
char	*expand_dollar(char *str, size_t start)
	char	*var;
	char	*expansion;
	char	*new;
	size_t	len_var;
	
	if (str[start + 1] == '?')
		len_var = 2;
	else
		len_var = ft_strlen_until(str[start], ft_isspace);
	var = ft_calloc(len_var + 1, sizeof(char));
	if (!var)
		return(ERROR);//
	ft_memcpy(var, str, len_var); //peut etre i + 1 ?
	expansion = getenv_mini(var);
	apply_expansion(str, expansion);
	del_one_garbage(expansion, ID); // on peut ne pas utiliser le gc dans getenv_mini
	del_one_garbage(str, ID);
	free(var);
	return (new);
}

//apply_expansion.c
char	*expand_tilde(char *str) //tilde n a pas besoin de start 
{
	char	*expansion;
	char	*new;
	size_t	len_sub;

	len_sub = 1;
	expansion = getenv_mini("HOME"); //ajouter ID dans getenv_mini
	apply_expansion(str, expansion, len_sub);
	del_one_garbage(expansion, ID); // on peut ne pas utiliser le gc dans getenv_mini
	del_one_garbage(str, ID);
	return (new);
}


int	str_contains_all_subs_ordered(char *str, char **subs)
{
	size_t	i;
	size_t	sub;
	size_t	j;
	
	if (!str || !subs || !subs[0])
		return (0);
	i = 0;
	sub = 0;
	j = 0;
	while(str[i])
	{
		if (subs[sub][j] == str[i])
			j++;
		if (!subs[sub][j])
		{
			printf("a");
			sub++;
			j = 0;
			if (!subs[sub])
				return (1);
			continue;
		}
		i++;
	}
	return (0);
}

#include <sys/types.h>
#include <dirent.h>

//apply_expansion.c
//cette expansion peut creer de nouveaux nodes 
char	*expand_wildcard(char *str, t_lister *list_tkn) //recupe la fonction du test
{
	// DIR				*dir;
	// char	*cwd;
	// char	*new;
	// size_t	len_sub;

	// len_sub = 1;
	// cwd = getenv_mini("PWD"); //ajouter ID dans getenv_mini
	// opendir(cwd);
	// apply_expansion(str, expansion, len_sub);
	// del_one_garbage(expansion, ID); // on peut ne pas utiliser le gc dans getenv_mini
	// del_one_garbage(str, ID);
	// return (new);
}

//libft
int	str_contains_char(char *str, char c)
{
	size_t	i;

	i = 0;
	while(str[i])
	{
		if (str[i] == c)
			return (1);
		i++;
	}
	return (0);
}

char	*expander_handle_word(char *str, t_lister *list_tkn)
{
	char *new;
	ssize_t	i;

	new = NULL;
	if (!str)
		return (ERROR); //utile ?
	if(str[0] == '~')
		new = expand_tilde(str);
	i = 0;
	while(str[i])
	{
		if (str[i] == '$')
			new = expand_dollar(str, i);
		if (str[i] == '*') // avant, pendant ou apres la boucle ??
			new = expand_wildcard(str, list_tkn);
		i++;
	}
	if(str_contains_char(str, '*') && !str_contains_char(str, '/')) // avant, pendant ou apres la boucle ??
		new = expand_wildcard(str, list_tkn);
	return (new)
}

char	*expander_handle_dquotes(char *str)
{
	char *new;
	size_t	i;

	new = NULL;
	if (!str)
		return (ERROR); //utile ?
	i = 0;
	while(str[i])
	{
		if (str[i] == '$')
			new = expand_dollar(str, i);
		i++;
	}
}



void	expander(t_lister *list_tkn)
{
	t_token	*curr;
	char	*tmp;

	curr = list_tkn->head;
	while (curr)
	{
		//expansion heredoc
		if(curr->type == WORD || curr->type == IN || curr->type == OUT || curr->type == APPEND)
		{
			tmp = expander_handle_word(curr->content, list_tkn);
			del_one_garbage(curr->content, ID);
			curr->content = tmp;
		}
		else if(curr->type == TWO_QUOTE)
		{
			curr->content = expander_handle_dquotes(curr->content);
			del_one_garbage(curr->content, ID);
			curr->content = tmp;
		}
		curr = curr->next;
	}
}

char	*strcut_gc(char const *str, size_t cut_begin, size_t cut_end, int id_gc)
{
	char	*dest;
	size_t	i;
	int		len;

	if (!str)
		return (NULL);
	len = ft_strlen(str) - cut_begin - cut_end;
	if (len <= 0)
		return (strdup_gc("", id_gc));
	dest = (char *)calloc_gc((len + 1), sizeof(char), id_gc); //utile pour le \0 automatique ?
	if (!dest)
		return (NULL);
	ft_strlcpy(dest, &str[cut_begin], len); //les char end sont bien retires sans compter le char de fin ?
	dest[len] = '\0'; //utile
	return (dest);
}

int	type_need_reducing(token_type type)
{
	if (type == IN || type == OUT || type == APPEND 
		|| type == HEREDOC || type == TWO_QUOTE || type == ONE_QUOTE)
		return (1);
	return(0);

}

void	reducer(t_token	*tkn) 
{
	char	*tmp;
	bool	flag;

	while (tkn)
	{
		flag = 0;
		//expansion heredoc
		if (type_need_reducing(tkn->type))
		{
			flag = 1;
			if(tkn->type == IN || tkn->type == OUT)
				tmp = remove_substr(tkn->content, 0, 1 + strlen_until_not(tkn->content, ft_isspace));
			else if (tkn->type == TWO_QUOTE || tkn->type == ONE_QUOTE || tkn->type == PARENTHESIS)
				tmp = strcut_gc(tkn->content, 1, 1, ID);
		}
		if (flag)
		{
			if (!tmp)
				return (ERROR);
			del_one_garbage(tkn->content, ID);
			tkn->content = tmp;
		}
		tkn = tkn->next;
	}
}

t_btree	*parser(t_lister *list_tkn)
{
	verify_syntax(list_tkn);
	reducer(list_tkn->head);
	expander(list_tkn->head);
	linker(list_tkn->head); //regroupe plusieurs nodes linkes en 1 seul, transformer les types quotes en word (puisque l' expamnsion est terminee)
	set_token_index(list_tkn->head);
	create_bin_tree(list_tkn->head);

}