# include "lexer.h"

typedef	struct s_btree
{
	char **content;
	int	io[2]; //in out (peut etre compose de pipe)
	t_btree	*next
}	t_btree;


t_btree	*create_bin_tree(t_token *tokens)
{
	static t_btree	*curr;
	t_btree	new;
	curr = btree_new();
	while(tokens)
}