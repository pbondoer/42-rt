typedef enum		e_prim_type
{
	INVALID = -1, SPHERE = 0, PLANE = 1, CONE = 2, CYLINDER = 3
}					t_prim_type;

typedef struct		s_material
{
	float4			color;
	float			diffuse;
	float			specular;
}					t_material;

typedef struct		s_camera
{
	float4			pos;
	float4			dir;
	float4			up;
	float4			right;
	float4			vpul;
	float2			vp_size;
}					t_camera;

typedef struct		s_primitive
{
	t_prim_type		type;
	float4			position;
	float4			direction;
	float			radius;
	uint			material;
}					t_primitive;

typedef struct		s_light
{
	float4			position;
	float4			color;
}					t_light;

typedef struct		s_argn
{
	int2			screen_size;
	int				nb_objects;
	int				nb_lights;
	float			ambient;
	float			direct;
	int				nb_materials;
}					t_argn;

typedef struct	s_ray
{
	float4		origin;
	float4		direction;
}				t_ray;

//j'aime les commentaires, et vous? :p
/*
   NOTE HYPER IMPORTANTE:
   pour la compatibilité AMD, il faut continuer d'utiliser les flags __global et les pointeurs
   (AMD ne copie pas les déférencements de pointeurs dans la stack, ce qui fait que les objets et la lumière n'étaient pas envoyé aux calculs)
   */
float4	local_normalize(float4 v);
float	local_dot(float4 v1, float4 v2);
float4	local_cross(float4 v1, float4 v2);
float	local_length(float4 v);

float	addv(float4 v);

int		plane_intersect(__global t_primitive *obj, t_ray *ray, float *dist);
int		sphere_intersect(__global t_primitive *obj, t_ray *ray, float *dist);
int		cylinder_intersect(__global t_primitive *obj, t_ray *ray, float *dist);
int		cone_intersect(__global t_primitive *obj, t_ray *ray, float *dist);
int		intersect(__global t_primitive *obj, t_ray *ray, float *dist);
int		solve_quadratic(float a, float b, float c, float *dist);
float4	get_normal(__global t_primitive *obj, float4 point);
float4	phong(float4 dir, float4 norm);
int		color_to_int(float4 color);

#if 0
# define DOT local_dot
# define NORMALIZE local_normalize
# define LENGTH local_length
# define CROSS local_cross

float4	local_cross(float4 v1, float4 v2)
{
	float4 res;

	res.x = v1.y * v2.z - v1.z * v2.y;
	res.y = v1.z * v2.x - v1.x * v2.z;
	res.z = v1.x * v2.y - v1.y * v2.x;
	return (res);
}

float	local_dot(float4 v1, float4 v2)
{
	return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
}

float4	local_normalize(float4 v)
{
	float	t;

	if ((v.x == 0.0f && v.y == 0.0f && v.z == 0.0f) || (t = (v.x * v.x + v.y * v.y + v.z * v.z)) == 1.0f)
		return (v);
	return (v / sqrt(t));
}

float	local_length(float4 v)
{
	return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}
#else
# define DOT dot
# define NORMALIZE normalize
# define LENGTH length
# define CROSS cross
#endif

// error correction
#ifndef EPSILON
# define EPSILON 0.0001f
#endif

float	addv(float4 v)
{
	return (v.x + v.y + v.z);
}

int		plane_intersect(__global t_primitive *obj, t_ray *ray, float *dist)
{
	float d = DOT(obj->direction, ray->direction);

	if (d == 0)
		return (0);

	float new_dist = DOT(obj->position - ray->origin, obj->direction) / d;

	if (new_dist > 0 && new_dist < *dist)
	{
		*dist = new_dist;
		return (d > 0 ? -1 : 1);
	}
	return (0);
}

int		sphere_intersect(__global t_primitive *obj, t_ray *ray, float *dist)
{
	float4 pos = obj->position - ray->origin;

	float a = DOT(ray->direction, ray->direction);
	float b = 2 * DOT(ray->direction, pos);
	float c = DOT(pos, pos) - pow(obj->radius, 2);
	return solve_quadratic(a, b, c, dist);
}

int		cylinder_intersect(__global t_primitive *obj, t_ray *ray, float *dist)
{
	float4 pos = obj->position - ray->origin;
	float4 p = CROSS(pos, obj->direction);
	float4 r = CROSS(ray->direction, obj->direction);

	float a = DOT(r, r);
	float b = 2 * DOT(r, p);
	float c = DOT(p, p) - pow(obj->radius, 2) * DOT(obj->direction, obj->direction);
	return solve_quadratic(a, b, c, dist);
}

int		cone_intersect(__global t_primitive *obj, t_ray *ray, float *dist)
{
	float4 pos = ray->origin - obj->position;
	float4 dir = -ray->direction;

	float r = 1 + pow(tan(obj->radius * M_PI / 180.0f), 2);
	float dd = DOT(dir, obj->direction);

	float a = DOT(dir, dir) - (r * pow(dd, 2));
	float b = 2 * (DOT(dir, pos) - (r * dd * DOT(pos, obj->direction)));
	float c = DOT(pos, pos) - (r * pow(DOT(pos, obj->direction), 2));
	return solve_quadratic(a, b, c, dist);
}

int		solve_quadratic(float a, float b, float c, float *dist)
{
	float delta = b * b - 4.0f * a * c;
	if (delta < 0)
		return (0);

	delta = sqrt(delta);
	float x1 = (b - delta) / (2.0f * a);
	float x2 = (b + delta) / (2.0f * a);

	if (x1 <= EPSILON)
	{
		//use x2
		if (x2 <= EPSILON)
		{
			return (0); // both are negative
		}
		else if (x2 < *dist) // x2 positive
		{
			*dist = x2;
			return (-1);
		}
	}
	else if (x1 < *dist) // x1 positive
	{
		*dist = x1;
		return (1);
	}

	return (0);
}

