/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbrice <kbrice@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 10:55:17 by kbrice            #+#    #+#             */
/*   Updated: 2023/08/07 11:20:06 by kbrice           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Updates the last found active quote (separated from loop for norm)
int	check_quote(char *str, int i, int unclosed, char *last_quote)
{
	if (is_quote(str[i]))
	{
		if (!(*last_quote))
		{
			(*last_quote) = str[i];
			unclosed = i;
		}
		else if (str[i] == (*last_quote))
			(*last_quote) = 0;
	}
	return (unclosed);
}
