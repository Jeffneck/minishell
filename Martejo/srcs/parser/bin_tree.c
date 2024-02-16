#include "../../include/minishell.h"

t_token	*find_prior_token(t_token *curr)
{
	t_token	*target;

	target = curr;
	//mecanisme de stop quand on rencontre un node qui a ete pop ? 
	//utiliser priority comme ci dessous : (sinon pour plus de clarte on peut ajouter un champ flag `a notre structure tkn)
	while (curr && curr->used_flag != 1)
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

// cree un nouveau node btree et y ajoute la commande et les arguments
t_btree	*btree_new(t_token	*tkn_toconvert)
{
	t_btree		*tree_el;
	t_token	*tkn_toargs;
	t_list	*new_arg; 

	ft_bzero(tree_el, sizeof(t_btree));
	tree_el->content = strdup_gc(tkn_toconvert->content, B_TREE);
	if (!(tree_el->content))
		exit (EXIT_FAILURE);
	tkn_toconvert->used_flag = 1;
	//si le tkn ne peut pas avoir d' arguments on retourne direct le node
	if (tkn_toconvert->type != WORD)
		return (tree_el);
	//sinon on ajoute les arguments dans une liste
	tkn_toargs = tkn_toconvert->next;
	while(tkn_toargs && tkn_toargs->used_flag == 0 && tkn_toargs->type == WORD)
	{
		new_arg = ft_lstnew_gc(tkn_toargs->content, B_TREE);
		if (!new_arg)
			exit (EXIT_FAILURE);
		ft_lstadd_back(&(tree_el->args), new_arg);
		tkn_toargs->used_flag = 1; //semble inutile avec la logique des branches
		tkn_toargs = tkn_toargs->next;
	}
	return (tree_el);
}

void depth_first_search(t_btree *node, void (*visit)(t_btree *))
{
    if (node) {
        depth_first_search(node->left, visit);
        visit(node);
        depth_first_search(node->right, visit);
    }
}

void display_node(t_btree *node) 
{
    if (node) {
        printf("Content: %s\n", node->content);
        printf("Arguments:\n");
        t_list *arg = node->args;
        while (arg) {
            printf("%s\n", (char *)arg->content);
            arg = arg->next;
        }
        printf("IO: [%d, %d]\n", node->io[0], node->io[1]);
        printf("Type: %d\n", node->type);
    } else {
        printf("Node is NULL\n");
    }
}

//syntaxic_tree.c
t_btree	*create_bin_tree(t_tknlist *tknlst, t_env *env)
{
	t_token	*prio_tkn;
	t_btree	*tree_root;

	while(tknlst->head)
	{
		if (tknlst->head->used_flag == 1)
		{
			pop_token_in_place(tknlst, tknlst->head);
			continue;
		}
		prio_tkn = find_prior_token(tknlst->head);
		if (!tree_root)
		 	tree_root = place_in_tree(tree_root, btree_new(prio_tkn), prio_tkn->index);
		else if (prio_tkn->type == PARENTHESIS)
			place_in_tree(tree_root, parser(lexer(prio_tkn->content), env), prio_tkn->index);
		else
			place_in_tree(tree_root, btree_new(prio_tkn), prio_tkn->index);
		
		//une fois le token insere dans l' arbre binaire, on le marque comme un flag.
		prio_tkn->used_flag = 1;
	}
	return tree_root;
}
//attention cela sera a la partie exec de setup les in out append heredoc ainsi que leurs consequences...

//j'avais oublie la logique d' utilisation des branches, du coup used flag semble inutile maintenant