/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sotherys <sotherys@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/27 12:21:36 by sotherys          #+#    #+#             */
/*   Updated: 2022/08/18 09:10:01 by sotherys         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OBJECT_H
# define OBJECT_H

# include "vector3.h"

typedef enum s_obj_type
{
	OBJ_SPHERE,
	OBJ_PLANE,
	OBJ_CYLINDER,
	OBJ_CONE
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

typedef struct s_disc
{
	t_plane	plane;
	double	radius;
}				t_disc;

typedef struct s_cylinder
{
	t_vector3	pos;
	t_vector3	axis;
	t_disc		top;
	t_disc		bot;
	double		height;
	double		radius;
}				t_cylinder;

typedef struct s_cone
{
	t_vector3	pos;
	t_vector3	axis;
	double		alpha;
	t_disc		top;
	t_disc		bot;
	double		up;
	double		down;
	double		sina;
	double		cosa;
}				t_cone;

#endif
