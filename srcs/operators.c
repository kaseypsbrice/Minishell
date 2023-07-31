/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operators.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbrice <kbrice@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/19 08:53:34 by kbrice            #+#    #+#             */
/*   Updated: 2023/07/19 14:24:38 by kbrice           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// NOTE: These functions are all WIP

/* Attaches remaining arguments after redirect 
   'echo test > file.txt 123' outputs 'test 123' into file.txt
*/
char	**attach_args(char **dest, char **src)
{
	int		s_len;
	int		d_len;
	int		i;
	char	**temp;

	s_len = -1;
	d_len = -1;
	temp = dest;
	while (src[++s_len])
		;
	while (dest[++d_len])
		;
	dest = (char **)malloc((s_len + d_len + 1) * sizeof(char *));
	if (!dest)
	{
		perror("malloc failed");
		exit(1);
	}
	i = -1;
	while (++i < d_len)
		dest[i] = temp[i];
	i--;
	while (++i < d_len + s_len)
		dest[i] = src[i - d_len];
	dest[i] = NULL;
	free(temp);
	return (dest);
}