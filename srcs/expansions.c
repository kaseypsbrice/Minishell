#include "minishell.h"

// Takes a str and index of a '$' eg: {dog$HOME'cat'} and returns {dog/home/alex'cat'}
char	*expand_env(char *str, int *idx)
{
	int		len;
	char	*name;
	char	*var;
	char	*new;

	len = 0;
	while (str[*idx + (++len) + 1] && ft_isalnum(str[*idx + len + 1]));
	name = (char *)malloc((len + 1) * sizeof(char));
	if (!name)
	{
		perror("malloc failed");
		exit(1);
	}
	ft_strlcpy(name, (str + *idx + 1), len + 1);
	var = getenv(name);
	if (!var)
		var = "";
	new = (char *)malloc((ft_strlen(str) + ft_strlen(var) - len) * sizeof(char));
	if (!new)
	{
		perror("malloc failed");
		exit(1);
	}
	ft_strlcpy(new, str, *idx + 1);
	ft_strlcat(new, var, ft_strlen(var) + ft_strlen(str));
	ft_strlcat(new, (str + *idx + len + 1), ft_strlen(var) + ft_strlen(str) + 1);
	free(str);
	free(name);
	*idx += ft_strlen(var) - 1;
	return (new);
}
/*As messy as all my functions are sorry :P
  len = length of the environment variable name $HOME = 4.
  name is copied out of the string and passed into getenv to get "/home/alex".
  It is then frankensteined into the new string and returned.
  The index is advanced so the do_expansions loop skips to the end: dog/home/alex(HERE)'cat'.
*/

/* Expands environment variables in the input string.
*/
char	*do_expansions(char *str)
{
	char	*res;
	char	last_quote;
	int		i;

	res = ft_strdup(str);
	last_quote = 0;
	i = 0;
	while (res[i])
	{
		if (res[i] == '\'' || res[i] == '\"')
		{
			if (!last_quote)
				last_quote = res[i];
			else if (res[i] == last_quote)
				last_quote = 0;
		}
		else if (res[i] == '$' && last_quote != '\'' && ft_isalnum(str[i + 1]))
			res = expand_env(res, &i);
		i++;
	}
	return (res);
}
/* Vars inside '' don't expand but vars inside "" do.
   Environment variables are terminated by non-alpanumeric chars. <- WIP solution
   last_quote tracks the last "active" quote ', " or null and expands if its not '
   Eg. ("'$HOME") the active quote when at '$' is the double quote not the single so it expands.
*/