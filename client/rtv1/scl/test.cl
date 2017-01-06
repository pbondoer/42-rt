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
}				t_camera;

typedef struct	s_ray
{
	float4		origin;
	float4		direction;
}				t_ray;

//j'aime les commentaires, et vous? :p
float4	calcul_lum(float4 colision, float4 norm, t_light light, __global t_argn *argn, __global t_primitive *objects);
int		dist_colision_sphere(float	*dist, float4 ray, __global t_primitive *sphr);
int		colision_sphere(float4 ray, float dist, __global t_primitive *objects, __global t_argn *argn, __global t_light *lights);
		

float4	calcul_lum(float4 colision, float4 norm, t_light light, __global t_argn *argn, __global t_primitive *objects)
{
	float	dist = 0;
	float4 tmp_l = light.position - colision;
	tmp_l = normalize(tmp_l);
	float scal = dot(tmp_l, norm);
	for (int i = 0; i < argn->nb_objects; i++)
	{
		if (dist_colision_sphere(&dist, tmp_l, &(objects[i])))
			return (0);
	}
	return (scal >= 0 ? (float4)(0, 0, 0x88 * scal, 0) : (float4)(0, 0, 0, 0));
}


int		dist_colision_sphere(float	*dist, float4 ray, __global t_primitive *sphr)
{
	float c = sphr[0].position.x * sphr[0].position.x + sphr[0].position.y * sphr[0].position.y + sphr[0].position.z * sphr[0].position.z - sphr[0].radius * sphr[0].radius;
	float a = ray.x * ray.x + ray.y * ray.y + ray.z * ray.z;
	float b = -2 * (ray.x * sphr[0].position.x + ray.y * sphr[0].position.y + ray.z * sphr[0].position.z);
	float delta = b * b - 4 * a * c;
	if (delta < 0)
		return (0);
	float x1 = (-b + sqrt(delta)) / (2 * a);
	float x2 = (-b - sqrt(delta)) / (2 * a);
	if (x2 >= 0)
	{
		if (dist[0] < x2)
		{
			dist[0] = x2;
			return (1);
		}
	}
	else
	{
		if (dist[0] < x1)
		{
			dist[0] = x1;
			return (-1);
		}
	}
	return (0);
}


int		colision_sphere(float4 ray, float dist, __global t_primitive *objects, __global t_argn *argn, __global t_light *lights)
{
	float4 colision = ray * dist;
	float4 norm = colision - objects[0].position;
	norm = normalize(norm);
	float4 fcolor = 0;
	for(int j = 0; j < argn->nb_lights; j++)
		fcolor += calcul_lum(colision, norm, lights[j], argn, objects);
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
	float4 ray = (float4)(0, 0, 0, 0);
	ray.x = (i % argn->screen_size.x) - (argn->screen_size.x / 2.0f);
	ray.y = (i / argn->screen_size.x) - (argn->screen_size.y / 2.0f);
	ray.z = 800;
	ray = normalize(ray);
	float dist = -MAXFLOAT;
	//printf("dist = %f", dist);
	float c = objects[0].position.x * objects[0].position.x + objects[0].position.y * objects[0].position.y + objects[0].position.z * objects[0].position.z - objects[0].radius * objects[0].radius;
	float a = ray.x * ray.x + ray.y * ray.y + ray.z * ray.z;
	float b = -2 * (ray.x * objects[0].position.x + ray.y * objects[0].position.y + ray.z * objects[0].position.z);
	float delta;
	delta = b * b - 4 * a * c;
	if (dist_colision_sphere(&dist, ray, objects))
	{
		out[i] = colision_sphere(ray, dist, objects, argn, lights);
		out[i] = out[i] > 0xff ? 0xff : out[i];
		//out[i] = out[i] <= 0x33 ? 0x33 : out[i];
		//if (i == 500 + 500 * 1000)
			//printf("%v4f\n", tmp_l);
	}
		if (i == 500 + 500 * 1000)
			out[i] = 0xffffff;
}
