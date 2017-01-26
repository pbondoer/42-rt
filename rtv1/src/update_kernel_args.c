/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_kernel_args.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/03 14:38:52 by hmartzol          #+#    #+#             */
/*   Updated: 2017/01/26 10:14:02 by pbondoer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <rtv1.h>
#include <stdio.h>

void		update_kernel_args(void)
{
	argn()->nb_materials = materials()->nb_materials;
	ftocl_clear_current_kernel_arg(0);
	ftocl_set_current_kernel_arg(CL_MEM_WRITE_ONLY, 0, sizeof(cl_int) *
		argn()->screen_size.x * argn()->screen_size.y, NULL);
	ftocl_clear_current_kernel_arg(1);
	ftocl_set_current_kernel_arg(CL_MEM_READ_ONLY, 1, sizeof(t_argn),
								(void*)argn());
	ftocl_clear_current_kernel_arg(2);
	ftocl_set_current_kernel_arg(CL_MEM_READ_ONLY, 2, sizeof(t_primitive) *
		argn()->nb_objects, (void*)*prim());
	ftocl_clear_current_kernel_arg(3);
	ftocl_set_current_kernel_arg(CL_MEM_READ_ONLY, 3, sizeof(t_light) *
		argn()->nb_lights, (void*)*lights());
	ftocl_clear_current_kernel_arg(4);
	ftocl_set_current_kernel_arg(CL_MEM_READ_ONLY, 4, sizeof(t_camera),
								(void*)cam());
	ftocl_clear_current_kernel_arg(5);
	ftocl_set_current_kernel_arg(CL_MEM_READ_ONLY, 5, sizeof(t_material) *
				argn()->nb_materials, (void*)materials()->materials);
}
