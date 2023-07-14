#include "minishell.h"

/* Note that cmd_op_tab, get_token_tab and get_token all march along the same input string.
   This means that they share the same iterator. i in cmd_op_tab = *idx in the others.
   Where you see (*idx)-- is to account for the incrementation in the parent function after returning.
   It's very messy and confusing but ""required""" to get (close) to norm compliance.

   Most buffers are arbitrary because calculating their size would double the functions/complexity.
   Only the strings are dynamically sized to the upper bound (remaining length of input string).
*/

// Copies an operator out of a string eg. '|' or '>>' and increments idx by the length of the operator.
char	*copy_operator(char *str, int *idx, char *token)
{
	int		i;
	char	op;

	i = -1;
	op = str[*idx];
	while (str[*idx + ++i] == op)
		token[i] = op;
	token[i] = 0;
	if (i > 2)
		unexpected_token(op);
	if (i == 2 && !(op == '>' || op == '<'))
		unexpected_token(op);
	(*idx) += i;
	return (token);
}

/* Extracts a non-operator token from a string using delimiters described in parse_type.
   Quoted delimiters are ignored.
*/
char	*get_token(char *str, int *idx)
{
	char	*token;
	char	last_quote;
	int		i;

	last_quote = 0;
	token = (char *)malloc((ft_strlen(&(str[*idx])) + 1) * sizeof(char));
	i = -1;
	(*idx)--;
	while (str[++(*idx)])
	{
		if (!last_quote && parse_type(str[*idx]) != P_NORMAL)
			break ;
		if (str[*idx] == '\'' || str[*idx] == '\"')
		{
			if (!last_quote)
			{
				last_quote = str[*idx];
				remove_at(str, idx);
				continue ;
			}
			else if (str[*idx] == last_quote)
			{
				last_quote = 0;
				remove_at(str, idx);
				continue ;
			}
		}
		token[++i] = str[*idx];
	}
	token[i + 1] = 0;
	(*idx)--;
	return (token);
}
/* The remove_at calls remove the active quotes from the token.
   Eg. (echo "'google.com'") is passed to execve later as (echo 'google.com').
*/

/* Returns an array of tokens (like is passed into execve).
   Excludes operators.
*/
char	**get_token_tab(char *str, int *idx)
{
	int		i;
	char	**tab;

	i = -1;
	tab = (char **)malloc(128 * sizeof(char *));
	(*idx)--;
	while (str[++(*idx)])
	{
		if (parse_type(str[*idx]) == P_SPACE)
			continue ;
		if (parse_type(str[*idx]) == P_OPERATOR)
			break ;
		tab[++i] = get_token(str, idx);
	}
	tab[i + 1] = 0;
	(*idx)--;
	return (tab);
}
/* This function essentially just moves the index along the string to the start of the next
   token which is then extracted with get_token until an operator is reached.
*/

/* Returns a table in form: cmd|operator|cmd|operator where cmd and operator are string tables.
   Operators are always in form ["operator"][NULL] eg. [">>"][NULL].
*/
char	***cmd_op_tab(char *input)
{
	int		i;
	int		j;
	char	***cmd_op;

	i = -1;
	j = -1;
	cmd_op = (char ***)malloc(1024 * sizeof(char **));
	rem_unprocessed(input);
	input = do_expansions(input);
	while (input[++i])
	{
		if (parse_type(input[i]) == P_SPACE)
			continue ;
		if (parse_type(input[i]) == P_OPERATOR)
		{
			cmd_op[++j] = (char **)malloc(2 * sizeof(char *));
			cmd_op[j][0] = (char *)malloc(3 * sizeof(char));
			copy_operator(input, &i, cmd_op[j][0]);
			cmd_op[j][1] = 0;
			i--;
		}
		else
			cmd_op[++j] = get_token_tab(input, &i);
	}
	cmd_op[j + 1] = 0;
	return (cmd_op);
}
/* 
   Removal of unprocessed characters such as ; and unclosed quotes is done in rem_unprocessed.
   Environment variables expansions are done in do_expansions.
   Operators are extracted directly here using copy_operator.
   The token functions only extract non-operator tokens aka commands.

   Potential issue where passing in '' returns a null entry cmd_op[0][0] = 0.
   Hopefully shouldn't cause any problems.
*/