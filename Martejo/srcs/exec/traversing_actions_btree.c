#include "../../include/minishell.h"

// void	close_used_fd(t_tkntype type, int fds_pipe[2], int fd_file, int mode)
// {
// 	if(mode = 0)
// 	{
// 		if(type == PIPE)
// 			close(fds_pipe[FD_WRITE]);
// 		if(type == IN || type == OUT || type == APPEND)
// 			close(fd_file);
// 	}
// 	else if(mode = 1 && type == PIPE)
// 		close(fds_pipe[FD_READ]);
// }




// t_io	io_redir_leftchild(t_tkntype *type, t_io io_inherited, int *fds_pipe, int fd_file) //donner io inherited 
// {
// 	t_io	io_redir;

// 	ft_memcpy(&io_redir, &io_inherited, sizeof(t_io));
// 	if (type == PIPE)
// 		io_redir.out = fds_pipe[FD_WRITE];
// 	else if (type == OUT || type == APPEND)
// 		io_redir.out = fd_file;
// 	else if (type == IN)
// 		io_redir.in = fd_file;
// }

// t_io	io_redir_rightchild(t_btree *tree_el, t_io io_inherited, int *fds_pipe)
// {
// 	t_io	io_redir;

// 	ft_memcpy(&io_redir, &io_inherited, sizeof(t_io));
// 	if (tree_el->type == PIPE)
// 		io_redir.in = fds_pipe[FD_READ];
// 	return (io_redir);
// }


// int	*fill_fds_if_pipe(t_tkntype type)
// {
// 	int	*fds_pipe;

// 	if(type != PIPE)
// 		return;
	
// 	return (fds_pipe);
// }

// int	fill_fd_if_redir(char *filename, t_tkntype type)
// {
// 	int	fd_file;

// 	fd_file = 0; //code special reconaissable, inutile car le type nous aidera a savoir que l' on est pas dans un node redir plus tard
// 	if (type == IN)
// 		fd_file = open(filename, O_RDONLY); //verifier que le file est accessible en lecture seule
// 	if (type == OUT)
// 		fd_file = open(filename, OWRONLY | O_CREATE | O_TRUNCATE); //add les permissions
// 	if (type == APPEND)
// 		fd_file = open(filename, O_APPEND); //creer ou utiliser file existant
// 	if (fd_file == -1)//valider les codes de retour erreur
// 		exit(EXIT_ERROR);//
// 	return (fd_file);
// }

