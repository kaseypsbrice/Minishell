/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbrice <kbrice@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 13:43:04 by bburston          #+#    #+#             */
/*   Updated: 2023/07/11 14:18:33 by kbrice           ###   ########.fr       */
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

/* Pipe Macros for Readability/Norm */
# define PIPE_RIGHT pipes_passed % 2
# define PIPE_LEFT (pipes_passed + 1) % 2
# define PIPE_READ 0
# define PIPE_WRITE 1

/* Minishell General Purpose Variables */
typedef struct s_mini
{
	char	**tokens; // ? 
	char	**command;
	char	*command_path;
	int		pipes[2][2];
} t_mini;

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
/* extern int	g_exit_status; */

/* Signals */
void		restore_prompt(int sig);
void		back_slash(int sig);
void		run_signals(int sig);

/* Termios Functions */
void		ft_suppress_output(void);

/* Environment Functions */
/* */

/* Main + Utils */
void		execute_command(char *command_path, char **command, int pipe_in, int pipe_out);
int			exec_builtins(/*t_cmd *cmd, */char **command);
void		print_prompt(void);
char		**get_input(char *input);
void		free_io(char *input, char **command);
char		*find_command_path(char *cmd);
void		remove_at(char *str, int *index);
void		rem_unclosed(char *input);
char		*get_token(char *input);

/* Pipes */
void		handle_pipes(t_mini *cmdline, char *input);

/* Operators */
char		***cmd_op_tab(char *input);

/* Expansions */
void		do_expansions(char **tokens);

/* Built-ins */
int			ft_cd(char *path);
int			ft_pwd(void);

/* Remakes */
int			ft_strcmp(const char *s1, const char *s2);
/* char	*ft_strtok(char *str, const char *delim); */

/* Debug */
void print_2d(char **arr);
void print_3d(char ***arr);

#endif