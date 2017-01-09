typedef enum	e_prim_type
{
	NONE = 0, SPHERE = 1, PLANE = 2, CONE = 3, CYLINDER = 4
}				t_prim_type;

typedef struct	s_primitive
{
	t_prim_type		type;
	float4		position;
	float4		normal;
	float4		color;
	float		radius;
}				t_primitive;

typedef struct	s_light
{
	float4		position;
	float4		color;
}				t_light;

typedef struct	s_argn		//structure containing the limit of out, rays and objects
{							//note the corespondance with the C structure (types end position of variables correspond, but types have a preceding "cl_")
	int2	screen_size;	//total screen size in pixels (accesed by .x and .y)
	int		nb_objects;		//total number of objects in the scene
	int		nb_lights;		//total number of lights in the scene
}				t_argn;		//norm42 magle

typedef struct	s_camera	//note: yes, this structure is not equivalent to s_camera in rtv1.h
{
	float4	pos;			//
	float4	dir;			//
	float4	up;				//
	float4	right;			//
	float4	vpul;			//
	int2	vp_size;		//
}				t_camera;

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
float4	local_length(float4 v);

int		plane_intersect(__global t_primitive *obj, t_ray *ray, float *dist);
int		sphere_intersect(__global t_primitive *obj, t_ray *ray, float *dist);
int		intersect(__global t_primitive *obj, t_ray *ray, float *dist);
float4	get_normal(__global t_primitive *obj, float4 point);
float4	phong(float4 dir, float4 norm);
int		color_to_int(float4 color);

#if 0
# define DOT local_dot
# define NORMALIZE local_normalize
# define LENGTH local_length
#else
# define DOT dot
# define NORMALIZE normalize
# define LENGTH length
#endif

#ifndef EPSILON
# define EPSILON 0.01f
#endif

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

float4 local_length(float4 v)
{
	return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

int		plane_intersect(__global t_primitive *obj, t_ray *ray, float *dist)
{
	float d = DOT(obj->normal, ray->direction);

	if (d == 0)
		return (0);

	float new_dist = DOT(obj->position - ray->origin, obj->normal) / d;

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
	pos.w = 0;

	float c = pos.x * pos.x + pos.y * pos.y + pos.z * pos.z - obj->radius * obj->radius;
	float a = ray->direction.x * ray->direction.x + ray->direction.y * ray->direction.y + ray->direction.z * ray->direction.z;
	float b = 2 * (ray->direction.x * pos.x + ray->direction.y * pos.y + ray->direction.z * pos.z);
	float delta = b * b - 4 * a * c;
	if (delta < 0)
		return (0);

	delta = sqrt(delta);
	float x1 = (b - delta) / (2 * a);
	float x2 = (b + delta) / (2 * a);
	if (x2 > 0)
	{
		if (x1 < 0)
		{
			if (x2 < *dist)
			{
				*dist = x2;
				return (-1);
			}
		}
		else
		{
			if (x1 < *dist)
			{
				*dist = x1;
				return (1);
			}
		}
	}
	return (0);
}

int		intersect(__global t_primitive *obj, t_ray *ray, float *dist)
{
	switch (obj->type)
	{
		case SPHERE:
			return sphere_intersect(obj, ray, dist);
		case PLANE:
			return plane_intersect(obj, ray, dist);
	}

	// unknown object type
	return (0);
}

float4	get_normal(__global t_primitive *obj, float4 point)
{
	switch (obj->type)
	{
		case SPHERE:
			return NORMALIZE(point - obj->position);
		case PLANE:
			return obj->normal;
	}
	return (float4)(0, 0, 0, 0);
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
					__global int *out,				//int bitmap, his size is equal to screen_size.x * screen_size.y
					__global t_argn *argn,			//structure containing important info on how to acces out, rays and objects
					__global t_primitive *objects,	//set of objects in the scene, the number of objects (and so the limit of this array), is stored in argn
					__global t_light *lights,
					__global t_camera *cam)			//vector3 rays stored has float4 for convenience, the size of the array is screen_size.x * screen_size.y
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
	ray.direction = NORMALIZE(cam->vpul + cam->right * x - cam->up * y);

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

	// no primitive was hit
	if (id == -1)
		return ;

	__global t_primitive *prim = &objects[id];
	float4 collision = ray.origin + ray.direction * dist;
	float4 color = (float4)(0, 0, 0, 0);

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
		ray_l.origin = collision + ray_l.direction * EPSILON;
		int hit = 0;

		dist = MAXFLOAT;
		for (cur = 0; cur < argn->nb_objects; cur++)
		{
			if ((hit = intersect(&objects[cur], &ray_l, &dist)))
			{
				if (dist < dist_l) // it is between us
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
		// diffuse lighting
		float scal;
		if ((scal = DOT(ray_l.direction, norm)) > 0)
			color += prim->color * light.color * scal; // * diffuse amount;

		// specular highlights (needs pow to make the curve sharper)
		float4 ir = phong(-ray_l.direction, norm);
		if (scal > 0 && (scal = DOT(ray_l.direction, ir)) > 0)
			color += light.color * pow(max(0.0f, scal), 20); // * specular amount;

	}

	// this isnt mathematically correct but is definitely better looking for now
	color /= (float)argn->nb_lights;

	out[i] = color_to_int(color);
}
