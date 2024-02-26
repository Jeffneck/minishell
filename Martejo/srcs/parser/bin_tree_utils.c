#include "../../include/minishell.h"

size_t	count_argc_cmd(t_token *curr)
{
	size_t	argc;
	
	argc = 0;
	while (curr && is_cmd_tkn(curr->type))
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
		// if (i > 0) //condition retirable mais plus clair ainsi ?
		curr->used_flag = 1;
		argv[i] = strdup_gc(curr->content, B_TREE);
		if (!argv[i])
			exit (EXIT_FAILURE);//
		curr = curr->next;
		i++;
	}
	return (argv);
}

char	**strdup_in_newchar2(char *delimiter)
{
	char	**cpy;
	cpy = (char **) calloc_gc(2, sizeof(char *), B_TREE);
	if (!cpy)
		exit (EXIT_FAILURE);//
	cpy[0] = strdup_gc(delimiter, B_TREE);
	if (!cpy[0])
		exit (EXIT_FAILURE);//
	return (cpy);
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

t_token	*find_prior_token(t_token *curr)
{
	t_token	*target;

	target = curr;
	while (curr && curr->used_flag != 1)
	{
		// if (curr->priority == 1 && curr->priority == target->priority && curr->index < target->index) /////////////////
		// 	target = curr;
		if (curr->priority > 0 && curr->priority >= target->priority)
			target = curr;
		curr = curr->next;
	}
	ft_printf("find_prior_token, target = %s\n", target->content);
	return (target);
}

int	cmd_is_inside_pipe(t_btree *root, int branch_id)
{
	t_btree	*curr;

	curr = root;
	while (curr && curr->branch != branch_id)
	{
		if (curr->type == PIPE)
			return (1);
		if (branch_id < curr->branch)
			curr = curr->left;
		else
			curr = curr->right;		
	}
	return (0);
}

