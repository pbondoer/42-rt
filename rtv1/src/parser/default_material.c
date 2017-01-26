/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   default_material.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/26 02:00:39 by hmartzol          #+#    #+#             */
/*   Updated: 2017/01/26 10:24:18 by pbondoer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <rtv1.h>

t_material			default_material(void)
{
	t_material	out;

	out.color = (cl_float4){.x = 1.0f, .y = 1.0f, .z = 1.0f, .w = 0.0f};
	out.diffuse = 1.0f;
	out.specular = 1.0f;
	return (out);
}
