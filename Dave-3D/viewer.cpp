#include <iostream>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <cmath>
#include "ifs.h"

#define UINTSIZE sizeof(unsigned int)
#define FLOATSIZE sizeof(float)
using namespace std;

IFS_DATA* data;
float shift_x,shift_y,shift_z;
float zoomer;

ssize_t read_uint32(int infd, unsigned int* uint_star) {
    if (read(infd, uint_star, UINTSIZE) == UINTSIZE) {
	return UINTSIZE;
    } else {
	fprintf(stderr, "Error reading size of a uint32 field\n");
	exit(-1);
    }
}

ssize_t read_float32(int infd, float* float_star) {
    if (read(infd, float_star, FLOATSIZE) == FLOATSIZE) {
        return FLOATSIZE;
    } else {
        fprintf(stderr, "Error reading size of a float32 field\n");
        exit(-1);
    }
}

ssize_t read_string32 (int infd, char** buf) {
    unsigned int mem_len = 0;
    read_uint32(infd, &mem_len);
    *buf = (char *)realloc(*buf, mem_len);
    if (mem_len == read(infd, *buf, mem_len)) {
	return mem_len;
    }
    else {
	fprintf(stderr, "Error reading a string32 field\n");
	exit(-1);
    }
}

IFS_DATA* load_ifs_file (const char* filename) {
    int infd;
    IFS_DATA* ifs_data = NULL;
    float version;
    char* ifstag = NULL;
    unsigned int i;
    unsigned int nVertices = 0;
    unsigned int nTriangles = 0;
    unsigned int tmp_Index = 0;
    
    if ((infd = open(filename, O_RDONLY)) < 2) {
	fprintf(stderr, "Error opening an input IFS file\n");
	exit(-1);
    }

    ifs_data = (IFS_DATA*) malloc(sizeof(IFS_DATA));
    ifs_data->modelName = NULL;
    ifs_data->numVertices = 0;
    ifs_data->vertices = NULL;
    ifs_data->numTriangles = 0;
    ifs_data->triangles = NULL;
    
    read_string32(infd, &ifstag);
    if (strcmp(ifstag, "IFS") != 0) {
	fprintf(stderr, "Not IFS filetype\n");
	exit(-1);
    }
    free(ifstag); ifstag = NULL;

    read_float32(infd, &version);
    if (version != 1.0) {
	fprintf(stderr, "Invalid version number: %f\n", version);
	exit(-1);
    }

    read_string32(infd, &(ifs_data->modelName));
    
    read_string32(infd, &ifstag);
    if (strcmp(ifstag, "VERTICES") != 0) {
	fprintf(stderr, "Not IFS filetype\n");
	exit(-1);
    }
    free(ifstag); ifstag = NULL;

    read_uint32(infd, &nVertices);
    ifs_data->numVertices = nVertices;

    ifs_data->vertices = (VERTEX*) malloc(nVertices * sizeof(VERTEX));
    for (i =0; i < ifs_data->numVertices; ++i) {
	ifs_data->vertices[i].id = i;
	read_float32(infd, &((ifs_data->vertices)[i].x));
	read_float32(infd, &((ifs_data->vertices)[i].y));
	read_float32(infd, &((ifs_data->vertices)[i].z));
    }
    
    read_string32(infd, &ifstag);
    if (strcmp(ifstag, "TRIANGLES") != 0) {
	fprintf(stderr, "Not IFS filetype\n");
	exit(-1);
    }
    free(ifstag); ifstag = NULL;
	
    read_uint32(infd, &nTriangles);
    ifs_data->numTriangles = nTriangles;
    
    ifs_data->triangles = (TRIANGLE*) malloc(nTriangles * sizeof(TRIANGLE));
    for (i =0; i < ifs_data->numTriangles; ++i) {
	read_uint32(infd, &tmp_Index);
	if (tmp_Index >= nVertices) {
	    fprintf(stderr, "Invalid Vertex index\n");
	    exit(-1);
	}
	ifs_data->triangles[i].a = &((ifs_data->vertices)[tmp_Index]);
	read_uint32(infd, &tmp_Index);
	if (tmp_Index >= nVertices) {
	    fprintf(stderr, "Invalid Vertex index\n");
	    exit(-1);
	}
	ifs_data->triangles[i].b = &((ifs_data->vertices)[tmp_Index]);
	read_uint32(infd, &tmp_Index);
	if (tmp_Index >= nVertices) {
	    fprintf(stderr, "Invalid Vertex index\n");
	    exit(-1);
	}
	ifs_data->triangles[i].c = &((ifs_data->vertices)[tmp_Index]);
    }

    if (close(infd) == -1) {
	fprintf(stderr, "Error closing an input IFS file\n");
	exit(-1);
    }
	
    return ifs_data;
}

void free_ifs_data(IFS_DATA** ifs_data) {
    if (ifs_data) {
	free((*ifs_data)->modelName);
	free((*ifs_data)->vertices);
	free((*ifs_data)->triangles);
    }
    free(*ifs_data);
    *ifs_data = NULL;
}

