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

/*	Removes outside quotes from token	*/
void	remove_quotes(char *str, size_t size)
{
	int		i;
	char	last_quote;

	i = -1;
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
		if (!last_quote && parse_type(str[(*i) + j]) == P_SPACE)
			break ;
		j++;
	}
	new->str = ft_substr(str, (*i), j - 1);
	if (!new)
		perror_exit("t_tok->str malloc failed", 1);
	remove_quotes(new->str, j);
	(*i) += j;
	return (new);
}

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

int	process_input(t_mini *cmdline, char *str)
{
	int		i;
	t_list	*toks;
	t_list	*cur;

	do_expansions(str);
	i = -1;
	toks = ft_lstnew(NULL);
	if (!toks)
		perror_exit("toks malloc failed", 1);
	cur = toks;
	cur->data = new_tok(str, i, COMMAND);
	while (str[++i])
	{
		if (parse_type(str[i]) == P_SPACE)
			continue ;
		cur->next = ft_lstnew((void *)new_tok(str, i, \
		tok_type(&(str[i]))));
		cur = cur->next;
	}
	return (0);
}