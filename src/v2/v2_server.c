/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   v2_verver.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvon-de <fvon-der@student.42heilbronn.d    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/03 01:10:38 by fvon-de           #+#    #+#             */
/*   Updated: 2024/11/03 01:11:00 by fvon-de          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "v2_minitalk.h"

static	void	bit_to_char(int sig, t_message *s)
{
	static int	bit;

	if (s->flag == 1)
	{
		s->flag = 0;
		s->c = 0;
	}
	if (sig == SIGUSR1)
		s->c = s->c | (1 << bit);
	bit++;
	send_bit(s->pid, SIGUSR1);
	if (bit == 8)
	{
		s->flag = 1;
		bit = 0;
	}
}


static void	set_and_print_str(int sig, t_message *s)
{
	bit_to_char(sig, s);
	if (s->flag == 1)
	{
		s->str[(s->bit_counter - sizeof(size_t) * 8) / 8 - 1] = s->c;
		if (s->bit_counter == s->str_len * 8 + sizeof(size_t) * 8)
		{
			ft_printf("%s\n", s->str);
			send_bit(s->pid, SIGUSR2);
			free(s->str);
			ft_memset(s, 0, sizeof(t_message));
		}
	}
}

static	size_t	get_str_len(int signum, t_message *s)
{
	if (s->bit_counter < (8 * sizeof(size_t)))
	{
		if (signum == SIGUSR1)
		{
			s->str_len |= (size_t)1 << s->bit_counter;
		}
		s->bit_counter++;
		send_bit(s->pid, SIGUSR1);
		if (s->bit_counter == sizeof(size_t) * 8)
		{
			return (1);
		}
	}
	else
	{
		s->bit_counter++;
	}
	return (0);
}

void	sig_handler(int signum, siginfo_t *info, void *context)
{
	static t_message	s = {0, NULL, 0, 0, 0, 0, 0};

	(void)context;
	if (s.pid == 0)
		s.pid = info->si_pid;
	if (s.pid != info->si_pid)
	{
		kill(s.pid, SIGUSR2);
		free(s.str);
		ft_memset(&s, 0, sizeof(t_message));
		print_exit("Error: you are sending from 2 clients at once\n", EXIT_FAILURE);
	}
/* 	if (get_str_len(signum, &s) == 1)
	{
		s.str = (char *)malloc(sizeof(char) * (s.str_len + 1));
		if (!s.str)
			print_exit("Error: server Malloc failed\n", EXIT_FAILURE);
		s.str[s.str_len] = '\0';
	} */
	if (s.bit_counter > sizeof(size_t) * 8)
		set_and_print_str(signum, &s);
}


int	main(int argc, char **argv)
{
	struct sigaction	sa;

	(void)argv;
	(void)argc;
	ft_printf("PID: %d\n", getpid());
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART | SA_SIGINFO;
	sa.sa_sigaction = sig_handler;
	if (sigaction(SIGUSR1, &sa, NULL) == -1)
		return (EXIT_FAILURE);
	if (sigaction(SIGUSR2, &sa, NULL) == -1)
		return (EXIT_FAILURE);
	while (1)
	{
		pause();
	}
	return (0);
}