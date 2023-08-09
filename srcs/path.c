/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbrice <kbrice@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 10:55:28 by kbrice            #+#    #+#             */
/*   Updated: 2023/08/09 11:31:37 by kbrice           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*free_and_return(char **tmp, char *dir)
{
	int	i;

	i = 0;
	while (tmp[i])
	{
		free(tmp[i]);
		i++;
	}
	free(tmp);
	return (dir);
}

char	**get_path(t_list *envvar_list)
{
	char	**path;
	char	*raw_path;
	char	buf[1000];

	raw_path = ft_getenv("PATH", envvar_list);
	if (!raw_path)
		return (NULL);
	raw_path = ft_strjoin(raw_path, ":");
	raw_path = ft_strjoinf(raw_path, getcwd(buf, sizeof(buf)), 0);
	path = ft_split(raw_path, ':');
	free(raw_path);
	return (path);
}

char	*find_command_path(char *cmd, t_list *envvar_list)
{
	char	*absolute_path;
	char	*env_path;
	char	**path;
	char	*dir;
	char	**tmp;

	if (str_isspace(cmd) || !ft_getenv("PATH", envvar_list))
		return (NULL);
	absolute_path = find_abs_path(cmd);
	if (absolute_path != NULL)
		return (absolute_path);
	env_path = ft_getenv("PATH", envvar_list);
	path = get_path(envvar_list);
	if (!path)
		return (NULL);
	tmp = path;
	while (*path)
	{
		dir = ft_strjoin(*path, "/");
		dir = ft_strjoinf(dir, cmd, 0);
		if ((access(dir, F_OK) == 0) && can_exec(dir) && !is_directory(dir))
			return (free_and_return(tmp, dir));
		free(dir);
		path++;
	}
	free(tmp);
	return (NULL);
}
/* The PATH environment variable contains a list of directories that can be 
 * searched to find a command. The function goes through each directoy listed
 * in the PATH variable and appends '/' plus the name of the command. If the
 * file exists and is accessible using that path, it returns the path. If the
 * file doesn't exist or is not accessible, it continues moving through the
 * list of directories, testing each one to check for the command. If the
 * command isn't found in any of the directories then the function
 * returns NULL.
 */

/*	Gets the command name for argv[0] in execve	*/
char	*find_command_name(char *cmd)
{
	int		i;
	int		len;
	char	*name;

	len = ft_strlen(cmd);
	if (len == 0)
		return (ft_strdup(""));
	i = len;
	while (i >= 0 && cmd[i] != '/')
		i--;
	i++;
	name = ft_substr(cmd, i, len - i);
	if (!name)
		perror_exit("cmd name malloc failed", 1);
	return (name);
}
/*	Iterates from the back of the string until '/' or start of string
	is reached and returns everything to the right	*/
