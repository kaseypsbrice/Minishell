#include "minishell.h"

void	restore_prompt(int sig)
{
	// g_ret_number = 130; // Unused according to compiler?
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	print_prompt();
	(void)sig;
}

void	ctrl_c(int sig)
{
	// g_ret_number = 130; // Unused according to compiler?
	write(1, "\n", 1);
	(void)sig;
}

/*
void	ctrl_l(int sig)
{
	rl_clear_history();
	rl_on_new_line();
	rl_redisplay();
	print_prompt();
	(void)sig;
}
Haven't figured out how to use signals properly yet.
Might have to use sigaction() instead of signal(). */

void	back_slash(int sig)
{
	(void)sig;
}

void	run_signals(int sig)
{
	if (sig == 1)
	{
		signal(SIGINT, restore_prompt);
		signal(SIGQUIT, SIG_IGN);
	}
	if (sig == 2)
	{
		signal(SIGINT, ctrl_c);
		signal(SIGQUIT, back_slash);
	}
	if (sig == 3)
	{
		printf("exit\n");
		exit(0);
	}
}