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
	float4 ray = (float4)(0, 0, 0, 0);
	ray.x = (i % argn->screen_size.x) - (argn->screen_size.x / 2.0f);
	ray.y = (i / argn->screen_size.x) - (argn->screen_size.y / 2.0f);
	ray.z = 800;
	ray = normalize(ray);
	float c = objects[0].position.x * objects[0].position.x + objects[0].position.y * objects[0].position.y + objects[0].position.z * objects[0].position.z - objects[0].radius * objects[0].radius;
	float a = ray.x * ray.x + ray.y * ray.y + ray.z * ray.z;
	float b = -2 * (ray.x * objects[0].position.x + ray.y * objects[0].position.y + ray.z * objects[0].position.z);
	float delta = b * b - 4 * a * c;
	if (delta >= 0)
	{
		float4 norm = (float4)(0,0,0,0);
		float4 tmp_l = (float4)(0,0,0,0);
		float4 tmp_r = (float4)(0,0,0,0);
		float x1 = (-b + sqrt(delta)) / (2 * a);
		float cx = ray.x * x1;
		float cy = ray.y * x1;
		float cz = ray.z * x1;
		if (i == 500 + 500 * 1000)
			printf("%f\n", cx);
		norm.x = cx - objects[0].position.x;
		norm.y = cy - objects[0].position.y;
		norm.z = cz - objects[0].position.z;
		norm = normalize(norm);
		tmp_l.x = objects[1].position.x - cx;
		tmp_l.y = objects[1].position.y - cy;
		tmp_l.z = objects[1].position.z - cz;
		float scal = dot(ray, norm);
		tmp_r.x = ray.x - 2 * scal * norm.x;
		tmp_r.y = ray.y - 2 * scal * norm.y;
		tmp_r.z = ray.z - 2 * scal * norm.z;
		tmp_l = normalize(tmp_l);
		//tmp_r = normalize(tmp_r);
		scal = dot(tmp_l, tmp_r);
		if (i == 500 + 500 * 1000)
			printf("%f\n", scal);
		scal < 0 ? out[i] = 0xaa * -scal : 0 ;
		out[i] += 0x55;
		if (i == 500 + 500 * 1000)
			printf("%v4f\n", tmp_l);
	}
		if (i == 500 + 500 * 1000)
			out[i] = 0xffffff;
}
