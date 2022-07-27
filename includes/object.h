/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sotherys <sotherys@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/27 12:21:36 by sotherys          #+#    #+#             */
/*   Updated: 2022/07/27 15:07:03 by sotherys         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OBJECT_H
# define OBJECT_H

# include "vector3.h"

typedef enum s_obj_type
{
	OBJ_SPHERE,
	OBJ_PLANE
}			t_obj_type;

typedef struct s_object
{
	t_obj_type		type;
	void			*obj;
	t_vector3		phit;
	t_vector3		nhit;
}				t_object;

typedef struct s_sphere
{
	t_vector3	center;
	double		radius;
}				t_sphere;

#endif