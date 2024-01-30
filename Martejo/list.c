#include "lexer.h"

t_token *create_node(int type, char *content)
{
	t_token	*node;

	if (!content)
		return (NULL);
	node = (t_token *)malloc(1 * sizeof(t_token));
	if (!node)
		return (NULL);
	node->content = content;
	node->type = type;
	node->next = NULL;
	node->prev = NULL;
	return (node);
}

int add_node(t_lister *list, t_token *node)
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

void free_lister(t_lister *list)
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


