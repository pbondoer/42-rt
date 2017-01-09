/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/09 09:15:54 by hmartzol          #+#    #+#             */
/*   Updated: 2017/01/09 04:11:28 by pbondoer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <rtv1.h>

int			cb_exit(int k, int s, void *p)
{
	(void)k;
	(void)p;
	if (s == FTX_KEY_STATUS_PRESSED)
		ft_end(0);
	return (0);
}

t_light	**lights(void)
{
	static t_light	*data = NULL;

	return (&data);
}

t_primitive	sphere(cl_float4 pos, cl_float radius, cl_float4 color)
{
	return ((t_primitive){.type = SPHERE, .position = pos,
		.normal = {.x = 0, .y = 0, .z = 1, .w = 0}, .radius = radius,
		.color = color});
}

t_primitive	plane(cl_float4 pos, cl_float4 norm, cl_float4 color)
{
	return ((t_primitive){.type = PLANE, .position = pos,
		.normal = norm, .radius = 0, .color = color});
}

t_light		light(cl_float4 pos, cl_float4 color)
{
	return ((t_light){.position = pos, .color = color});
}

#include <stdio.h>

void		calc_vpul(void)
{
	float	y;
	float	x;

	y = ((float)cam()->vp_size.y / 2.0f);
	x = -((float)cam()->vp_size.x / 2.0f);
	cam()->vpul.x = ((cam()->dir.x * cam()->dist) +
					(cam()->up.x * y)) + (cam()->right.x * x);
	cam()->vpul.y = ((cam()->dir.y * cam()->dist) +
					(cam()->up.y * y)) + (cam()->right.y * x);
	cam()->vpul.z = ((cam()->dir.z * cam()->dist) +
					(cam()->up.z * y)) + (cam()->right.z * x);
	cam()->vpul.w = 0.0f;
}

/*
int			cam_key(int key, int status, void *data)
{
	t_ubmp		out;

	(void)data;
	if (status != FTX_KEY_STATUS_PRESSED)
		return (0);
	if (key == KEY_W)
		prim()[0][0].position.y += 10;
	if (key == KEY_Q)
		prim()[0][0].position.z += 10;
	if (key == KEY_E)
		prim()[0][0].position.z -= 10;
	if (key == KEY_S)
		prim()[0][0].position.y -= 10;
	if (key == KEY_D)
		prim()[0][0].position.x -= 10;
	if (key == KEY_A)
		prim()[0][0].position.x += 10;
	//if (key == KEY_SPACE)
	//	cam()->pos.y += 100;
	//if (key == KEY_SHIFT_LEFT)
	//	cam()->pos.y -= 100;
	out.size = ft_point(SW, SH);
	out.data = (int*)ft_memalloc(sizeof(int) * SW * SH);
	ftocl_clear_current_kernel_arg(2);
	ftocl_set_current_kernel_arg(CL_MEM_READ_ONLY, 2, sizeof(t_primitive) *
		argn()->nb_objects, (void*)*prim());
	ftocl_start_current_kernel(1, (const size_t[1]){SW * SH}, NULL);
	ftocl_read_current_kernel_arg(0, out.data);
	ftx_fill_image(ftx_data()->focused_window->vbuffer, 0x00FFFF, 1);
	ftx_put_ubmp_img(ftx_data()->focused_window->vbuffer, ft_point(0, 0), &out,
					NOMASK);
	ftx_refresh_window(ftx_data()->focused_window);
	return (0);
}
*/

/*
void		keys(void)
{
	ftx_key_hook(KEY_W, cam_key, NULL);
	ftx_key_hook(KEY_A, cam_key, NULL);
	ftx_key_hook(KEY_S, cam_key, NULL);
	ftx_key_hook(KEY_D, cam_key, NULL);
	ftx_key_hook(KEY_Q, cam_key, NULL);
	ftx_key_hook(KEY_E, cam_key, NULL);
//	ftx_key_hook(KEY_SPACE, cam_key, NULL);
//	ftx_key_hook(KEY_SHIFT_LEFT, cam_key, NULL);
}
*/

