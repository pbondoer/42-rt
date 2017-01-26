/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_json_check_array_types.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/22 03:12:26 by hmartzol          #+#    #+#             */
/*   Updated: 2017/01/25 10:27:47 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_json.h>
#include <stdlib.h>

int	ft_json_check_array_types(const t_json_value *node,
					const unsigned long size, const t_json_value_type *types)
{
	unsigned long	i;
	t_json_array	*ar;

	if (node == NULL || types == NULL || node->type != array ||
		(ar = (t_json_array*)node->ptr) == NULL ||
		ar->nb_values != size || ar->value == NULL)
		return (0);
	i = -1;
	while (++i < ar->nb_values)
		if (ar->value[i] == NULL)
			return (0);
		else if ((ar->value[i]->type & types[i]) != ar->value[i]->type)
			return (0);
	return (1);
}