t_redir_fd search_redir_fd(t_tkntype el_type, char *el_content)
{
	t_redir_fd redir;

	ft_bzero(&redir, sizeof(redir));
	if (el_type == PIPE)
	{
		pipe(redir.fds_pipe);
		if (redir.fds_pipe == -1)
			exit(EXIT_ERROR);////utiliser fstat pour la gestion d' erreurs ?? GEOF
	}
	if (el_type == IN || el_type == OUT || el_type == APPEND)
	{
		//valider les flags d'open
		if (el_type == IN)
			redir.fd_file = open(el_content, O_RDONLY); 
		else if (el_type == OUT)
			redir.fd_file = open(el_content, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else if (el_type == APPEND)
			redir.fd_file = open(el_content, O_APPEND | O_CREAT, 0644);
		if (redir.fd_file == -1)
			exit(EXIT_ERROR);//utiliser fstat pour la gestion d' erreurs ?? GEOF
	}
	return (redir);
}

t_io	io_redir_leftchild(t_tkntype *el_type, t_io io_inherited, t_redir_fd redir)
{
	t_io	io_totransmit;

	ft_memcpy(&io_totransmit, &io_inherited, sizeof(t_io));
	if (el_type == PIPE)
		io_totransmit.fd_out = redir.fds_pipe[FD_WRITE];
	else if (el_type == OUT || el_type == APPEND)
		io_totransmit.fd_out = redir.fd_file;
	else if (el_type == IN)
		io_totransmit.fd_in = redir.fd_file;
	return(io_totransmit);
}

t_io	io_redir_rightchild(t_tkntype *el_type, t_io io_inherited, t_redir_fd redir)
{
	t_io	io_totransmit;

	ft_memcpy(&io_totransmit, &io_inherited, sizeof(t_io));
	if (el_type == PIPE)
		io_totransmit.fd_in = redir.fds_pipe[FD_READ];
	return (io_totransmit);
}

int	exec_rightchild_ok(t_tkntype *el_type) //bool
{
	if(el_type == IN || el_type == OUT || el_type == APPEND || el_type == HEREDOC)//valider heredoc
		return (0);
	else if (el_type == AND && g_status != 0)
		return (0);
	else if (el_type == OR && g_status == 0)
		return (0);
	return (1);
}

// void traverse_and_execute_actions(t_mini *mini, t_btree *tree_el, t_io io_inherited)
// {
// 	t_redir_fd redir;

// 	if(!tree_el)
// 		return;	
// 	redir = search_redir_fd(tree_el->type, tree_el->content);
// 	traverse_and_execute_actions(mini, tree_el->left, io_redir_leftchild(tree_el->type, io_inherited, redir));
// 	if(tree_el->type == WORD || tree_el->type == ONE_QUOTE || tree_el->type == TWO_QUOTE) //transformer toutes les commandes en type word serait plus simple, ou fonction tree_el_is_cmd()
// 		fork_exec(mini->env, tree_el, io_inherited);
// 	close_used_redir_fd(tree_el, redir, 0); //valider le process de fermeture ici (semble utile car dans le cas de plusieurs redir successives, la commande finale n' aura pas acces aux fd qui ont ete open mais inutilises)
// 	if (exec_rightchild_ok(tree_el->type))// mal fait, attention aux cas ou il n' y a ni and ni or ...
// 		traverse_and_execute_actions(mini, tree_el->right, io_redir_rightchild(tree_el->type, io_inherited, redir));
// 	close_used_redir_fd(tree_el->type, redir, 1);
// }

void redir_visiter(t_btree	*tree_el, int phase);
{
	if(!(tree_el->type == IN || tree_el->type == OUT || tree_el->type == APPEND || tree_el->type == HEREDOC))
		return (0);
	
}
// void cmd_visiter()
// {
// 	fork_exec(mini->env, tree_el, io_inherited)
// }

// void logical_operators_visiter();

// void traverse_and_execute_actions(t_mini *mini, t_btree *tree_el, t_io io_inherited)
// {
// 	t_redir_fd redir;

// 	if(!tree_el)
// 		return;	
// 	redir = search_redir_fd(tree_el->type, tree_el->content);
// 	traverse_and_execute_actions(mini, tree_el->left, io_redir_leftchild(tree_el->type, io_inherited, redir));
// 	if(tree_el->type == WORD || tree_el->type == ONE_QUOTE || tree_el->type == TWO_QUOTE) //transformer toutes les commandes en type word serait plus simple, ou fonction tree_el_is_cmd()
// 		fork_exec(mini->env, tree_el, io_inherited);
// 	close_used_redir_fd(tree_el, redir, 0); //valider le process de fermeture ici (semble utile car dans le cas de plusieurs redir successives, la commande finale n' aura pas acces aux fd qui ont ete open mais inutilises)
// 	if (exec_rightchild_ok(tree_el->type))// mal fait, attention aux cas ou il n' y a ni and ni or ...
// 		traverse_and_execute_actions(mini, tree_el->right, io_redir_rightchild(tree_el->type, io_inherited, redir));
// 	close_used_redir_fd(tree_el->type, redir, 1);
// }

void actions_redir_node(t_mini *mini, t_btree *tree_el, t_io io_inherited)
{
	t_redir_fd redir;

	if(!tree_el)
		return;	
	redir = search_redir_fd(tree_el->type, tree_el->content);
	traverse_and_execute_actions(mini, tree_el->left, io_redir_leftchild(tree_el->type, io_inherited, redir));
	if(tree_el->type == WORD || tree_el->type == ONE_QUOTE || tree_el->type == TWO_QUOTE) //transformer toutes les commandes en type word serait plus simple, ou fonction tree_el_is_cmd()
		fork_exec(mini->env, tree_el, io_inherited);
	close_used_redir_fd(tree_el, redir, 0); //valider le process de fermeture ici (semble utile car dans le cas de plusieurs redir successives, la commande finale n' aura pas acces aux fd qui ont ete open mais inutilises)
	if (exec_rightchild_ok(tree_el->type))// mal fait, attention aux cas ou il n' y a ni and ni or ...
		traverse_and_execute_actions(mini, tree_el->right, io_redir_rightchild(tree_el->type, io_inherited, redir));
	close_used_redir_fd(tree_el->type, redir, 1);
}

void traverse_and_execute_actions(t_mini *mini, t_btree *tree_el, t_io io_inherited)
{
	t_redir_fd redir;

	if(!tree_el)
		return;
	if(tree_el->type == PIPE)
		traverse_pipe_node(mini,tree_el, io_inherited);
	if(tree_el->type == IN || tree_el->type == OUT || tree_el->type == APPEND || tree_el->type == HEREDOC)
		traverse_redir_node(mini,tree_el, io_inherited);
	if(tree_el->type == WORD || tree_el->type == ONE_QUOTE || tree_el->type == TWO_QUOTE) //transformer toutes les commandes en type word serait plus simple, ou fonction tree_el_is_cmd()
		fork_exec(mini->env, tree_el, io_inherited);
	if (exec_rightchild_ok(tree_el->type))// mal fait, attention aux cas ou il n' y a ni and ni or ...
		traverse_and_execute_actions(mini, tree_el->right, io_redir_rightchild(tree_el->type, io_inherited, redir));
	close_used_redir_fd(tree_el->type, redir, 1);
}


int	main()
{

	traverse_and_execute_actions(mini, mini->btree_root, mini->io);
}