#include "../../include/minishell.h"
void traverse_heredoc_node(t_mini *mini, t_btree *tree_el, t_io io_inherited)
{
    int fd_pipe[2];
    char *line;
	t_io	io_transmitted;
	
	ft_memcpy(&io_transmitted, &io_inherited, sizeof(t_io));
    if (pipe(fd_pipe) == -1)
        exit(EXIT_FAILURE);
    printf("> ");
    line = get_next_line(STDIN_FILENO); //gerer les erreurs de malloc semble impossible pour le moment avec cette fonction 
	while (line)
	{
		// Vérifie si le délimiteur a été entré
        if (ft_strcmp(line, tree_el->cmds[0]) == 0) // attention aux sauts de lignes contenus dans get next line qui peuvent faire foirer la cmp
		{
			free(line);
            break;
		}
        ft_putstr_fd(line, fd_pipe[FD_WRITE]);
        printf("> ");
    	free(line);
	    line = get_next_line(STDIN_FILENO);//gerer les erreurs de malloc semble impossible pour le moment avec cette fonction
    }
    close(fd_pipe[FD_WRITE]);
    // Mise à jour de io_inherited pour utiliser le pipe comme nouvelle entrée
    if (io_inherited.fd_in != 0)
        close(io_inherited.fd_in); // Ferme l'ancien fd_in si nécessaire
    io_transmitted.fd_in = fd_pipe[FD_READ]; // Utilise le côté lecture du pipe comme nouvelle entrée
    browse_tree(mini, tree_el->left, io_transmitted);
    close(fd_pipe[0]); // Assurez-vous de fermer le côté lecture du pipe après utilisation
}

void traverse_redir_input_node(t_mini *mini, t_btree *tree_el, t_io io_inherited)
{
	t_io	io_transmitted;
	
	// Ferme l'ancien fd_in s' il y a une redir_in a overwrite
	if (io_inherited.fd_in != 0)
        close(io_inherited.fd_in);
	ft_memcpy(&io_transmitted, &io_inherited, sizeof(t_io));
	io_transmitted.fd_in = open(tree_el->cmds[0], O_RDONLY);
	if (io_transmitted.fd_in == -1)
		exit(EXIT_FAILURE);//
	browse_tree(mini, tree_el->left, io_transmitted);
}

void traverse_redir_output_node(t_mini *mini, t_btree *tree_el, t_io io_inherited)
{
	t_io	io_transmitted;
	
	// Ferme l'ancien fd_in s' il y a une redir_in a overwrite
	if (io_inherited.fd_out != 0)
        close(io_inherited.fd_out);
	ft_memcpy(&io_transmitted, &io_inherited, sizeof(t_io));
	if(tree_el->type == OUT)
		io_transmitted.fd_out = open(tree_el->cmds[0], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
		io_transmitted.fd_out = open(tree_el->cmds[0], O_APPEND | O_CREAT, 0644);
	if (io_transmitted.fd_out == -1)
		exit(EXIT_FAILURE);//
	browse_tree(mini, tree_el->left, io_transmitted);
}

void traverse_pipe_node(t_mini *mini, t_btree *tree_el, t_io io_inherited)
{
	int	fd_pipe[2];
	t_io	io_transmitted;
	ft_memcpy(&io_transmitted, &io_inherited, sizeof(t_io));
	if(pipe(fd_pipe) == -1)
		exit(EXIT_FAILURE);//
	io_transmitted.fd_out = fd_pipe[FD_WRITE];
	browse_tree(mini, tree_el->left, io_transmitted); //cmd 1
	close(fd_pipe[FD_WRITE]);
	io_transmitted.fd_in = fd_pipe[FD_READ];
	io_transmitted.fd_out = io_inherited.fd_out;
	browse_tree(mini, tree_el->right, io_transmitted); //cmd2
	close(fd_pipe[FD_READ]);

}

void traverse_logical_op_node(t_mini *mini, t_btree *tree_el, t_io io_inherited)
{
	browse_tree(mini, tree_el->left, io_inherited);
	if ((tree_el->type == AND && g_status == 0) || (tree_el->type == OR && g_status != 0))
		browse_tree(mini, tree_el->right, io_inherited);
}

void browse_tree(t_mini *mini, t_btree *tree_el, t_io io_inherited)
{
	if(!tree_el)
		return;
	if (tree_el->type == AND || tree_el->type == OR)
		traverse_logical_op_node(mini,tree_el, io_inherited);
	else if(tree_el->type == PIPE)
		traverse_pipe_node(mini,tree_el, io_inherited);
	else if(tree_el->type == IN)
		traverse_redir_input_node(mini,tree_el, io_inherited);
	else if(tree_el->type == HEREDOC)
		traverse_heredoc_node(mini,tree_el, io_inherited);
	else if(tree_el->type == OUT || tree_el->type == APPEND)
		traverse_redir_output_node(mini,tree_el, io_inherited);
	// else if(tree_el->type == WORD)
	// 	exec_handler(mini,tree_el, io_inherited); // ajouter quand partie de geoffrey ok
}
