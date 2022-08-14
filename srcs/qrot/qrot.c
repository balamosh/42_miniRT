/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   qrot.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sotherys <sotherys@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/08 12:18:56 by sotherys          #+#    #+#             */
/*   Updated: 2022/08/06 22:01:16 by sotherys         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "qrot.h"

t_qrot	ft_qrot(t_vector3 axis, double angle)
{
	t_qrot	q;

	q.axis = vec3_normalize(axis);
	q.angle = angle;
	return (q);
}

t_qrot		ft_qrot_from_vec3(t_vector3 v_old, t_vector3 v_new)
{
	v_old = vec3_normalize(v_old);
	v_new = vec3_normalize(v_new);
	return ((t_qrot)\
			{vec3_normalize(vec3_cross(v_old, v_new)), \
			acos(vec3_dot(v_old, v_new))});
}
