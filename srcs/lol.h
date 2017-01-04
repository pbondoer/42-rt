#ifndef FRACTOL_H
# define FRACTOL_H

# include "mlx.h"
# include "key_define.h"
# include "libft.h"
# include <pthread.h>
# include <time.h>
# include <fcntl.h>
# include <math.h>

# define W_W 1000
# define W_H 1000
# define M_DEPTH 1000


typedef struct		s_image
{
	int				width;
	int				height;
	char			*buffer;
	float			vertexes[8];
	struct s_image	*next;
}					t_image;

typedef struct		s_ray
{
	double			x;
	double			y;
	double			z;
	double			i;
}					t_ray;

typedef struct		s_sfr
{
	int				x;
	int				y;
	int				z;
	int				r;
}					t_sfr;

typedef struct		s_env
{
	t_sfr			sfr;
	t_ray			lum;
	int				push[270];
	void			*mlxp;
	void			*mlxw;
	t_image			*mlxi;
	int				lolx;
	int				loly;
}					t_env;

int		loop(void *venv);
int		key_on(int key, void *venv);
int		key_off(int key, void *venv);
void	graf(t_env *env);
void	put_pixel_to_img(t_env *env, unsigned int color, int x, int y);


#endif