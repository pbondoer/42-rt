/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_kernel_args.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/03 14:38:52 by hmartzol          #+#    #+#             */
/*   Updated: 2017/01/26 04:18:31 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <rtv1.h>
#include <stdio.h>

void		update_kernel_args(void)
{
	printf("lights %p\n", *lights());
	argn()->nb_info = textures_holder()->nb_info;
	argn()->nb_materials = materials()->nb_materials;
	printf("0\n");
		printf("lights %p\n", *lights());
	ftocl_clear_current_kernel_arg(0);
	ftocl_set_current_kernel_arg(CL_MEM_WRITE_ONLY, 0, sizeof(cl_int) *
		argn()->screen_size.x * argn()->screen_size.y, NULL);
	printf("1\n");
		printf("lights %p\n", *lights());
	ftocl_clear_current_kernel_arg(1);
	ftocl_set_current_kernel_arg(CL_MEM_READ_ONLY, 1, sizeof(t_argn),
								(void*)argn());
	printf("2\n");
		printf("lights %p\n", *lights());
	ftocl_clear_current_kernel_arg(2);
	ftocl_set_current_kernel_arg(CL_MEM_READ_ONLY, 2, sizeof(t_primitive) *
		argn()->nb_objects, (void*)*prim());
	printf("3\n");
	ftocl_clear_current_kernel_arg(3);
	printf("lights %p\n", *lights());
	ftocl_set_current_kernel_arg(CL_MEM_READ_ONLY, 3, sizeof(t_light) *
		argn()->nb_lights, (void*)*lights());
	printf("4\n");
	ftocl_clear_current_kernel_arg(4);
	ftocl_set_current_kernel_arg(CL_MEM_READ_ONLY, 4, sizeof(t_camera),
								(void*)cam());
	printf("5\n");
	ftocl_clear_current_kernel_arg(5);
	ftocl_set_current_kernel_arg(CL_MEM_READ_ONLY, 5, sizeof(t_img_info) *
					argn()->nb_info, (void*)textures_holder()->info);
	printf("6\n");
	ftocl_clear_current_kernel_arg(6);
	ftocl_set_current_kernel_arg(CL_MEM_READ_ONLY, 6, sizeof(t_material) *
				argn()->nb_materials, (void*)materials()->materials);
	printf("7\n");
	ftocl_clear_current_kernel_arg(7);
	ftocl_set_current_kernel_arg(CL_MEM_READ_ONLY, 7, sizeof(cl_int) *
		textures_holder()->total_raw_size, (void*)textures_holder()->raw_bmp);
}
