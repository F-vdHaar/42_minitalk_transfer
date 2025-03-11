/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   v4_server.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvon-de <fvon-der@student.42heilbronn.d    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 11:30:40 by fvon-de           #+#    #+#             */
/*   Updated: 2025/03/11 13:25:42 by fvon-de          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "v4_minitalk.h"

static void set_and_print_str(int sig, siginfo_t *info);
static int get_utf8_byte_count(unsigned char c);
static void handle_utf8_character(unsigned char c);
static void sig_handler(int signum, siginfo_t *info, void *context);

static char g_utf8_char[UTF8_MAX_LEN + 1] = {0xFF, 0xFF, 0xFF, 0xFF, 0};

static int get_utf8_byte_count(unsigned char c)
{
    if ((c & 0b10000000) == 0)
        return (1);
    if ((c & 0b11100000) == 0b11000000)
        return (2);
    if ((c & 0b11110000) == 0b11100000)
        return (3);
    if ((c & 0b11111000) == 0b11110000)
        return (4);
    return (0);
}

static void handle_utf8_character(unsigned char c)
{
    int i;
    int byte_count;

    if (g_utf8_char[0] == (char)0xFF)
    {
        g_utf8_char[0] = c;
        byte_count = get_utf8_byte_count(c);
        if (byte_count == 0)
        {
            ft_printf("[ERROR] Invalid UTF-8 start byte: 0x%x\n", c);
            ft_memset(g_utf8_char, 0xFF, UTF8_MAX_LEN);
            return;
        }
        i = 1;
    }
    else
    {
        i = 1;
        while (i < UTF8_MAX_LEN && g_utf8_char[i] != (char)0xFF)
            i++;
        if (i > 0 && (c & 0xC0) != 0x80)
        {
            ft_printf("[ERROR] Invalid continuation byte: 0x%x\n", c);
            ft_memset(g_utf8_char, 0xFF, UTF8_MAX_LEN);
            return;
        }
        g_utf8_char[i] = c;
    }
	if (get_utf8_byte_count(g_utf8_char[0]) == 1)
    {
        ft_printf("%c", c);
        fflush(stdout);
        ft_memset(g_utf8_char, 0xFF, UTF8_MAX_LEN);
    }
    else if (i == get_utf8_byte_count(g_utf8_char[0] - 1))
    {
        ft_memset(g_utf8_char + i + 1, 0, UTF8_MAX_LEN - i - 1); 
        //ft_printf("[DEBUG] UTF-8 char buffer: %s\n", g_utf8_char);
        ft_printf("%s", g_utf8_char); 
        fflush(stdout); 
        ft_memset(g_utf8_char, 0xFF, UTF8_MAX_LEN); 

    }
}



static void set_and_print_str(int sig, siginfo_t *info)
{
    static int bit = 0;
    static unsigned char c = 0;

    //ft_printf("[DEBUG] Received signal: %d, bit position: %d\n", sig, bit);
    if (sig == SIGUSR2)
		c |= (1 << (7 - bit));
    bit++;
    //ft_printf("[DEBUG] Updated char: 0x%x\n", c);
    if (bit == 8)
    {
        //ft_printf("[DEBUG] Full byte received: 0x%x (%c)\n", c, c);
        if (c == '\0')
        {
			write(1, &"\n", 2);
            //ft_printf("[DEBUG] Null terminator detected, sending SIGUSR2 to %d\n", info->si_pid);
            send_bit(info->si_pid, SIGUSR2);
        }
        else
            handle_utf8_character(c);
        bit = 0;
        c = 0;
    }
    send_bit(info->si_pid, SIGUSR1);
}

//ft_printf("[DEBUG] Signal handler triggered: %d\n", signum);
static void sig_handler(int signum, siginfo_t *info, void *context)
{
    (void)context;
    set_and_print_str(signum, info);
}

int main(void)
{
    struct sigaction sa;

    ft_printf("[INFO] Server PID: %d\n", getpid());
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART | SA_SIGINFO | SA_NODEFER;
    sa.sa_sigaction = sig_handler;
    if (sigaction(SIGUSR1, &sa, NULL) == -1 || sigaction(SIGUSR2, &sa, NULL) == -1)
    {
        ft_printf("[ERROR] Failed to set up signal handlers\n");
        return (EXIT_FAILURE);
    }
    while (1)
        pause();
    return (0);
}
