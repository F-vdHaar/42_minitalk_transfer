/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   v3_server.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvon-de <fvon-der@student.42heilbronn.d    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/03 01:10:38 by fvon-de           #+#    #+#             */
/*   Updated: 2025/03/04 18:52:53 by fvon-de          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "v3_minitalk.h"

char	*message = NULL;

static void get_str_len(int pid_client, int signum);
static void set_and_print_str(int pid_client, int signum);

static void sig_handler(int signum, siginfo_t *info, void *context)
{
	(void)context;
	if (message == NULL)
	{
		get_str_len(info->si_pid, signum);
		return;
	}
	else
	{
		set_and_print_str(info->si_pid, signum);
	}
}

static void	get_str_len(int pid_client, int signum)
{
	static size_t message_length = 0;
	static int bit_index = 0;

	if (signum == SIGUSR1)
	{
		if (bit_index < sizeof(size_t) * 8)
		{
			message_length |= ((size_t)1 << bit_index);
		}
	}
	bit_index++;
	if (bit_index >= sizeof(size_t) * 8)
	{
		message = calloc(message_length + 1, sizeof(char));
		if (!message)
		{
			write(2, "Error: Allocation Error\n", 24);
			exit(EXIT_FAILURE);
		}
		bit_index = 0;
	}
	send_bit(pid_client, SIGUSR1);
}

static void	set_and_print_str(int pid_client, int signum)
{
	static int bit_count = 0;

	if (!message)
	{
		return;
	}
	if (signum == SIGUSR1)
	{
		message[bit_count / 8] |= (1 << (7 - (bit_count % 8)));
	}
	else if (signum == SIGUSR2)
	{
		message[bit_count / 8] &= ~(1 << (7 - (bit_count % 8)));
	}
	bit_count++;
	if (message[bit_count] == '\0')
	{
		write(1, message, ft_strlen(message));
		write(1, "\n", 1);
		fflush(stdout);
		send_bit(pid_client, SIGUSR2);
		free(message);
		message = NULL;
		bit_count = 0;
		return;
	}
	send_bit(pid_client, SIGUSR1);
}

int	main(int argc, char **argv)
{
	struct sigaction sa;

	(void)argc;
	(void)argv;
	printf("PID: %d\n", getpid());
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART | SA_SIGINFO;
	sa.sa_sigaction = sig_handler;
	if (sigaction(SIGUSR1, &sa, NULL) == -1 || sigaction(SIGUSR2, &sa, NULL) == -1)
	{
		write(2, "Error: received invalid signal\n", 31);
		return (EXIT_FAILURE);
	}
	while (1)
	{
		pause();
	}
	return (EXIT_SUCCESS);
}
