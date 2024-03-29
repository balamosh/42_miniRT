/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_find.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sotherys <sotherys@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/27 11:25:45 by sotherys          #+#    #+#             */
/*   Updated: 2022/07/29 09:42:10 by sotherys         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*lst_find(t_list *lst, void *data, int (*cmp)(void *, void *))
{
	if (!(lst && cmp))
		return (NULL);
	while (lst)
	{
		if (cmp(lst->data, data) == 0)
			return (lst);
		lst = lst->next;
	}
	return (NULL);
}
