/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbrice <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/10 15:32:16 by kbrice            #+#    #+#             */
/*   Updated: 2023/08/10 15:32:19 by kbrice           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	del_env(void *ptr)
{
	t_env	*env;

	env = (t_env *)ptr;
	free(env->cur_envvar);
	free(env->cur_key);
	free(env->cur_value);
	free(ptr);
}
