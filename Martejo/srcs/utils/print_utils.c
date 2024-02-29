/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gemartel <gemartel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 14:20:13 by gemartel          #+#    #+#             */
/*   Updated: 2024/02/29 14:29:17 by gemartel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	print_and_exit(char *msg, char *color, int exit_status)
{
	write(2, color, ft_strlen(color));
	write(2, msg, ft_strlen(msg));
	write(2, RESET, ft_strlen(RESET));
	if (exit_status)
		free_and_exit(exit_status);
}

void	print_path_error(char *arg, int exit_status, int error)
{
	ft_putstr_fd("Minishell: ", 2);
	ft_putstr_fd(arg, 2);
	if (error == 1)
		ft_printf_fd(2, ": command not found");
	if (error == 2)
		ft_printf_fd(2, ": permission denied");
	if (error == 3)
		ft_printf_fd(2, ": No such file or directory");
	if (error == 4)
		ft_printf_fd(2, ": Is a directory");
	if (error == 5)
		ft_printf_fd(2, ": filename argument required\n.: usage: . filename [arguments]");
	free_and_exit(exit_status);
}