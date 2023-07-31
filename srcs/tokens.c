/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbrice <kbrice@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/19 09:17:57 by kbrice            #+#    #+#             */
/*   Updated: 2023/07/19 13:37:14 by kbrice           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*	This is easily the worst file in the project, I'm throwing around iterators again
	but look theres no arbitrary buffers its all ft_strdup :)	*/

/*	Returns the type of redirect for t_tok->type and
	advances the iterator	*/
int	get_redir_type(char *str, int *i)
{
	if (str[*i] == '>' && str[*i + 1] != '>')
	{
		(*i)++;
		return (R_OUTPUT);
	}
	if (str[*i] == '<' && str[*i + 1] != '<')
	{
		(*i)++;
		return (R_INPUT);
	}
	if (str[*i] == '>')
	{
		(*i) += 2;
		return (R_OUTPUT_A);
	}
	if (str[*i] == '<')
	{
		(*i) += 2;
		return (R_HEREDOC);
	}
}
/*	Wait apparently I'm not even using this	*/

/*	Removes outside quotes from token	*/
void	remove_quotes(char *str)
{
	int		i;
	char	last_quote;

	i = 0;
	last_quote = 0;
	while (str[i])
	{
		if (is_quote(str[i]))
		{
			if (!last_quote)
			{
				last_quote = str[i];
				remove_at(str, i);
				continue ;
			}
			if (str[i] == last_quote)
			{
				last_quote = 0;
				remove_at(str, i);
				continue ;
			}
		}
		i++;
	}
}

/*	Creates a new token of type at str[i]	*/
t_tok	*new_tok(char *str, int *i, int type)
{
	t_tok	*new;
	int		j;
	char	last_quote;

	new = (t_tok *)malloc(sizeof(t_tok));
	if (!new)
		perror_exit("t_tok malloc failed", 1);
	new->type = type;
	while (parse_type(str[(*i)]) != P_NORMAL)
		(*i)++;
	j = 0;
	last_quote = 0;
	while (str[(*i) + j])
	{
		check_quote(str, (*i) + j, 0, &last_quote);
		if (!last_quote && parse_type(str[(*i) + j]) != P_NORMAL)
			break ;
		j++;
	}
	new->str = ft_substr(str, (*i), j);
	if (!new)
		perror_exit("t_tok->str malloc failed", 1);
	new->str = do_expansions(new->str);
	remove_quotes(new->str);
	(*i) += j - 1;
	return (new);
}
/*	1. Step through operators and spaces to find the content of the token
	2. Update last_quote to reflect whether we are in a quoted portion
	3. If we aren't quoted and we hit a space or operator, break
	4. Copy out the content from the string using the length j
	5. Expand environment variables
	6. Remove the outside quotes
	7. Advance the iterator again to the end of the token content
	8. Bon appetit	*/

/*	Returns the type of token at str	*/
int	tok_type(char *str)
{
	if (*str == '|')
		return (COMMAND);
	if (parse_type(*str) == P_NORMAL)
		return (ARGUMENT);
	if (*str == '>' && *(str + 1) != '>')
		return (R_OUTPUT);
	if (*str == '<' && *(str + 1) != '<')
		return (R_INPUT);
	if (*str == '>')
		return (R_OUTPUT_A);
	return (R_HEREDOC);
}
/*	This is the one I'm using	*/

/*	Creates a list of tokens with a type and str from the input str	*/
t_list	*get_tokens(char *str)
{
	int		i;
	t_list	*toks;
	t_list	*cur;

	i = 0;
	toks = ft_lstnew(NULL);
	if (!toks)
		perror_exit("toks malloc failed", 1);
	cur = toks;
	cur->data = new_tok(str, &i, COMMAND);
	while (str[++i])
	{
		if (parse_type(str[i]) == P_SPACE)
			continue ;
		cur->next = ft_lstnew((void *)new_tok(str, &i, \
		tok_type(&(str[i]))));
		cur = cur->next;
	}
	return (toks);
}
/*	The first token is presumed to be a command and future commands
	are identified by a '|'
	A new list is created with data = t_tok * (which is typecasted to void *)
	Iterator is advanced mostly in new_tok but also here to skip over whitespace	*/