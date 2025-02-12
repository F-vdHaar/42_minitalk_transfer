/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   v2_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvon-de <fvon-der@student.42heilbronn.d    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/03 01:11:24 by fvon-de           #+#    #+#             */
/*   Updated: 2025/02/12 18:28:28 by fvon-de          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "v2_minitalk.h"

void	send_bit(pid_t pid, int signal)
{
	//ft_printf("DEBUG: Sending signal  %d to PID: %d\n", signal,  pid);
	if (signal != SIGUSR1 && signal != SIGUSR2)
	{
		ft_printf("Invalid signal: %d\n", signal);
		return ;
	}
	if (kill(pid, signal) == -1)
	{
		perror("kill");
		print_exit("Could not send signal", 1);
	}
}

void	print_exit(const char *message, int mode)
{
	if (!message)
    	return ;
	if (mode == 0)
	{
		ft_printf("%s\n", message);
		exit(EXIT_SUCCESS);
	}
	else if (mode == 1)
	{
		ft_printf("ERROR: %s\n", message);
		exit(EXIT_FAILURE);
	}
}

void	handle_errors(int error_flag)
{
	if (error_flag == 1)
		print_exit("Error: Memory allocation failed\n", EXIT_FAILURE);
	if (error_flag == 2)
		print_exit("Error: Received invalid message length\n", EXIT_FAILURE);
}

// Function to print a byte in binary using a while loop
void print_binary(unsigned char byte)
{
    char bit;
    int i = 7;

    while (i >= 0)
    {
        if ((byte >> i) & 1)
            bit = '1';
        else
            bit = '0';        
        write(1, &bit, 1);
        i--;
    }
	write(1, "\n", 2);
}