int		intersect(__global t_primitive *obj, t_ray *ray, float *dist)
{
	obj->direction = NORMALIZE(obj->direction);
	switch (obj->type)
	{
		case SPHERE:
			return sphere_intersect(obj, ray, dist);
		case PLANE:
			return plane_intersect(obj, ray, dist);
		case CONE:
			return cone_intersect(obj, ray, dist);
		case CYLINDER:
			return cylinder_intersect(obj, ray, dist);
		default:
			return (0);
	}

	return (0);
}

float4	get_normal(__global t_primitive *obj, float4 point)
{
	float4 n = (float4)(0, 0, 0, 0);

	obj->direction = NORMALIZE(obj->direction);
	switch (obj->type)
	{
		case SPHERE:
			n = point - obj->position;
			break;
		case PLANE:
			n = obj->direction;
			break;
		case CYLINDER:
			n = point - obj->position + (obj->direction * DOT(obj->direction, obj->position - point));
			break;
		case CONE:
			n = point - obj->position + (obj->direction * -DOT(point, obj->direction) / pow(cos(obj->radius), 2));
			break;
		default:
			return (n);
	}

	return NORMALIZE(n);
}

float4	phong(float4 dir, float4 norm)
{
	return (dir - 2 * DOT(dir, norm) * norm);
}

int		color_to_int(float4 color)
{
	int r = (int)clamp(color.x * 255.0f, 0.0f, 255.0f);
	int g = (int)clamp(color.y * 255.0f, 0.0f, 255.0f);
	int b = (int)clamp(color.z * 255.0f, 0.0f, 255.0f);

	return (int)((r << 16) + (g << 8) + b);
}

__kernel void	example(							//main kernel, called for each ray
		__global int			*out,				//int bitmap, his size is equal to screen_size.x * screen_size.y
		__global t_argn			*argn,			//structure containing important info on how to acces out, rays and objects
		__global t_primitive	*objects,	//set of objects in the scene, the number of objects (and so the limit of this array), is stored in argn
		__global t_light		*lights,
		__global t_camera		*cam,
		__global t_material		*materials)
{
	//mode 2: we use 1D Kernels:
	size_t i = get_global_id(0);	//id of the kernel in the global call
	out[i] = 0;

	if (i >= (size_t)argn->screen_size.x * (size_t)argn->screen_size.y)	//the number of kernel executed can overflow the number initialy needed, this is a simple protection to avoid bad memory acces
		return ;

	float	x = (float)(i % argn->screen_size.x) /*- ((float)argn->screen_size.x / 2.0f)*/;
	float	y = (float)(i / argn->screen_size.x) /*- ((float)argn->screen_size.y / 2.0f)*/;
	t_ray ray;
	ray.origin = cam->pos;

	float4 color = (float4)(0, 0, 0, 0);

	ray.direction = NORMALIZE(cam->vpul + NORMALIZE(cam->right) * x - NORMALIZE(cam->up) * y);

	float dist = MAXFLOAT;
	int id = -1;
	int cur = 0;
	int	t_hit = 0;
	int s_hit = 0;
	for (cur = 0; cur < argn->nb_objects; cur++)
	{
		if ((t_hit = intersect(&objects[cur], &ray, &dist))) //on sauvegarde dans une variable temporaire la veleur de hit (pour éviter de devoir rappeler la fonction intersect)
		{
			id = cur;
			s_hit = t_hit; //si il y avait collision, on sauvegarde la valeur de t_hit dans s_hit pour l'eventuelle inversion de normale plus tard
		}
	}

	// no primitive was hit, no need to go further
	if (id == -1)
		return ;

	__global t_primitive *prim = &objects[id];
	t_material mat = materials[prim->material];
	float4 collision = ray.origin + ray.direction * dist;
	float4 cur_color = (float4)(0, 0, 0, 0);

	// we hit something... lights, maestro!
	int cur_l;
	t_ray ray_l;
	for (cur_l = 0; cur_l < argn->nb_lights; cur_l++)
	{
		t_light light = lights[cur_l];

		// check if our light source is blocked by an object
		// shadows start a tiny amount from the actual sphere to prevent rounding errors
		float dist_l = LENGTH(light.position - collision);
		ray_l.direction = NORMALIZE(light.position - collision);
		ray_l.origin = collision + ray_l.direction * 0.1f;
		int hit = 0;

		dist = MAXFLOAT;
		for (cur = 0; cur < argn->nb_objects; cur++)
		{
			if ((hit = intersect(&objects[cur], &ray_l, &dist))	> 0)
			{
				if (dist > EPSILON && dist < dist_l) // it is between us
					break ;
				else // it is behind us
					hit = 0;
			}
		}

		// did we hit something? don't calculate color for this light, shadow!
		if (hit)
			continue ;

		// get the color for this light
		float4 norm = get_normal(prim, collision);
		if (s_hit == -1) //si on était dans la primitive, on inverse la normale
			norm = -norm;

		// ambient light
		cur_color += mat.color * argn->ambient;

		// diffuse lighting
		float scal;
		if ((scal = DOT(ray_l.direction, norm)) > 0)
			cur_color += mat.color * mat.diffuse * light.color * scal;

		// specular highlights (needs pow to make the curve sharper)
		float4 ir = phong(-ray_l.direction, norm);
		if (scal > 0 && (scal = DOT(ray_l.direction, ir)) > 0)
			cur_color += light.color * pow(scal, 20) * mat.specular;
	}

	color += cur_color / (float)argn->nb_lights;

	out[i] = color_to_int(color);
}