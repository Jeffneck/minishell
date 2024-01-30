#include "lexer.h"

int	main()
{
	char	*buffer;
	char	*pwd;
	t_lister	list;

	list.head = NULL;
	list.tail = NULL;
	pwd = getcwd(NULL, 0);
	pwd = ft_strjoin(pwd, " $> ");
	while (1)
	{
		buffer = readline(pwd);
		
		if (buffer == NULL || strcmp(buffer, "exit") == 0)
			break;
		printf("%s\n", buffer);
		lexer(buffer, &list);
		t_token *node;
		node = list.head;
		while (node)
		{
			printf("type = %d\n", node->type);
			printf("content = %s\n", node->content);
			node = node->next;
		}
		
		free(buffer);
	}
	if (buffer)
		free(buffer);
	if (pwd)
		free(pwd);
	rl_clear_history();	
}