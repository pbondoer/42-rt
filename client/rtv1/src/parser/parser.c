/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/21 19:24:09 by hmartzol          #+#    #+#             */
/*   Updated: 2017/01/26 10:45:13 by pbondoer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <rtv1.h>
#include <stdio.h>

void				parser(const char *src)
{
	t_json_value	*root;
	t_json_value	*v;

	root = ft_json_parse_src(ft_str_clear_commentaries((char*)src));
	v = ft_json_search_pair_in_object(root,
		(t_json_string){.length = 14, .ptr = "render_options"});
	parse_render_options(v);
	v = ft_json_search_pair_in_object(root,
		(t_json_string){.length = 6, .ptr = "camera"});
	parse_camera(v);
	v = ft_json_search_pair_in_object(root,
		(t_json_string){.length = 9, .ptr = "materials"});
	parse_materials(v);
	v = ft_json_search_pair_in_object(root,
		(t_json_string){.length = 7, .ptr = "objects"});
	parse_objects(v);
	v = ft_json_search_pair_in_object(root,
		(t_json_string){.length = 6, .ptr = "lights"});
	parse_lights(v);
	if (!check_parsed_data())
		ft_error(EINTERN, "Invalid json values\n");
	ft_json_free(root);


	printf("\n\nnb_objects: %d\n\n", argn()->nb_objects);
	for (int i = 0; i < argn()->nb_objects; ++i)
	{
		t_primitive p = (*prim())[i];
		printf("\ntype: %d\n", p.type);
		printf("position: %f, %f, %f, %f\n", p.position.x, p.position.y, p.position.z, p.position.w);
		printf("direction: %f, %f, %f, %f\n", p.direction.x, p.direction.y, p.direction.z, p.direction.w);
		printf("radius: %f\n", p.radius);
		printf("material: %u\n", p.material);
	}
}
