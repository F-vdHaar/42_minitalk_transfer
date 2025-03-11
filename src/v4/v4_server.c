/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   v4_server.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvon-de <fvon-der@student.42heilbronn.d    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 11:30:40 by fvon-de           #+#    #+#             */
/*   Updated: 2025/03/11 15:23:47 by fvon-de          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "v4_minitalk.h"

static int	get_utf8_byte_count(unsigned char c);
static void	handle_utf8_character(unsigned char c);
static int	validate_utf8_byte(unsigned char c, int *i);
static void	sig_handler(int signum, siginfo_t *info, void *context);

static char	g_utf8_char[UTF8_MAX_LEN + 1] = {0xFF, 0xFF, 0xFF, 0xFF, 0};

static int	get_utf8_byte_count(unsigned char c)
{
	if ((c & 0b10000000) == 0)
		return (1);
	if ((c & 0b11100000) == 0b11000000)
		return (2);
	if ((c & 0b11110000) == 0b11100000)
		return (3);
	if ((c & 0b11111000) == 0b11110000)
		return (4);
	return (0);
}

static int	validate_utf8_byte(unsigned char c, int *i)
{
	int	byte_count;
	int	expected_length;

	if (g_utf8_char[0] == (char)0xFF)
	{
		byte_count = get_utf8_byte_count(c);
		if (byte_count == 0)
			return (-1);
		g_utf8_char[0] = c;
		*i = 1;
		return (byte_count);
	}
	expected_length = get_utf8_byte_count(g_utf8_char[0]);
	if (*i < expected_length && (c & 0xC0) == 0x80)
	{
		g_utf8_char[*i] = c;
		(*i)++;
		if (*i == expected_length)
			return (g_utf8_char[*i] = '\0', expected_length);
		return (0);
	}
	memset(g_utf8_char, 0xFF, UTF8_MAX_LEN);
	return (-1);
}

static void	handle_utf8_character(unsigned char c)
{
	static int	i = 0;
	int			byte_count;

	byte_count = validate_utf8_byte(c, &i);
	if (byte_count == -1)
	{
		i = 0;
		return ;
	}
	if (get_utf8_byte_count(g_utf8_char[0]) == 1)
	{
		write(1, &c, 1);
		fflush(stdout);
		memset(g_utf8_char, 0xFF, UTF8_MAX_LEN);
		i = 0;
	}
	else if (i == get_utf8_byte_count(g_utf8_char[0]))
	{
		write(1, g_utf8_char, i);
		fflush(stdout);
		memset(g_utf8_char, 0xFF, UTF8_MAX_LEN);
		i = 0;
	}
}

static void	sig_handler(int sig, siginfo_t *info, void *context)
{
	static int				bit = 0;
	static unsigned char	c = 0;

	(void)context;
	if (sig == SIGUSR2)
		c |= (1 << (7 - bit));
	bit++;
	if (bit == 8)
	{
		if (c == '\0')
		{
			write(1, &"\n", 2);
			send_bit(info->si_pid, SIGUSR2);
		}
		else
			handle_utf8_character(c);
		bit = 0;
		c = 0;
	}
	send_bit(info->si_pid, SIGUSR1);
}

int	main(void)
{
	struct sigaction	sa;
	char				*pid_str;

	pid_str = ft_itoa(getpid());
	write(1, "[INFO] Server PID: ", 20);
	write(1, pid_str, ft_strlen(pid_str));
	write(1, &"\n", 2);
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART | SA_SIGINFO | SA_NODEFER;
	sa.sa_sigaction = sig_handler;
	if (sigaction(SIGUSR1, &sa, NULL) == -1
		|| sigaction(SIGUSR2, &sa, NULL) == -1)
	{
		write(2, "[ERROR] Failed to set up signal handlers\n", 42);
		return (EXIT_FAILURE);
	}
	while (1)
		pause();
	return (EXIT_SUCCESS);
}
