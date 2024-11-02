/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   v1_client.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvon-de <fvon-der@student.42heilbronn.d    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/03 00:46:42 by fvon-de           #+#    #+#             */
/*   Updated: 2024/11/03 00:55:37 by fvon-de          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

static void	send_string(pid_t server_pid, const char *str);
static void	send_char(pid_t server_pid, char c);

static void	send_char(pid_t server_pid, char c)
{
    int i = 0;
    while (i < 8) {
        if (c & (1 << (7 - i))) 
            send_bit(server_pid, SIGUSR2); 
        else
            send_bit(server_pid, SIGUSR1); 
        
        usleep(100);
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
}

int	main(int argc, char *argv[])
{
	pid_t	server_pid;
	const char	*message;

	if (argc != 3)
		print_exit("Usage: ./client <PID> <string>", 0);

	server_pid = ft_atoi(argv[1]);
	message = argv[2];

	send_string(server_pid, message);

	return (EXIT_SUCCESS);
}
