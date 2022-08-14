/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector3_func2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sotherys <sotherys@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/04 13:58:28 by sotherys          #+#    #+#             */
/*   Updated: 2022/08/08 11:48:07 by sotherys         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vector3.h"

double	vec3_dot(t_vector3 a, t_vector3 b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}

t_vector3	vec3_cross(t_vector3 a, t_vector3 b)
{
	return ((t_vector3){a.y * b.z - a.z * b.y, \
						a.z * b.x - a.x * b.z, \
						a.x * b.y - a.y * b.x});
}

double	vec3_len(t_vector3 a)
{
	return (sqrt(vec3_dot(a, a)));
}

t_vector3	vec3_normalize(t_vector3 a)
{
	double	len;

	len = vec3_len(a);
	if (len > EPS)
		return (vec3_scale(1 / len, a));
	return (a);
}
