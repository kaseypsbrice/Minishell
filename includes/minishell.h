/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbrice <kbrice@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 13:43:04 by bburston          #+#    #+#             */
/*   Updated: 2023/07/05 16:45:42 by kbrice           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL
# define MINISHELL

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <signal.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <errno.h>
# include <readline/readline.h>
# include <fcntl.h>
# include "../libft/libft.h"
# include <curses.h>
# include <termios.h>

/* Exit Statuses */
# define ERROR 2
# define CMD_NOT_FOUND 127
# define CMD_INTERRUPTED 130
# define FORMAT_ERROR 258

/* Buffer */
# define BUFF 256

/* Minishell General Purpose Variables */
typedef struct s_mini
{
	char	**tokens; // ? 
	char	**command;
	char	*command_path;
} t_mini;

/* Pipe function variables */
typedef struct s_pipe
{
	char **cmd1;
    char **cmd2;
    char *cmd1_path;
    char *cmd2_path;
} t_pipe;

/* Struct for Environment Variables */
typedef struct s_env
{
	char	**env;
	int		len;
	char	**key;
	char	**content;
	int		index;
} t_env;

/* Global Variable */
/* Keeps track of exit statuses and the reason for terminating the program. */
// extern int	g_exit_status;

/* Signals */
void		restore_prompt(int sig);
void		ctrl_l(int sig);
void		ctrl_c(int sig);
void		back_slash(int sig);
void		run_signals(int sig);

/* Environment Functions */
void	ft_envvar();

/* Main + Utils */
void		execute_command(char *command_path, char **command, char *temp, int pipe_in, int pipe_out);
int			exec_builtins(/*t_cmd *cmd, */char **command);
void		print_prompt(void);
char		**get_input(char *input);
void		free_io(char *input, char **command);
char		*find_command(char *cmd);

/* Pipes */
void		handle_pipes(t_mini cmdline, char *input);

/* Built-ins */
int			ft_cd(char *path);
int			ft_pwd(void);

/* Remakes */
int			ft_strcmp(const char *s1, const char *s2);
// char	*ft_strtok(char *str, const char *delim);

#endif