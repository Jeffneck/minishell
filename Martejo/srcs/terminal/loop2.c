#include "../../include/minishell.h"


int8_t	input_error(char **input, char **shell_prompt,
	char **envp[], int save_in)
{
	if (!*input)
	{
		close(save_in);
		eof_exit();
	}
	*input = strtrim_gc(*input, " ", LOOP);
	if (!*input)
		print_message("Minishell: Allocation error.\n", RED, 1);
	if (ft_strequ(*input, ""))
		return (-1);
	if (near_unexpected_token_error(input, shell_prompt, envp))
		return (-1);
	return (0);
}

uint8_t input_first_read(char **input, char **shell_prompt,
                         char **envp[], int save_in) {
    int save_status;

    save_status = g_status;
    g_status = -256;
    *input = readline(*shell_prompt);
    if (!*input) {
        // Utilisateur a interrompu l'entrée avec Ctrl+C
        g_status = 130;
        return 2;
    } else if (g_status < -256) {
        g_status = 130;
    } else {
        g_status = save_status;
    }
    if (input_error(input, shell_prompt, envp, save_in)) {
        return 1;
    }
    return 0;
}

int first_read_processing(int *save_in, char **input,
                          char **shell_prompt, char **envp[]) {
    int ret_first_read;

    signal(SIGINT, sig_handler);
    signal(SIGQUIT, sig_handler);
    // signal(SIGTSTP, sig_handler);
    *save_in = dup(STDIN_FILENO);
    ret_first_read = input_first_read(input, shell_prompt, envp, *save_in);
    if (ret_first_read == 1) {
        close(*save_in);
        return 2;
    }
    if (ret_first_read == 2) {
        dup2(*save_in, STDIN_FILENO);
        close(*save_in);
        return 1;
    }
    return 0;
}

uint8_t parsing_and_exec_processing(int save_in, char **input, char **envp[]) {
    t_list *cmd_list;

    cmd_list = global_parsing(*input, save_in);
    if (*input && **input) {
        add_history(*input);
    }
    if (!cmd_list) {
        return 1;
    }
    if (!exec_main(cmd_list, envp)) {
        return 0;
    }
    return 1;
}

void prompt_loop(char **envp[], char *line_read, char *prompt) {
    int save_in;
    int display_line;
    int ret_first_read;

    display_line = 0;
    while (1) {
        if (display_line)
        {
            rl_on_new_line();
            rl_redisplay();
        }
        display_line = 1;
        ret_first_read = get_line(&save_in, &line_read,
                                               &prompt, envp);
        if (ret_first_read) {
            if (ret_first_read == 2) {
                display_line = 0;
            }
            continue;
        }
        display_line = parsing_and_exec_processing(save_in, &line_read, envp);
        //pas besoin de ca car on ne touche pas a stdin ?
        // dup2(save_in, STDIN_FILENO);
        // close(save_in);
        clear_garbage();
        free(line_read);
        line_read = NULL;
        prompt = create_prompt(envp);
    }
}

//display line == 0 > la ligne n'est pas affichee
//idee while (1)
//  prompot_loop(create_prompt)

int	main(int argc, char **argv, char **envp)
{
	t_mini	mini;
	(void)argc;
	(void)argv;
	
	
	mini.env = init_env(envp);
	mini.io_global = (t_io){0, 1};
	prompt_loop(&mini, "", create_prompt(&mini));
	return (0);
}