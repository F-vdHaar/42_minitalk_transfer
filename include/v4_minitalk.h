/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   v4_minitalk.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvon-de <fvon-der@student.42heilbronn.d    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/03 01:05:29 by fvon-de           #+#    #+#             */
/*   Updated: 2025/03/11 11:22:06 by fvon-de          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef V4_MINITALK_H
# define V4_MINITALK_H

# include <signal.h>
# include <stddef.h>
# include <stdlib.h>
# include <fcntl.h>
# include "libft.h" 
# include "ft_printf.h"

#define UTF8_MAX_LEN 4

void	send_bit(pid_t pid, int signal);
void	handle_errors(int flag);
void	print_binary(unsigned char byte);
void	reset_signal_handlers(void);

#endif