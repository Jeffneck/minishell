#include "lexer.h"

int	main()
{
	char		*buffer;
	t_tknlist	*list;

	buffer = strdup_gc("<<\"EOF\" cat | grep '\"salut\"' &&echo 'welldone' bitch >>outfile lol", 1);
	printf("%s\n", buffer);
	list = lexer(buffer);
	t_token *node;
	node = list->head;
	while (node)
	{
		printf("type = %d and link = %d\n", node->type, node->link);
		printf("content = %s\n", node->content);
		node = node->next;
	}
	clear_garbage(1, free);
}