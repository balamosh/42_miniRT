/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sotherys <sotherys@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/15 01:00:37 by sotherys          #+#    #+#             */
/*   Updated: 2022/07/29 19:33:09 by sotherys         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CAMERA_H
# define CAMERA_H

# include "qrot.h"
# include "res.h"

typedef struct s_camera_plane
{
	double	left;
	double	right;
	double	up;
	double	down;
	double	width;
	double	height;
	double	pixel_width;
	double	pixel_height;
}				t_camera_plane;

typedef struct s_camera
{
	t_qrot		orient;
	t_qrot		projection;
	t_qrot		yaw;
	t_qrot		pitch;
	t_camera_plane		plane;
	t_res		res;
	t_vector3	pos;
	t_vector3	dir;
	t_vector3	up;
	double		fov;
}				t_camera;

void	ft_camera_isometric(t_camera *camera, t_res res);
void	ft_camera_dimetric(t_camera *camera, t_res res);
void	ft_camera_trimetric(t_camera *camera, t_res res);
void	ft_camera_top(t_camera *camera, t_res res);
void	ft_camera_front(t_camera *camera, t_res res);
void	ft_camera_side(t_camera *camera, t_res res);
void	ft_camera_update_projection(t_camera *camera);
void	ft_camera_update_plane(t_camera *camera);
void	ft_camera_update_plane_ratio(t_camera *cam);
void	ft_camera_rotate(t_camera *camera, double a_yaw, double a_pitch);
void	ft_camera_move(t_camera *camera, double up, double right);
void	ft_camera_copy_orient(t_camera *from, t_camera *to);
void	ft_camera_zoom(int x, int y, t_camera *camera, double scale);

#endif
