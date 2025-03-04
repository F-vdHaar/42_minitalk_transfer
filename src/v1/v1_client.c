/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   v1_client.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvon-de <fvon-der@student.42heilbronn.d    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/03 00:46:42 by fvon-de           #+#    #+#             */
/*   Updated: 2025/03/04 17:38:31 by fvon-de          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "v1_minitalk.h"

static void	send_string(pid_t server_pid, const char *str);
static void	send_char(pid_t server_pid, char c);
static void	handle_ack(int sig);

volatile sig_atomic_t	g_ack_received = 0;

static void	send_char(pid_t server_pid, char c)
{
	g_ack_received = 0;
	int i = 0;
	while (i < 8)
	{
		g_ack_received = 0;
		if (c & (1 << (7 - i))) 
			send_bit(server_pid, SIGUSR2); 
		else
			send_bit(server_pid, SIGUSR1); 
		
		while (!g_ack_received)
			{
				pause();
			}
		i++;
	}
}

static void	send_string(pid_t server_pid, const char *str)
{
	while (*str)
	{
		send_char(server_pid, *str);
		str++;
	}
	send_char(server_pid, '\0');
}

int	main(int argc, char *argv[])
{
	pid_t	server_pid;
	const char	*message;
	struct sigaction	sa;

	if (argc != 3)
	{
			write(1, &"Usage: ./client <PID> <string>", 31);
			return (EXIT_FAILURE);		
	}
	sa.sa_flags = 0;
	sa.sa_handler = handle_ack;
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIGUSR1, &sa, NULL) == -1
		|| sigaction(SIGUSR2, &sa, NULL) == -1)
		return (ft_printf(" Error: [client] sigaction"), 1);
	server_pid = ft_atoi(argv[1]);
	message = argv[2];
	send_string(server_pid, message);
	return (EXIT_SUCCESS);
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