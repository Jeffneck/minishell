#include "../../include/minishell.h"
#include "stdbool.h"

//dans ce fichier il va falloir differencier les builtins des bins et savoir si nous devons fork ou pas pour les builtins

//A partir du moment ou il y a un pipe les builtins sont en fork

int exec_handler(t_mini *mini, t_btree *tree_el, t_io fds)
{
	int		ret_status;
	bool	cmd_builtin;
	bool	is_pipe;
	ft_printf("exec_handler\n cmd = %s\n", tree_el->cmds[0]);

	cmd_builtin = is_builtin(tree_el->cmds[0]);
	is_pipe = cmd_is_inside_pipe(mini->b_tree, tree_el->branch);

	if (is_pipe == true && cmd_builtin == true)
	{
		ft_printf("1\n");
		ret_status = fork_builtin(&(mini->env), tree_el, fds);
	}
	else if (is_pipe == false && cmd_builtin == true)
	{
		ft_printf("2\n");
		ret_status = exec_builtin(&(mini->env), tree_el, fds);

	}
	else
		ret_status = exec_bin(mini->env, tree_el, fds);
	return (ret_status);
}