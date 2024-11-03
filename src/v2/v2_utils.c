/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   v2_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvon-de <fvon-der@student.42heilbronn.d    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/03 01:11:24 by fvon-de           #+#    #+#             */
/*   Updated: 2024/11/03 01:11:51 by fvon-de          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "v2_minitalk.h"

void	send_bit(pid_t pid, int signal)
{
	//ft_printf("DEBUG: send_bit in utils \n Sending signal  %d to PID: %d\n", signal,  pid);
	if (signal != SIGUSR1 && signal != SIGUSR2)
	{
		ft_printf("Invalid signal: %d\n", signal);
		return ;
	}
	if (kill(pid, signal) == -1)
		print_exit("Could not send signal", 1);
}

void	print_exit(char *message, int mode)
{
	if (mode == 0)
	{
		ft_printf("%s\n", message);
		exit(EXIT_SUCCESS);
	}
	else if (mode == 1)
	{
		ft_printf("ERROR: %s\n", message);
		exit(EXIT_FAILURE);
	}
}