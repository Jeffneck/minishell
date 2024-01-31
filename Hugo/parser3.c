# include "lexer.h"

typedef	struct s_args
{
	char **content;
	t_args *
	int	io[2]; //in out (peut etre compose de pipe)
	int	branch;
	token_type	type;
	struct s_btree	*left;
	struct s_btree	*right;
}	t_btree;

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
		target = expand()
		if (root == NULL)
		 	root = place(root, btree_new(target), target->index);
		else if (target->type == PARENTHESE)
			place(root, parser(lexer(target->content)), target->index);
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



char	*apply_expand_dollar(char *str, char *p_dollar)
{
	size_t	i;
	char	*expansion;

	i = 1;
	while(p_dollar[i] && !is_separator(p_dollar[i]))
		/////////////////
		expansion = //
		replace_var(str, &str[i], expansion);
}


char	*expand_type_word(char *str)
{
	//*
	//$ /$?
	//~ => est expand seulement si c' est le 1er char du word

	char	*new;
	int	i;

	i = 0;
	if (str[i] == '~')
		new = expand_tilde(&str[i]);
	while (str[i])
	{
		if (str[i] == '$')
			new = expand_dollar(&str[i]);
		if (str[i] == '*')
			new = expand_wildcard(&str[i]);
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


void expander(t_lister *list_tkn)
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

t_btree	*parser(t_lister *list_tkn)
{
	verify_syntax()
	expander(list_tkn);
	set_token_index(list_tkn);
	create_bin_tree(list_tkn);

}