#include "lexer.h"

typedef struct s_ast_node {
    token_type type;             // Type du nœud (CMD, ARG, IN, OUT, etc.)
    char **content;               // Contenu associé au nœud (par exemple, cmd et arguments)
    struct s_ast_node *prev;   // Nœud parent (utile pour la navigation dans l'AST)
    struct s_ast_node *next;     // Nœud suivant (s'il y a plusieurs nœuds de même niveau)
} t_ast_node;

t_ast_node *create_ast_node(token_type type, char **content)
{
    t_ast_node *new_node = malloc(sizeof(t_ast_node));
    if (!new_node)
        return (NULL);
    new_node->type = type;
    new_node->content = char2dup(content);
    if (!new_node->content)
        return(free(new_node), NULL)
    new_node->prev = NULL;
    new_node->next = NULL;
    return new_node;
}

parse(t_token *tokens)
{
    t_ast_node *root = NULL;
    t_ast_node *current_node = NULL;

    while (tokens != NULL) {
        t_token *token = tokens;

        // Créez un nouveau nœud AST en fonction du type de token
        t_ast_node *new_node = create_ast_node(token->type, token->content);

        // Ajoutez le nœud à l'AST
        if (root == NULL)
        {
            root = new_node;
            current_node = root;
        }
        else
        {
            // Ajoutez le nouveau nœud comme enfant du nœud courant
            current_node->children = new_node;
            new_node->parent = current_node;
            current_node = new_node;
        }
        tokens = tokens->next;
    }
    return root;

}




