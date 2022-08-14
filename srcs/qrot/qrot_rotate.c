/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   qrot_rotate.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sotherys <sotherys@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/08 12:18:56 by sotherys          #+#    #+#             */
/*   Updated: 2022/08/05 17:09:58 by sotherys         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "qrot.h"

t_vector3	ft_qrot_rotate(t_vector3 pt, t_qrot q)
{
	if (ft_fabs(q.angle) < EPS)
		return (pt);
	return (\
	vec3_sum3(\
		vec3_scale(cos(q.angle), pt), \
		vec3_scale(sin(q.angle), vec3_cross(q.axis, pt)), \
		vec3_scale((1 - cos(q.angle)) * vec3_dot(q.axis, pt), \
							q.axis) \
	));
}
