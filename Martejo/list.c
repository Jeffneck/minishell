#include "lexer.h"

t_token *create_node(e_tkntype type, char *content, int link)
{
	t_token	*node;

	if (!content)
		return (NULL);
	node = malloc_gc(sizeof(t_token), 1);
	if (!node)
		error_handler_lexer(1, "Malloc error\n");//close
	node->content = content;
	node->type = type;
	node->link = link;
	node->next = NULL;
	node->prev = NULL;
	return (node);
}

int add_node(t_tknlist *list, t_token *node)
{
	if (!node)
		return (-1);
	if (!list->head)
	{
		list->head = node;
		list->tail = node;
	}
	else
	{
		node->prev = list->tail;
		list->tail->next = node;
		list->tail = node;
	}
	return (1);
}

t_tknlist	*init_list(t_tknlist **list)
{
	(*list) = malloc_gc(sizeof(t_tknlist), 1);
	if (!(*list))
		error_handler_lexer(1, "Malloc error\n");//close
	(*list)->head = NULL;
	(*list)->tail = NULL;
}

// Utiliser le garbage
void free_lister(t_tknlist *list)
{
	t_token *current;
	t_token *next;

	if (list == NULL)
		return;

	current = list->head;
	while (current != NULL)
	{
		next = current->next;
		free(current->content);
		free(current);
		current = next;
	}
	// free(list); // A liberer si list mallocer
}


