/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_render_options.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/26 01:56:45 by hmartzol          #+#    #+#             */
/*   Updated: 2017/01/26 10:20:11 by pbondoer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <rtv1.h>

void				parse_render_options(t_json_value *ro)
{
	t_json_value		*v[2];
	t_json_value_type	*la_norm_est_chiante;

	v[0] = ft_json_search_pair_in_object(ro,
		(t_json_string){.length = 4, .ptr = "size"});
	la_norm_est_chiante = (t_json_value_type[2]){integer, integer};
	ft_json_check_array_types(v[0], 2, la_norm_est_chiante) ?
		(void)(argn()->screen_size = (cl_int2){.x =
		(cl_int) * (int*)((t_json_array*)v[0]->ptr)->value[0]->ptr, .y =
		(cl_int) * (int*)((t_json_array*)v[0]->ptr)->value[1]->ptr}) : 0;
	v[0] = ft_json_search_pair_in_object(ro,
		(t_json_string){.length = 8, .ptr = "lighting"});
	v[1] = ft_json_search_pair_in_object(v[0],
		(t_json_string){.length = 7, .ptr = "ambient"});
	(v[1] != NULL && v[1]->type == number) ? argn()->ambient =
		ft_clampd(*(double*)v[1]->ptr, 0.0, 1.0) : 0;
}
