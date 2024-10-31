/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   v3_minitalk.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvon-de <fvon-der@student.42heilbronn.d    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 15:39:05 by fvon-de           #+#    #+#             */
/*   Updated: 2024/10/31 15:49:53 by fvon-de          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef V3_MINITALK_H
#define V3_MINITALK_H

#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <libft.h>
#include <libftprintf.h>

// Structure to hold message information
typedef struct s_message
{
    pid_t   pid;        // PID of the client
    char    c;          // Current character being processed
    int     bit;        // Current bit position
    int     flag;       // Flag to indicate character processing state
    int     str_index;  // Current index in the string
    char    str[256];   // Buffer to store the received string
} t_message;

// Function prototypes
void    send_bit(pid_t pid, int bit);
void    handle_ack(int sig);
void    send_message(pid_t pid, char *str);
void    sig_handler(int signum, siginfo_t *info, void *context);
void    set_and_print_str(int sig, t_message *s);
void    handle_utf8_character(t_message *s);
// UTILS
void    bit_to_char(int sig, t_message *s);
void    print_exit(const char *message, int status);

#endif // V3_MINITALK_H