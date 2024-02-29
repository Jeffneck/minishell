#include "../../include/minishell.h"

void	err_if_unexpected_tkn_lexer(char *following, char c_curr)
{
	char	c_evaluated;

	c_evaluated = following[ft_strlen_until_not_char(following, ' ')];
	if(c_evaluated == '|' || c_evaluated == '&')
	{
		ft_printf_fd(2, "bash: syntax error near unexpected token %c", c_curr);
		free_and_exit(2);
	}
}

//lire prompt 

//mettre a jour char displayed et nb_char_displayed si | apres | ou si & apres &
//
int		is_unexpected_char(char c)
{
	if(c == '|' || c == '&' || c == ')')
		return (1);
	return (0);
	
}

int		is_special_char(char c)
{
	if(c == '|' || c == '&' || c == ')' || c == '*')
		return (1);
	return (0);
}

void	display_syntax_err(char *flag)
{
	ft_printf_fd(2, "bash: syntax error near unexpected token `%c", flag[0]);
	write(2, &flag[i], 1);
	if((flag[0] == '&' || flag[0] == '|') && flag[1] == flag[0])
	{
		write(2, &flag[1], 1);
	}
	ft_printf_fd(2, "'\n");
}

int		is_double_operators(char *str)
{
	if((str[0] == '&' || str[0] == '|') && str[1] == str[0])
		return (1);
}

void	research_syntax_err()
{
	if(flag[ft_strlen_until])/////////////////////
}

void	verify_syntax_lexer(char *input, char c_curr)
{
	size_t	i;
	int		nb_chars;
	char	*flag;

	i = 0;
	if (!input)
		return ;
	flag = &input[0];
	if (is_unexpected_char(flag))
		display_syntax_err(flag);
	while(input[i])
	{
		if(is_double_operators(&input[i]))
			flag = &input[i];
		else if(is_operator_char(input[i]) || is_double_operators(&input[i]))
			flag = &input[i];
		research_syntax_err(flag);
	}
}

