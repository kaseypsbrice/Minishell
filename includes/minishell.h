/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbrice <kbrice@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 13:43:04 by bburston          #+#    #+#             */
/*   Updated: 2023/08/07 14:15:05 by kbrice           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

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
# include <readline/history.h>
# include <fcntl.h>
# include "../libft/libft.h"
# include <curses.h>
# include <termios.h>
# include <termcap.h>

/* Exit Statuses */
# define ERROR 2
# define CMD_NOT_FOUND 127
# define CMD_INTERRUPTED 130
# define FORMAT_ERROR 258

/* Buffer */
# define BUFF 256

/* Pipe Macros for Readability/Norm */
# define PIPE_READ 0
# define PIPE_WRITE 1

/* Parse Macros */
# define P_NORMAL 0
# define P_OPERATOR 1
# define P_SPACE 2
# define P_DELETE 3

# define R_INPUT 0
# define R_OUTPUT 1
# define R_OUTPUT_A 2
# define R_HEREDOC 3
# define COMMAND 4
# define ARGUMENT 5

/* Holds a redirection for a command excluding pipes */
typedef struct s_redir
{
	int		type;
	char	*str;
}	t_redir;

typedef struct s_tok
{
	int		type;
	char	*str;
}	t_tok;


/* Hold a command, separating its arguments from its redirections */
typedef struct s_cmd
{
	t_list	*args;
	t_list	*redirs;
	char	*name;
	char	*path;
	char	**argv;
	int		fd_in;
	int		fd_out;
	int		builtin;
}	t_cmd;

/* Minishell General Purpose Variables */
typedef struct s_mini
{
	t_list	*toks;
	t_list	*cmds;
	int		cmd_io[2];
	int		pipes[2][2];
}	t_mini;
/* cmd_op stores the table of commands and operators
   cmd_io stores the input/output file descriptors (-1 for stdin/out) of the command being executed
   pipes stores two pipes which are leapfrogged down the command chain for piping
*/

/* Struct for Environment Variables */
typedef struct s_env
{
	// --------------
	char	*cur_envvar;
    char	*cur_key;
    char	*cur_value;
	// ---------------
	char	*key;
	char	*value;
}	t_env;

/* Built-ins */
int			ft_cd(char **argv);
int			ft_pwd(void);
int			ft_echo(char **command);
int			ft_env(t_list *envvar_list);
int			ft_export(char **args, t_list **envvar_list);
int			ft_unset(char **var_name, t_list **envvar_list);
int			ft_exit(char **argv);

/* Built-in Helper Functions */
char		**get_env_arr(t_list *envvar_list);
t_list		*store_envvars(char **envp);
void		free_envvar(t_env *envvar);
void		free_envvar_list(t_list *envvar_list);
void		set_envvar(t_list **envvar_list, char *key, char *value);

/* TERMIOS */
void		restore_term_settings(struct termios *original_attr);
void		ft_suppress_output(void);

/* Global Variable */
/* Keeps track of exit statuses and the reason for terminating the program. */
extern int	g_exit_status;

/* Signals */
void		restore_prompt(int sig);
void		back_slash(int sig);
void		run_signals(int sig);

/* Main + Utils */
int			execute_command(t_cmd *cmd, t_list *envvar_list);
int			is_builtin(char *command);
int			exec_builtins(t_cmd *cmd, t_list *envvar_list);
void		print_prompt(void);
int			is_directory(const char *path);
char		*ft_strjoinf(char *s1, char *s2, int which);
int			is_empty(char *str);

/* Path */
char		*find_abs_path(char *cmd);
char		*find_command_path(char *cmd, t_list *envvar_list);
char		*find_command_name(char *cmd);

/* Command + Utils */
t_mini		*new_cmdline(char *str, t_list *envvar_list);
char		**assemble_command(t_cmd *cmd);
int			has_redir(t_cmd *cmd, int redir);

/* Validate + Utils */
int			validate_input(char *str);
int			check_quote(char *str, int i, int unclosed, char *last_quote);

/* Parse Utils */
int 		is_quote(char c);
int			parse_type(char c);
void		remove_at(char *str, int index);
void		rem_unprocessed(char *input);

/* Tokens */
t_list		*get_tokens(char *str, t_list *envvar_list);
void		remove_quotes(char *str);

/* Process + Utils */
void		process(t_mini *cmdline, t_list *envvar_list);
int			update_pipes(t_mini *cmdline, int i);
int			init_pipes(t_mini *cmdline);

/* Heredoc */
int			heredoc(t_cmd *cmd, char *eof);

/* Expansions */
char		*do_expansions(char *str, t_list *envvar_list);
char		*ft_getenv(char *name, t_list *envvar_list);

/* Remakes */
int			ft_strcmp(const char *s1, const char *s2);

/* Clean */
void		del_cmdline(t_mini *cmdline);
void		del_envvar_arr(char **envvar_arr);

/* Debug */
void		print_2d(char **arr);
void		print_3d(char ***arr);
void		print_tokens(t_list *lst);
void		print_commands(t_list *cmds);

/* Error */
void		perror_exit(const char *msg, int status);
char		*invalid_syntax(char c);
int			display_errno(char *filepath);
int			command_not_found(char *name);
int			command_is_directory(char *name);

#endif