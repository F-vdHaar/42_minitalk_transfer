/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvon-der <fvon-der@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 20:30:39 by fvon-der          #+#    #+#             */
/*   Updated: 2024/07/31 20:30:39 by fvon-der         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "v3_minitalk.h"

static void	bit_to_char(int sig, t_message *s)
{
	ft_printf("DEBUG: server bit to char func\n");
	ft_printf("DEBUG: Initial state - flag: %d, c: %c, bit: %d\n", s->flag, s->c, s->bit);
	if (s->flag == 1)
	{
		s->flag = 0;
		s->c = 0;
		s->bit = 0;
	}
	if (sig == SIGUSR2)
		s->c |= (1 << s->bit);
	s->bit++;
	ft_printf("DEBUG: Updated state - c: %c, bit: %d\n", s->c, s->bit);
	if (s->bit == 8)
	{
		s->flag = 1;
		s->bit = 0;
		ft_printf("DEBUG: Character complete - c: %c\n", s->c);
	}
}

static void	handle_utf8_character(t_message *s)
{
	int	byte_count;

	ft_printf("DEBUG: server handle utf8 func\n");
	ft_printf("DEBUG: Handling character - c: %c, str_index: %d\n", s->c, s->str_index);
	if ((s->c & 0b10000000) == 0)
		byte_count = 1;
	else if ((s->c & 0b11100000) == 0b11000000)
		byte_count = 2;
	else if ((s->c & 0b11110000) == 0b11100000)
		byte_count = 3;
	else if ((s->c & 0b11111000) == 0b11110000)
		byte_count = 4;
	else
		return ;
	if (s->c == '\0')
	{
		send_bit(s->pid, SIGUSR2);
		s->str[s->str_index - 1] = '\0';
		ft_printf("DEBUG: Null character received - String: %s\n", s->str);
		s->str_index = 0;
	}
	else if (--byte_count == 0)
	{
		s->str[s->str_index] = '\0';
		ft_printf("DEBUG: Character complete - String: %s\n", s->str);
		s->str_index = 0;
	}
}

static void	set_and_print_str(int sig, t_message *s)
{
	//ft_printf("DEBUG: server set and print string func\n");
	//ft_printf("DEBUG Processing signal: %d, bit position: %d, char: %c\n", sig, s->bit, s->c);
	bit_to_char(sig, s);
	send_bit(s->pid, SIGUSR1);
	if (s->flag == 1)
	{
		handle_utf8_character(s);
		s->flag = 0;
	}
}

void	sig_handler(int signum, siginfo_t *info, void *context)
{
	static t_message	s = {0, 0, 0, 0, 0, {0}};
//	ft_printf(" DEBUG: sig handler func\n");
	(void)context;
	if (s.pid == 0)
		s.pid = info->si_pid;
	if (s.pid != info->si_pid)
	{
		ft_printf("Error: multiple clients detected\n");
		return ;
	}
	set_and_print_str(signum, &s);
	if (s.c == '\0')
	{
		send_bit(s.pid, SIGUSR2);
		ft_memset(&s, 0, sizeof(t_message));
	}
}

int	main(void)
{
	struct sigaction	sa;

	ft_printf("PID: %d\n", getpid());
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART | SA_SIGINFO | SA_NODEFER;
	sa.sa_sigaction = sig_handler;
	if (sigaction(SIGUSR1, &sa, NULL) == -1)
		return (EXIT_FAILURE);
	if (sigaction(SIGUSR2, &sa, NULL) == -1)
		return (EXIT_FAILURE);
	while (1)
		pause();
	return (0);
}
