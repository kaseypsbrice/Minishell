/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ade-geor <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/15 11:13:07 by ade-geor          #+#    #+#             */
/*   Updated: 2021/09/15 11:13:08 by ade-geor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new;
	t_list	*new_begin;

	if (!lst || !f)
		return (NULL);
	new_begin = ft_lstnew((*f)(lst->data));
	if (!new_begin)
	{
		ft_lstclear(&lst, del);
		return (NULL);
	}
	lst = lst->next;
	while (lst)
	{
		new = ft_lstnew((*f)(lst->data));
		if (!new)
		{
			ft_lstclear(&lst, del);
			ft_lstclear(&new_begin, del);
			break ;
		}
		lst = lst->next;
		ft_lstadd_back(&new_begin, new);
	}
	return (new_begin);
}
