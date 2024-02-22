// void	pipe_node()
// {
// 		int	fds_pipe[2];

// 		pipe(fds_pipe);
// 		depth_first_search(node->left, visit);
//         visit(node);
//         depth_first_search(node->right, visit);
// }

// void 

// void depth_first_search(t_mini mini, t_btree *node)
// {
//     if (node && node->type == PIPE) 
// 	{
//         pipe_node();
//     }
// 	else if
// }

int	*return_fds_pipe()
{
	int	*fds_pipe;

	if (pipe(fds_pipe) == -1)
		exit(EXIT_ERROR);//
	return (fds_pipe);
}

int	return_fd_file(char *filename, t_tkntype type)
{
	int	fd_file;

	if (type == IN)
		fd_file = open(); //verifier que le file est accessible en lecture seule
	if (type == OUT)
		fd_file = open(); //creer ou utiliser file existant
	if (fd_file == -1)//valider les codes de retour erreur
		exit(EXIT_ERROR);//
	return (fd_file);
}


int	*redirect_child_left(int *io_inherited, int *fds_pipe, int fd_file)
{
	int	*io_totransmit;
	
	io_totransmit = io_inherited;
	// io_totransmit = (int *) calloc_gc(2, sizeof(int), BTREE);
	if (tree_el->type == PIPE)
		io_totransmit = (int *){io_inherited[IN], fds_pipe[WRITE]};
	else if (tree_el->type == IN)
		io_totransmit = (int *){fd_file, io_inherited[OUT]};
	else if (tree_el->type == OUT)
		io_totransmit = (int *){io_inherited[IN], io_inherited[OUT]};
	return(io_totransmit);
}

int	*redirect_child_right(int *io_inherited, int *fds_pipe, int fd_file)
{
	int	*io_totransmit;
	
	io_totransmit = io_inherited;
	// io_totransmit = (int *) calloc_gc(2, sizeof(int), BTREE);
	if (tree_el->type == PIPE)
		io_totransmit = (int *){fds_pipe[READ], io_inherited[OUT]};
	return(io_totransmit);
}


void depth_first_search(t_mini *mini, t_btree *tree_el, int *io_inherited)
{
	int	*io_totransmit;
	int	*fds_pipe;
	int	fd_file;

	// io_totransmit = io_inherited;
	if (tree_el = mini->btree_root)
		io_totransmit = (int *) calloc_gc(2, sizeof(int), BTREE);
	fds_pipe = NULL;
	if (tree_el->type == PIPE)
		fds_pipe = return_fds_pipe(); 
	fd_file = NULL;
	if (tree_el->type == IN || tree_el->type == OUT)
		return_fd_redir(tree_el->content, tree_el->type);
	if (tree_el) 
	{
		// if (tree_el->type == IN || tree_el->type == OUT || tree_el->type == PIPE)
		io_totransmit = redirect_child_left(io_inherited, fds_pipe, fd_file);
		depth_first_search(mini, tree_el->left, io_totransmit);
        // if (tree_el->type == PIPE)
		io_totransmit = redirect_child_right(io_inherited, fds_pipe, fd_file);
        depth_first_search(mini, tree_el->right, io_totransmit);
    }
}

//version du code qui utilise une struct de faire une structure 

typedef struct s_io
{
	int	*io_totransmit;
	int	*io_inherited;
	int	*fds_pipe;
	int	fd_file;
}	t_io;


int	*return_fds_pipe()
{
	int	*fds_pipe;

	if (pipe(fds_pipe) == -1)
		exit(EXIT_ERROR);//
	return (fds_pipe);
}

int	return_fd_file(char *filename, t_tkntype type)
{
	int	fd_file;

	if (type == IN)
		fd_file = open(); //verifier que le file est accessible en lecture seule
	if (type == OUT)
		fd_file = open(); //creer ou utiliser file existant
	if (fd_file == -1)//valider les codes de retour erreur
		exit(EXIT_ERROR);//
	return (fd_file);
}


int	*redirect_child_left(int *io_inherited, int *fds_pipe, int fd_file)
{
	int	*io_totransmit;
	
	io_totransmit = io_inherited;
	// io_totransmit = (int *) calloc_gc(2, sizeof(int), BTREE);
	if (tree_el->type == PIPE)
		io_totransmit = (int *){io_inherited[IN], fds_pipe[WRITE]};
	else if (tree_el->type == IN)
		io_totransmit = (int *){fd_file, io_inherited[OUT]};
	else if (tree_el->type == OUT)
		io_totransmit = (int *){io_inherited[IN], io_inherited[OUT]};
	return(io_totransmit);
}

int	*redirect_child_right(int *io_inherited, int *fds_pipe, int fd_file)
{
	int	*io_totransmit;
	
	io_totransmit = io_inherited;
	// io_totransmit = (int *) calloc_gc(2, sizeof(int), BTREE);
	if (tree_el->type == PIPE)
		io_totransmit = (int *){fds_pipe[READ], io_inherited[OUT]};
	return(io_totransmit);
}


