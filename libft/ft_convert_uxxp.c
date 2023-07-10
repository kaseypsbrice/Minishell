/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_convert_uxxp.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbrice <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/01 10:32:15 by kbrice            #+#    #+#             */
/*   Updated: 2023/01/31 11:47:21 by kbrice           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

void	ft_p(size_t	p, int *len)
{
	char	str[25];
	int		i;
	char	*base_char;

	base_char = "0123456789abcdef";
	i = 0;
	write(1, "0x", 2);
	(*len) += 2;
	if (p == 0)
	{
		ft_putchar_len('0', len);
		return ;
	}
	while (p != 0)
	{
		str[i] = base_char[p % 16];
		p = p / 16;
		i++;
	}
	while (i--)
	{
		ft_putchar_len(str[i], len);
	}
}

void	ft_xx(unsigned int x, int *len, char x_or_x)
{
	char	str[25];
	char	*base_char;
	int		i;

	if (x_or_x == 'X')
		base_char = "0123456789ABCDEF";
	else
		base_char = "0123456789abcdef";
	i = 0;
	if (x == 0)
	{
		ft_putchar_len('0', len);
		return ;
	}
	while (x != 0)
	{
		str[i] = base_char [x % 16];
		x = x / 16;
		i++;
	}
	while (i--)
		ft_putchar_len(str[i], len);
}

void	ft_u(unsigned int u, int *len)
{
	if (u >= 10)
		ft_u(u / 10, len);
	ft_putchar_len(u % 10 + '0', len);
}
