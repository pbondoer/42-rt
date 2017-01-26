/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/09 09:15:54 by hmartzol          #+#    #+#             */
/*   Updated: 2017/01/26 11:09:13 by pbondoer         ###   ########.fr       */
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
	static t_ubmp		out = {.size = {0, 0}, .data = NULL};
	size_t				size;

	if (u <= 0)
		return ;
	out.size.x = argn()->screen_size.x;
	out.size.y = argn()->screen_size.y;
	size = argn()->screen_size.x * argn()->screen_size.y;
	if (out.data == NULL)
		out.data = (int*)ft_memalloc(sizeof(int) * out.size.x * out.size.y);
	ftocl_clear_current_kernel_arg(4);
	ftocl_set_current_kernel_arg(CL_MEM_READ_ONLY, 4, sizeof(t_camera),
			(void*)cam());
	ftocl_start_current_kernel(1, &size, NULL);
	ftocl_read_current_kernel_arg(0, out.data);
	ftx_put_ubmp_img(ftx_data()->focused_window->vbuffer, ft_point(0, 0), &out,
			NOMASK);
	ftx_refresh_window(ftx_data()->focused_window);
}

int			keys(t_ftx_data *data)
{
	int i;

	i = 0;
	if (data->keymap[KEY_W].status == FTX_KEY_STATUS_PRESSED && ++i)
		cam()->pos.y += 10;
	if (data->keymap[KEY_S].status == FTX_KEY_STATUS_PRESSED && ++i)
		cam()->pos.y -= 10;
	if (data->keymap[KEY_D].status == FTX_KEY_STATUS_PRESSED && ++i)
		cam()->pos.x += 10;
	if (data->keymap[KEY_A].status == FTX_KEY_STATUS_PRESSED && ++i)
		cam()->pos.x -= 10;
	if (data->keymap[KEY_Q].status == FTX_KEY_STATUS_PRESSED && ++i)
		cam()->pos.z += 10;
	if (data->keymap[KEY_E].status == FTX_KEY_STATUS_PRESSED && ++i)
		cam()->pos.z -= 10;
	if (data->keymap[KEY_LEFT].status == FTX_KEY_STATUS_PRESSED && ++i)
		rotate_cam(-0.05, ft_vector(0, 1, 0));
	if (data->keymap[KEY_RIGHT].status == FTX_KEY_STATUS_PRESSED && ++i)
		rotate_cam(0.05, ft_vector(0, 1, 0));
	if (data->keymap[KEY_UP].status == FTX_KEY_STATUS_PRESSED && ++i)
		rotate_cam(-0.05, ft_vector(1, 0, 0));
	if (data->keymap[KEY_DOWN].status == FTX_KEY_STATUS_PRESSED && ++i)
		rotate_cam(0.05, ft_vector(1, 0, 0));
	update(i);
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

int			main(const int argc, char **argv, char **env)
{
	int		fd;
	char	*src;

	ft_init(env);
	if (argc != 2)
		ft_end(0 * ft_printf("\nUsage: \t%s/%s <scene.json>\n\n", ft_pwd(),
					ft_path_name(argv[0])));
	if ((fd = open(argv[1], O_RDONLY)) == -1)
		ft_end(-1);
	parser(src = ft_readfile(fd));
	close(fd);
	ft_free(src);
	if ((fd = open(OCL_SOURCE_PATH, O_RDONLY)) == -1)
		ft_end(-1);
	ftocl_make_program(ft_str_to_id64("rtv1"),
			src = ft_str_clear_commentaries(ft_readfile(fd)), NULL);
	close(fd);
	if (!(fd = ftocl_set_current_kernel(ft_str_to_id64("raytracer"))))
		rtv1();
	ft_free(src);
	if (fd == 1)
		ft_end(0 * ft_printf("kernel was not found\n"));
	ft_end(0);
}
