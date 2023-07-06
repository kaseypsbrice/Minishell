#include "minishell.h"

void	restore_prompt(int sig)
{
	// g_ret_number = 130; // Unused according to compiler
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	print_prompt();
	(void)sig;
}

void	run_signals(int sig)
{
	ft_suppress_output();
	if (sig == 1)
	{
		signal(SIGINT, restore_prompt);
		signal(SIGQUIT, SIG_IGN);
	}/*
	if (sig == 2)
	{
		//
	}*/
	if (sig == 3)
	{
		printf("exit\n");
		exit(0);
	}
}