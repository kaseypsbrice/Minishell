/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbrice <kbrice@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/19 09:14:38 by kbrice            #+#    #+#             */
/*   Updated: 2023/07/19 09:15:58 by kbrice           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int is_quote(char c)
{
	if (c == '\'' || c == '\"' || c == '`')
		return (1);
	return (0);
}

// Returns an int indicating how the character should be parsed
int	parse_type(char c)
{
	if (c == '|' || c == '>' || c == '<')
		return (P_OPERATOR);
	if (c == ' ' || c == '	')
		return (P_SPACE);
	if (c == ';' || c == '\\')
		return (P_DELETE);
	return (P_NORMAL);
}

// Removes char at index and decrements index to account for ++i in parent
// This is dumb in hindsight I can just pass in i-- since it decrements after
void	remove_at(char *str, int index)
{
	int	len;
	int	i;

	len = ft_strlen(str);
	i = index - 1;
	while (++i < len)
		str[i] = str[i + 1];
}


