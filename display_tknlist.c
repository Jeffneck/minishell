#include "../include/minishell.h"

void	display_tknlist(t_tknlist *list)
{
		t_token *curr;

		curr = list->head;
		while (curr)
		{
			printf("\n--NODE--\n");
			printf("type = %d\n", curr->type);
			printf("content ='%s'\n", curr->content);
			printf("link =%d\n", curr->link);
			printf("index =%d\n", (unsigned int) curr->index);
			printf("priority =%d\n\n", curr->priority);
			curr = curr->next;
		}
		//sleep(5);
}