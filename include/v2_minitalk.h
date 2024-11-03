/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   v2_minitalk.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvon-de <fvon-der@student.42heilbronn.d    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/03 01:05:29 by fvon-de           #+#    #+#             */
/*   Updated: 2024/11/03 01:05:58 by fvon-de          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef V2_MINITALK_H
# define V2_MINITALK_H

typedef struct sigaction	t_sigaction;

typedef struct s_message
{
	int		pid;
	char	c;
	int		flag;
	int		bit;
	int		str_index;
	char	str[MAX_BUFFER_SIZE];
}	t_message;

void	send_bit(pid_t pid, int signal);
void	print_exit(char *message, int mode);

#endif