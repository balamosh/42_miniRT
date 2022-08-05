/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_geo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sotherys <sotherys@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/16 10:41:04 by sotherys          #+#    #+#             */
/*   Updated: 2022/08/05 17:08:07 by sotherys         ###   ########.fr       */
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

int	ft_intersect_sphere(t_object *obj, t_ray ray)
{
	t_sphere	*sphere;
	t_vector3	l;
	double		tca, thc, d, t, t0, t1;

	sphere = (t_sphere *) obj->data;
	l = ft_vector3_diff(sphere->center, ray.pos);
	tca = ft_vector3_dot(l, ray.dir);
	if (ft_vector3_len(l) * ft_vector3_len(l) - tca * tca < 0)
		return (0);
	else
	{
		d = sqrt(ft_vector3_len(l) * ft_vector3_len(l) - tca * tca);
		if (d > sphere->radius)
			return (0);
		thc = sqrt(sphere->radius * sphere->radius - d * d);
		t0 = tca - thc;
		t1 = tca + thc;
		if (t0 > 0 && t1 > 0)
			t = t0;
		else if (t0 < 0 && t1 > 0)
			t = t1;
		else
			return (0);
	}
	obj->phit = ft_vector3_sum2(ray.pos, ft_vector3_scale(t, ray.dir));
	obj->nhit = ft_vector3_normalize(ft_vector3_diff(obj->phit, sphere->center));
	obj->distance = ft_vector3_len(ft_vector3_diff(obj->phit, ray.pos));
	return (1);
}

int	ft_intersect_plane(t_object *obj, t_ray ray)
{
	t_plane	*plane;
	double	denominator;
	double	t;

	plane = (t_plane *) obj->data;
	denominator = ft_vector3_dot(plane->n, ray.dir);
	if (ft_fabs(denominator) < 0.0000001)
		return (0);
	t = ft_vector3_dot(plane->n, ft_vector3_diff(plane->pos, ray.pos)) / denominator;
	if (t < 0)
		return (0);
	obj->phit = ft_vector3_sum2(ray.pos, ft_vector3_scale(t, ray.dir));
	obj->nhit = plane->n;
	obj->distance = ft_vector3_len(ft_vector3_diff(obj->phit, ray.pos));
	return (1);
}

/*
int	ft_intersect_cylinder(t_object *obj, t_ray ray)
{
	t_cylinder	*cylinder;
	t_vector3	x, y, z, d;

	cylinder = (t_cylinder) obj->data;
	x = ft_vector3_cross(cylinder->n, ray.dir);
	z = cylinder->n;
	y = ft_vector3_cross(z, x);
	d = ft_vector3_scale(ft_vector3_dot()) 
}
*/

t_object	*ft_find_intersection(t_list *objects, t_ray ray)
{
	t_list		*curr_node;
	t_object	*curr_object;
	t_object	*found;
	double		min_dist;
	static int	(*intersect[2])(t_object *, t_ray) = {ft_intersect_sphere, ft_intersect_plane};

	found = NULL;
	min_dist = -1;
	curr_node = objects;
	while (curr_node)
	{
		curr_object = (t_object *)curr_node->data;
		if (intersect[curr_object->type](curr_object, ray) && \
			(min_dist < 0 || curr_object->distance < min_dist))
		{
			found = curr_object;
			min_dist = found->distance;
		}
		curr_node = curr_node->next;
	}
	return (found);
}

int	color_to_int(t_vector3 color)
{
	color.x = ft_fmin(color.x, 1);
	color.y = ft_fmin(color.y, 1);
	color.z = ft_fmin(color.z, 1);
	color = ft_vector3_scale(255, color);
	return (255 << 24 | (int) color.x << 16 | (int) color.y << 8 | (int) color.z);
}

t_vector3	color_mult(t_vector3 c1, t_vector3 c2)
{
	return ((t_vector3){c1.x * c2.x, c1.y * c2.y, c1.z * c2.z});
}

