/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remakes.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbrice <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/19 09:22:21 by kbrice            #+#    #+#             */
/*   Updated: 2023/07/19 09:22:24 by kbrice           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int ft_strcmp(const char *s1, const char *s2)
{
    while (*s1 && (*s1 == *s2))
    {
        s1++;
        s2++;
    }
    return *(unsigned char *)s1 - *(unsigned char *)s2;
}
/*
char	*ft_strtok(char *str, const char *delim)
{
    
}
*/
