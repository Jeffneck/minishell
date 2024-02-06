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


t_btree	*create_bin_tree(t_lister *list_tkn)
{
	t_token	*target;
	t_btree	*root;

	while(list_tkn->head)
	{
		target = find_prior_token(list_tkn);
		if (!root)
		 	root = place(root, btree_new(target), target->index);
		else if (target->type == PARENTHESE)
		{
			// verify_syntax_inside_parenthesis
			place(root, parser(lexer(target->content)), target->index);
		}
		else
			place(root, btree_new(target), target->index);
	}
	return root;
}

t_btree	*place(t_btree *root, t_btree *toplace, int index)
{
	if (root == NULL)
		return (toplace);
	// if (root->type == PARENTHESE)//
	// 	root->left = toplace;//
	if (index < root->branch)
		root->left = place(root->left, toplace);
	else if (index > root->branch)
		root->right = place(root->right, toplace);
	return (root);
}

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


char	*replace_endstr(char *str, char *replacement, size_t end_pos)
{
	//utiliser pour $var
	char	*new;
	size_t	len_str;
	size_t	size_new;

	if (!str || !replacement)
		return (NULL);
	len_str = ft_strlen(str);
	size_new = (len_str - (len_str - end_pos) + ft_strlen(replacement) + 1) * sizeof(char);
	new = (char *) malloc_gc(size_new, ID);
	if (!new)
		return (NULL);// appliquer gestion d' erreur
	ft_memcpy(new, str, end_pos); //ou end_pos + 1 ou end_pos - 1
	ft_strcat(new, replacement);
	return (new);
}

//avance tant que la fonction ne detecte pas un char cible
size_t	ft_strlen_until(const char *str, int(*f)(char))
{
	size_t	len;

	len = 0;
	while (str[len] && !f(str[len]))
		len++;
	return (len);
}

//avance tant que la fonction detecte un char cible
size_t	ft_strlen_until_not(const char *str, int(*f)(char))
{
	size_t	len;

	len = 0;
	while (str[len] && f(str[len]))
		len++;
	return (len);
}


char	*expand_dollar(char *str, size_t start) // un espace peut separer la var de la suite du txt donc utiliser replace sub
{
	char	*var;
	char	*expansion;
	char	*new;
	size_t	len_var;
	
	if (str[start + 1] == '?')
	{
		len_var = 2;

	}
	else
	{
		len_var = ft_strlen_until(str[start], ft_isspace);
		var = ft_calloc(len_var, sizeof(char)); //TMP
		if (!var)
			return(ERROR);//
		ft_memcpy(var, str, len_var); //peut etre i + 1 ?
		expansion = getenv_mini(var);
	}
	//add in new funct-------
	if (expansion)
		new = replace_substr(str, expansion, start, len_var);
	else
		new = remove_substr(str, start, len_var);
	//add in new funct---------
	del_one_garbage(expansion, ID); // on peut ne pas utiliser le gc dans getenv_mini
	
	del_one_garbage(str, ID);
	free(var);
	return (new);
}

char	*expand_tilde(char *str) //tilde n a pas besoin de start 
{
	char	*expansion;
	char	*new;
	size_t	len_sub;

	len_sub = 1;
	expansion = getenv_mini("HOME");
	//add in new funct-------
	if (expansion)
		new = replace_substr(str, expansion, 0, len_sub);
	else
		new = remove_substr(str, 0, len_sub);
	//add in new funct-------
	del_one_garbage(expansion, ID); // on peut ne pas utiliser le gc dans getenv_mini
	
	del_one_garbage(str, ID);
	return (new);
}

char	*expand_in_word(char *str)
{
	char *new;
	size_t	i;

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
		if (str[i] == '*')
			new = expand_wildcard(str, i);
		i++;
	}
	return (new)

}

char	*expand_in_dquotes(char *str)
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

int	char_is_in_word(char *begin_word, char c)
{
	size_t	i;

	i = 0;
	while(begin_word[i] && !is_space(begin_word[i]))
	{
		if (begin_word[i] == c)
			return (1);
		i++;
	}
	return (0);
}

int	ft_is_word_begin(char *str, size_t pos)
{
	if(pos == 0)
		return(1);
	else if (is_space(str[pos - 1])
		return(1);
	return (0);
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
			tmp = expand_in_word(curr->content);
			del_one_garbage(curr->content, ID);
			curr->content = tmp;
		}
		else if(curr->type == TWO_QUOTE)
		{
			curr->content = expand_in_dquotes(curr->content);
			del_one_garbage(curr->content, ID);
			curr->content = tmp;
		}
		curr = curr->next;
	}
}

char	*remove_n_prefix(char *str, size_t nb_toremove)
{
	char	*new;

	new = strcut_gc(str,  ID); ///////////////////////
	if (!new)
		return (NULL);
	del_one_garbage(str, ID2);
	return (new);
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

void	reducer(t_lister *list_tkn) 
{
	t_token	*curr;
	char	*str;
	bool	flag;


	curr = list_tkn->head;
	while (curr)
	{
		
		str = curr->content;
		//expansion heredoc
		if(curr->type == IN || curr->type == OUT)
		{
			tmp = remove_substr(str, 0, 1 + strlen_until_not(str, ft_isspace));
			if (!tmp)
				return (ERROR);
			del_one_garbage(curr->content, ID);
		}

		if(curr->type == TWO_QUOTE || curr->type == ONE_QUOTE || curr->type == PARENTHESIS)
		{
			tmp = strcut_gc(tmp, 1, 1, ID);

			
		}
		if (flag)
		{
			if (!tmp)
				return (ERROR);
			del_one_garbage(curr->content, ID);
			flag = 0;
		}
		curr->content = tmp;
		curr = curr->next;
	}

extract(, debut, fin)   //strndup_gc 
}

void	verify_syntax(t_lister *list_tkn)
{
	//un token parenthese ne peut qu etre au debut ou a la fin de la ligne de cmd
	//et etre precede ou succede par un operateur | || &&
	t_token *curr;

	curr = list_tkn->head;

}

t_btree	*parser(t_lister *list_tkn)
{
	verify_syntax(list_tkn);
	reducer(list_tkn);
	expander(list_tkn);
	linker(list_tkn); //regroupe plusieurs nodes linkes en 1 seul, transformer les types quotes en word (puisque l' expamnsion est terminee)
	set_token_index(list_tkn);
	create_bin_tree(list_tkn);

}