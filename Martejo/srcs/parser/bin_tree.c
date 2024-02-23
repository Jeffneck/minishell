#include "../../include/minishell.h"

//syntaxic_tree.c
t_btree	*place_in_tree(t_btree *tree_el, t_btree *toplace, int index)
{
	ft_printf("place_in_tree\n");
	if (tree_el == NULL)
		return (toplace);
	// if (tree_el->type == PARENTHESE)//
	// 	tree_el->left = toplace;//
	if (index < tree_el->branch)
		tree_el->left = place_in_tree(tree_el->left, toplace, index);
	else if (index > tree_el->branch)
		tree_el->right = place_in_tree(tree_el->right, toplace, index);
	return (tree_el);
}

t_btree	*btree_new(t_token	*tkn_toconvert)
{
	ft_printf("btree_new\n");
	t_btree		*tree_el;

	tkn_toconvert->used_flag = 1;
	tree_el = calloc_gc(1, sizeof(t_btree), B_TREE);
	if (!tree_el)
		exit (EXIT_FAILURE);//
	tree_el->branch = tkn_toconvert->index;
	tree_el->type = tkn_toconvert->index;
	if(is_cmd_related_tkn(tkn_toconvert->type))
	{
		tree_el->type == WORD;
		tree_el->cmds = extract_cmd_argv(tkn_toconvert);//exit integre si malloc err
	}
	if (tkn_toconvert->type == HEREDOC)
		tree_el->cmds = cpy_heredoc_delim_char2(tkn_toconvert->content);
	return (tree_el);
}

t_btree	*create_bin_tree(t_tknlist *tknlst, t_env *env)
{
	ft_printf("create_bin_tree\n");
	t_token	*prio_tkn;
	t_btree	*btree_root;

	btree_root = NULL;
	while(tknlst->head)
	{
		while(tknlst->head->used_flag == 1)
			pop_token_in_place(tknlst, tknlst->head);
		prio_tkn = find_prior_token(tknlst->head);
		if (!btree_root)
		 	btree_root = btree_new(prio_tkn);
		else if (prio_tkn->type == PARENTHESIS)
			place_in_tree(btree_root, parser(lexer(prio_tkn->content), env), prio_tkn->index);
		else
			place_in_tree(btree_root, btree_new(prio_tkn), prio_tkn->index);
		prio_tkn->used_flag = 1;
	}
	clear_garbage(TKN_LIST, free);//plus besoin de ca
	return btree_root;
}