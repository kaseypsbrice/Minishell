#include "minishell.h"

void print_2d(char **arr)
{
	int i;

	i = -1;
	while (arr[++i])
		printf("%s\n", arr[i]);
}

void print_3d(char ***arr)
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