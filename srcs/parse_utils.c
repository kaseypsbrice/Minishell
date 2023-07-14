#include "minishell.h"

// Returns an int indicating how the character should be parsed
int	parse_type(char c)
{
	if (c == '|' || c == '>' || c == '<')
		return (P_OPERATOR);
	if (c == ' ')
		return(P_SPACE);
	if (c == ';' || c == '\\')
		return(P_DELETE);
	return (P_NORMAL);
}

// Removes char at index and decrements index to account for ++i in parent
// This is dumb in hindsight I can just pass in i-- since it decrements after
void	remove_at(char *str, int *index)
{
	int	len;
	int	i;

	len = ft_strlen(str);
	i = *index - 1;
	while (++i < len)
		str[i] = str[i + 1];
	(*index)--;
}

// Removes unprocessed characters such as unclosed quotes, ';' and '\'.
void	rem_unprocessed(char *input)
{
	int		unclosed;
	char	last_quote;
	int		i;

	last_quote = 0;
	i = -1;
	while (input[++i])
	{
		if (!last_quote && parse_type(input[i]) == P_DELETE)
		{
			remove_at(input, &i);
			continue ;
		}
		if (input[i] == '\'' || input[i] == '\"')
		{
			if (!last_quote)
			{
				last_quote = input[i];
				unclosed = i;
			}
			else if (input[i] == last_quote)
				last_quote = 0;
		}
	}
	if (last_quote)
	{
		remove_at(input, &unclosed);
		rem_unprocessed(input); //in case of double unclosed ('")
	}
}