/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   v4_server.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvon-de <fvon-der@student.42heilbronn.d    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 11:30:40 by fvon-de           #+#    #+#             */
/*   Updated: 2025/03/11 14:33:28 by fvon-de          ###   ########.fr       */
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
	if (g_utf8_char[0] == (char)0xFF)
	{
		g_utf8_char[0] = c;
		*i = 1;
		return (get_utf8_byte_count(c));
	}
	else
	{
		*i = 1;
		while (*i < UTF8_MAX_LEN && g_utf8_char[*i] != (char)0xFF)
			(*i)++;
		if (*i > 0 && (c & 0xC0) != 0x80)
		{
			ft_memset(g_utf8_char, 0xFF, UTF8_MAX_LEN);
			return (-1);
		}
		g_utf8_char[*i] = c;
	}
	return (0);
}

static void	handle_utf8_character(unsigned char c)
{
	int	i;
	int	byte_count;

	byte_count = validate_utf8_byte(c, &i);
	if (byte_count == -1)
		return ;
	if (get_utf8_byte_count(g_utf8_char[0]) == 1)
	{
		write(1, &c, 1);
		fflush(stdout);
		ft_memset(g_utf8_char, 0xFF, UTF8_MAX_LEN);
	}
	else if (i == get_utf8_byte_count(g_utf8_char[0]) - 1)
	{
		ft_memset(g_utf8_char + i + 1, 0, UTF8_MAX_LEN - i - 1);
		write(1, g_utf8_char, i + 1);
		fflush(stdout);
		ft_memset(g_utf8_char, 0xFF, UTF8_MAX_LEN);
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
