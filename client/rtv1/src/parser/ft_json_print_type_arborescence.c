/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_json_print_type_arborescence.c                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/26 02:04:56 by hmartzol          #+#    #+#             */
/*   Updated: 2017/01/26 02:14:53 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <rtv1.h>
#include <stdio.h>

void	sf_json_print_type_arborescence_0(t_json_value *v, int depth)
{
	unsigned long	i;
	int				l;
	t_json_object	*obj;

	if (v->type == object && (obj = (t_json_object*)v->ptr) != NULL &&
		obj->nb_pairs != 0 && obj->pair != NULL && (i = -1))
		while (++i < obj->nb_pairs)
		{
			l = -1;
			while (++l < depth)
				write(1, "\t", 1);
			printf("%s\n", (obj->pair[i] != NULL && obj->pair[i]->key != NULL) ?
					obj->pair[i]->key->ptr : NULL);
			sf_json_print_type_arborescence(
				obj->pair[i] != NULL ? obj->pair[i]->value : NULL, depth);
		}
}

void	sf_json_print_type_arborescence(t_json_value *v, int depth)
{
	unsigned long	i;
	int				l;
	t_json_array	*ar;

	l = -1;
	while (++l < depth)
		write(1, "\t", 1);
	ft_json_print_type(v);
	++depth;
	if (v == NULL)
		return ;
	if (v->type == array && (ar = (t_json_array*)v->ptr) != NULL &&
		ar->nb_values != 0 && ar->value != NULL && (i = -1))
		while (++i < ar->nb_values)
			sf_json_print_type_arborescence(ar->value[i], depth);
	sf_json_print_type_arborescence_0(v, depth);
}

void	ft_json_print_type_arborescence(t_json_value *root)
{
	sf_json_print_type_arborescence(root, 0);
}
