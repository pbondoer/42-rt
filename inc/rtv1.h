/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rtv1.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/15 00:05:50 by hmartzol          #+#    #+#             */
/*   Updated: 2017/01/27 09:41:51 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RTV1_H
# define RTV1_H

# include <libft.h>
# include <libftx.h>
# include <libftocl.h>
# include <libftjson.h>

# define OCL_SOURCE_PATH "./scl/rtv1.cl"

typedef enum		e_prim_type
{
	INVALID = -1, SPHERE = 0, PLANE = 1, CONE = 2, CYLINDER = 3
}					t_prim_type;

typedef struct		s_material
{
	cl_float4		color;
	cl_float		diffuse;
	cl_float		specular;
}					t_material;

typedef struct		s_pair
{
	char			*name;
	cl_ulong		index;
}					t_pair;

typedef struct		s_material_holder
{
	int				nb_materials;
	char			**name;
	t_material		*materials;
}					t_material_holder;

typedef struct		s_camera
{
	cl_float4		pos;
	cl_float4		dir;
	cl_float4		up;
	cl_float4		right;
	cl_float4		vpul;
	cl_float2		vp_size;
	float			dist;
	t_quaternion	orientation;
	t_vector		origin_dir;
	t_vector		origin_up;
	t_vector		origin_right;
}					t_camera;

typedef struct		s_primitive
{
	t_prim_type		type;
	cl_float4		position;
	cl_float4		direction;
	cl_float		radius;
	cl_uint			material;
}					t_primitive;

typedef struct		s_light
{
	cl_float4		position;
	cl_float4		color;
}					t_light;

typedef struct		s_argn
{
	cl_int2			screen_size;
	cl_int			nb_objects;
	cl_int			nb_lights;
	cl_float		ambient;
	cl_float		direct;
	cl_int			nb_materials;
}					t_argn;

void				rtv1(void);
t_camera			*cam(void);
void				rotate_cam(double angle, t_vector axe);
void				calc_vpul(void);
void				update_kernel_args(void);
t_primitive			**prim(void);
t_argn				*argn(void);
t_light				**lights(void);
void				parser(const char *src);
cl_float4			cl_vector_from_json_array(t_json_value *node,
						cl_float4 default_return);
t_material_holder	*materials(void);
t_material			default_material(void);
void				parse_camera(t_json_value *c);
int					check_parsed_data(void);
void				parse_lights(t_json_value *l);
void				parse_objects(t_json_value *o);
void				parse_render_options(t_json_value *ro);
void				*parse_materials(t_json_value *m);
t_material			parse_material(t_json_value *m, t_material out);
cl_float4			cl_float4_add(cl_float4 a, cl_float4 b);
void				cl_float4_p_add(cl_float4 *a, cl_float4 b);
cl_float4			cl_float4_scale(cl_float4 v, cl_float s);
cl_float4			cl_float4_sub(cl_float4 a, cl_float4 b);
void				cl_float4_p_sub(cl_float4 *a, cl_float4 b);

#endif
