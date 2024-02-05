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

//probleme, on ne peut pas gerer id_gc (idee : creer une variable char ** qui contient str et replacement)
char	*replace_substr(char *str, char *replacement, size_t start, size_t length)
{
	char	*new;
	size_t	size_new;

	size_new = (ft_strlen(str) - length + ft_strlen(replacement)) + 1;
	new = (char *) malloc_gc(size_new, ID);
	if (new == NULL)
		return (NULL);// appliquer gestion d' erreur
	ft_memcpy(new, str, start); //ou start + 1 ou start - 1
	ft_strcat(new, replacement);
	ft_strcat(new, &str[start + length]); //valider
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

char	*expand_dollar(char *str, size_t start, size_t len) // un espace peut separer la var de la suite du txt donc utiliser replace sub
{
	char	*var;
	char	*expansion;
	char	*new;
	size_t	i;
	
	while(str[start + i] && !is_space(str[start + i]))
		i++;
	var = ft_calloc(i, sizeof(char)); //TMP
	if (!var)
		return(ERROR);//
	ft_memcpy(var, str, i); //peut etre i + 1 ?
	expansion = getenv_mini(var);
	if (expansion)
		new = replace_substr(str, expansion, start, len);
	else
		new = strdup_gc()////////////////
	free(var);
	return (new);
}

char	*expand_tilde(char *str, size_t start) //tilde n a pas besoin de start 
{
	char	*expansion;
	char	*new;
	size_t	len_sub;

	len_sub = 2;
	expansion = getenv("HOME"); //il ne faut pas liberer la memoire
	if (!expansion)
		new = replace_substr(str, "\n", start, len_sub);
	else
		new = replace_substr(str, expansion, start, len_sub);
	return (new);
}

char	*expand_lastcmd(char *str, size_t start)
{
	char	*expansion;
	char	*new;
	size_t	len_sub;

	len_sub = 2;
	expansion = getenv("HOME"); //il ne faut pas liberer la memoire
	if (!expansion)
		new = replace_substr(str, "\n", start, len_sub);
	else
		new = replace_substr(str, expansion, start, len_sub);
	return (new);
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

bool	is_word_begin(char *str, size_t pos)
{
	if(pos == 0)
		return(TRUE);
	else if (is_space(str[pos - 1])
		return(TRUE);
	return (FALSE);
}

// char	*handle_wordtype_expand(char *str)
// {
// 	//*
// 	//$ /$?
// 	//~ => est expand seulement si c' est le 1er char du word

// 	char	*new;
// 	size_t	i;
// 	size_t	j;

// 	i = 0;
// 	while (str[i])
// 	{
// 		if (ft_strcmp(&str[i], "$?") == 0)
// 			new = expand_lastcmd(str, &str[i]);
// 		else if (str[i] == '$')
// 		{
// 			j = 0;
// 			while (str[i + j] && !is_space(str[i + j]))
// 				j++;
// 			new = expand_dollar(str, i, j);
// 		}
// 		else if (is_word_begin(str, i) && str[i] == '~')
// 			new = expand_tilde(&str[i], i);
// 		else if (is_word_begin(str, i) && char_is_in_word(&str[i], '*')) //on detecte directement au debut du mot s' il y a un tilde.
// 			new = expand_wildcard(str, i);
// 		else if (is_word_begin(str, i) && char_is_in_word(&str[i], '*')) //on detecte directement au debut du mot s' il y a un tilde.
// 			new = expand_wildcard(str, i);
// 	}
// 	return (new);
// }

char	*expand_everything(char *word)
{
	char *tmp;
	char *new;
	char *words;

	new = NULL;
	if(word[0] == '~')
	{
		tmp = getenv_mini("HOME");
		new =replace_substr(word, tmp, 0, 1); //
		
	}
	while(word)
	{

	}



}

char	**expand_type_dquotes(char *str)
{
	char	*replace;
	char	*new;
	int	i;

	i = 0;
	
	while (str[i])
	{
		if (str[i] == '$')
		{
			replace = expand_envp(&str[i]);
			new = replace_var(str, &str[i], replace);
		}

	}
	
}

void	expander(t_lister *list_tkn)
{
	t_token	*curr;

	curr = list_tkn->head;
	while (curr)
	{
		//expansion heredoc
		if(curr->type == WORD || curr->type == IN || curr->type == OUT || curr->type == APPEND)
			curr->content = expand_everything(curr->content);
		if(curr->type == TWO_QUOTE)
			curr->content = expand_type_dquotes(curr->content);
		// if(curr->type == ONE_QUOTE) //inutile car il n' y a rien a expand
		// 	curr->content = expand_type_squotes(curr->content);
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

void	reducer(t_lister *list_tkn) 
{
	t_token	*curr;

	curr = list_tkn->head;
	while (curr)
	{
		//expansion heredoc
		if(curr->type == IN || curr->type == OUT)
		{
			curr->content = remove_n_prefix(curr->content, 2); //pourquoi ne pas extraire directement le nom 
			curr->content = remove_spaces(curr->content, 2); //pourquoi ne pas extraire directement le nom 
		}

		if(curr->type == TWO_QUOTE || curr->type == ONE_QUOTE || curr->type == PARENTHESIS)
			curr->content = remove_prefix(curr->content);
			curr->content = remove_suffix(curr->content);
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