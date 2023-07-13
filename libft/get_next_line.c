/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbrice <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 11:52:12 by kbrice            #+#    #+#             */
/*   Updated: 2023/01/31 11:53:22 by kbrice           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

static char	*new_line(char *line)

{
	char	*res;
	size_t	i;

	i = 0;
	while (line[i] != '\0' && line[i] != '\n')
		i++;
	if (line[i] == '\0')
		return (NULL);
	res = ft_gnl_substr(line, i + 1, ft_gnl_strlen(line) - i);
	if (*res == '\0')
	{
		free(res);
		res = NULL;
	}
	line[i + 1] = '\0';
	return (res);
}

static char	*find_new_line(int fd, char *buff, char *backup)

{
	int		bytes_read;
	char	*temp;

	bytes_read = 1;
	while (bytes_read)
	{
		bytes_read = read(fd, buff, BUFFER_SIZE);
		if (bytes_read == -1)
			return (NULL);
		else if (bytes_read == 0)
			break ;
		buff[bytes_read] = '\0';
		if (!backup)
			backup = ft_gnl_strdup();
		temp = backup;
		backup = ft_gnl_strjoin(temp, buff);
		free(temp);
		if (ft_gnl_strchr(buff, '\n'))
			break ;
	}
	return (backup);
}

char	*get_next_line(int fd)

{
	char		*line;
	char		*buff;
	static char	*backup;

	if ((fd < 0) || (BUFFER_SIZE <= 0))
		return (NULL);
	buff = malloc(sizeof(char) * BUFFER_SIZE + 1);
	if (!buff)
		return (NULL);
	line = find_new_line(fd, buff, backup);
	free(buff);
	if (!line)
		return (line);
	backup = new_line(line);
	return (line);
}
