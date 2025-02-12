/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   v2_minitalk.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvon-de <fvon-der@student.42heilbronn.d    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/03 01:05:29 by fvon-de           #+#    #+#             */
/*   Updated: 2025/02/12 18:26:55 by fvon-de          ###   ########.fr       */
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
	char*	data;
	int		pid_client;
	int		pid_server;
	size_t	bit_index;
	size_t	char_index;
	int		len_flag;
	int		error_flag;
	int		clean_flag;
}	t_message;

void	send_bit(pid_t pid, int signal);
void	print_exit(const char *message, int mode);
void	handle_errors();
void	print_binary(unsigned char byte);

#endif