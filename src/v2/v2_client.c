/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   v2_client.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvon-de <fvon-der@student.42heilbronn.d    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/03 01:06:47 by fvon-de           #+#    #+#             */
/*   Updated: 2025/02/22 14:28:41 by fvon-de          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "v2_minitalk.h"

static void	send_message_length(pid_t pid, size_t len);
static void	send_message(pid_t pid, char *str);
static void	client_send_bit(pid_t pid, int bit);
static void	handle_ack(int sig);

volatile sig_atomic_t	g_ack_received = 0;

int	main(int argc, char **argv)
{
	struct sigaction	sa;
	int					server_pid;
	const char			*message;

	if (argc != 3)
	{
		ft_printf("Usage: %s <PID>  <Message>\n", argv[0]);
		exit(EXIT_SUCCESS);
	}
	if (ft_atoi(argv[1]) <= 0 || ft_strlen(argv[2]) == 0)
	{
		ft_printf("Invalid PID or Message");
		exit(EXIT_SUCCESS);
	}
	server_pid = ft_atoi(argv[1]);
	message = argv[2];
	sa.sa_flags = 0;
	sa.sa_handler = handle_ack;
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIGUSR1, &sa, NULL) == -1
		|| sigaction(SIGUSR2, &sa, NULL) == -1)
		ft_printf(" Error: [client] sigaction");
	send_message_length(server_pid, ft_strlen(message));
	send_message(server_pid, (char *)message);
	return (0);
}

static void	handle_ack(int sig)
{
	if (sig == SIGUSR1)
	{
		g_ack_received = 1;
	}
	if (sig == SIGUSR2)
	{
		ft_printf("Server confirmed receiving the message.\n");
		exit(EXIT_SUCCESS);
	}
}

// Use SIGUSR1 for bit 1
// Use SIGUSR2 for bit 0
// Reset the acknowledgment flag before sending the bit
// Send the signal to the client using the send_bit function
// Wait until acknowledgment is received from server
// pause() function is used to make the client wait until it gets a signal
static void	client_send_bit(pid_t pid, int bit)
{
	int	signal;

	g_ack_received = 0;
	if (bit == 1)
		signal = SIGUSR1;
	else if (bit == 0)
		signal = SIGUSR2;
	else
	{
		ft_printf("Error: [client_send_bit] Invalid bit value:");
		return ;
	}
	send_bit(pid, signal);
	while (!g_ack_received)
	{
		pause();
	}
}

// - inform the server about the message size before sending the actual content.
// Iterate through each bit of the message length 
// Check if the current bit of the length is set to 1
// starting from the least significant bit.
static void	send_message_length(pid_t pid, size_t len)
{
	size_t	bit;
	int		signal;

	bit = 0;
	while (bit < (sizeof(size_t) * 8))
	{
		signal = 0;
		if ((len & ((size_t)1 << bit)) != 0)
			signal = 1;
		client_send_bit(pid, signal);
		bit++;
	}
}

// Extract each bit, starting from the most significant bit
//  bit = (str[i] >> (7 - j)) & 1;  // Right shift and mask the bit
static void	send_message(pid_t pid, char *str)
{
	int	i;
	int	j;
	int	bit;

	i = 0;
	while (str[i])
	{
		j = 0;
		while (j < 8)
		{
			bit = (str[i] >> (7 - j)) & 1;
			client_send_bit(pid, bit);
			j++;
		}
		i++;
	}
}
