typedef enum	e_prim_type
{
	NONE = 0, LIGHT = 1, SPHERE = 2, PLANE = 3, CONE = 4, CYLINDER = 5
}				t_prim_type;

typedef struct	s_primitive	//structure containing generic and specific data on the objects of the scene
{							//note the corespondance with the C structure (types end position of variables correspond, but types have a preceding "cl_")
	t_prim_type	type;		//numerical type, ex: 0 = light, 1 = spheroid, 2 = planoid, 3 = cylinder, 4 = conoid, 5 = cuboid
	float4		position;	//position of the object (accesed by .x, .y, .z, .w, w is discarded in 3D space)
	float4		normal;		//direction vector of the object (accesed by .x, .y, .z, .w, w is discarded in 3D space)
	float		radius;		//radius (if needed) of the object
	int			color;		//color (in 0xAARRGGBB) of the object
}				t_primitive;//norm42 magle

typedef struct	s_argn		//structure containing the limit of out, rays and objects
{							//note the corespondance with the C structure (types end position of variables correspond, but types have a preceding "cl_")
	int2	screen_size;	//total screen size in pixels (accesed by .x and .y)
	int		nb_objects;		//total number of object in the scene
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

int sphere_intersect(__global t_primitive *p, t_ray ray, float *dist, int i);

int sphere_intersect(__global t_primitive *p, t_ray ray, float *dist, int i)
{
	float4 v = ray.origin - p->position;		//centrage du rayon sur la sphere (coordonnees)
	float b = -dot(v, ray.direction );	//produit scalaire entre le rayon temporaire et la direction dudit rayon (renvoit un ecart scalaire)
	//float l = length(v);				//calcule la longeur (#pythagore) du vecteur en prevision du carre suivant
	float det = (b * b) - dot(v, v) + (p->radius * p->radius); //calcul du determinant
	int retval = 0;
	if (i == 0)
//	printf("%v4f\tintersect\n", ray.direction);
	if (det >= 0)
	{
		det = sqrt(det);
		float i1 = b - det;
		float i2 = b + det;
		if (i2 > 0)
		{
			if (i1 < 0)
			{
				if (i2 < *dist)
				{
					*dist = i2;
					retval = -1;
				}
			}
			else
			{
				if (i1 < *dist)
				{
					*dist = i1;
					retval = 1;
				}
			}
		}
	}
	return retval;
}

__kernel void	example(							//main kernel, called for each ray
					__global int *out,				//int bitmap, his size is equal to screen_size.x * screen_size.y
					__global t_argn *argn,			//structure containing important info on how to acces out, rays and objects
					__global t_primitive *objects,	//set of objects in the scene, the number of objects (and so the limit of this array), is stored in argn
					__global t_camera *cam)			//vector3 rays stored has float4 for convenience, the size of the array is screen_size.x * screen_size.y
{
	//mode 2: we use 1D Kernels:
	size_t i = get_global_id(0);	//id of the kernel in the global call

	if (i >= (size_t)argn->screen_size.x * (size_t)argn->screen_size.y)	//the number of kernel executed can overflow the number initialy needed, this is a simple protection to avoid bad memory acces
		return ;

	size_t x = i % argn->screen_size.x;
	size_t y = i / argn->screen_size.x;
	float	dist = MAXFLOAT;
	t_ray ray = {cam->pos,
		(float4)((float)argn->screen_size.x / 2.0f + (float)x,
				(float)-argn->screen_size.y / 2.0f + (float)y , 0, 0)};
	ray.direction = normalize(ray.direction);
	if (sphere_intersect(objects, ray, &dist, i) == 1)
	{
		if (i == 0)
		printf("%v4f\tintersect\n", ray.direction);
		out[i] = 0x00FF0000;
	}
	else
	{
		if (i == 0)
		printf("%v4f\tmiss\n", ray.direction);
		out[i] = 0;
	}
}
