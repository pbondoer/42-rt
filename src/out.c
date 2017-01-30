/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   out.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbondoer <pbondoer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/30 13:14:16 by pbondoer          #+#    #+#             */
/*   Updated: 2017/01/30 14:22:57 by pbondoer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <rt.h>

t_ubmp	*out()
{
	static t_ubmp		out = {.size = {.x = 0, .y = 0}, .data = NULL};

	return (&out);
}

t_ubmp	*prim_map()
{
	static t_ubmp		out = {.size = {.x = 0, .y = 0}, .data = NULL};

	return (&out);
}
