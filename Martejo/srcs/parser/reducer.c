#include "parser.h"
#include "../../include/minishell.h"

int	type_need_reducing(t_tkntype type)
{
	if (type == IN || type == OUT || type == APPEND 
		|| type == HEREDOC || type == TWO_QUOTE || type == ONE_QUOTE)
		return (1);
	return(0);

}

void	reducer(t_token	*tkn) 
{
	char	*tmp;
	e_bool	flag;

	while (tkn)
	{
		flag = false;
		//expansion heredoc
		if (type_need_reducing(tkn->type))
		{
			flag = true;
			if(tkn->type == IN || tkn->type == OUT)
				tmp = remove_substr(tkn->content, 0, 1 + strlen_until_not(tkn->content, ft_isspace));
			else if (tkn->type == TWO_QUOTE || tkn->type == ONE_QUOTE || tkn->type == PARENTHESIS)
				tmp = strcut_gc(tkn->content, 1, 1, TKN_LIST);
		}
		if (flag)
		{
			if (!tmp)
				exit(EXIT_FAILURE); //err management
			del_one_garbage(tkn->content, TKN_LIST);
			tkn->content = tmp;
		}
		tkn = tkn->next;
	}
}