/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_geo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sotherys <sotherys@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/16 10:41:04 by sotherys          #+#    #+#             */
/*   Updated: 2022/08/18 11:18:45 by sotherys         ###   ########.fr       */
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
	l = vec3_diff(sphere->center, ray.pos);
	tca = vec3_dot(l, ray.dir);
	if (vec3_len(l) * vec3_len(l) - tca * tca < 0)
		return (0);
	else
	{
		d = sqrt(vec3_len(l) * vec3_len(l) - tca * tca);
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
	obj->phit = vec3_sum2(ray.pos, vec3_scale(t, ray.dir));
	obj->nhit = vec3_normalize(vec3_diff(obj->phit, sphere->center));
	obj->distance = vec3_len(vec3_diff(obj->phit, ray.pos));
	return (1);
}

int	ft_intersect_plane(t_object *obj, t_ray ray)
{
	t_plane	*plane;
	double	denominator;
	double	t;

	plane = (t_plane *) obj->data;
	denominator = vec3_dot(plane->n, ray.dir);
	if (ft_fabs(denominator) < 0.0000001)
		return (0);
	t = vec3_dot(plane->n, vec3_diff(plane->pos, ray.pos)) / denominator;
	if (t < 0)
		return (0);
	obj->phit = vec3_sum2(ray.pos, vec3_scale(t, ray.dir));
	obj->nhit = plane->n;
	obj->distance = vec3_len(vec3_diff(obj->phit, ray.pos));
	return (1);
}

void	check_intersect_disc(t_object *obj, t_disc *disc, t_ray ray)
{
	t_object	p;
	t_vector3	v;

	p.data = (void *) &disc->plane;
	if (ft_intersect_plane(&p, ray) && (obj->distance < 0 || p.distance < obj->distance))
	{
		v = vec3_diff(p.phit, disc->plane.pos);
		if (vec3_dot(v, v) < disc->radius * disc->radius)
		{
			obj->phit = p.phit;
			obj->nhit = p.nhit;
			obj->distance = p.distance;
		}
	}
}

void	cylinder_intersect_tube(t_object *obj, t_ray ray)
{
	t_cylinder	*cylinder;
	t_vector3	dp, v1, v2, phit;
	double		a, b, c, d, t, z, distance;

	cylinder = (t_cylinder *) obj->data;
	dp = vec3_diff(ray.pos, cylinder->pos);
	v1 = vec3_diff(ray.dir, vec3_scale(vec3_dot(ray.dir, cylinder->axis), cylinder->axis));
	v2 = vec3_diff(dp, vec3_scale(vec3_dot(dp, cylinder->axis), cylinder->axis));
	a = vec3_dot(v1, v1);
	b = 2 * vec3_dot(v1, v2);
	c = vec3_dot(v2, v2) - cylinder->radius * cylinder->radius;
	d = b * b - 4 * a * c;
	if (d < 0)
		return ;
	t = -(b + sqrt(d)) / (2 * a);
	phit = vec3_sum2(ray.pos, vec3_scale(t, ray.dir));
	//distance = vec3_len(vec3_diff(phit, ray.pos));
	distance = t;
	z = vec3_dot(phit, cylinder->axis) - vec3_dot(cylinder->pos, cylinder->axis);
	if (z >= 0 && z <= cylinder->height && (obj->distance < 0 || distance < obj->distance))
	{
		obj->phit = phit;
		obj->distance = distance;
		obj->nhit = vec3_normalize(vec3_diff(obj->phit, 
											vec3_sum2(cylinder->pos, 
											vec3_scale(vec3_dot(obj->phit, cylinder->axis), cylinder->axis))));
	}
}

int	ft_intersect_cylinder(t_object *obj, t_ray ray)
{
	t_cylinder	*cylinder;

	cylinder = (t_cylinder *) obj->data;
	obj->distance = -1;
	cylinder_intersect_tube(obj, ray);
	check_intersect_disc(obj, &cylinder->bot, ray);
	check_intersect_disc(obj, &cylinder->top, ray);
	if (obj->distance >= 0)
		return (1);
	return (0);
}

void	cone_intersect_tube(t_object *obj, t_ray ray)
{
	t_cone		*cone;
	t_vector3	dp, v1, v2, phit, o_phit;
	double		v3, v4;
	double		a, b, c, d, t, z, distance;

	cone = (t_cone *) obj->data;
	dp = vec3_diff(ray.pos, cone->pos);
	v1 = vec3_diff(ray.dir, vec3_scale(vec3_dot(ray.dir, cone->axis), cone->axis));
	v2 = vec3_diff(dp, vec3_scale(vec3_dot(dp, cone->axis), cone->axis));
	v1 = vec3_scale(cone->cosa, v1);
	v2 = vec3_scale(cone->cosa, v2);
	v3 = cone->sina * vec3_dot(ray.dir, cone->axis);
	v4 = cone->sina * vec3_dot(dp, cone->axis);
	a = vec3_dot(v1, v1) - v3 * v3;
	b = 2 * vec3_dot(v1, v2) - 2 * v3 * v4;
	c = vec3_dot(v2, v2) - v4 * v4;
	d = b * b - 4 * a * c;
	if (d < 0)
		return ;
	t = -(b + sqrt(d)) / (2 * a);
	phit = vec3_sum2(ray.pos, vec3_scale(t, ray.dir));
	//distance = vec3_len(vec3_diff(phit, ray.pos));
	distance = t;
	z = vec3_dot(phit, cone->axis) - vec3_dot(cone->pos, cone->axis);
	if (z >= cone->down && z <= cone->up && (obj->distance < 0 || distance < obj->distance))
	{
		obj->phit = phit;
		obj->distance = distance;
		o_phit = vec3_diff(phit, cone->pos);
		obj->nhit = vec3_normalize(vec3_diff(obj->phit, 
											vec3_sum2(cone->pos, 
											vec3_scale(vec3_len(o_phit) / cone->cosa * vec3_dot(cone->axis, vec3_normalize(o_phit)), 
														cone->axis))));
	}
}

