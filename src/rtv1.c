/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rtv1.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/27 09:40:29 by hmartzol          #+#    #+#             */
/*   Updated: 2017/01/27 09:44:31 by hmartzol         ###   ########.fr       */
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

void		update(int u)
{
	static t_ubmp		out = {.size = {.x = 0, .y = 0}, .data = NULL};
	size_t				size;

	if (u <= 0)
		return ;
	if (out.data == NULL)
	{
		out.size.x = argn()->screen_size.x;
		out.size.y = argn()->screen_size.y;
		if ((out.data =
			(int*)ft_memalloc(sizeof(int) * out.size.x * out.size.y)) == NULL)
		{
			ft_error(ENOMEM, "Couldn't allocate GPU's output memory handler\n");
			ft_end(0);
		}
	}
	size = out.size.x * out.size.y;
	ftocl_clear_current_kernel_arg(4);
	ftocl_set_current_kernel_arg(CL_MEM_READ_ONLY, 4, sizeof(t_camera),
			(void*)cam());
	ftocl_start_current_kernel(1, &size, NULL);
	ftocl_read_current_kernel_arg(0, out.data);
	ftx_put_ubmp_img(ftx_data()->focused_window->vbuffer, ft_point(0, 0), &out,
			NOMASK);
	ftx_refresh_window(ftx_data()->focused_window);
}

int			keys_0(t_ftx_data *data, int i)
{
	if (data->keymap[KEY_LEFT].status == FTX_KEY_STATUS_PRESSED && ++i)
		rotate_cam(-0.05, cam()->origin_up);
	if (data->keymap[KEY_RIGHT].status == FTX_KEY_STATUS_PRESSED && ++i)
		rotate_cam(0.05, cam()->origin_up);
	if (data->keymap[KEY_UP].status == FTX_KEY_STATUS_PRESSED && ++i)
		rotate_cam(-0.05, cam()->origin_right);
	if (data->keymap[KEY_DOWN].status == FTX_KEY_STATUS_PRESSED && ++i)
		rotate_cam(0.05, cam()->origin_right);
	if (data->keymap[KEY_Q].status == FTX_KEY_STATUS_PRESSED && ++i)
		rotate_cam(-0.05, cam()->origin_dir);
	if (data->keymap[KEY_E].status == FTX_KEY_STATUS_PRESSED && ++i)
		rotate_cam(0.05, cam()->origin_dir);
	return (i);
}

int			keys(t_ftx_data *data)
{
	int i;

	i = 0;
	if (data->keymap[KEY_SHIFT_LEFT].status == FTX_KEY_STATUS_PRESSED && ++i)
		cl_float4_p_sub(&cam()->pos, cl_float4_scale(cam()->up, 10));
	if (data->keymap[KEY_SPACE].status == FTX_KEY_STATUS_PRESSED && ++i)
		cl_float4_p_add(&cam()->pos, cl_float4_scale(cam()->up, 10));
	if (data->keymap[KEY_D].status == FTX_KEY_STATUS_PRESSED && ++i)
		cl_float4_p_add(&cam()->pos, cl_float4_scale(cam()->right, 10));
	if (data->keymap[KEY_A].status == FTX_KEY_STATUS_PRESSED && ++i)
		cl_float4_p_sub(&cam()->pos, cl_float4_scale(cam()->right, 10));
	if (data->keymap[KEY_W].status == FTX_KEY_STATUS_PRESSED && ++i)
		cl_float4_p_add(&cam()->pos, cl_float4_scale(cam()->dir, 10));
	if (data->keymap[KEY_S].status == FTX_KEY_STATUS_PRESSED && ++i)
		cl_float4_p_sub(&cam()->pos, cl_float4_scale(cam()->dir, 10));
	update(keys_0(data, i));
	return (0);
}

void		rtv1(void)
{
	calc_vpul();
	update_kernel_args();
	ftx_new_window(ft_point(argn()->screen_size.x, argn()->screen_size.y),
			"RTv1", NULL);
	ftx_key_hook(KEY_EXIT, cb_exit, NULL);
	update(1);
	ftx_loop_hook(&keys);
	ftx_start();
}
