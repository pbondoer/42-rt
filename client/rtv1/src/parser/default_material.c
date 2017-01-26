/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   default_material.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/26 02:00:39 by hmartzol          #+#    #+#             */
/*   Updated: 2017/01/26 02:00:54 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <rtv1.h>

t_material			default_material(void)
{
	t_material	out;

	out.color = (cl_float4){.x = 255.0f, .y = 255.0f, .z = 255.0f, .w = 0.0f};
	out.diffuse = 1.0f;
	out.specular = 1.0f;
	out.refraction = 0.0f;
	out.alpha = 1.0f;
	out.refraction = 1.0f;
	out.perturbation.normal = NONE;
	out.perturbation.color = NONE;
	out.texture.info_index = 0;
	out.texture.stretch = (cl_float2){.x = 100.0f, .y = 100.0f};
	out.bumpmap.info_index = 0;
	out.bumpmap.stretch = (cl_float2){.x = 100.0f, .y = 100.0f};
	return (out);
}
