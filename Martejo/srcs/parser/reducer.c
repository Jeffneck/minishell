#include "../../include/minishell.h"

int	type_need_reducing(t_tkntype type)
{
	if (type == IN || type == OUT || type == APPEND 
		|| type == HEREDOC || type == TWO_QUOTE || type == ONE_QUOTE || type == PARENTHESIS)
		return (1);
	return(0);

}

void	reducer(t_token	*tkn) 
{
	ft_printf("reducer\n");
	char	*tmp;
	bool	flag;

	if (g_status != 0);
		return;
	while (tkn)
	{
		flag = false;
		//expansion heredoc
		if (type_need_reducing(tkn->type))
		{
			flag = true;
			if(tkn->type == IN || tkn->type == OUT)
				tmp = remove_substr(tkn->content, 0, 1 + ft_strlen_until_not(&tkn->content[1], ft_isspace));
			if(tkn->type == HEREDOC || tkn->type == APPEND)
				tmp = remove_substr(tkn->content, 0, 2 + ft_strlen_until_not(&tkn->content[2], ft_isspace));
			else if (tkn->type == TWO_QUOTE || tkn->type == ONE_QUOTE || tkn->type == PARENTHESIS)
				tmp = ft_strcut(tkn->content, 1, 1);
		}
		if (flag)
		{
			if (!tmp)
				exit(EXIT_FAILURE); //err management
			add_to_garbage(tmp, TKN_LIST);
			del_one_garbage(tkn->content, TKN_LIST);
			tkn->content = tmp;
		}
		tkn = tkn->next;
	}
}