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
float4	calcul_lum(float4 colision, float4 norm, __global t_light *light, __global t_argn *argn, __global t_primitive *objects);
int		dist_colision_sphere(float	*dist, t_ray ray, __global t_primitive *object);
int		colision_sphere(t_ray ray, float dist, __global t_primitive *objects, __global t_argn *argn, __global t_light *lights);
float4	local_normalize(float4 v);
float	local_dot(float4 v1, float4 v2);

#if 0
# define DOT local_dot
# define NORMALIZE local_normalize
#else
# define DOT dot
# define NORMALIZE normalize
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

float4	calcul_lum(float4 colision, float4 norm, __global t_light *light, __global t_argn *argn, __global t_primitive *objects)
{
	float	dist = 0;
	t_ray tmp_l;
	tmp_l.direction = light->position - colision;
	tmp_l.direction = NORMALIZE(tmp_l.direction);
	float scal = DOT(tmp_l.direction, norm);
	for (int i = 0; i < argn->nb_objects; i++)
	{
		if (dist_colision_sphere(&dist, tmp_l, &objects[i]))
			return (0);
	}
	return (scal >= 0 ? (float4)(0, 0, 0x88 * scal, 0) : (float4)(0, 0, 0, 0));
}


int		dist_colision_sphere(float	*dist, t_ray ray, __global t_primitive *object)
{
	object->position = object->position - ray.origin;
	float c = object->position.x * object->position.x + object->position.y * object->position.y + object->position.z * object->position.z - object->radius * object->radius;
	float a = ray.direction.x * ray.direction.x + ray.direction.y * ray.direction.y + ray.direction.z * ray.direction.z;
	float b = 2 * (ray.direction.x * object->position.x + ray.direction.y * object->position.y + ray.direction.z * object->position.z);
	float delta = b * b - 4 * a * c;
	if (delta < 0)
		return (0);
	float x1 = (b - sqrt(delta)) / (2 * a);
	float x2 = (b + sqrt(delta)) / (2 * a);
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

int		colision_sphere(t_ray ray, float dist, __global t_primitive *objects, __global t_argn *argn, __global t_light *lights)
{
	float4 colision = ray.origin + ray.direction * dist;
	float4 norm = colision - objects[0].position;
	norm = NORMALIZE(norm);
	float4 fcolor = 0;
	for(int j = 0; j < argn->nb_lights; j++)
		fcolor += calcul_lum(colision, norm, &lights[j], argn, objects);
	//printf("okidoki\n");
	return ((int)fcolor.x * 0x10000 + (int)fcolor.y * 0x100 + (int)fcolor.z);
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
//	printf("%v4f\n", ray.direction);
	float dist = MAXFLOAT;
	//printf("dist = %f", dist);
	if (dist_colision_sphere(&dist, ray, objects))
	{
		out[i] = colision_sphere(ray, dist, objects, argn, lights);
		out[i] = out[i] > 0xff ? 0xff : out[i];
//		out[i] = 0xffffff;
		//out[i] = out[i] <= 0x33 ? 0x33 : out[i];
		//if (i == 500 + 500 * 1000)
			//printf("%v4f\n", tmp_l);
	}
		if (i == 500 + 500 * 1000)
			out[i] = 0xffffff;
}
