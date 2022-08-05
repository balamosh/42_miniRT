/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sotherys <sotherys@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/27 15:07:54 by sotherys          #+#    #+#             */
/*   Updated: 2022/08/03 14:15:09 by sotherys         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIGHT_H
# define LIGHT_H

# include "vector3.h"

typedef enum s_light_type
{
	LIGHT_SPOT,
	LIGHT_AMBIENT
}			t_light_type;

typedef struct s_light
{
	t_light_type	type;
	t_vector3		pos;
	t_vector3		color;
	double			intensity;
	double			ks;
	double			kd;
}				t_light;

#endif