void print_ifs_summary(FILE* target, IFS_DATA* ifs_data) {
    unsigned int i;
    fprintf(target, "=====  IFS  SUMMARY  =====\n");
    fprintf(target, " Model name          : %s\n", ifs_data->modelName);
    fprintf(target, " Number of vertices  : %d\n", ifs_data->numVertices);
    for (i=0; i<ifs_data->numVertices; ++i) {
	fprintf(target, " v_%06d : (%8f, %8f, %8f)\n",
		ifs_data->vertices[i].id,
		ifs_data->vertices[i].x, 
		ifs_data->vertices[i].y,
		ifs_data->vertices[i].z);
    }
    fprintf(target, " Number of triangles : %d\n", ifs_data->numTriangles);
    for (i=0; i<ifs_data->numTriangles; ++i) {
	fprintf(target, " t_%06d : (v_%06d, v_%06d, v_%06d)\n", i,
		(ifs_data->triangles[i].a)->id, 
		(ifs_data->triangles[i].b)->id,
		(ifs_data->triangles[i].c)->id);
    }
    fprintf(target, "===== END OF SUMMARY =====\n");
}

class Vector4{
	public:
	float x;
	float y;
	float z;
	float w;
	
	Vector4 * homogenize(){
		x=x/w;
		y=y/w;
		z=z/w;
		w=1;
	}
	
	Vector4 * add(Vector4 *v){
		Vector4 *v1= new Vector4;
		v->homogenize();
		homogenize();
		v1->x=x+v->x;
		v1->y=y+v->y;
		v1->z=z+v->z;
		v1->w=1;
		return v1;
	}
	
	Vector4 * subtract(Vector4 *v){
		Vector4 *v1= new Vector4;
		v->homogenize();
		homogenize();
		v1->x=x-v->x;
		v1->y=y-v->y;
		v1->z=z-v->z;
		v1->w=1;
		return v1;
	}
	
	
	float  dot(Vector4 *v){
		float sum=0;
		v->homogenize();
		homogenize();
		sum+=x*(v->x);
		sum+=y*(v->y);
		sum+=z*(v->z);
		return sum;
	}
	
	Vector4 * cross(Vector4 *v){
		Vector4 *v1= new Vector4;
		v->homogenize();
		homogenize();
		v1->x=y*(v->z)-z*(v->y);
		v1->y=z*(v->x)-x*(v->z);
		v1->z=x*(v->y)-y*(v->x);
		return v1;
	}
	
	int relational(Vector4 *v){
	float r1,r2;
	r1=sqrt(x*x+y*y+z*z);
	r2=sqrt(v->x*v->x+v->y*v->y+v->z*v->z);
	if (r1>r2)
		return -1;
	if (r1==r2)
		return 0;
	return 1;
	
	}
};

class triangle_ids{
	public:
		int a;
		int b;
		int c;

};


Vector4 *va;
triangle_ids *ta;

void normalize(float &aa,float &bb,float &cc){
	float r=sqrt(aa*aa+bb*bb+cc*cc);
	aa/=r;
	bb/=r;
	cc/=r;
}


void display_child(){
	float x,y,z,X,Y,Z,t=45,id1,id2;
	
	
   	//glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
   	Vector4 *v,*v1,*v2,*v3,*v4,*v5;
    	GLfloat mat_specular[] = { 0.1, 0.3, 0.2, 1.0 };
  			GLfloat light_diffuse[] = { 0.1, 0.3, 0.3, 1 };
  			GLfloat mat_shininess[] = { 50.0 };
  			glShadeModel(GL_SMOOTH);
   			GLfloat mat_amb_diff[] = { 0, 0, 0, 1.0 };
  			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
   			glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
   			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, light_diffuse);
   	 for (int i=0; i<data->numTriangles; ++i){
   	 v= new Vector4;
   	 glBegin(GL_POLYGON);
   	 v1=&va[data->triangles[i].a->id];
   	 v2=&va[data->triangles[i].b->id];
   	 v3=&va[data->triangles[i].c->id];
   	 
   	 v4=v1->subtract(v2);
   	 v5=v1->subtract(v3);
   	 v3=v4->cross(v5);
   	 
   	 normalize(v3->x,v3->y,v3->z);
   	 glNormal3f(v3->x, v3->y, v3->z);
   	 glColor3f(0.0,1.0,0);
   	 
   	
   	 int id;
   	 id=data->triangles[i].a->id;
   	//  cout<<"dkdj"<<endl;
   	 v->x=va[id].x;
   	 v->y=va[id].y;
   	 v->z=va[id].z;
   	 v->w=1;
   //	 LookAt(v);
   	
	glVertex3f(v->x,v->y,v->z);

	id=data->triangles[i].b->id;
   	 v->x=va[id].x;
   	 v->y=va[id].y;
   	 v->z=va[id].z;
   	 v->w=1;
   //	 LookAt(v);
   	
	glVertex3f(v->x,v->y,v->z);
	
	
	id=data->triangles[i].c->id;
   	 v->x=va[id].x;
   	 v->y=va[id].y;
   	 v->z=va[id].z;
   	 v->w=1;
   	// LookAt(v);
   	 
	glVertex3f(v->x,v->y,v->z);
	glEnd();
    	}
	
	
}

