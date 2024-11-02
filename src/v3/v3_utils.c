/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   v3_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvon-de <fvon-der@student.42heilbronn.d    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 15:45:47 by fvon-de           #+#    #+#             */
/*   Updated: 2024/11/02 22:11:59 by fvon-de          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "v3_minitalk.h"

void	send_bit(pid_t pid, int signal)
{
	if (signal != SIGUSR1 && signal != SIGUSR2)
	{
		ft_printf("Invalid signal: %d\n", signal);
		return ;
	}
	if (kill(pid, signal) == -1)
		print_exit("ERROR: Could not send signal", 0);
}

void	print_exit(const char *message, int mode)
{
	if (mode == 1)
	{
		ft_printf("%s\n", message);
		exit(EXIT_SUCCESS);
	}
	else
	{
		ft_printf("%s\n", message);
		exit(EXIT_FAILURE);
	}
}