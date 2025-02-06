/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   v3_minitalk.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvon-der <fvon-der@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 21:22:47 by fvon-de           #+#    #+#             */
/*   Updated: 2024/11/04 18:35:29 by fvon-der         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef V3_MINITALK_H
# define V3_MINITALK_H

# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include "libft.h"
# include "ft_printf.h"

// Define the message structure
typedef struct s_message {
	pid_t	pid;
	char	c;
	int		flag;
	int		bit;
	int		str_index;
	char	str[4];
}	t_message;

// Function declarations
// UTILS
void	send_bit(pid_t pid, int signal);
void	print_exit(const char *message, int mode);
// CLIENT
void	handle_ack(int sig);
void	send_message(pid_t pid, const char *str);
void	sig_handler(int signum, siginfo_t *info, void *context);

#endif // V3_MINITALK_H