t_vector3	trace(t_camera *cam, t_ray ray, t_list *objects, t_light *light, int depth)
{
	t_camera	r_cam;
	t_vector3	p, n, l, r, v, ip, phit_offset, v2, r2;
	t_vector3	color, r_color;
	t_object	*obj;
	int			not_in_shadow;

	if (depth > 3)
		return ((t_vector3){0, 0, 0});
	obj = ft_find_intersection(objects, ray);
	if (obj == NULL)
		return ((t_vector3){0, 0, 0});
	p = obj->phit;
	n = obj->nhit;
	phit_offset = ft_vector3_sum2(p, ft_vector3_scale(0.00001, n));
	r_cam.pos = phit_offset;
	r_cam.pos = cam->pos;
	not_in_shadow = 1;
	l = ft_vector3_normalize(ft_vector3_diff(light->pos, p));
	if (ft_find_intersection(objects, (t_ray){phit_offset, l}))
		not_in_shadow = 0;
	r = ft_vector3_diff(ft_vector3_scale(2 * ft_vector3_dot(l, n), n), l);
	r = ft_vector3_normalize(r);
	v = ft_vector3_normalize(ft_vector3_diff(ray.pos, p));
	v2 = ft_vector3_normalize(ft_vector3_diff(ray.pos, p));
	r2 = ft_vector3_diff(ft_vector3_scale(2 * ft_vector3_dot(v2, n), n), v2);
	color = color_mult(light->color, obj->color);
	if (light->type == LIGHT_AMBIENT)
		ip = color;
	else if (light->type == LIGHT_SPOT)
		ip = ft_vector3_sum2(ft_vector3_scale(not_in_shadow * light->kd * ft_fmax(ft_vector3_dot(l, n), 0), color), 
							ft_vector3_scale(not_in_shadow * light->ks * ft_fpow(ft_fmax(ft_vector3_dot(r, v), 0), 16), light->color));
	ip = ft_vector3_scale(light->intensity, ip);
	r_color = trace(&r_cam, (t_ray){phit_offset, r2}, objects, light, depth + 1);
	ip = ft_vector3_sum2(ip, 
						ft_vector3_scale(
							not_in_shadow * light->ks, 
							r_color));
	return (ip);
}

void	ft_render_geo(t_image *img, t_list *objects, t_list *lights, t_camera *cam)
{
	int	i;
	int	j;
	double	gx, gy;
	t_vector3	bn, qx, qy, p1m, color;
	t_ray	primray;
	t_pixel	pix;
	t_light	*curr_light;
	t_list	*curr_node;

	gx = tan(cam->fov / 2);
	gy = gx * (cam->res.height - 1) / (cam->res.width - 1);
	bn = ft_vector3_normalize(ft_vector3_cross(cam->dir, cam->up));
	qx = ft_vector3_scale(2 * gx / (cam->res.width - 1), bn);
	qy = ft_vector3_scale(2 * gy / (cam->res.height - 1), cam->up);
	p1m = ft_vector3_sum3(cam->dir, ft_vector3_scale(-gx, bn), ft_vector3_scale(-gy, cam->up));
	i = 0;
	while (i < cam->res.width)
	{
		j = 0;
		while (j < cam->res.height)
		{
			pix.x = i;
			pix.y = cam->res.height - j;
			primray.dir = ft_vector3_sum3(p1m, ft_vector3_scale(i, qx), ft_vector3_scale(j, qy));
			primray.pos = ft_vector3_sum2(cam->pos, primray.dir);
			primray.dir = ft_vector3_normalize(primray.dir);
			color = (t_vector3){0, 0, 0};
			curr_node = lights;
			while (curr_node)
			{
				curr_light = (t_light *) curr_node->data;
				color = ft_vector3_sum2(color, trace(cam, primray, objects, curr_light, 0));
				curr_node = curr_node->next;
			}
			pix.cd = color_to_int(color);
			ft_image_pixel_put(img, &pix);
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
