#include "../../include/minishell.h"

int	is_unexpected_char(char c)
{
	if (c == '|' || c == '&' || c == ')')
		return (1);
	return (0);
}

void	display_syntax_err(char *flag)
{
	ft_printf_fd(2,
		"Minishell: syntax error near unexpected token `%c", flag[0]);
	if ((flag[0] == '&' || flag[0] == '|') && flag[1] == flag[0])
	{
		write(2, &flag[1], 1);
	}
	ft_printf_fd(2, "'\n");
}

int	is_double_operators(char *str)
{
	if ((str[0] == '&' || str[0] == '|') && str[1] == str[0])
		return (1);
	return (0);
}

int	research_syntax_err(char *flag)
{
	size_t	i;

	i = 0;
	while (ft_isspace(flag[i]))
		i++;
	if (is_unexpected_char(flag[i]))
	{
		display_syntax_err(flag);
		return (2);
	}
	return (0);
}

void	verify_input_syntax(char *input)
{
	size_t	i;
	char	*flag;

	i = 0;
	if (!input)
		return ;
	flag = &input[0];
	if (is_unexpected_char(flag[0]))
		display_syntax_err(flag);
	while (input[i])
	{
		if (is_double_operators(&input[i]))
			flag = &input[i + 1];
		else if (is_operator_char(input[i]))
		 	flag = &input[i]; // a gerer 
		if (research_syntax_err(flag) == 2)
		{
			g_status = 2;
			return ;
		}
		i++;
	}
}
