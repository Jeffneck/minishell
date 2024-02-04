#include "lexer.h"

// int	main()
// {
// 	char		*buffer;
// 	char		*pwd;
// 	char		*pwd2;
// 	t_tknlist	*list;

// 	pwd = getcwd(NULL, 0);
// 	pwd2 = ft_strjoin(pwd, " $> ");
// 	free(pwd);
// 	while (1)
// 	{
// 		buffer = readline(pwd2);
		
// 		if (buffer == NULL || strcmp(buffer, "exit") == 0)
// 			break;
// 		if (!add_to_garbage(buffer, 1))
// 			printf("error gc\n"); //close
// 		printf("%s\n", buffer);
// 		list = lexer(buffer);
// 		t_token *node;
// 		node = list->head;
// 		while (node)
// 		{
// 			printf("type = %d\n", node->type);
// 			printf("content = %s\n", node->content);
// 			node = node->next;
// 		}
		
// 	}
// 	if (pwd2)
// 		free(pwd2);
// 	clear_garbage(1, free);
// 	rl_clear_history();	
// }

int	main()
{
	char		*buffer;
	t_tknlist	*list;

	buffer = strdup_gc("cat eeee\"sa lut\" | echo \"\"'salut' && (zoupi || echo \" bonjour\"\"eee)", 1);
	printf("%s\n", buffer);
	list = lexer(buffer);
	t_token *node;
	node = list->head;
	while (node)
	{
		printf("type = %d\n", node->type);
		printf("content = %s\n", node->content);
		node = node->next;
	}
	clear_garbage(1, free);
}