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

int	str_isspace(char *str)
{
	int i;

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

int	can_exec(char *path)
{
	struct stat statbuf;

	if (stat(path, &statbuf) != 0)
		return (0);
	if (statbuf.st_mode & S_IXUSR)
		return (1);
	return (0);
}

char	*find_command_path(char *cmd)
{
	char	*absolute_path;
	char	*env_path;
	char	*path;
	char	*dir;
	char	*tmp;

	if (str_isspace(cmd))
		return (NULL);
	absolute_path = find_abs_path(cmd);
	if (absolute_path != NULL)
		return (absolute_path);
	env_path = getenv("PATH");
	path = strtok(ft_strdup(env_path), ":");
	tmp = path;
	while (path)
	{
		dir = ft_strjoin(path, "/");
		dir = ft_strjoinf(dir, cmd, 0);
		if ((access(dir, F_OK) == 0) && can_exec(dir))
		{
			free(tmp);
			return (dir);
		}
		free(dir);
		path = strtok(NULL, ":");
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