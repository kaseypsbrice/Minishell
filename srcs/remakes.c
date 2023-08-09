/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remakes.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbrice <kbrice@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/19 09:22:21 by kbrice            #+#    #+#             */
/*   Updated: 2023/08/09 09:14:23 by kbrice           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && (*s1 == *s2))
	{
		s1++;
		s2++;
	}
	return (*(unsigned char *)s1 - *(unsigned char *)s2);
}

int	char_index(char *s, char c)
{
	int	i;

	i = ft_strlen(s) - 1;
	while (i >= 0)
	{
		if (s[i] == c)
			return (i);
		i--;
	}
	return (-1);
}

char	**ft_splitone(char const *s, char c)
{
	char	**res;
	int		c_ind;

	res = (char **)malloc(2 * sizeof(char *));
	if (!res)
		return (NULL);
	c_ind = 0;
	while (s[c_ind] && s[c_ind] != c)
		c_ind++;
	res[0] = ft_substr(s, 0, c_ind);
	res[1] = ft_substr(s, c_ind + 1, ft_strlen(s) - c_ind - 1);
	return (res);
}
