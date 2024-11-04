/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_v1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvon-der <fvon-der@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 22:51:26 by fvon-der          #+#    #+#             */
/*   Updated: 2024/08/29 22:51:26 by fvon-der         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "v3_minitalk.h"

volatile sig_atomic_t	g_ack_received = 0;

void	handle_ack(int sig)
{
	ft_printf("DEBUG: Ack Signal :%d\n", sig);
	if (sig == SIGUSR1)
	{
		if (g_ack_received == 1)
		{
			print_exit("Duplicate ACK received\n", 1);
		}
		g_ack_received = 1;
	}
	else if (sig == SIGUSR2)
	{
		g_ack_received = 1;
		print_exit("Server recieved message", 0);
	}
}

void	send_message(pid_t pid, const char *str)
{
	int	i;
	int	j;
	int	bit;

	i = 0;
	//ft_printf("DEBUG: Send message func\n");
	while (str[i] != '\0')
	{
		//ft_printf("DEBUG: Sending char '%c' as binary: ", str[i]);
		j = 0;
		while (j < 8)
		{
			if (str[i] & (1 << j))
				bit = SIGUSR2;
			else
				bit = SIGUSR1;
			g_ack_received = 0;
			send_bit(pid, bit);
			while (!g_ack_received)
				usleep(10000000);
			j++;
		}
		i++;
	}
}

int	main(int argc, char **argv)
{
	struct sigaction	sa;
	int					server_pid;

	if (argc != 3)
	{
		ft_printf("Usage: %s <PID> <Message>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	server_pid = atoi(argv[1]);
	sa.sa_flags = SA_RESTART;
	sa.sa_handler = handle_ack;
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIGUSR1, &sa, NULL) == -1
		|| sigaction(SIGUSR2, &sa, NULL) == -1)
	{
		ft_printf("Error: sigaction failed\n");
		exit(EXIT_FAILURE);
	}
	send_message(server_pid, argv[2]);
	return (0);
}
