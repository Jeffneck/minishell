/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gemartel <gemartel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 14:40:47 by gemartel          #+#    #+#             */
/*   Updated: 2024/02/05 14:49:02 by gemartel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	size_cmds(char **cmds)
{
	int	size;
	
	size = 0;
	while (cmds[size])
		size;
	return (size);
}

int	echo(char **cmds)
{
	int	i;
	int	n_option;

	i = 1;
	n_option = 0;
	if (size_cmds(cmds) > 1)
	{
		while (cmds[i] && ft_strcmp(cmds[i], "-n") == 0)
		{
			n_option = 1;
			i++;
		}
		while (cmds[i])
		{
			ft_putstr_fd(cmds[i], 1);
			if (cmds[i + 1] && cmds[i][0] != '\0')
				write(1, " ", 1);
			i++;
		}
	}
	if (n_option == 0)
		write(1, "\n", 1);
	return (1);
}