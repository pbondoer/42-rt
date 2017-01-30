/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbondoer <pbondoer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/30 13:34:41 by pbondoer          #+#    #+#             */
/*   Updated: 2017/01/30 14:36:37 by pbondoer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <rt.h>

t_mouse *mouse()
{
	static t_mouse mouse = (t_mouse){.is_select = 0, .x = 0, .y = 0};
	return (&mouse);
}

int	mouse_click(int key, int x, int y, void *data)
{
	int check;

	(void)data;
	if (!(x >= 0 && y >= 0 && x < prim_map()->size.x && y < prim_map()->size.y))
		return (0);
	check = prim_map()->data[prim_map()->size.x * y + x];
	if (check > 0 && key == 4)
		prim()[0][check - 1].radius *= 1.04f;
	if (check > 0 && key == 5)
		prim()[0][check - 1].radius /= 1.04f;
	if (check > 0 && key == 1)
		mouse()->is_select = check;
	ft_printf("%d\n", mouse()->is_select);
	ftocl_clear_current_kernel_arg(3);
	ftocl_set_current_kernel_arg(CL_MEM_READ_ONLY, 3, sizeof(t_primitive) *
		argn()->nb_objects, (void*)*prim());
	update(1);
	return (0);
}

int mouse_off(int key, int x, int y, void *data)
{
	(void)x;
	(void)y;
	(void)data;
	if (key == 1)
		mouse()->is_select = 0;
	return (0);
}

int mouse_move(int x, int y, void *data)
{
	static int upd = 0;
	if (!mouse()->is_select)
		return(0);
	(void)data;
	upd++;
	if (upd == UPD)
	{
		ft_printf("x = %f, y = %f\n",prim()[0][mouse()->is_select - 1].position.x,prim()[0][mouse()->is_select - 1].position.y);
		prim()[0][mouse()->is_select - 1].position = cl_float4_add(cam()->pos,
		ft_vector_thales(cam()->pos,
		 cl_float4_add(cl_float4_add(cl_float4_scale(cam()->right, (cl_float)(mouse()->x - 500)), cl_float4_scale(cam()->up, -(cl_float)(mouse()->y - 500))), cl_float4_scale(cam()->dir, cam()->dist)),
		  cl_float4_sub(prim()[0][mouse()->is_select -1].position, cam()->pos),
		  cl_float4_add(cl_float4_add(cl_float4_scale(cam()->right, (cl_float)(x - 500)), cl_float4_scale(cam()->up, -(cl_float)(y - 500))), cl_float4_scale(cam()->dir, cam()->dist))));
		upd = 0;
		mouse()->x = x;
		mouse()->y = y;
	ftocl_clear_current_kernel_arg(3);
	ftocl_set_current_kernel_arg(CL_MEM_READ_ONLY, 3, sizeof(t_primitive) *
		argn()->nb_objects, (void*)*prim());
	update(1);
	}
	return(0);
}
