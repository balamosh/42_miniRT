/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   qrot_mult.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sotherys <sotherys@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/08 12:18:56 by sotherys          #+#    #+#             */
/*   Updated: 2022/08/08 11:50:20 by sotherys         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "qrot.h"

t_qrot	ft_qrot_mult(t_qrot a, t_qrot b)
{
	t_qrot	q;

	if (ft_fabs(a.angle) < EPS || vec3_len(a.axis) < EPS)
		return (b);
	if (ft_fabs(b.angle) < EPS || vec3_len(b.axis) < EPS)
		return (a);
	q.angle = 2 * acos(cos(b.angle / 2) * cos(a.angle / 2) - \
						sin(b.angle / 2) * sin(a.angle / 2) * \
						vec3_dot(b.axis, a.axis));
	q.axis = vec3_normalize(vec3_sum3(\
	vec3_scale(sin(a.angle / 2) * cos(b.angle / 2), a.axis), \
	vec3_scale(sin(b.angle / 2) * cos(a.angle / 2), b.axis), \
	vec3_scale(sin(b.angle / 2) * sin(a.angle / 2), \
						vec3_cross(b.axis, a.axis) \
	)));
	return (q);
}
