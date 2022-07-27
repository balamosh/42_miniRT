/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_geo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sotherys <sotherys@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/16 10:41:04 by sotherys          #+#    #+#             */
/*   Updated: 2022/07/27 15:37:28 by sotherys         ###   ########.fr       */
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
#include "object.h"
#include "light.h"
#include <stdio.h>

t_object	*ft_intersect(t_object *obj, t_ray ray)
{
	t_sphere	*sphere;
	t_vector3	l;
	double		tca, thc, d, t, t0, t1;

	sphere = (t_sphere *) obj->obj;
	l = ft_vector3_diff(sphere->center, ray.pos);
	tca = ft_vector3_dot(l, ray.dir);
	if (ft_vector3_len(l) * ft_vector3_len(l) - tca * tca < 0)
		return (NULL);
	else
	{
		d = sqrt(ft_vector3_len(l) * ft_vector3_len(l) - tca * tca);
		thc = sqrt(sphere->radius * sphere->radius - d * d);
		if (d > sphere->radius)
			return (NULL);
		t0 = tca - thc;
		t1 = tca + thc;
		if (t0 > 0 && t1 > 0)
			t = t0;
		else if (t0 < 0 && t1 > 0)
			t = t1;
		else
			return (NULL);
	}
	obj->phit = ft_vector3_sum2(ray.pos, ft_vector3_scale(t, ray.dir));
	obj->nhit = ft_vector3_normalize(ft_vector3_diff(obj->phit, sphere->center));
	return (obj);
}

int	calc_color(t_camera *cam, t_object *obj, t_light *light)
{
	t_vector3	l, r, v, ip;
	int			color;

	l = ft_vector3_normalize(ft_vector3_diff(light->pos, obj->phit));
	r = ft_vector3_diff(ft_vector3_scale(2 * ft_vector3_dot(l, obj->nhit), obj->nhit), l);
	r = ft_vector3_normalize(r);
	v = ft_vector3_normalize(ft_vector3_diff(cam->pos, obj->phit));
	ip = ft_vector3_sum2(ft_vector3_scale(light->kd * ft_fmax(ft_vector3_dot(l, obj->nhit), 0), light->id), 
						ft_vector3_scale(light->ks * ft_fmax(ft_vector3_dot(r, v), 0), light->is));
	ip.x = ft_fmin(ip.x, 1);
	ip.y = ft_fmin(ip.y, 1);
	ip.z = ft_fmin(ip.z, 1);
	ip = ft_vector3_scale(255, ip);
	color = 255 << 24 | (int) ip.x << 16 | (int) ip.y << 8 | (int) ip.z;
	return (color);
}

void	ft_render_geo(t_image *img, t_object *obj, t_light *light, t_camera *cam)
{
	int	i;
	int	j;
	double	gx, gy, transmission;
	t_vector3	bn, qx, qy, p1m;
	t_ray	primray;
	t_pixel	pix;
	void	*object;

	gx = tan(cam->fov / 2);
	gy = gx * (cam->res.height - 1) / (cam->res.width - 1);
	bn = ft_vector3_cross(cam->up, cam->dir);
	qx = ft_vector3_scale(2 * gx / (cam->res.width - 1), bn);
	qy = ft_vector3_scale(2 * gy / (cam->res.height - 1), cam->up);
	p1m = ft_vector3_sum3(cam->dir, ft_vector3_scale(-gx, bn), ft_vector3_scale(-gy, cam->up));
	i = 0;
	while (i < cam->res.width)
	{
		j = 0;
		while (j < cam->res.height)
		{
			primray.dir = ft_vector3_sum3(p1m, ft_vector3_scale(i - 1, qx), ft_vector3_scale(j - 1, qy));
			primray.pos = ft_vector3_sum2(cam->pos, primray.dir);
			primray.dir = ft_vector3_normalize(primray.dir);
			object = ft_intersect(obj, primray);
			pix.x = i;
			pix.y = cam->res.height - j;
			if (object != NULL)
			{
				transmission = 1;
				pix.cd = calc_color(cam, object, light);
				//if (ft_intersect(geo, (t_ray) {phit, light_dir}, &t0, &t1))
				//	transmission = 0;
				(void)transmission;
				ft_image_pixel_put(img, &pix);
			}
			else
			{
				pix.cd = 0xFF000000;
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
