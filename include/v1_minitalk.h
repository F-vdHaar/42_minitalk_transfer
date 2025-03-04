/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   v1_minitalk.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvon-de <fvon-der@student.42heilbronn.d    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 17:15:47 by fvon-de           #+#    #+#             */
/*   Updated: 2025/03/04 17:16:10 by fvon-de          ###   ########.fr       */
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


void	send_bit(pid_t pid, int signal);
void	handle_errors(int flag);
void	print_binary(unsigned char byte);


#endif