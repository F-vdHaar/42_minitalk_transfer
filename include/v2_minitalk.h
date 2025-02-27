/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   v2_minitalk.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvon-der <fvon-der@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/03 01:05:29 by fvon-de           #+#    #+#             */
/*   Updated: 2025/02/27 00:47:25 by fvon-der         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef V2_MINITALK_H
# define V2_MINITALK_H

# include <signal.h>
# include <stddef.h>
# include <stdlib.h>
# include <fcntl.h>
# include "libft.h" 
# include "ft_printf.h"

typedef struct s_message
{
	size_t	length;
	char	*data;
	int		pid_client;
	int		pid_server;
	size_t	bit_index;
	size_t	char_index;
	int		len_flag;
	int		error_flag;
	int		clean_flag;
}	t_message;

void	send_bit(pid_t pid, int signal);
void	handle_errors(int flag);
void	print_binary(unsigned char byte);
void	reset_signal_handlers(void);

#endif