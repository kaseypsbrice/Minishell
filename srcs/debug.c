/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbrice <kbrice@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/19 08:33:59 by kbrice            #+#    #+#             */
/*   Updated: 2023/07/19 13:32:46 by kbrice           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Debug commands for printing str arrays and str array arrays (cmd_tab)
void	print_2d(char **arr)
{
	int	i;

	i = -1;
	while (arr[++i])
		printf("%s\n", arr[i]);
}

void	print_3d(char ***arr)
{
	int	i;
	int	j;

	printf("printing 3d\n");
	i = -1;
	while (arr[++i])
	{
		j = -1;
		while (arr[i][++j])
		{
			printf("%s ", arr[i][j]);
		}
		printf("\n");
	}
}