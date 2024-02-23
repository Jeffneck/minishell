#include "../../include/minishell.h"

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

void display_node(t_btree *tree_el, t_io io, int depth)
{
    size_t i;

	i = 0 ;
	if (!tree_el) 
        printf("Node is NULL\n");
	printf("DEPTH: %d\n", depth);
	printf("Type: %d\n", tree_el->type);
	if (tree_el->cmds)
	{
		printf("Cmds:\n");
		while (tree_el->cmds && tree_el->cmds[i]) 
		{
			printf("%s\n", tree_el->cmds[i]);
			i++;
		}
	}
	printf("IO: [%d, %d]\n", io.fd_in, io.fd_out);
	printf("\n\n");
}