int			keys(t_ftx_data *data)
{
	t_ubmp		out;

	(void)data;
	if (data->keymap[KEY_W].status == FTX_KEY_STATUS_PRESSED)
		prim()[0][0].position.y += 10;
	if (data->keymap[KEY_S].status == FTX_KEY_STATUS_PRESSED)
		prim()[0][0].position.y -= 10;
	if (data->keymap[KEY_D].status == FTX_KEY_STATUS_PRESSED)
		prim()[0][0].position.x += 10;
	if (data->keymap[KEY_A].status == FTX_KEY_STATUS_PRESSED)
		prim()[0][0].position.x -= 10;
	if (data->keymap[KEY_Q].status == FTX_KEY_STATUS_PRESSED)
		prim()[0][0].position.z += 10;
	if (data->keymap[KEY_E].status == FTX_KEY_STATUS_PRESSED)
		prim()[0][0].position.z -= 10;
	out.size = ft_point(SW, SH);
	out.data = (int*)ft_memalloc(sizeof(int) * SW * SH);
	ftocl_clear_current_kernel_arg(2);
	ftocl_set_current_kernel_arg(CL_MEM_READ_ONLY, 2, sizeof(t_primitive) *
		argn()->nb_objects, (void*)*prim());
	ftocl_start_current_kernel(1, (const size_t[1]){SW * SH}, NULL);
	ftocl_read_current_kernel_arg(0, out.data);
	ftx_fill_image(ftx_data()->focused_window->vbuffer, 0x00FFFF, 1);
	ftx_put_ubmp_img(ftx_data()->focused_window->vbuffer, ft_point(0, 0), &out,
					NOMASK);
	ftx_refresh_window(ftx_data()->focused_window);

	return (0);
}

void		rtv1(void)
{
	t_ubmp		out;
	cl_event	event;

	argn()->screen_size = (cl_int2){.x = SW, .y = SH};
	argn()->nb_objects = 3;
	argn()->nb_lights = 3;
	*prim() = (t_primitive*)ft_malloc(sizeof(t_primitive) * argn()->nb_objects);
	*lights() = (t_light*)ft_malloc(sizeof(t_light) * argn()->nb_lights);
	prim()[0][0] = sphere((cl_float4){.x = 150, .y = 0, .z = 500, .w = 0}, 50, (cl_float4){.x = 1, .y = 1, .z = 0, .w = 0});
	prim()[0][1] = sphere((cl_float4){.x = -150, .y = 0, .z = 500, .w = 0}, 100, (cl_float4){.x = 1, .y = 0, .z = 0, .w = 0});
	prim()[0][2] = plane((cl_float4){.x = 0, .y = -200, .z = 0, .w = 0}, (cl_float4){.x = 0, .y = 1, .z = 0, .w = 0}, (cl_float4){.x = 1, .y = 1, .z = 1, .w = 0});
	lights()[0][0] = light((cl_float4){.x = 0, .y = 0, .z = 0, .w = 0},  (cl_float4){.x = 1, .y = 1, .z = 1, .w = 0});
	lights()[0][1] = light((cl_float4){.x = 0, .y = 300, .z = 600, .w = 0}, (cl_float4){.x = 1, .y = 1, .z = 1, .w = 0});
	lights()[0][2] = light((cl_float4){.x = 0, .y = 30, .z = 500, .w = 0}, (cl_float4){.x = 1, .y = 1, .z = 1, .w = 0});
	cam()->pos = (cl_float4){.x = 0, .y = 0, .z = 0, .w = 0};
	cam()->vp_size = (cl_int2){.x = SW, .y = SH};
	cam()->dist = 800;
	calc_vpul();
	out.size = ft_point(SW, SH);
	out.data = (int*)ft_malloc(sizeof(int) * SW * SH);
	update_kernel_args();
	event = ftocl_start_current_kernel(1, (const size_t[1]){SW * SH}, NULL);
	clWaitForEvents(1, &event);
	clReleaseEvent(event);
	ftocl_read_current_kernel_arg(0, out.data);
	ftx_new_window(ft_point(SW, SH), "test", NULL);
	ftx_put_ubmp_img(ftx_data()->focused_window->vbuffer, ft_point(0, 0), &out,
					NOMASK);
	printf("Kernel: %.8s was succesfully loaded\n",
				(char*)&ftocl_data()->current_kernel->id);
	ftx_key_hook(KEY_EXIT, cb_exit, NULL);
	//keys();
	ftx_loop_hook(&keys);
	ftx_set_cursor(ftx_data()->focused_window->vbuffer, 10, 10);
	ftx_write(ftx_data()->focused_window->vbuffer, "thing\ntruc", 10, 0xFFFFFF);
	ftx_refresh_window(ftx_data()->focused_window);
	ftx_start();
}

int			main(const int argc, char **argv, char **env)
{
	int		fd;

	ft_init(env);
	if (argc != 2)
		ft_end(0 * printf("\nUsage: \t%s/%s <scene.json>\n\n", ft_pwd(),
										ft_path_name(argv[0])));
	if ((fd = open(OCL_SOURCE_PATH, O_RDONLY)) == -1)
		ft_end(-1);
	ftocl_make_program(ft_str_to_id64("rtv1"),
		ft_str_clear_commentaries(ft_readfile(fd)), NULL);
	close(fd);
	if (!(fd = ftocl_set_current_kernel(ft_str_to_id64("example"))))
		rtv1();
	if (fd == 1)
		ft_end(0 * printf("kernel was not found\n"));
	ft_end(0);
}
