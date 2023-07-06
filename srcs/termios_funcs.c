#include "minishell.h"

void	ft_suppress_output(void)
{
    struct termios    new_settings;

    if (tcgetattr(0, &new_settings))
        perror("minishell: tcsetattr");
    new_settings.c_lflag &= ~ECHOCTL;
    if (tcsetattr(0, 0, &new_settings))
        perror("minishell: tcsetattr");
}
/* Modifies the terminal settings to clear the ECHOCTL bit flag.
 * It supresses ^C from appearing on the command line when trying to display
 * a new prompt. To be honest, I didn't need to do this since some shells show it,
 * but I wanted it to be like the shell we use at 42 :) 
 */

void	restore_term_settings(struct termios *original_attr)
{
	tcsetattr(STDIN_FILENO, TCSANOW, original_attr);
}
/* Makes sure that when exiting the program the terminal attributes return to their 
 * original settings.
 */