int	ft_intersect_cone(t_object *obj, t_ray ray)
{
	t_cone	*cone;

	cone = (t_cone *) obj->data;
	obj->distance = -1;
	check_intersect_disc(obj, &cone->bot, ray);
	check_intersect_disc(obj, &cone->top, ray);
	cone_intersect_tube(obj, ray);
	if (obj->distance >= 0)
		return (1);
	return (0);
}

t_object	*ft_find_intersection(t_list *objects, t_ray ray)
{
	t_list		*curr_node;
	t_object	*curr_object;
	t_object	*found;
	double		min_dist;
	static int	(*intersect[4])(t_object *, t_ray) = {ft_intersect_sphere, ft_intersect_plane, ft_intersect_cylinder, ft_intersect_cone};

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
	color = vec3_scale(255, color);
	return (255 << 24 | (int) color.x << 16 | (int) color.y << 8 | (int) color.z);
}

t_vector3	color_mult(t_vector3 c1, t_vector3 c2)
{
	return ((t_vector3){c1.x * c2.x, c1.y * c2.y, c1.z * c2.z});
}

t_vector3	trace(t_camera *cam, t_ray ray, t_list *objects, t_light *light, int depth)
{
	t_camera	r_cam;
	t_vector3	p, n, l, r, v, ip, phit_offset;
	t_vector3	v2, r2;
	t_vector3	color;
	t_object	*obj, *shadow_obj;
	int			not_in_shadow;

	if (depth > 3)
		return ((t_vector3){0, 0, 0});
	obj = ft_find_intersection(objects, ray);
	if (obj == NULL)
		return ((t_vector3){0, 0, 0});
	p = obj->phit;
	n = obj->nhit;
	phit_offset = vec3_sum2(p, vec3_scale(0.00001, n));
	r_cam.pos = phit_offset;
	r_cam.pos = cam->pos;
	(void)r_cam;
	not_in_shadow = 1;
	l = vec3_normalize(vec3_diff(light->pos, p));
	shadow_obj = ft_find_intersection(objects, (t_ray){phit_offset, l});
	if (shadow_obj && shadow_obj->distance < vec3_len(vec3_diff(light->pos, p)))
		not_in_shadow = 0;
	r = vec3_diff(vec3_scale(2 * vec3_dot(l, n), n), l);
	r = vec3_normalize(r);
	v = vec3_normalize(vec3_diff(ray.pos, p));
	v2 = vec3_normalize(vec3_diff(ray.pos, p));
	r2 = vec3_diff(vec3_scale(2 * vec3_dot(v2, n), n), v2);
	color = color_mult(light->color, obj->color);
	if (light->type == LIGHT_AMBIENT)
		ip = color;
	else if (light->type == LIGHT_SPOT)
		ip = vec3_sum2(vec3_scale(not_in_shadow * light->kd * ft_fmax(vec3_dot(l, n), 0), color), 
							vec3_scale(not_in_shadow * light->ks * ft_fpow(ft_fmax(vec3_dot(r, v), 0), 16), light->color));
	ip = vec3_scale(light->intensity, ip);
	ip = vec3_sum2(ip, vec3_scale(0.5 * not_in_shadow * light->ks, \
									trace(&r_cam, (t_ray){phit_offset, r2}, objects, light, depth + 1)));
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
	bn = vec3_normalize(vec3_cross(cam->dir, cam->up));
	qx = vec3_scale(2 * gx / (cam->res.width - 1), bn);
	qy = vec3_scale(2 * gy / (cam->res.height - 1), cam->up);
	p1m = vec3_sum3(cam->dir, vec3_scale(-gx, bn), vec3_scale(-gy, cam->up));
	i = 0;
	while (i < cam->res.width)
	{
		j = 0;
		while (j < cam->res.height)
		{
			pix.x = i;
			pix.y = cam->res.height - j;
			primray.dir = vec3_sum3(p1m, vec3_scale(i, qx), vec3_scale(j, qy));
			primray.pos = vec3_sum2(cam->pos, primray.dir);
			primray.dir = vec3_normalize(primray.dir);
			color = (t_vector3){0, 0, 0};
			curr_node = lights;
			while (curr_node)
			{
				curr_light = (t_light *) curr_node->data;
				color = vec3_sum2(color, trace(cam, primray, objects, curr_light, 0));
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
