/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_texture.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/26 02:17:24 by hmartzol          #+#    #+#             */
/*   Updated: 2017/01/26 02:23:55 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <rtv1.h>

void				parse_texture_0(t_texture *out, t_json_value *v,
									char buff[PATH_MAX])
{
	v = ft_json_search_pair_in_object(v,
		(t_json_string){.length = 4, .ptr = "file"});
	out->info_index = -1;
	if (v != NULL && v->type == string && v->ptr != NULL)
		while (++out->info_index < (unsigned long)textures_holder()->nb_info)
			if (!ft_strcmp(textures_holder()->path[out->info_index],
					ft_realpath((char*)v->ptr, buff)))
				break ;
}

t_texture			parse_texture(t_json_value *t, t_texture default_return)
{
	t_json_value		*v;
	t_json_value_type	*paf_paf_paf_la_norm;
	t_texture			out;
	char				buff[PATH_MAX];

	out = default_return;
	v = ft_json_search_pair_in_object(t,
		(t_json_string){.length = 7, .ptr = "stretch"});
	paf_paf_paf_la_norm =
		(t_json_value_type[2]){integer | number, integer | number};
	if (ft_json_check_array_types(v, 2, paf_paf_paf_la_norm))
	{
		out.stretch.x = ((t_json_array*)v->ptr)->value[0]->type == integer ?
			(cl_float) * (int*)((t_json_array*)v->ptr)->value[0]->ptr :
			(cl_float) * (double*)((t_json_array*)v->ptr)->value[0]->ptr;
		out.stretch.y = ((t_json_array*)v->ptr)->value[1]->type == integer ?
			(cl_float) * (int*)((t_json_array*)v->ptr)->value[1]->ptr :
			(cl_float) * (double*)((t_json_array*)v->ptr)->value[1]->ptr;
	}
	parse_texture_0(&out, t, buff);
	return (out);
}
