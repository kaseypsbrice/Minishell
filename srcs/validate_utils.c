# include "minishell.h"

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

