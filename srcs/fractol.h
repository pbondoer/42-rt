/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmarot <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/14 21:02:30 by hmarot            #+#    #+#             */
/*   Updated: 2016/11/16 23:41:43 by hmarot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FRACTOL_H
# define FRACTOL_H
# include "mlx.h"
# include "libft.h"
# include "printf.h"
# include "key_define.h"
# include <pthread.h>
# include <time.h>
# include <fcntl.h>
# include <math.h>

# define W_W 1000
# define W_H 1000

typedef struct		s_fract
{
	double	z_r;
	double	z_i;
	double	c_r;
	double	c_i;
	double	tmp;
}					t_fract;

typedef struct		s_image
{
	int				width;
	int				height;
	char			*buffer;
	float			vertexes[8];
	struct s_image	*next;
}					t_image;

typedef	struct		s_graf
{
	void			*mlx_p;
	void			*mlx_w;
	t_image			*mlx_i;
	int				color;
	double			pad;
	unsigned int	start;
	float			zoom;
}					t_graf;

typedef struct		s_genv
{
	int				fractal;
	t_graf			*graf;
	char			push[270];
	int				iter;
	int				lock;
	int				mx;
	int				my;
	double			posx;
	double			posy;
	double			time;
	int				is_time;
}					t_genv;

/*
** --- main.c
*/

void				fractol_init(t_genv *genv, char *frac);
t_graf				*fractol_init_graf(char *name);
void				ft_error_fract(int code, t_genv *genv);

/*
** --- fractol_loop.c
*/

int					main_loop(void *vgenv);
void				check_button(t_genv *genv);

/*
** --- key.c
*/

int					key_on(int key, void *vgenv);
int					key_off(int key, void *vgenv);

/*
** --- graf.c
*/

void				put_pixel_to_img(t_graf *graf, unsigned int
	color, int x, int y);
unsigned int		get_color(unsigned int i);
void				get_color_bis(unsigned char *red, unsigned char
	*green, unsigned char *blue, unsigned int i);

/*
** --- mandel.c
*/

void				man_init(t_fract *f, t_genv *genv, int x, int y);
void				mandel(t_genv *genv);

/*
** --- julia.c
*/

void				jul_init(t_fract *f, t_genv *genv, int x, int y);
void				julia(t_genv *genv);

/*
** --- jubis.c
*/

void				jub_init(t_fract *f, t_genv *genv, int x, int y);
void				jubis(t_genv *genv);

/*
** --- burn.c
*/

void				bur_init(t_fract *f, t_genv *genv, int x, int y);
void				burning(t_genv *genv);

/*
** --- mouse.c
*/

int					mouse(int x, int y, t_genv *genv);
int					mouse_m(int button, int x, int y, t_genv *genv);

#endif
