/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbrice <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/01 07:53:18 by kbrice            #+#    #+#             */
/*   Updated: 2023/01/31 11:34:22 by kbrice           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

void	ft_putchar_len(char c, int *len)
{
	write(1, &c, 1);
	(*len)++;
}

void	ft_printf_putstr(char *args, int *len)
{
	size_t	i;

	i = 0;
	if (!args)
	{
		write(1, "(null)", 6);
		(*len) += 6;
		return ;
	}
	while (args[i] != '\0')
	{
		ft_putchar_len(args[i], len);
		i++;
	}
}

void	ft_printf_putnbr(int n, int *len)
{
	if (n == -2147483648)
	{
		write(1, "-2147483648", 11);
		(*len) += 11;
		return ;
	}
	if (n < 0)
	{
		ft_putchar_len('-', len);
		ft_printf_putnbr(n * -1, len);
	}
	else
	{
		if (n > 9)
			ft_printf_putnbr(n / 10, len);
		ft_putchar_len(n % 10 + '0', len);
	}
}
