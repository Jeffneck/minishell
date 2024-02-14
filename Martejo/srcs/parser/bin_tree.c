#include "parser.h"
#include "../../include/minishell.h"

//token_utils.c
t_token	*find_prior_token(t_tknlist *list_tkn)
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
t_btree	*create_bin_tree(t_tknlist *list_tkn)
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