/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rtv1.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/15 00:05:50 by hmartzol          #+#    #+#             */
/*   Updated: 2017/01/06 18:29:49 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RTV1_H
# define RTV1_H

# include <libft.h>
# include <libftx.h>
# include <libftocl.h>

# define OCL_SOURCE_PATH "./scl/test.cl"

# define SW 1000
# define SH 1000

typedef enum	e_prim_type
{
	NONE = 0, SPHERE = 1, PLANE = 2, CONE = 3, CYLINDER = 4
}				t_prim_type;

/*
** note: the quaternion part of camera is discarded on GPU side
** (copied but not accessible via struct camera)
*/

typedef struct	s_camera
{
	cl_float4		pos;
	cl_float4		dir;
	cl_float4		up;
	cl_float4		right;
	t_quaternion	orientation;
}				t_camera;

typedef struct	s_primitive
{
	t_prim_type		type;
	cl_float4		position;
	cl_float4		normal;
	cl_float4		color;
	cl_float		radius;
}				t_primitive;

typedef struct	s_light
{
	cl_float4		position;
	cl_float4		color;
}				t_light;

typedef struct	s_argn
{
	cl_int2			screen_size;
	cl_int			nb_objects;
	cl_int			nb_lights;
}				t_argn;

cl_float4		*ray_plane(const t_camera *cam, const cl_int2 screen_size);
t_camera		*cam(void);
void			update_kernel_args(void);
t_primitive		**prim(void);
t_argn			*argn(void);
t_light			**lights(void);

#endif
