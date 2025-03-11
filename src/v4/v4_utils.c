/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   v4_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvon-de <fvon-der@student.42heilbronn.d    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 17:45:42 by fvon-de           #+#    #+#             */
/*   Updated: 2025/03/11 14:11:44 by fvon-de          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "v4_minitalk.h"

void	send_bit(pid_t pid, int signal)
{
	if (signal != SIGUSR1 && signal != SIGUSR2)
	{
		write(2, "Invalid signal: ", 16);
		write(2, &signal, sizeof(signal));
		write(2, "\n", 1);
		return ;
	}
	if (kill(pid, signal) == -1)
	{
		perror("kill");
		write(2, "Could not send signal\n", 22);
		exit (EXIT_FAILURE);
	}
}

// Function to print a byte in binary using a while loop
void	print_binary(unsigned char byte)
{
	char	bit;
	int		i;

	i = 7;
	while (i >= 0)
	{
		if ((byte >> i) & 1)
			bit = '1';
		else
			bit = '0';
		write(1, &bit, 1);
		i--;
	}
	write(1, "\n", 2);
}
