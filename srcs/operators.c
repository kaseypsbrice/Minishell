#include "minishell.h"
void	cmd_op_tab_iter(char ***tab, char **raw_tab)
{
	int	i;
	int	j;
	int	k;

	i = 0;
	j = 0;
	k = 0;
	tab[j] = (char **)malloc(1024 * sizeof(char*)); // arbitrary buffer size
	while (raw_tab[i])
	{
		if (ft_strcmp(raw_tab[i], "|") == 0)
		{
			tab[j][k] = NULL;
			tab[j + 1] = (char **)malloc(2 * sizeof(char*));
			tab[j + 1][0] = ft_strdup(raw_tab[i]);
			tab[j + 1][1] = NULL;
			j += 2;
			tab[j] = (char **)malloc(1024 * sizeof(char*)); // arbitrary buffer size
			k = 0;
		}
		else {
			tab[j][k] = ft_strdup(raw_tab[i]);
			k++;
		}
		i++;
	}
	tab[j][k] = NULL;
	tab[j + 1] = NULL;
}

char	***cmd_op_tab(char *input)
{
	char	***tab;
	char	**raw_tab;

	tab = (char ***)malloc(1024 * sizeof(char**));// arbitrary buffer size
	raw_tab = get_input(input); //tokenization
	do_expansions(raw_tab); //quotes and expansions
	cmd_op_tab_iter(tab, raw_tab);
	return (tab);
}