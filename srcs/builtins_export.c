#include "minishell.h"

static char **split_expvar(char *arg)
{
    char **temp = ft_split(arg, "=");
    return (temp);
}
/* Separates the key and the value of the given
 * export variable. 
 */

static bool valid_envvar(char *var)
{
    int i;

    i = 0;
    if (!*var || !var || !ft_isalpha(*var) && *var != '_')
        return (false);
    while (var[i] && var[i] != '=')
    {
        if (ft_isalnum(var[i] != '='))
            return (false);
        i++;
    }
    return (true);
}
// Checks the validity of the given export variable

void ft_export(char **argv, t_list **envvar_list)
{
    int     i;
    char    **temp;

    i = 1;
    if (!argv[i])
        return (ft_env(envvar_list));
    while (argv[i])
    {
        if (!valid_envvar(argv[i]))
        {
            ft_putstr_fd("export : not a valid env variable", STDERR_FILENO);
            return (EXIT_FAILURE);
        }
        else if (ft_strchr(argv[i], '=') != NULL)
        {
            temp = split_expvar(argv[i]);
            // set_env()
            free(temp);
        }
        i++;
    }
    return (EXIT_SUCCESS);
}
// WIP