void depth_first_search(t_mini *mini, t_btree *tree_el, int *io_inherited)
{
	fds_pipe = NULL;
	if (tree_el->type == PIPE)
		fds_pipe = return_fds_pipe(); 
	fd_file = NULL;
	if (tree_el->type == IN || tree_el->type == OUT)
		return_fd_redir(tree_el->content, tree_el->type);
	if (tree_el) 
	{
		// if (tree_el->type == IN || tree_el->type == OUT || tree_el->type == PIPE)
		io_totransmit = redirect_child_left(io_inherited, fds_pipe, fd_file);
		depth_first_search(mini, tree_el->left, io_totransmit);
        // if (tree_el->type == PIPE)
		io_totransmit = redirect_child_right(io_inherited, fds_pipe, fd_file);
        depth_first_search(mini, tree_el->right, io_totransmit);
    }
}

int	main()
{
	t_io fd_io; //on le met dans mini ?

	depth_first_search()
}



//version du code qui part du principe que chaque node a un fd_in, fd_out

int	*return_fds_pipe()
{
	int	*fds_pipe;

	if(tree_el->type != PIPE)
		return;
	if (pipe(fds_pipe) == -1)
		exit(EXIT_ERROR);//
	return (fds_pipe);
}

int	return_fd_file(char *filename, t_tkntype type)
{
	int	fd_file;

	fd_file = -1; //code special reconaissable
	if (type == IN)
		fd_file = open(filename, O_RDONLY); //verifier que le file est accessible en lecture seule
	if (type == OUT)
		fd_file = open(filename, OWRONLY | O_CREATE | O_TRUNCATE); //add les permissions
	if (type == APPEND)
		fd_file = open(filename, O_APPEND); //creer ou utiliser file existant
	if (fd_file == -1)//valider les codes de retour erreur
		exit(EXIT_ERROR);//
	return (fd_file);
}


int	*redirect_io_child_left(t_btree *tree_el, int *fds_pipe, int fd_file)
{
	int	io_redir[2];

	ft_memcpy(io_redir, tree_el->io);
	if (tree_el->type == PIPE)
		io_redir[OUT] = fds_pipe[WRITE];
	else if (tree_el->type == OUT || tree_el->type == APPEND)
		io_redir [OUT]= fd_file;
	else if (tree_el->type == IN)
		io_redir[IN] = fd_file;
}

int *redirect_io_child_right(t_btree *tree_el, int io[2], int fds_pipe[2])
{
	int	io_redir[2];

	ft_memcpy(io_redir, tree_el->io);
	if (tree_el->type == PIPE)
		io_redir[IN] = fds_pipe[READ];
	return (io_redir);
}

void	exec_process(t_btree *tree_el)
{
	char *cmdpath;
	cmdpath = get_cmdpath(tree_el->content);
	if (!cmdpath)
		exit(EXIT_FAILURE);//
	dup2(tree_el->io[IN], STDIN_FILENO);
	dup2(tree_el->io[OUT], STDOUT_FILENO);
	close(tree_el->io[IN]);
	close(tree_el->io[OUT]);
	g_status = execve(cmdpath, tree_el->args, env_to_char2(tree_el->env));
}

void	fork_exec(t_env *env, t_btree *tree_el)
{
	pid_t	pid;
	pid = fork();
	if (pid == -1)
		exit(EXIT_FAILURE);
	if(pid == 0)
		exec_process(tree_el)
}

void	close_used_fd(t_btree *tree_el, int fds_pipe[2], int fd_file, int mode)
{
	if(mode = 0)
	{
		if(tree_el->type == PIPE)
			close(fds_pipe[WRITE]);
		if(tree_el->type == IN || tree_el->type == OUT || tree_el->type == APPEND)
			close(fd_file);
	}
	else if(mode = 1 && tree_el->type == PIPE)
		close(fds_pipe[READ]);
}

void depth_first_search(t_mini *mini, t_btree *tree_el, int io_inherited[2])
{
	int	io_child[2];
	int	fds_pipe[2];
	int	fd_file;

	if(!tree_el)
		return;
	ft_memcpy(tree_el->io, io_inherited, sizeof(int[2]));
	ft_memset(fds_pipe, 0, sizeof(int[2]));//definir la valeur par defaut de fds_pipe; un code negatif special semble adapte
	fds_pipe = return_fds_pipe(tree_el->type); 
	fd_file = return_fd_file(tree_el->content, tree_el->type);
	
	io_child = redirect_io_child_left(tree_el, fds_pipe, fd_file);
	depth_first_search(mini, tree_el->left, io_child);
	if(tree_el->type == WORD || tree_el->type == ONE_QUOTE || tree_el->type == TWO_QUOTE) //transformer toutes les commandes en type word serait plus simple, ou fonction tree_el_is_cmd()
		fork_exec(mini->env, tree_el, tree_el->io);
	close_used_fd(tree_el, fds_pipe, fd_file, 0);
	if (AND OR et gstatus)// mal fait, attention aux cas ou il n' y a ni and ni or ...
	{
		io_child = redirect_io_child_right(tree_el, fds_pipe, fd_file);
		depth_first_search(mini, tree_el->right, io_child);
		close_used_fd(tree_el, fds_pipe, fd_file, 0);

	}
}

int	main()
{

	depth_first_search(mini, mini->btree_root, mini->io);
}