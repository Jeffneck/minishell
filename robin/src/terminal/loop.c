/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 20:29:44 by rbicanic          #+#    #+#             */
/*   Updated: 2022/03/25 18:58:57 by cberganz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Cette fonction effectue la première lecture de l'entrée utilisateur.
// Elle appelle la fonction readline() pour obtenir une ligne de commande.
// Le statut de retour de cette fonction est stocké dans g_status. Si l'utilisateur a interrompu l'entrée avec Ctrl+C (signal SIGINT), le statut sera 130.
// La fonction input_error() est appelée pour gérer les erreurs de lecture.
// Retourne 0 si la lecture s'est bien déroulée, 1 en cas d'erreur de lecture, et 2 si l'utilisateur a interrompu l'entrée.
uint8_t	input_first_read(char **input, char **shell_prompt,
	char **envp[], int save_in)
{
	int	save_status;

	save_status = g_status;
	g_status = -256;
	*input = garbage_addptr(readline(*shell_prompt), LOOP);
	if (g_status == 130)
		return (2);
	else if (g_status < -256)
		g_status = 130;
	else
		g_status = save_status;
	if (input_error(input, shell_prompt, envp, save_in))
		return (1);
	return (0);
}

// Cette fonction gère le traitement initial de la lecture de l'entrée utilisateur.
// Elle enregistre les signaux SIGINT, SIGQUIT et SIGTSTP pour éviter leur traitement par le processus en cours d'exécution.
// Elle duplique le descripteur de fichier STDIN pour pouvoir restaurer plus tard l'entrée standard après son éventuelle modification.
// Elle appelle input_first_read() pour effectuer la lecture initiale.
// En cas d'erreur de lecture, elle ferme le descripteur dupliqué et retourne 2.
// Si l'utilisateur interrompt l'entrée, elle restaure STDIN et retourne 1.
// Retourne 0 si la lecture s'est bien déroulée et aucune interruption n'a eu lieu
int	first_read_processing(int *save_in, char **input,
	char **shell_prompt, char **envp[])
{
	int		ret_first_read;

	signal(SIGINT, sig_handler);
	signal(SIGQUIT, sig_handler);
	signal(SIGTSTP, sig_handler);
	*save_in = dup(STDIN_FILENO);
	ret_first_read = input_first_read(input, shell_prompt, envp, *save_in);
	if (ret_first_read == 1)
	{
		close(*save_in);
		return (2);
	}
	if (ret_first_read == 2)
	{
		dup2(*save_in, STDIN_FILENO);
		close(*save_in);
		return (1);
	}
	return (0);
}

// Cette fonction effectue le traitement et l'exécution de la ligne de commande.
// Elle utilise une fonction global_parsing() pour analyser la ligne de commande et construire une liste de commandes à exécuter.
// Elle ajoute la commande à l'historique si la ligne n'est pas vide.
// Elle exécute ensuite les commandes à l'aide de exec_main().
// Retourne 1 en cas d'erreur, sinon 0.
uint8_t	parsing_and_exec_processing(int save_in, char **input, char **envp[])
{
	t_list	*cmd_list;

	cmd_list = global_parsing(*input, save_in);
	if (!ft_strequ(*input, ""))
		add_history(*input);
	if (!cmd_list)
		return (1);
	if (!exec_main(cmd_list, envp))
		return (0);
	return (1);
}

// Cette fonction constitue la boucle principale du shell, affichant continuellement le prompt et traitant les entrées utilisateur.
// Elle appelle first_read_processing() pour obtenir et traiter la première ligne de commande.
// Si une commande a été exécutée avec succès, elle restaure STDIN, nettoie la mémoire tampon et réinitialise les variables pour la prochaine itération de la boucle.
// Si une erreur s'est produite lors de la lecture de l'entrée, elle continue la boucle sans exécuter le traitement et l'exécution de la commande.
// Si l'utilisateur a interrompu l'entrée, elle réinitialise le traitement de la nouvelle ligne sans afficher de nouveau prompt.
// La boucle continue jusqu'à ce que le programme soit terminé explicitement.
void	prompt_loop(char **envp[], char *input, char *shell_prompt)
{
	int		save_in;
	int		new_line;
	int		ret_first_read;

	new_line = 0;
	while (1)
	{
		if (new_line)
			ft_putendl_fd("", 2);
		new_line = 1;
		ret_first_read = first_read_processing(&save_in, &input,
				&shell_prompt, envp);
		if (ret_first_read)
		{
			if (ret_first_read == 2)
				new_line = 0;
			continue ;
		}
		new_line = parsing_and_exec_processing(save_in, &input, envp);
		dup2(save_in, STDIN_FILENO);
		close(save_in);
		del_garbage(LOOP);
		input = "";
		shell_prompt = create_prompt(envp);
	}
}

//VARIABLES UTILISEES

// save_in
//utilisée pour stocker le descripteur de fichier de stdin avant toute modification. 
// Cela permet au shell de restaurer facilement le flux d'entrée standard à son état initial après avoir effectué les opérations nécessaires. 
// Par exemple, après avoir lu une commande à partir du terminal ou après avoir terminé l'exécution d'une commande, 
// le flux d'entrée standard doit être restauré pour que le shell puisse lire de nouvelles commandes à partir du terminal.