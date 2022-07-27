/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_find.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sotherys <sotherys@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/27 11:25:45 by sotherys          #+#    #+#             */
/*   Updated: 2022/07/27 11:30:00 by sotherys         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*lst_find(t_list *lst, void *data, void (*cmp)(void *, void *))
{
	if (!(lst && f))
		return ;
	while (lst)
	{
		if (cmp(lst->data, data) == 0)
			return (lst);
		lst = lst->next;
	}
	return (NULL);
}
