/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sotherys <sotherys@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/27 15:07:54 by sotherys          #+#    #+#             */
/*   Updated: 2022/07/27 15:10:54 by sotherys         ###   ########.fr       */
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
	double	ks;
	double	kd;
	t_vector3	is;
	t_vector3	id;

}				t_light;

#endif
