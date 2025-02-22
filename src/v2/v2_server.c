/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   v2_server.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvon-de <fvon-der@student.42heilbronn.d    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/03 01:10:38 by fvon-de           #+#    #+#             */
/*   Updated: 2025/02/22 17:36:27 by fvon-de          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "v2_minitalk.h"

static void	sig_handler(int signum, siginfo_t *info, void *context);
static void	process_message(void);
static void	get_str_len(int signum);
static void	set_and_print_str(int signum);

static t_message	g_s = {0, NULL, 0, 0, 0, 0, 0, 0, 0};

static void	get_str_len(int signum)
{
	if (signum == SIGUSR1)
	{
		if (g_s.bit_index < sizeof(size_t) * 8)
			g_s.length |= ((size_t)1 << g_s.bit_index);
	}
	g_s.bit_index++;
	if (g_s.bit_index >= sizeof(size_t) * 8)
	{
		if (g_s.length == 0)
			g_s.error_flag = 2;
		g_s.len_flag = 1;
		g_s.bit_index = 0;
	}
	send_bit(g_s.pid_client, SIGUSR1);
}

static void	set_and_print_str(int signum)
{
	if (!g_s.data)
		return ;
	if (signum == SIGUSR1)
		g_s.data[g_s.char_index] |= (1 << (7 - (g_s.bit_index % 8)));
	else if (signum == SIGUSR2)
		g_s.data[g_s.char_index] &= ~(1 << (7 - (g_s.bit_index % 8)));
	g_s.bit_index++;
	if (g_s.bit_index % 8 == 0)
	{
		g_s.char_index++;
		if (g_s.char_index < g_s.length)
			g_s.data[g_s.char_index] = 0;
	}
	if (g_s.char_index == g_s.length)
	{
		g_s.data[g_s.length] = '\0';
		write(1, g_s.data, ft_strlen(g_s.data) + 1);
		write(1, "\n", 2);
		fflush(stdout);
		send_bit(g_s.pid_client, SIGUSR2);
		g_s.clean_flag = 1;
		return ;
	}
	send_bit(g_s.pid_client, SIGUSR1);
}

// Access the t_message struct through the si_value field  
static void	sig_handler(int signum, siginfo_t *info, void *context)
{
	(void)context;
	if (g_s.pid_client == 0)
		g_s.pid_client = info->si_pid;
	if (g_s.pid_client != info->si_pid)
		g_s.error_flag = 3;
	if (g_s.len_flag == 0)
	{
		get_str_len(signum);
		return ;
	}
	if (g_s.len_flag != 0)
	{
		if (g_s.data == NULL)
			g_s.error_flag = 2;
		set_and_print_str(signum);
	}
	return ;
}

// Register SIGUSR1 and SIGUSR2 signals to be handled by sig_handler
// Wait for incoming signals
int	main(int argc, char **argv)
{
	struct sigaction	sa;

	(void)argc;
	(void)argv;
	ft_printf("PID: %d\n", getpid());
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART | SA_SIGINFO;
	sa.sa_sigaction = sig_handler;
	if (sigaction(SIGUSR1, &sa, NULL) == -1
		|| sigaction(SIGUSR2, &sa, NULL) == -1)
		ft_printf("Error: recieved invalid signal\n");
	process_message();
	return (0);
}

static void	process_message(void)
{
	while (1)
	{
		pause();
		if (g_s.length && g_s.len_flag && !g_s.data)
		{
			g_s.data = (char *)ft_calloc(g_s.length + 1, sizeof(char));
			if (!g_s.data)
				g_s.error_flag = 1;
		}
		if (g_s.error_flag)
			handle_errors(g_s.error_flag);
		if (g_s.clean_flag)
		{
			if (g_s.data)
				free(g_s.data);
			g_s.data = NULL;
			g_s.length = 0;
			g_s.len_flag = 0;
			g_s.char_index = 0;
			g_s.bit_index = 0;
			g_s.error_flag = 0;
			g_s.clean_flag = 0;
			g_s.pid_client = 0;
		}
	}
}
