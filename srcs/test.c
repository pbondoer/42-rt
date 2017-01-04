#include "lol.h"

int main(void)
{
	t_env	env;

	env.mlxp = mlx_init();
	env.mlxw = mlx_new_window(env.mlxp, W_W, W_H, "lapin");
	env.mlxi = (t_image *)mlx_new_image(env.mlxp, W_W, W_H);
	env.lolx = 0;
	env.loly = 0;
	env.sfr.x = 0;
	env.sfr.y = 0;
	env.sfr.z = -500;
	env.sfr.r = 100;
	env.lum.x = 0;
	env.lum.y = 500;
	env.lum.z = 0;
	env.lum.i = 1;
	ft_memset(env.push, 0, 270);
	mlx_hook(env.mlxw, 2, 1, key_on, &env);
	mlx_key_hook(env.mlxw, key_off, &env);
	mlx_loop_hook(env.mlxp, loop, &env);
	mlx_loop(env.mlxp);
	return(0);
}

int		loop(void *venv)
{
	t_env	*env;

	env = (t_env *)venv;
	env->push[UP] ? env->sfr.y+=10 : 0;
	env->push[DN] ? env->sfr.y-=10 : 0;
	env->push[LT] ? env->sfr.x+=10 : 0;
	env->push[RT] ? env->sfr.x-=10 : 0;
	env->push[N1] ? env->sfr.z++ : 0;
	env->push[N2] ? env->sfr.z-- : 0;
	env->push[N4] ? env->sfr.r++ : 0;
	env->push[N5] ? env->sfr.r-- : 0;
	env->push[ADD] && env->lum.i < 1 ? env->lum.i *= 1.02 : 0;
	env->push[SUB] && env->lum.i > 0 ? env->lum.i *= 0.98 : 0;
	ft_memset(env->mlxi->buffer, 0, env->mlxi->width * env->mlxi->height * 4);
	graf(env);
	mlx_put_image_to_window(env->mlxp, env->mlxw, env->mlxi, 0, 0);
	return(0);
}

int		key_on(int key, void *venv)
{
	t_env	*env;

	env = (t_env *)venv;
	env->push[key] = 1;
	key == ESC ? exit(0): 0; 
	return(0);
}

int		key_off(int key, void *venv)
{
	t_env	*env;

	env = (t_env *)venv;
	env->push[key] = 0;
	return(0);
}
void	put_pixel_to_img(t_env *env, unsigned int color, int x, int y)
{
        if (x < env->mlxi->width && y < env->mlxi->height)
                ((unsigned int *)(env->mlxi->buffer))[x + y *
                env->mlxi->width] = color;
}

double	ft_norm(const t_ray ray)
{
	return (sqrt(ray.x * ray.x + ray.y * ray.y + ray.z * ray.z));
}

double	ft_p_scal(t_ray a, t_ray b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}

void	ft_normi(t_ray *ray)
{
	double norm = ft_norm(*ray);
	ray->x /= norm;
	ray->y /= norm;
	ray->z /= norm;
}

void	graf(t_env *env)
{
	int		x;
	int		y;
	int		f_color;
	float	delta;
	t_ray	ray;
	t_ray	tmp_l;
	t_ray	tmp_r;
	t_ray	norm;

	y = 0;
	while (y < W_H)
	{
		f_color = 0;
		ray.x = x - (W_W / 2.0f);
		ray.y = y - (W_H / 2.0f);
		ray.z = 800;
		//ft_normi(&ray);
		double c = env->sfr.x * env->sfr.x + env->sfr.y * env->sfr.y + env->sfr.z * env->sfr.z - env->sfr.r * env->sfr.r;
		double a = ray.x * ray.x + ray.y * ray.y + ray.z * ray.z;
		double b = -2 * (ray.x * env->sfr.x + ray.y * env->sfr.y + ray.z * env->sfr.z);
		delta = b * b - 4 * a * c;
		if (delta >= 0)// ????
		{
			ft_normi(&ray);
			double x1 = (-(double)b + sqrt(delta)) / (double)(2 * a);
			//double x2 = ((double)b + sqrt(delta)) / (double)(2 * a);
			double xx = ray.x * x1 + (double)env->sfr.x;
			double yy = ray.y * x1 + (double)env->sfr.y;
			double zz = ray.z * x1 + (double)env->sfr.z;
			norm.x = xx - env->sfr.x;
			norm.y = yy - env->sfr.y;
			norm.z = zz - env->sfr.z;
			tmp_l.x = env->lum.x - xx;
			tmp_l.y = env->lum.y - yy;
			tmp_l.z = env->lum.z - zz;
			ft_normi(&norm);
			double scal = ft_p_scal(ray, norm);
			tmp_r.x = ray.x - 2 * scal * norm.x;
			tmp_r.y = ray.y - 2 * scal * norm.y;
			tmp_r.z = ray.z - 2 * scal * norm.z;
			ft_normi(&tmp_l);
			scal =  ft_p_scal(tmp_l, tmp_r);
			scal <= 0 ? f_color = 0xff * (1 + scal) * env->lum.i : 0;
			x == 500 && y == 500 ? printf("delta = %f\n",scal) : 0;
		}
		
		put_pixel_to_img(env, f_color, x, y);
		x++;
		if (x == W_W)
		{
			x = 0;
			y++;
		}
	}
	(void)env;
	return;
}
