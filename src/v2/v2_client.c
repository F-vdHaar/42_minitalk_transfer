/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   v2_client.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvon-de <fvon-der@student.42heilbronn.d    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/03 01:06:47 by fvon-de           #+#    #+#             */
/*   Updated: 2024/11/03 01:09:47 by fvon-de          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "v2_minitalk.h"

static void send_message_length(pid_t pid, size_t len);
static void send_message(pid_t pid, char *str);
static void client_send_bit(pid_t pid, int bit);
static void handle_ack(int sig);

volatile sig_atomic_t g_ack_received = 0;

int main(int argc, char **argv)
{
	struct sigaction	sa;
	int					server_pid;
	const char			*message;

	if (argc != 3)
	{
		ft_printf("Usage: %s <PID>  <Message>\n", argv[0]);
		print_exit("Error: User Input", EXIT_SUCCESS);
	}
	server_pid = atoi(argv[1]);
	message = argv[2];
	sa.sa_flags = 0;
	sa.sa_handler = handle_ack;
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIGUSR1, &sa, NULL) == -1 || sigaction(SIGUSR2, &sa, NULL) == -1)
		{
			print_exit("sigaction", EXIT_FAILURE);
		}
	send_message_length(server_pid, strlen(message));
	send_message(server_pid, (char *)message);
	return (0);
}


static void handle_ack(int sig)
{
	if (sig == SIGUSR1)
	{
		g_ack_received = 1;
	}
	if (sig == SIGUSR2)
	{
		print_exit("Server recieved message", 1);
	}
}

static void client_send_bit(pid_t pid, int bit)
{
	int signal;
	
	if (bit == 1)
		signal = SIGUSR1;
	else if (bit == 0)
		signal = SIGUSR2;
	else
	{
		print_exit("Invalid bit value:", 0);
		return;
	}
	g_ack_received = 0;
	send_bit(pid, signal);
	while (!g_ack_received)
		usleep(50);
}


static void send_message_length(pid_t pid, size_t len)
{
	size_t  bit;
    
    bit  = 0;
	while (bit < (sizeof(size_t) * 8))
	{
		int bin = 0;
		if ((len & ((size_t)1 << bit)) != 0)
			bin = 1;
		client_send_bit(pid, bin);
		bit++;
	}
}

static void send_message(pid_t pid, char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		int j = 0;
		while (j < 8)
		{
			int bit = 0;
			if ((str[i] & (1 << j)) != 0)
			{
				bit = 1;
			}
			client_send_bit(pid, bit);
			j++;
		}
		i++;
	}
}