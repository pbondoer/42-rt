/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/21 19:24:09 by hmartzol          #+#    #+#             */
/*   Updated: 2017/01/26 10:53:53 by pbondoer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <rtv1.h>

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
}
