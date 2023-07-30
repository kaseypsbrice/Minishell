#include "minishell.h"

char	*find_abs_path(char *cmd)
{
	if (cmd == NULL)
		return (NULL);
	if (cmd[0] == '/')
	{
		if (access(cmd, F_OK) == 0)
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

char	*find_command_path(char *cmd)
{
	char	*absolute_path;
	char	*env_path;
	char	*path;
	char	*dir;

	printf("find command path: %s\n", cmd);
	absolute_path = find_abs_path(cmd);
	if (absolute_path != NULL)
		return (absolute_path);
	env_path = getenv("PATH");
	path = strtok(strdup(env_path), ":");
	while (path)
	{
		dir = ft_strjoin(path, "/");
		dir = ft_strjoin(dir, cmd);
		if ((access(dir, F_OK) == 0) || (access(dir, F_OK) == 0))
			return (dir);
		free(dir);
		path = strtok(NULL, ":");
	}
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

char	*find_command_name(char *cmd)
{
	int		i;
	int		len;
	char	*name;

	printf("find command name: %s\n", cmd);
	len = ft_strlen(cmd);
	printf("len %d\n", len);
	if (len == 0)
		return (cmd);
	i = len;
	while (i >= 0 && cmd[i] != '/')
		i--;
	i++;
	printf("i: %d\n", i);
	name = ft_substr(cmd, i, len - i);
	if (!name)
		perror_exit("cmd name malloc failed", 1);
	return (name);
}