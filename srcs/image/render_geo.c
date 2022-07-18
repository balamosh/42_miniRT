/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_geo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sotherys <sotherys@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/16 10:41:04 by sotherys          #+#    #+#             */
/*   Updated: 2022/07/18 21:15:56 by sotherys         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "image.h"

void	ft_point_to_pixel(t_camera *cam, t_point *pt, t_pixel *pix)
{
	t_vector3	project_pt;

	project_pt = ft_qrot_rotate(pt->p, cam->orient);
	if (ft_fabs(project_pt.x) < EPS)
		project_pt.x = 0.0;
	if (ft_fabs(project_pt.y) < EPS)
		project_pt.y = 0.0;
	pix->x = cam->plane.pixel_width * (project_pt.x - cam->plane.left);
	pix->y = cam->plane.pixel_height * (cam->plane.up - project_pt.y);
	pix->cd = pt->cd;
}

#include "ray.h"

void	ft_render_geo(t_image *img, t_geometry *geo, t_camera *cam)
{
	int	i;
	int	j;
	double	gx, gy, t1, t2, t, r_sphere;
	t_vector3	bn, qx, qy, p1m, c_sphere;
	t_ray	primray;
	t_pixel	pix;
	void	*object;

	(void)geo;
	cam->pos = (t_vector3){-5, 0, 0};
	cam->dir = (t_vector3){1, 0, 0};
	gx = tan(cam->fov / 2);
	gy = gx * (cam->res.height - 1) / (cam->res.width - 1);
	bn = ft_vector3_cross(cam->up, cam->dir);
	qx = ft_vector3_scale(2 * gx / (cam->res.width - 1), bn);
	qy = ft_vector3_scale(2 * gy / (cam->res.height - 1), cam->up);
	p1m = ft_vector3_sum3(cam->dir, ft_vector3_scale(-gx, bn), ft_vector3_scale(-gy, cam->up));
	c_sphere = (t_vector3) {0, 0, 0};
	r_sphere = 1;
	object = NULL;
	i = 0;
	while (i < cam->res.width)
	{
		j = 0;
		while (j < cam->res.height)
		{
			primray.pos = ft_vector3_sum3(p1m, ft_vector3_scale(i - 1, qx), ft_vector3_scale(j - 1, qy));
			primray.dir = ft_vector3_normalize(primray.pos);
			t1 = -ft_vector3_dot(ft_vector3_diff(primray.pos, c_sphere), primray.dir) + r_sphere;
			t2 = -ft_vector3_dot(ft_vector3_diff(primray.pos, c_sphere), primray.dir) - r_sphere;
			object = (void *) &object;
			if (t1 > 0 && t2 > 0)
				t = t1;
			else if (t1 < 0 && t2 > 0)
				t = t2;
			else
				object = NULL;
			pix.x = t;
			pix.x = i;
			pix.y = j;
			pix.cd = 0xFF00FF00 * 1;
			if (object != NULL)\
			{
				pix.cd = 0xFF00FF00 * 1;
				ft_image_pixel_put(img, &pix);
			}
			else
			{
				pix.cd = 0xFFFFFFFF;
				ft_image_pixel_put(img, &pix);
			}
			++j;
		}
		++i;
	}
}

void	ft_camera_fit_geo(t_camera *cam, t_geometry *geo)
{
	t_vector3	min;
	t_vector3	max;
	t_vector3	curr;
	size_t		i;

	if (!geo->npts)
		return ;
	min = ft_qrot_rotate(geo->pts[0].p, cam->orient);
	max = min;
	i = 1;
	while (i < geo->npts)
	{
		curr = ft_qrot_rotate(geo->pts[i++].p, cam->orient);
		max = (t_vector3){ft_fmax(max.x, curr.x), ft_fmax(max.y, curr.y), 0};
		min = (t_vector3){ft_fmin(min.x, curr.x), ft_fmin(min.y, curr.y), 0};
	}
	cam->plane.left = min.x;
	cam->plane.right = max.x;
	cam->plane.up = max.y;
	cam->plane.down = min.y;
	ft_camera_update_plane(cam);
	ft_camera_update_plane_ratio(cam);
	ft_camera_zoom(cam->res.width / 2, cam->res.height / 2, cam, -0.1);
}
