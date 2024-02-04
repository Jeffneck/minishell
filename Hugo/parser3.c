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

char	*apply_expand_dollar(char *str, char *p_dollar, size_t pos_dollar) // un espace peut separer la var de la suite du txt donc utiliser replace sub
{
	char	*expansion;
	char	*new;

	expansion = getenv(&str[1]); //il ne faut pas liberer la memoire
	if (!expansion)
		new = replace_endstr(str, &str[0], "\n");
	else
		new = replace_endstr(str, &str[0], expansion);
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

char	*expand_lastcmd_var(char *str, size_t start)
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


char	*handle_wordtype_expand(char *str)
{
	//*
	//$ /$?
	//~ => est expand seulement si c' est le 1er char du word

	char	*new;
	size_t	i;

	i = 0;
	if (str[i] == '~')
		new = expand_tilde(&str[i]);
	while (str[i])
	{
		if (ft_strcmp(&str[i], "$?") == 0)
			new = expand_lastcmd_var(str, &str[i]);
		else if (str[i] == '$')
			new = expand_dollar(str, &str[i], i);
		else if (str[i] == '*')
			new = expand_wildcard(&str[i]);
		else if (str[i] == '~')
			new = expand_tilde(&str[i]);
	}
	return (new);
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
		if(curr->type == WORD)
			curr->content = expand_type_word(curr->content);
		if(curr->type == TWO_QUOTE)
			curr->content = expand_type_dquotes(curr->content);
		if(curr->type == ONE_QUOTE)
			curr->content = expand_type_squotes(curr->content);
		curr = curr->next;
	}

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
	expander(list_tkn);
	linker(list_tkn); //regroupe plusieurs nodes linkes en 1 seul
	set_token_index(list_tkn);
	create_bin_tree(list_tkn);

}