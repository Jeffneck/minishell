#include "../../include/minishell.h"
#include "stdbool.h"

//dans ce fichier il va falloir differencier les builtins des bins et savoir si nous devons fork ou pas pour les builtins

//A partir du moment ou il y a un pipe les builtins sont en fork

exec_handler(t_mini *mini, t_btree *tree_el, t_io fds)
{
	int	ret_status;
	bool	cmd_builtin;
	bool	is_pipe;

	cmd_builtin = is_builtin(tree_el->cmds[0]);
	is_pipe = cmd_is_inside_pipe(mini->b_tree, tree_el->branch);

	if (is_pipe == true && cmd_builtin == true)
	{
		//fork handler puis builtin handler
	}
	else if (is_pipe == false && cmd_builtin== false)
	{
		ret_status = builtin_handler(mini, tree_el, fds); // A faire
	}
	else
		//fork handler puis exec_bin
}