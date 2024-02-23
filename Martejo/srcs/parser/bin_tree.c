#include "../../include/minishell.h"

t_token	*find_prior_token(t_token *curr)
{
	t_token	*target;

	target = curr;
	while (curr && curr->used_flag != 1)
	{
		if (curr->priority >= target->priority)
			target = curr;
		curr = curr->next;
	}
	ft_printf("find_prior_token, target = %s\n", target->content);
	return (target);
}

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


// cree un nouveau node btree et y ajoute la commande et les arguments
// t_btree	*btree_new(t_token	*tkn_toconvert)
// {
// 	t_btree		*tree_el;
// 	t_token	*tkn_toargs;
// 	t_list	*new_arg; 

// 	ft_bzero(tree_el, sizeof(t_btree));
// 	tree_el->content = strdup_gc(tkn_toconvert->content, B_TREE);
// 	if (!(tree_el->content))
// 		exit (EXIT_FAILURE);//
// 	tkn_toconvert->used_flag = 1;
// 	//si le tkn ne peut pas avoir d' arguments on retourne direct le node
// 	if (tkn_toconvert->type != WORD) //le type word est il le seul a ce moment capable d' avoir des arguments ?
// 		return (tree_el);
// 	//sinon on ajoute les arguments dans une liste
// 	tkn_toargs = tkn_toconvert->next;
// 	while(tkn_toargs && tkn_toargs->used_flag == 0 && tkn_toargs->type == WORD) //used flag semble inutile avec la logique des branches
// 	{
// 		new_arg = ft_lstnew_gc(tkn_toargs->content, B_TREE);
// 		if (!new_arg)
// 			exit (EXIT_FAILURE);
// 		ft_lstadd_back(&(tree_el->args), new_arg);
// 		tkn_toargs->used_flag = 1; //used flag semble inutile avec la logique des branches
// 		tkn_toargs = tkn_toargs->next;
// 	}
// 	return (tree_el);
// }

// char	**pick_args(t_token *tkn_toargs)
// {
// 	ft_printf("pick_args\n");
// 	t_token *curr;
// 	size_t	size_char2;
// 	char	**args;

// 	curr = tkn_toargs;
// 	size_char2 = 0;
// 	if(!tkn_toargs)
// 		return (NULL);
// 	while (curr && (curr->type == WORD || curr->type == ONE_QUOTE || curr->type == TWO_QUOTE))
// 	{
// 		size_char2++;
// 		curr = curr->next;
// 	}
// 	// ft_printf("size char = %d\n", size_char2);
// 	args = (char **) calloc_gc(size_char2 + 1, sizeof(char *), B_TREE);
// 	if (!args)
// 		exit (EXIT_FAILURE);//
// 	size_char2 = 0;
// 	while (tkn_toargs && (tkn_toargs->type == WORD || tkn_toargs->type == ONE_QUOTE || tkn_toargs->type == TWO_QUOTE))
// 	{
// 		args[size_char2] = strdup_gc(tkn_toargs->content, B_TREE);
// 		if (!args[size_char2])
// 			exit (EXIT_FAILURE);//
// 		tkn_toargs->used_flag = 1;
// 		tkn_toargs = tkn_toargs->next;
// 		size_char2++;
// 	}
// 	return (args);
// }

int		is_cmd_related_tkn(t_tkntype tkntype)
{
	if (tkntype == WORD || tkntype == ONE_QUOTE || tkntype == TWO_QUOTE)
		return(1);
	return (0);
}

size_t	count_argc_cmd(t_token *curr)
{
	size_t	argc;
	
	argc = 0;
	while (curr && is_cmd_related_tkn(curr->type))
	{
		argc++;
		curr = curr->next;
	}
	return(argc);
}

char	**get_argv_cmd(t_token *curr, size_t argc)
{
	char	**argv;
	size_t	i;

	argv = (char **) calloc_gc(argc + 1, sizeof(char *), B_TREE);
	if (!argv)
		exit (EXIT_FAILURE);//
	i = 0;
	while (i < argc)
	{
		argv[i] = strdup_gc(curr->content, B_TREE);
		if (!argv[i])
			exit (EXIT_FAILURE);//
		curr->used_flag = 1;
		curr = curr->next;
		i++;
	}
	return (argv);
}
char	**extract_cmd_argv(t_token *curr)
{
	ft_printf("pick_args\n");
	size_t	argc;
	char	**argv;

	argc = count_argc_cmd(curr);
	argv = get_argv_cmd(curr, argc);
	if (!argv)
		exit (EXIT_FAILURE);//
	return (argv);
}


t_btree	*btree_new(t_token	*tkn_toconvert)
{
	ft_printf("btree_new\n");
	t_btree		*tree_el;

	tree_el = calloc_gc(1, sizeof(t_btree), B_TREE);
	tree_el->branch = tkn_toconvert->index;
	if(is_cmd_related_tkn(tkn_toconvert->type))
	{
		tree_el->type == WORD;
		tree_el->content = extract_cmd_argv(tkn_toconvert);
		if (!(tree_el->content))
			exit (EXIT_FAILURE);//
	}
	else
	{
		tree_el->type = tkn_toconvert->index;
		tkn_toconvert->used_flag = 1;
	}
	return (tree_el);
}

void depth_first_search(t_btree *tree_el, void (*visit)(t_btree *, int))
{
	// ft_printf("depth_first_search\n");
	static int depth = -1;
	
	depth++;
	if (tree_el) 
	{
        depth_first_search(tree_el->left, visit);
        visit(tree_el, depth);
        depth_first_search(tree_el->right, visit);
    }
	depth--;
}

void display_node(t_btree *tree_el, int depth)
{
    size_t i;

	i = 0 ;
	if (tree_el) {
        printf("DEPTH: %d\n", depth);
        printf("cmds: %s\n", tree_el->cmds);
        printf("Arguments:\n");
        while (tree_el->argv && tree_el->argv[i]) 
		{
            printf("%s\n", tree_el->argv[i]);
            i++;
        }
        printf("IO: [%d, %d]\n", tree_el->io[0], tree_el->io[1]);
        printf("Type: %d\n", tree_el->type);
    } else {
        printf("Node is NULL\n");
    }
	printf("\n");
}

//syntaxic_tree.c
t_btree	*create_bin_tree(t_tknlist *tknlst, t_env *env)
{
	ft_printf("create_bin_tree\n");
	t_token	*prio_tkn;
	t_btree	*btree_root;

	btree_root = NULL;
	while(tknlst->head)
	{
		if ((tknlst->head)->used_flag == 1)//used flag + tete de liste = el inutile
		{
			pop_token_in_place(tknlst, tknlst->head);
			continue;
		}
		prio_tkn = find_prior_token(tknlst->head);
		if (!btree_root)
		 	btree_root = btree_new(prio_tkn);
		else if (prio_tkn->type == PARENTHESIS)
			place_in_tree(btree_root, parser(lexer(prio_tkn->content), env), prio_tkn->index);
		else
			place_in_tree(btree_root, btree_new(prio_tkn), prio_tkn->index);
		//une fois le token insere dans l' arbre binaire, on le marque comme un flag.
		prio_tkn->used_flag = 1;
	}
	return btree_root;
}
//attention cela sera a la partie exec de setup les in out append heredoc ainsi que leurs consequences...
