#include "../../include/minishell.h"
void traverse_heredoc_node(t_mini *mini, t_btree *tree_el, t_io io_inherited)
{
	//ft_printf("traverse_heredoc_node\n");
    
	int fd_pipe[2];
    char *line;
	t_io	io_transmitted;
	
	ft_memcpy(&io_transmitted, &io_inherited, sizeof(t_io));
    if (pipe(fd_pipe) == -1)
		print_and_exit("Minishell: pipe error\n", RED, 1);
    ft_printf_fd(2, "> ");
    line = get_next_line(STDIN_FILENO); //gerer les erreurs de malloc semble impossible pour le moment avec cette fonction 
	while (line)
	{
		//ft_printf("line = %s\n", line);
		if (g_status == 130)
			break ;
		// Vérifie si le délimiteur a été entré
        if (ft_strlen(line) > 0 && ft_strncmp(line, tree_el->cmds[0], ft_strlen(line) - 1) == 0) // attention aux sauts de lignes contenus dans get next line qui peuvent faire foirer la cmp
		{
			//ft_printf("line break\n");
			free(line);
            break;
		}
        ft_putstr_fd(line, fd_pipe[FD_WRITE]);
        ft_printf_fd(2, "> ");
    	free(line);
	    line = get_next_line(STDIN_FILENO);//gerer les erreurs de malloc semble impossible pour le moment avec cette fonction
    }
	//ft_printf("fin heredoc\n");
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
//	ft_printf("traverse_redir_input_node\n");
	t_io	io_transmitted;
	
	// Ferme l'ancien fd_in s' il y a une redir_in a overwrite
	if (io_inherited.fd_in != 0)
        close(io_inherited.fd_in);
	ft_memcpy(&io_transmitted, &io_inherited, sizeof(t_io));
	io_transmitted.fd_in = open(tree_el->cmds[0], O_RDONLY);
	if (io_transmitted.fd_in == -1)
	{
		print_strerror(tree_el->cmds[0], RED, 1);
		return ;
	}
	browse_tree(mini, tree_el->left, io_transmitted);
	if (io_transmitted.fd_in != 0)
		close(io_transmitted.fd_in);
}

void traverse_redir_output_node(t_mini *mini, t_btree *tree_el, t_io io_inherited)
{
	//ft_printf("traverse_redir_output_node\n");
	t_io	io_transmitted;
	
	
	// Ferme l'ancien fd_in s' il y a une redir_in a overwrite
	if (io_inherited.fd_out != 0 && io_inherited.fd_out != 1)
        close(io_inherited.fd_out);
	ft_memcpy(&io_transmitted, &io_inherited, sizeof(t_io));
	if(tree_el->type == OUT)
		io_transmitted.fd_out = open(tree_el->cmds[0], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
		io_transmitted.fd_out = open(tree_el->cmds[0], O_WRONLY | O_APPEND | O_CREAT, 0644);
	if (io_transmitted.fd_out == -1)
	{
		print_strerror(tree_el->cmds[0], RED, 1);
		return ;
	}
	
	browse_tree(mini, tree_el->left, io_transmitted);
	if (io_transmitted.fd_out != 0 && io_transmitted.fd_out != 1)
		close(io_transmitted.fd_out);
}

void traverse_pipe_node(t_mini *mini, t_btree *tree_el, t_io io_inherited)
{
	//ft_printf("traverse_pipe_node\n");
	int	fd_pipe[2];
	t_io	io_transmitted;
	ft_memcpy(&io_transmitted, &io_inherited, sizeof(t_io));
	if(pipe(fd_pipe) == -1)
	{
		print_strerror("pipe", RED, 1);
		return;
	}
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
	//ft_printf("traverse_logical_op_node\n");
	browse_tree(mini, tree_el->left, io_inherited);
	if ((tree_el->type == AND && g_status == 0) || (tree_el->type == OR && g_status != 0))
		browse_tree(mini, tree_el->right, io_inherited);
}

int traverse_parenthesis_node(t_mini *mini, t_btree *tree_el, t_io io_inherited)
{
	pid_t	pid;
	int		status;
	int		exit_status;

	pid = fork(); 
	if (pid == -1)
		exit(EXIT_FAILURE); //
	//process child
	if (pid == 0)
	{
		//ft_printf("JE SUIS L ENFANT\n");//
		char	*new_prompt;
		new_prompt = strdup_gc(tree_el->cmds[0], TMP);
		clear_garbage(B_TREE, free);
		mini->b_tree = NULL; //surement inutile
		mini->tkn_lst = lexer(new_prompt);
		mini->b_tree = parser(mini);
		browse_tree(mini, mini->b_tree, io_inherited);
		free_and_exit(g_status);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))  // Vérifie si le processus s'est terminé normalement
    	exit_status = WEXITSTATUS(status);
	else
	{
		//ft_printf("pas de retour status\n"); // voir si on peut creer ce cas
		return (1);
	}
	return (exit_status);
}

void browse_tree(t_mini *mini, t_btree *tree_el, t_io io_inherited)
{
	//ft_printf("browse_tree\n");
	if(!tree_el || g_status == 130) // attention la f continue meme si g_status != 0;
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
	else if(tree_el->type == PARENTHESIS)
		g_status = traverse_parenthesis_node(mini,tree_el, io_inherited);
	else if(tree_el->type == WORD)
		g_status = exec_handler(mini,tree_el, io_inherited); // ajouter quand partie de geoffrey ok
}
