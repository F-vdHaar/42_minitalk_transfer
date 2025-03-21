/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   v2_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvon-der <fvon-der@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/03 01:11:24 by fvon-de           #+#    #+#             */
/*   Updated: 2025/02/27 01:00:24 by fvon-der         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "v2_minitalk.h"

void	send_bit(pid_t pid, int signal)
{
	if (signal != SIGUSR1 && signal != SIGUSR2)
	{
		ft_printf("Invalid signal: %d\n", signal);
		return ;
	}
	if (kill(pid, signal) == -1)
	{
		perror("kill");
		ft_printf("Could not send signal", 1);
	}
}

void	handle_errors(int error_flag)
{
	if (error_flag == 1)
		ft_printf("Error: Memory allocation failed\n");
	if (error_flag == 2)
		ft_printf("Error: Received invalid message length\n");
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

void	reset_signal_handlers(void)
{
	struct sigaction	default_sa;

	default_sa.sa_handler = SIG_DFL;
	default_sa.sa_flags = 0;
	sigemptyset(&default_sa.sa_mask);
	sigaction(SIGUSR1, &default_sa, NULL);
	sigaction(SIGUSR2, &default_sa, NULL);
}
