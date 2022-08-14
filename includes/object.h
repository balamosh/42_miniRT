/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sotherys <sotherys@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/27 12:21:36 by sotherys          #+#    #+#             */
/*   Updated: 2022/08/14 10:04:13 by sotherys         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OBJECT_H
# define OBJECT_H

# include "vector3.h"

typedef enum s_obj_type
{
	OBJ_SPHERE,
	OBJ_PLANE,
	OBJ_CYLINDER
}			t_obj_type;

typedef struct s_object
{
	t_obj_type		type;
	void			*data;
	t_vector3		color;
	t_vector3		phit;
	t_vector3		nhit;
	double			distance;
}				t_object;

typedef struct s_sphere
{
	t_vector3	center;
	double		radius;
}				t_sphere;

typedef struct s_plane
{
	t_vector3	pos;
	t_vector3	n;
}				t_plane;

typedef struct s_cylinder
{
	t_vector3	pos;
	t_vector3	axis;
	t_plane		top;
	t_plane		bot;
	double		height;
	double		radius;
}				t_cylinder;

#endif
