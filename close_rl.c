// #include <stdio.h>
// #include <stdlib.h>
// #include <signal.h>
// #include <readline/readline.h>
// #include <readline/history.h>

// void sigint_handler(int sig) {
//     // Ne rien faire ici, readline sera arrêté automatiquement
//     // lorsqu'un signal SIGINT est reçu
// }

// int main() {
//     // Installation du gestionnaire de signaux SIGINT
//     signal(SIGINT, sigint_handler);

//     while (1) {
//         // Affichage du prompt
//         char *line_read = readline("> ");

//         // Si l'utilisateur appuie sur Ctrl+C, readline sera arrêté automatiquement
//         // sans qu'aucune action supplémentaire ne soit nécessaire
//         printf("test");
//         if (line_read && *line_read) {
//             // Ajout de l'entrée à l'historique
//             add_history(line_read);
//             printf("You entered: %s\n", line_read);
//             free(line_read);
//         }
//     }

//     return 0;
// }


// #include <readline/readline.h>
// #include <readline/history.h>

// int main() {
//     // Initialisation de readline
//     // rl_bind_key('\t', rl_complete);
//     char *line_read = readline("> ");
//     // Affichage d'une ligne de commande
//     rl_replace_line("Nouvelle ligne de commande", 1); // 1 pour effacer l'historique des modifications (undo)

//     // Rafraîchissement de l'affichage
//     rl_redisplay();

//     return 0;
// }


// #include <stdio.h>
// #include <stdlib.h>
// #include <signal.h>
// #include <readline/readline.h>
// #include <readline/history.h>

// volatile sig_atomic_t stop_readline = 0;

// void sigint_handler(int sig) {
//     stop_readline = 1;
// }

// int main() {
//     signal(SIGINT, sigint_handler);

//     while (1) {
//         char *line_read = NULL;

//         if (stop_readline) {
//             // Si SIGINT est reçu, stop_readline sera défini à 1,
//             // alors nous devons arrêter la saisie en cours et déclencher une nouvelle saisie.
//             rl_on_new_line(); // Aller à une nouvelle ligne
//             printf("");
//             rl_replace_line("", 0); // Effacer la ligne de commande actuelle
//             printf("aaa");
//             rl_redisplay(); // Rafraîchir l'affichage
//             printf("aaa");
//             rl_on_new_line(); // Aller à une nouvelle ligne
//             stop_readline = 0; // Réinitialiser le drapeau
//             printf("aaa");
//         }

//         line_read = readline("> ");

//         if (line_read && *line_read) {
//             add_history(line_read);
//             printf("You entered: %s\n", line_read);
//             free(line_read);
//         }
//     }

//     return 0;
// }


#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>

void sigint_handler(int sig_num) {
    // Supprimer l'avertissement de variable inutilisée
    (void)sig_num;

    // Nettoyer la ligne courante
    // Nettoie la ligne courante sur laquelle readline attendait une entrée
    rl_replace_line("", 0); //
    // Déplace le curseur à une nouvelle ligne
    rl_on_new_line();
    // Redessine le prompt
    rl_redisplay();

    // Afficher un nouveau prompt (le `\n` garantit que le buffer est vidé si en mode tampon ligne)
    write(STDOUT_FILENO, "\nNouveau prompt> ", 16);
}

int main(void) {
    char *input;

    // Configurer le gestionnaire de SIGINT pour gérer Ctrl+C
    signal(SIGINT, sigint_handler);

    while (1) {
        input = readline("prompt> ");
        printf("line = %s\n", input);
        if (input == NULL) {
            // Gérer EOF (Ctrl+D)
            write(STDOUT_FILENO, "\nFin du shell\n", 14);
            break;
        }
        if (*input) { // Si la ligne n'est pas vide
            add_history(input);
            // Traiter l'entrée ici
        }
        free(input);
    }

    return 0;
}