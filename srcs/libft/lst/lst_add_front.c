/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_add_front.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sotherys <sotherys@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/27 11:21:03 by sotherys          #+#    #+#             */
/*   Updated: 2022/07/27 11:21:51 by sotherys         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	lst_add_front(t_list **lst, t_list *new)
{
	if (!(lst && new))
		return ;
	new->next = *lst;
	*lst = new;
}
