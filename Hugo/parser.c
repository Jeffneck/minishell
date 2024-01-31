#include "lexer.h"

typedef struct s_ast_node {
    token_type type;             // Type du nœud (CMD, ARG, IN, OUT, etc.)
    char *content;               // Contenu associé au nœud (par exemple, le texte du token)
    struct s_ast_node *children; // Liste des enfants (nœuds fils)
    struct s_ast_node *parent;   // Nœud parent (utile pour la navigation dans l'AST)
    struct s_ast_node *next;     // Nœud suivant (s'il y a plusieurs nœuds de même niveau)
} t_ast_node;

t_ast_node *create_ast_node(token_type type, char **content)
{
    t_ast_node *new_node = malloc(sizeof(t_ast_node));
    if (!new_node)
        return (NULL);
    new_node->type = type;
    new_node->content = ft_strdup(content);
    if (!new_node->content)
        return(free(new_node), NULL)
    new_node->children = NULL;
    new_node->parent = NULL;
    new_node->next = NULL;
    return new_node;
}

void add_child(t_ast_node *parent, t_ast_node *child)
{
    if (parent != NULL && child != NULL) {
        child->parent = parent;
        if (parent->children == NULL) {
            parent->children = child;
        } else {
            // Ajoutez le nouveau nœud comme un frère du dernier enfant existant.
            t_ast_node *last_child = parent->children;
            while (last_child->next != NULL) {
                last_child = last_child->next;
            }
            last_child->next = child;
        }
    }
}

void add_sibling(t_ast_node *node, t_ast_node *sibling)
{
    if (node != NULL && sibling != NULL) {
        t_ast_node *parent = node->parent;
        if (parent != NULL) {
            // Trouvez le dernier frère existant et ajoutez le nouveau nœud après lui.
            t_ast_node *last_sibling = node;
            while (last_sibling->next != NULL) {
                last_sibling = last_sibling->next;
            }
            last_sibling->next = sibling;
        }
    }
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




