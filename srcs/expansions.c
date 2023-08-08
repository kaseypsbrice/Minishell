/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbrice <kbrice@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/19 09:22:53 by kbrice            #+#    #+#             */
/*   Updated: 2023/08/07 10:54:47 by kbrice           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_getenv(char *name, t_list *envvar_list)
{
	t_list	*cur;
	t_env	*cur_env;

	cur = envvar_list;
	while (cur)
	{
		cur_env = (t_env *)cur->data;
		if (!ft_strcmp(name, cur_env->cur_key))
			return (cur_env->cur_value);
		cur = cur->next;
	}
	return (NULL);
}

/*	Takes a str and index of a '$' eg: {dog$HOME'cat'}
	and returns {dog'/'home'/'alex'cat'} 
	pass 0 into len for norm	*/
char	*_expand_env(char *str, int *idx, int len, t_list *envvar_list)
{
	char	*name;
	char	*var;
	char	*new;

	while (str[*idx + (++len) + 1] && ft_isalnum(str[*idx + len + 1]))
		;
	name = (char *)malloc((len + 1) * sizeof(char));
	if (!name)
		perror_exit("env malloc failed", 1);
	ft_strlcpy(name, (str + *idx + 1), len + 1);
	var = ft_getenv(name, envvar_list);
	if (!var)
		var = "";
	new = malloc((ft_strlen(str) + ft_strlen(var) - len) * sizeof(char));
	if (!new)
		perror_exit("env malloc failed", 1);
	ft_strlcpy(new, str, *idx + 1);
	ft_strlcat(new, var, ft_strlen(var) + ft_strlen(str));
	ft_strlcat(new, (str + *idx + len + 1), ft_strlen(var) \
	+ ft_strlen(str) + 1);
	free(str);
	free(name);
	*idx += ft_strlen(var) - 1;
	return (new);
}
/*	As messy as all my functions are sorry :P
	len = length of the environment variable name $HOME = 4.
	name is copied out of the string and passed into
	getenv to get "/home/alex". It is then frankensteined 
	into the new string and returned. The index is advanced 
	so the do_expansions loop skips to the end: dog/home/alex(HERE)'cat'.
*/

char	*expand_env(char *str, int *idx, int len, t_list *envvar_list)
{
	char	*var;
	char	*new;

	if (!ft_isalnum(str[*idx + 1]) && str[*idx + 1] != '?')
		return (str);
	if (str[*idx + 1] == '?')
	{
		var = ft_itoa(g_exit_status);
		if (!var)
			perror_exit("env var malloc failed", 1);
		new = malloc((ft_strlen(str) + ft_strlen(var) - len) * sizeof(char));
		if (!new)
			perror_exit("env malloc failed", 1);
		ft_strlcpy(new, str, *idx + 1);
		ft_strlcat(new, var, ft_strlen(var) + ft_strlen(str));
		ft_strlcat(new, (str + *idx + 2), ft_strlen(var) \
		+ ft_strlen(str) + 1);
		free(str);
		*idx += ft_strlen(var) - 1;
		free(var);
		return (new);
	}
	return (_expand_env(str, idx, len, envvar_list));
}

/* Expands environment variables in the input string.
*/
char	*do_expansions(char *str, t_list *envvar_list)
{
	char	*res;
	char	last_quote;
	int		i;

	res = ft_strdup(str);
	if (!res)
		perror_exit("exp res malloc failed", 1);
	last_quote = 0;
	i = 0;
	while (res[i])
	{
		if (is_quote(res[i]))
		{
			if (!last_quote)
				last_quote = res[i];
			else if (res[i] == last_quote)
				last_quote = 0;
		}
		else if (res[i] == '$' && last_quote != '\'')
			res = expand_env(res, &i, 0, envvar_list);
		i++;
	}
	free (str);
	return (res);
}
/* Vars inside '' don't expand but vars inside "" do.
   Environment variables are terminated 
   by non-alpanumeric chars. <- WIP solution
   last_quote tracks the last "active" quote ', " or 
   null and expands if its not ' Eg. ("'$HOME") the active quote when at 
   '$' is the double quote not the single so it expands.
*/
