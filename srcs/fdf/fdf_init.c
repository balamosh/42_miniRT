/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sotherys <sotherys@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/27 03:03:34 by sotherys          #+#    #+#             */
/*   Updated: 2022/08/17 18:06:13 by sotherys         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static t_bool	ft_mlx_init(void **mlx)
{
	*mlx = mlx_init();
	if (!*mlx)
		return (false);
	return (true);
}

t_bool	ft_fdf_images_init(t_image *image, void *mlx, int width, int height)
{
	size_t	i;

	i = 0;
	while (i < FDF_TMP_IMG)
	{
		if (!ft_image(&image[i++], mlx, width, height))
			return (false);
	}
	return (true);
}

static void	ft_fdf_null(t_fdf *tab)
{
	size_t	i;

	tab->mlx = NULL;
	tab->window.ptr = NULL;
	i = 0;
	while (i < FDF_TMP_IMG)
		tab->image[i++].img_ptr = NULL;
	tab->geo.pts = NULL;
	tab->geo.edges = NULL;
	tab->bbox.pts = NULL;
	tab->bbox.edges = NULL;
	tab->axis_geo.pts = NULL;
	tab->axis_geo.edges = NULL;
}

void	cylinder_disc_init(t_cylinder *cylinder)
{
	cylinder->bot.plane = (t_plane){cylinder->pos, vec3_scale(-1, cylinder->axis)};
	cylinder->top.plane = (t_plane){vec3_sum2(cylinder->pos, vec3_scale(cylinder->height, cylinder->axis)), cylinder->axis};
	cylinder->bot.radius = cylinder->radius;
	cylinder->top.radius = cylinder->radius;
}

void	cone_disc_init(t_cone *cone)
{
	cone->bot.plane = (t_plane){vec3_sum2(cone->pos, vec3_scale(cone->down, cone->axis)), cone->axis};
	cone->top.plane = (t_plane){vec3_sum2(cone->pos, vec3_scale(cone->up, cone->axis)), cone->axis};
	cone->bot.radius = tan(cone->alpha) * ft_fabs(cone->down);
	cone->top.radius = tan(cone->alpha) * ft_fabs(cone->up);
	cone->sin2a = sin(cone->alpha);
	cone->sin2a *= cone->sin2a;
	cone->cos2a = cos(cone->alpha);
	cone->cos2a *= cone->cos2a;
}

t_bool	ft_fdf_init(t_fdf *tab)
{
	ft_fdf_null(tab);
	tab->error = true;
	if (!(ft_fdf_geo_init(&tab->geo, &tab->bbox, tab->filename) \
		&& ft_geometry_axis(&tab->axis_geo) \
		&& ft_mlx_init(&tab->mlx) \
		&& ft_fdf_images_init(tab->image, tab->mlx, \
								FDF_WINDOW_WIDTH, FDF_WINDOW_HEIGHT) \
		&& ft_window(&tab->window, tab->mlx, \
						(t_res){FDF_WINDOW_WIDTH, FDF_WINDOW_HEIGHT}, "fdf")))
		return (false);
	ft_camera_isometric(&tab->camera, \
	(t_res){FDF_WINDOW_WIDTH, FDF_WINDOW_HEIGHT});
	ft_camera_isometric(&tab->axis_cam, \
	(t_res){ft_min(FDF_WINDOW_WIDTH, FDF_WINDOW_HEIGHT) / 10, \
			ft_min(FDF_WINDOW_WIDTH, FDF_WINDOW_HEIGHT) / 10});
	ft_camera_fit_geo(&tab->camera, &tab->geo);
	
	tab->camera.pos = (t_vector3){0, 0, -3};
	tab->camera.dir = (t_vector3){0, 0, 1};
	tab->camera.up = (t_vector3){0, 1, 0};
	tab->camera.fov = PI / 2;
	tab->camera.yaw = (t_qrot) {(t_vector3) {0, -1, 0}, 0};
	tab->camera.pitch = (t_qrot) {(t_vector3) {1, 0, 0}, 0};
	tab->camera.orient = ft_qrot_mult(tab->camera.yaw, tab->camera.pitch);
	tab->camera.projection = (t_qrot) {(t_vector3) {0, 0, 1}, 0};

	tab->objects = NULL;
	tab->lights = NULL;
	tab->test_sphere[0].center = (t_vector3){-1, 0, 0};
	tab->test_sphere[0].radius = 0.4;
	tab->test_obj[0].data = (void *) &tab->test_sphere[0];
	tab->test_obj[0].color = (t_vector3){1, 0, 1};
	tab->test_obj[0].type = OBJ_SPHERE;
	tab->test_sphere[1].center = (t_vector3){1, 0, 0};
	tab->test_sphere[1].radius = 0.4;
	tab->test_obj[1].data = (void *) &tab->test_sphere[1];
	tab->test_obj[1].color = (t_vector3){0, 1, 0};
	tab->test_obj[1].type = OBJ_SPHERE;
	tab->test_sphere[2].center = (t_vector3){1.5, 0.5, 0};
	tab->test_sphere[2].radius = 0.4;
	tab->test_obj[2].data = (void *) &tab->test_sphere[2];
	tab->test_obj[2].color = (t_vector3){0, 0, 1};
	tab->test_obj[2].type = OBJ_SPHERE;
	tab->test_plane[1].pos = (t_vector3){0, 0, -3};
	tab->test_plane[1].n = (t_vector3){0, 0, 1};
	tab->test_obj[3].data = (void *) &tab->test_plane[1];
	tab->test_obj[3].color = (t_vector3){1, 1, 0};
	tab->test_obj[3].type = OBJ_PLANE;
	tab->test_cylinder[0].pos = (t_vector3){0, 0, 0};
	tab->test_cylinder[0].axis = (t_vector3){0, 1, 0};
	tab->test_cylinder[0].radius = 0.5;
	tab->test_cylinder[0].height = 2;
	tab->test_obj[4].data = (void *) &tab->test_cylinder[0];
	tab->test_obj[4].color = (t_vector3){1, 0, 0};
	tab->test_obj[4].type = OBJ_CYLINDER;
	tab->test_cone[0].pos = (t_vector3){0, 0, 0};
	tab->test_cone[0].axis = (t_vector3){0, 1, 0};
	tab->test_cone[0].alpha = PI / 6;
	tab->test_cone[0].up = 0.5;
	tab->test_cone[0].down = -1.5;
	tab->test_obj[5].data = (void *) &tab->test_cone[0];
	tab->test_obj[5].color = (t_vector3){1, 0, 0};
	tab->test_obj[5].type = OBJ_CONE;
	cylinder_disc_init(&tab->test_cylinder[0]);
	cone_disc_init(&tab->test_cone[0]);
	lst_add_front(&tab->objects, lst_new((void *)&tab->test_obj[0]));
	//lst_add_front(&tab->objects, lst_new((void *)&tab->test_obj[1]));
	//lst_add_front(&tab->objects, lst_new((void *)&tab->test_obj[2]));
	//lst_add_front(&tab->objects, lst_new((void *)&tab->test_obj[3]));
	//lst_add_front(&tab->objects, lst_new((void *)&tab->test_obj[4]));
	lst_add_front(&tab->objects, lst_new((void *)&tab->test_obj[5]));

	tab->test_light[0].type = LIGHT_SPOT;
	tab->test_light[0].color = (t_vector3) {1, 1, 1};
	tab->test_light[0].intensity = 1;
	tab->test_light[0].pos = (t_vector3) {-1, 5, -2};
	tab->test_light[0].ks = 0.5;
	tab->test_light[0].kd = 1;
	tab->test_light[1].type = LIGHT_AMBIENT;
	tab->test_light[1].color = (t_vector3) {1, 1, 1};
	tab->test_light[1].intensity = 0.2;
	tab->test_light[2].type = LIGHT_SPOT;
	tab->test_light[2].color = (t_vector3) {1, 1, 1};
	tab->test_light[2].intensity = 2;
	tab->test_light[2].pos = (t_vector3) {-3, 0, -2};
	tab->test_light[2].ks = 0.5;
	tab->test_light[2].kd = 1;
	lst_add_front(&tab->lights, lst_new((void *)&tab->test_light[0]));
	lst_add_front(&tab->lights, lst_new((void *)&tab->test_light[1]));
	//lst_add_front(&tab->lights, lst_new((void *)&tab->test_light[2]));

	tab->img_id = 0;
	tab->lmb = false;
	tab->mmb = false;
	tab->render_bbox = false;
	tab->error = false;
	return (true);
}
