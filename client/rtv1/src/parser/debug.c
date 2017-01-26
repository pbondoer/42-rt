/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/26 01:54:25 by hmartzol          #+#    #+#             */
/*   Updated: 2017/01/26 02:06:52 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <rtv1.h>
#include <stdio.h>

void				print_argn(void)
{
	printf("screen_size: %d, %d\n", argn()->screen_size.x,
									argn()->screen_size.y);
	printf("nb_objects: %d\n", argn()->nb_objects);
	printf("nb_lights: %d\n", argn()->nb_lights);
	printf("ambient: %f\n", argn()->ambient);
	printf("direct: %f\n", argn()->direct);
	printf("global: %f\n", argn()->global);
	printf("global_illumination_samples: %d\n",
			argn()->global_illumination_samples);
	printf("antialias: %d\n", argn()->antialias);
	printf("bounce_depth: %d\n", argn()->bounce_depth);
	printf("filter: %d\n", argn()->filter);
	printf("stereoscopy: %d\n", argn()->stereoscopy);
}

void				print_cam(void)
{
	printf("pos: %f, %f, %f\n", cam()->pos.x, cam()->pos.y, cam()->pos.z);
	printf("dir: %f, %f, %f\n", cam()->dir.x, cam()->dir.y, cam()->dir.z);
	printf("up: %f, %f, %f\n", cam()->up.x, cam()->up.y, cam()->up.z);
	printf("right: %f, %f, %f\n", cam()->right.x, cam()->right.y, cam()->right.z);
	printf("vpul: %f, %f, %f\n", cam()->vpul.x, cam()->vpul.y, cam()->vpul.z);
	printf("vp_size: %f, %f\n", cam()->vp_size.x, cam()->vp_size.y);
	printf("dist: %f\n", cam()->dist);
	printf("orientation: %f, %f, %f, %f\n", cam()->orientation.r, cam()->orientation.i, cam()->orientation.j, cam()->orientation.k);
}

void				print_materials(void)
{
	for (int i = 0; i < materials()->nb_materials; ++i)
	{
		t_material	m = materials()->materials[i];
		printf("\nmaterial: %d\n", i);
		printf("color: %f, %f, %f, %f\n", m.color.x, m.color.y, m.color.z, m.color.w);
		printf("diffuse: %f\n", m.diffuse);
		printf("specular: %f\n", m.specular);
		printf("alpha: %f\n", m.alpha);
		printf("refraction: %f\n", m.refraction);
		printf("perturbation:\n");
		printf("normal: %d\n", m.perturbation.normal);
		printf("color: %d\n", m.perturbation.color);
		printf("texture index: %llu\n", m.texture.info_index);
		printf("texture stretch: %f, %f\n", m.texture.stretch.x, m.texture.stretch.y);
		printf("bumpmap index: %llu\n", m.bumpmap.info_index);
		printf("bumpmap stretch: %f, %f\n", m.bumpmap.stretch.x, m.texture.stretch.y);
	}
}

void				print_objects(void)
{
	printf("\n\nnb_objects: %d\n\n", argn()->nb_objects);
	for (int i = 0; i < argn()->nb_objects; ++i)
	{
		t_primitive p = (*prim())[i];
		printf("\ntype: %d\n", p.type);
		printf("position: %f, %f, %f, %f\n", p.position.x, p.position.y, p.position.z, p.position.w);
		printf("direction: %f, %f, %f, %f\n", p.direction.x, p.direction.y, p.direction.z, p.direction.w);
		printf("radius: %f\n", p.radius);
		printf("material: %u\n", p.material);
		printf("limit:\n");
		printf("relative: %d\n", p.limit.relative);
		printf("high: %f, %f, %f, %f\n", p.limit.high.x, p.limit.high.y, p.limit.high.z, p.limit.high.w);
		printf("low: %f, %f, %f, %f\n", p.limit.low.x, p.limit.low.y, p.limit.low.z, p.limit.low.w);
	}
}

void				print_lights(void)
{
	printf("\n\nnb_lights: %d\n\n", argn()->nb_lights);
	for (int i = 0; i < argn()->nb_lights; ++i)
	{
		t_light p = (*lights())[i];
		printf("position: %f, %f, %f, %f\n", p.position.x, p.position.y, p.position.z, p.position.w);
		printf("color: %f, %f, %f, %f\n", p.color.x, p.color.y, p.color.z, p.color.w);
	}
}
