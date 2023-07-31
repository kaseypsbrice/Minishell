#include "minishell.h"

/*	Removes unprocessed characters such as unclosed quotes, ';' and '\'.	*/
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
			remove_at(input, i--);
			continue ;
		}
		unclosed = check_quote(input, i, unclosed, &last_quote);
	}
	if (last_quote)
	{
		remove_at(input, unclosed);
		rem_unprocessed(input);
	}
}

/*	Checks if an operator is valid,
	"><, ||, |>, <<<" are invalid	*/
int	invalid_operator(char *str, int i)
{
	if (parse_type(str[i]) != P_OPERATOR)
		return (0);
	if (str[i] == '|' && parse_type(str[i + 1]) == P_OPERATOR)
	{
		invalid_syntax(str[i + 1]);
		return (1);
	}
	if (parse_type(str[i + 1]) == P_OPERATOR && str[i] != str[i + 1])
	{
		invalid_syntax(str[i + 1]);
		return (1);
	}
	if (str[i] == str[i + 1] && str[i + 2] && \
	parse_type(str[i + 2]) == P_OPERATOR)
	{
		invalid_syntax(str[i + 2]);
		return (1);
	}
	return (0);
}

/*	Checks if an operator is followed by a non-operator (command)	*/
int	invalid_order(char *str, int i, int op_last)
{
	if (op_last || parse_type(str[i]) != P_OPERATOR)
		return (0);
	if (str[i] == str[i + 1])
		i++;
	i++;
	while (str[i] && parse_type(str[i]) == P_SPACE)
		i++;
	if (!str[i])
	{
		invalid_syntax('\n');
		return (1);
	}
	if (parse_type(str[i]) == P_OPERATOR)
	{
		invalid_syntax(str[i]);
		return (1);
	}
	return (0);
}

/*	Returns str if operators are valid and seperated by non-operators else NULL	*/
char	*cmd_order(char *str)
{
	int		i;
	int		op_last;
	char	last_quote;

	i = -1;
	last_quote = 0;
	op_last = 0;
	while (str[++i])
	{
		if (parse_type(str[i]) == P_SPACE)
			continue ;
		check_quote(str, i, 0, &last_quote);
		if (!last_quote)
		{
			if (invalid_operator(str, i) || invalid_order(str, i, op_last))
				return (NULL);
			if (parse_type(str[i]) == P_OPERATOR)
				op_last = 1;
			else
				op_last = 0;
		}
	}
	if (op_last)
		return (invalid_syntax('\n'));
	return (str);
}

/*	Returns 0 if the input string contains all valid syntax
	and removes unprocessed chars such as '\' and ';'	*/
int	validate_input(char *str)
{
	int	i;
	int	last;

	i = 0;
	last = P_NORMAL;
	rem_unprocessed(str);
	while (str[i] && parse_type(str[i]) == P_SPACE)
		i++;
	if (!str[i])
		return (0);
	if (parse_type(str[i]) != P_NORMAL)
	{
		invalid_syntax('\n');
		return (1);
	}
	if (!cmd_order(str))
		return (1);
	return (0);
}