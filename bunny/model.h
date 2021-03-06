#ifndef _MODEL_H
#define _MODEL_H
#include <GL/glut.h>
#include "rply/rply.h"
#include "control.h"

typedef struct vec3{
	double x;
	double y;
	double z;
	}vec3;

typedef struct ivec3{
	int x;
	int y;
	int z;
	}ivec3;
	
typedef vec3 vertex;
typedef vec3 color;
typedef vec3 normal;
typedef ivec3 triangle;

class Model{
	
	public:
		static long nvertices,ntriangles;
		static long count_v, count_t;
		static vertex *vertices;
		static triangle *triangles;
		static normal *nml;
		static char filename[100];
		static int init(char *filename);
		static int display();
	private:
		static int compute_normal(int pos);
		static int face_cb(p_ply_argument argument);
		static int vertex_cb(p_ply_argument argument);
		static float x_min,x_max,y_min,y_max,z_min,z_max;
};

#endif
