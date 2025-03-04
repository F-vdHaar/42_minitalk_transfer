/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   v1_server.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvon-de <fvon-der@student.42heilbronn.d    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 17:45:22 by fvon-de           #+#    #+#             */
/*   Updated: 2025/03/04 17:45:24 by fvon-de          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "v1_minitalk.h"

// Signal handler for SIGUSR1 and SIGUSR2 signals
	// Shift current_char left to make room for the next bit
	// Set least significant bit to 1 if SIGUSR2
	// If 8 bits (1 character) have been received, print the character
// (void)  -> this is for some backwards compatibility stuff
void	handle_signal(int sig, siginfo_t *info, void *context)
{
	static int		bit_count = 0;
	static char		current_char = 0;

	(void)context;
	current_char <<= 1;
	if (sig == SIGUSR2)
		current_char |= 1;
	bit_count++;
	if (bit_count == 8)
	{
		write(1, &current_char, 1);
		if (current_char == '\0')
		{
			write(1, &"\n", 2);
			send_bit(info->si_pid, SIGUSR2);
		}
		bit_count = 0;
		current_char = 0;
	}
	send_bit(info->si_pid, SIGUSR1);
}

// Set up signal handling for SIGUSR1 and SIGUSR2
void	setup_signal_handling(void)
{
	struct sigaction	sa;

	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = handle_signal;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGUSR1, &sa, NULL);
	sigaction(SIGUSR2, &sa, NULL);
}

	// Print the server's PID
	// Setup signal handlers
	// Server runs indefinitely, waiting for signals
int	main(void)
{
	printf("Server PID: %d\n", getpid());
	setup_signal_handling();
	while (1)
		pause();
	return (0);
}
