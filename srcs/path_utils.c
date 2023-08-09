/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbrice <kbrice@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/09 09:06:08 by kbrice            #+#    #+#             */
/*   Updated: 2023/08/09 09:06:53 by kbrice           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	can_exec(char *path)
{
	struct stat	statbuf;

	if (stat(path, &statbuf) != 0)
		return (0);
	if (statbuf.st_mode & S_IXUSR)
		return (1);
	return (0);
}

char	*find_abs_path(char *cmd)
{
	if (cmd == NULL)
		return (NULL);
	if (cmd[0] == '/')
	{
		if (access(cmd, F_OK) == 0 && can_exec(cmd) && !is_directory(cmd))
			return (strdup(cmd));
		else
			return (NULL);
	}
	return (NULL);
}
/* Checks if the command is an absolute path and whether or not it's accessible.
 * If it is, it returns a copy of the path. Returns NULL if the command doesn't 
 * exist, if it isn't accessible, or if it's not an absolute path.
 */

int	str_isspace(char *str)
{
	int	i;

	i = -1;
	while (str[++i])
	{
		if (parse_type(str[i]) != P_SPACE)
			return (0);
	}
	return (1);
}

char	*add_cwd(char *path)
{
	char	cwd[1000];

	if (!path)
		return (NULL);
	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		path = ft_strjoinf(path, ":", 0);
		path = ft_strjoinf(path, cwd, 0);
	}
	return (path);
}
// ^^^ ITS RIGHT HERE????? Did I just delete the line that called it
