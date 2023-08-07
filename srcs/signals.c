/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbrice <kbrice@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/19 08:41:55 by kbrice            #+#    #+#             */
/*   Updated: 2023/08/07 11:19:28 by kbrice           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	restore_prompt(int sig)
{
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	print_prompt();
	(void)sig;
}
/* Clears the input line and moves the cursor to a new line before
 * redisplaying the prompt. 
 * 
 * Exit status 130 is returned when the script is terminated by Ctrl+C.
 * May have to use a global variable to return exit statuses later
 * on when coding the expected output of "$?". Either that or by using
 * a struct variable, not sure.
 */

void	run_signals(int sig)
{
	ft_suppress_output();
	if (sig == 1)
	{
		signal(SIGINT, restore_prompt);
		signal(SIGQUIT, SIG_IGN);
	}
}
/* Calls ft_suppress_output() to silence ^C or anything else from appearing 
 * on the command line. 
 * SIGINT == Ctrl+C 
 * SIGQUIT == Ctrl+\ 
 * SIG_IGN will prevent anything from happening so it'll do nothing. 
 * restore_prompt will create a new input line and redisplay the prompt.
 */