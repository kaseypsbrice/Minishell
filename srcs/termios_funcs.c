/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   termios_funcs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbrice <kbrice@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/19 08:37:31 by kbrice            #+#    #+#             */
/*   Updated: 2023/08/07 11:19:31 by kbrice           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*	tcsetattr is causing an error and I'm not sure why	*/
void	ft_suppress_output(void)
{
	struct termios	new_settings;

	if (tcgetattr(0, &new_settings))
		perror("minishell: tcsetattr");
	new_settings.c_lflag &= ~ECHOCTL;
	if (tcsetattr(0, 0, &new_settings))
		perror("minishell: tcsetattr");
}
/* Modifies the terminal settings to clear the ECHOCTL bit flag.
 * It supresses ^C from appearing on the command line when trying to display
 * a new prompt. To be honest, I didn't need to do this since some shells show it,
 * but I did it anyway so that it's like the shell at 42.
 */

void	restore_term_settings(struct termios *original_attr)
{
	tcsetattr(STDIN_FILENO, TCSANOW, original_attr);
}

/*
void restore_terminal_mode()
{
    struct termios term_settings;
    tcgetattr(STDIN_FILENO, &term_settings);
    term_settings.c_lflag |= (ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &term_settings);
}
*/
/* Makes sure that when exiting the program the terminal attributes return to their 
 * original settings.
 */

/* NOTE:
 * Might have to use the termios.h functions to prevent the prompt from disappearing on
 * the command line. Keeps disappearing whenever you type one character then backspace 
 * or when you use Ctrl+L to clear the screen.
 */