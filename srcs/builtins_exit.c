#include "minishell.h"

static bool	ft_isdigit_str(char *str)
{
	if (str == NULL)
		return (0);
	while (*str != '\0')
	{
		if (*str < '0' || *str > '9')
			return (false);
		str++;
	}
	return (true);
}

static size_t	count_args(char **arr)
{
	size_t	count;

	count = 0;
	while (arr[count])
		count++;
	return (count);
}

int	ft_exit(char **argv)
{
	int	argc;
	int	status;

	argc = count_args(argv);
	if ((argv[1] && ft_isdigit_str(argv[1]) == true))
	{
		status = ft_atoi(argv[1]);
		if (status >= 0 && status <= 255)
		{
			exit(status);
		}
	}
	else if (argc == 1)
	{
		status = g_exit_status;
		exit(status);
	}
	return (EXIT_FAILURE);
}
// printf("Exiting with status: %d\n", status); // Debugging line
