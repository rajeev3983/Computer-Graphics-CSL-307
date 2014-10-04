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
double eyeX,eyeY,eyeZ,centreX,centreY,centreZ,upX,upY,upZ,X,Y,Z,X1,Y1,Z1,X2,Y2,Z2;

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

class Matrix4{
	public:
	float M[4][4];
	
	Matrix4 *add(Matrix4 *m){
		Matrix4 *m1=new Matrix4;
		for(int i=0;i<4;i++){
			for(int j=0;j<4;j++){
				m1->M[i][j]=M[i][j]+m->M[i][j];
			
			}
		}
	return m1;
	}
	
	Matrix4 *subtract(Matrix4 *m){
		Matrix4 *m1=new Matrix4;
		for(int i=0;i<4;i++){
			for(int j=0;j<4;j++){
				m1->M[i][j]=M[i][j]-m->M[i][j];
			
			}
		}
	return m1;
	}
	
	Matrix4 * matrix_multiplication(Matrix4 *m){
		Matrix4 *m1=new Matrix4;
		for(int i=0;i<4;i++){
			for(int j=0;j<4;j++){
        			m1->M[i][j]=0;
        			for(int k=0;k<4;k++){
					m1->M[i][j]+=(M[i][k] * m->M[k][j]);
						}
					}
				}
		return m1;
	}
	
	Matrix4 * scaler_multiplication(float x){
		Matrix4 *m1=new Matrix4;
		for(int i=0;i<4;i++){
			for(int j=0;j<4;j++){
        			m1->M[i][j]=M[i][j]*x;
        			
		
	}
	}
	return m1;
	}
	
	Matrix4 *transpose(){
		Matrix4 *m1=new Matrix4;
		for(int i=0;i<4;i++){
			for(int j=0;j<4;j++){
				m1->M[j][i]=M[i][j];
			
			}
		}
	return m1;
	}
	
	Vector4 *vector_multiplication(Vector4 *v){
		Vector4 *v1 = new Vector4;
		v1->x=M[0][0]*v->x+M[0][1]*v->y+M[0][2]*v->z+M[0][3]*v->w;
		v1->y=M[1][0]*v->x+M[1][1]*v->y+M[1][2]*v->z+M[1][3]*v->w;
		v1->z=M[2][0]*v->x+M[2][1]*v->y+M[2][2]*v->z+M[2][3]*v->w;
		v1->w=1;
		return v1;
	}
	
	float Determinant(){
		float det=0.0;
		det=M[0][0]*M[1][1]*M[2][2]*M[3][3]  +  M[0][0]*M[1][2]*M[2][3]*M[3][1] + M[0][0]*M[1][3]*M[2][1]*M[3][2]
		+   M[0][1]*M[1][0]*M[2][3]*M[3][2]  +  M[0][1]*M[1][2]*M[2][0]*M[3][3] + M[0][1]*M[1][3]*M[2][2]*M[3][0]
		+   M[0][2]*M[1][0]*M[2][1]*M[3][3]  +  M[0][2]*M[1][1]*M[2][3]*M[3][0] + M[0][2]*M[1][3]*M[2][0]*M[3][1]
		+   M[0][3]*M[1][0]*M[2][2]*M[3][1]  +  M[0][3]*M[1][1]*M[2][0]*M[3][2] + M[0][3]*M[1][2]*M[2][1]*M[3][0]
		
		
		-   M[0][0]*M[1][1]*M[2][3]*M[3][2]  -  M[0][0]*M[1][2]*M[2][1]*M[3][3] - M[0][0]*M[1][3]*M[2][2]*M[3][1]
		-   M[0][1]*M[1][0]*M[2][2]*M[3][3]  -  M[0][1]*M[1][2]*M[2][3]*M[3][0] - M[0][1]*M[1][3]*M[2][0]*M[3][2]
		-   M[0][2]*M[1][0]*M[2][3]*M[3][1]  -  M[0][2]*M[1][1]*M[2][0]*M[3][3] - M[0][2]*M[1][3]*M[2][1]*M[3][0]
		-   M[0][3]*M[1][0]*M[2][1]*M[3][2]  -  M[0][3]*M[1][1]*M[2][2]*M[3][0] - M[0][3]*M[1][3]*M[2][0]*M[3][1];
		return det;
	}
	
	Matrix4 *Inverse(){
		cout<<Determinant()<<endl;
		float invdet=1.0/Determinant();
		Matrix4 *m1= new Matrix4;
		m1->M[0][0]=(M[1][1]*M[2][2]*M[3][3]+M[1][2]*M[2][3]*M[3][1]+M[1][3]*M[2][1]*M[3][2]-M[1][1]*M[2][3]*M[3][2]-M[1][2]*M[2][1]*M[3][3]-M[1][3]*M[2][2]*M[3][1])*invdet;
		
		m1->M[0][1]=(M[0][1]*M[2][3]*M[3][2]+M[0][2]*M[2][1]*M[3][3]+M[0][3]*M[2][2]*M[3][1]-M[0][1]*M[2][2]*M[3][3]-M[0][2]*M[2][3]*M[3][1]-M[0][3]*M[2][1]*M[3][2])*invdet;
		
		m1->M[0][2]=(M[0][1]*M[1][2]*M[3][3]+M[0][2]*M[1][3]*M[3][1]+M[0][3]*M[1][1]*M[3][2]-M[0][1]*M[1][3]*M[3][2]-M[0][2]*M[1][1]*M[3][3]-M[0][3]*M[1][2]*M[3][1])*invdet;
		
		m1->M[0][3]=(M[0][1]*M[1][3]*M[2][2]+M[0][2]*M[1][1]*M[2][3]+M[0][3]*M[1][2]*M[2][1]-M[0][1]*M[1][2]*M[2][3]-M[0][2]*M[1][3]*M[2][1]-M[0][3]*M[1][1]*M[2][2])*invdet;
		
		m1->M[1][0]=(M[1][0]*M[2][3]*M[3][2]+M[1][2]*M[2][0]*M[3][3]+M[1][3]*M[2][2]*M[3][0]-M[1][0]*M[2][2]*M[3][3]-M[1][2]*M[2][3]*M[3][0]-M[1][3]*M[2][0]*M[3][2])*invdet;
		
		m1->M[1][1]=(M[0][0]*M[2][2]*M[3][3]+M[0][2]*M[2][3]*M[3][0]+M[0][3]*M[2][0]*M[3][2]-M[0][0]*M[2][3]*M[3][2]-M[0][2]*M[2][0]*M[3][3]-M[0][3]*M[2][2]*M[3][0])*invdet;
		
		m1->M[1][2]=(M[0][0]*M[1][3]*M[3][2]+M[0][2]*M[1][0]*M[3][3]+M[0][3]*M[1][2]*M[3][0]-M[0][0]*M[1][2]*M[3][3]-M[0][2]*M[1][3]*M[3][0]-M[0][3]*M[1][0]*M[3][2])*invdet;
		
		m1->M[1][3]=(M[0][0]*M[1][2]*M[2][3]+M[0][2]*M[1][3]*M[2][0]+M[0][3]*M[1][0]*M[2][2]-M[0][0]*M[1][3]*M[2][2]-M[0][2]*M[1][0]*M[2][3]-M[0][3]*M[1][2]*M[2][0])*invdet;
	
		m1->M[2][0]=(M[1][0]*M[2][1]*M[3][3]+M[1][1]*M[2][3]*M[3][0]+M[1][3]*M[2][0]*M[3][1]-M[1][0]*M[2][3]*M[3][1]-M[1][1]*M[2][0]*M[3][3]-M[1][3]*M[2][1]*M[3][0])*invdet;
		
		m1->M[2][1]=(M[0][0]*M[2][3]*M[3][1]+M[0][1]*M[2][0]*M[3][3]+M[0][3]*M[2][1]*M[3][0]-M[0][0]*M[2][1]*M[3][3]-M[0][1]*M[2][3]*M[3][0]-M[0][3]*M[2][1]*M[3][1])*invdet;
		
		m1->M[2][2]=(M[0][0]*M[1][1]*M[3][3]+M[0][1]*M[1][3]*M[3][0]+M[0][3]*M[1][0]*M[3][1]-M[0][0]*M[1][3]*M[3][1]-M[0][1]*M[1][0]*M[3][3]-M[0][3]*M[1][1]*M[3][0])*invdet;
		
		m1->M[2][3]=(M[0][0]*M[1][3]*M[2][1]+M[0][1]*M[1][0]*M[2][3]+M[0][3]*M[1][1]*M[2][0]-M[0][0]*M[1][1]*M[2][3]-M[0][1]*M[1][3]*M[2][0]-M[0][3]*M[1][0]*M[2][1])*invdet;
		
		
		m1->M[3][0]=(M[1][0]*M[2][2]*M[3][1]+M[1][1]*M[2][0]*M[3][2]+M[1][2]*M[2][1]*M[3][0]-M[1][0]*M[2][1]*M[3][2]-M[1][1]*M[2][2]*M[3][0]-M[1][2]*M[2][0]*M[3][1])*invdet;
		
		m1->M[3][1]=(M[0][0]*M[2][1]*M[3][2]+M[0][1]*M[2][2]*M[3][0]+M[0][2]*M[2][0]*M[3][1]-M[0][0]*M[2][2]*M[3][1]-M[0][1]*M[2][0]*M[3][2]-M[0][2]*M[2][1]*M[3][0])*invdet;
		
		m1->M[3][2]=(M[0][0]*M[1][2]*M[3][1]+M[0][1]*M[1][0]*M[3][2]+M[0][2]*M[1][1]*M[3][0]-M[0][0]*M[1][1]*M[3][2]-M[0][1]*M[1][2]*M[3][0]-M[0][2]*M[1][0]*M[3][1])*invdet;
		
		m1->M[3][3]=(M[0][0]*M[1][1]*M[2][2]+M[0][1]*M[1][2]*M[2][0]+M[0][2]*M[1][0]*M[2][1]-M[0][0]*M[1][2]*M[2][1]-M[0][1]*M[1][0]*M[2][2]-M[0][2]*M[1][1]*M[2][0])*invdet;
		
		for(int i=0;i<4;i++){
			for(int j=0;j<4;j++){
				cout<<m1->M[i][j]<<"  ";
			}
			cout<<endl;
		}
		
		return m1;
	
	}
		
};

void Translate(float dx,float dy, float dz,Vector4 *v){
	Matrix4 *m= new Matrix4;
	m->M[0][0]=1;
	m->M[0][1]=0;
	m->M[0][2]=0;
	m->M[0][3]=dx;
	m->M[1][0]=0;
	m->M[1][1]=1;
	m->M[1][2]=0;
	m->M[1][3]=dy;
	m->M[2][0]=0;
	m->M[2][1]=0;
	m->M[2][2]=1;
	m->M[2][3]=dz;
	m->M[3][0]=0;
	m->M[3][1]=0;
	m->M[3][2]=0;
	m->M[3][3]=1;
	v->homogenize();
	Vector4 *v1=m->vector_multiplication(v);
	v->x=v1->x;
	v->y=v1->y;
	v->z=v1->z;
	v->w=1;
	
	/*
	v.homogenize();
	v->x=v->x+dx;
	v->x=v->x+dx;
	v->x=v->x+dx;
	*/
}

void RotateX(Vector4 *v,float theta){
	Matrix4 *m= new Matrix4;
	m->M[0][0]=1;
	m->M[0][1]=0;
	m->M[0][2]=0;
	m->M[0][3]=0;
	m->M[1][0]=0;
	m->M[1][1]=cos(theta);
	m->M[1][2]=sin(theta);
	m->M[1][3]=0;
	m->M[2][0]=0;
	m->M[2][1]=-sin(theta);
	m->M[2][2]=cos(theta);
	m->M[2][3]=0;
	m->M[3][0]=0;
	m->M[3][1]=0;
	m->M[3][2]=0;
	m->M[3][3]=1;
	v->homogenize();
	Vector4 *v1=m->vector_multiplication(v);
	v->x=v1->x;
	v->y=v1->y;
	v->z=v1->z;
	v->w=1;
	
	
	/*Vector4 *v1= new Vector4;
	v1->x=v->x;
	v1->y=v->y*cos(theta)-v->z*sin(theta);
	v1->z=v->y*sin(theta)+v->z*cos(theta);
	v->x=v1->x;
	v->y=v1->y;
	v->z=v1->z;*/
}

void RotateY(Vector4 *v,float theta){
	Matrix4 *m= new Matrix4;
	m->M[0][0]=cos(theta);
	m->M[0][1]=0;
	m->M[0][2]=-sin(theta);
	m->M[0][3]=0;
	m->M[1][0]=0;
	m->M[1][1]=1;
	m->M[1][2]=0;
	m->M[1][3]=0;
	m->M[2][0]=sin(theta);
	m->M[2][1]=0;
	m->M[2][2]=cos(theta);
	m->M[2][3]=0;
	m->M[3][0]=0;
	m->M[3][1]=0;
	m->M[3][2]=0;
	m->M[3][3]=1;
	v->homogenize();
	Vector4 *v1=m->vector_multiplication(v);
	v->x=v1->x;
	v->y=v1->y;
	v->z=v1->z;
	v->w=1;
	
	
	/*Vector4 *v1= new Vector4;
	v1->x=v->x*cos(theta)+v->z*sin(theta);
	v1->y=v->y;
	v1->z=-v->x*sin(theta)+v->z*cos(theta);
	v->x=v1->x;
	v->y=v1->y;
	v->z=v1->z;
	*/
}

void RotateZ(Vector4 *v,float theta){
	Matrix4 *m= new Matrix4;
	m->M[0][0]=cos(theta);
	m->M[0][1]=sin(theta);
	m->M[0][2]=0;
	m->M[0][3]=0;
	m->M[1][0]=-sin(theta);
	m->M[1][1]=cos(theta);
	m->M[1][2]=0;
	m->M[1][3]=0;
	m->M[2][0]=0;
	m->M[2][1]=0;
	m->M[2][2]=1;
	m->M[2][3]=0;
	m->M[3][0]=0;
	m->M[3][1]=0;
	m->M[3][2]=0;
	m->M[3][3]=1;
	v->homogenize();
	Vector4 *v1=m->vector_multiplication(v);
	v->x=v1->x;
	v->y=v1->y;
	v->z=v1->z;
	v->w=1;
	
	/*
	Vector4 *v1= new Vector4;
	v1->x=v->x*cos(theta)-v->y*sin(theta);
	v1->y=v->x*sin(theta)+v->y*cos(theta);
	v1->z=v->z;
	v->x=v1->x;
	v->y=v1->y;
	v->z=v1->z;
	*/
}


void arbit_Rotation(float a,float b,float c,float u,float v,float w,float x,float y, float z,float theta,float &x1,float &y1,float &z1){
	float L=sqrt(u*u+v*v+w*w);
	x1=(a*(v*v +w*w)-u*(b*v+c*w-u*x-v*y-w*z))*(1-cos(theta))+L*x*cos(theta)+sqrt(L)*(-c*v+b*w-w*y+v*z)*sin(theta)/L;
	y1=(b*(u*u+w*w)-v*(a*u+c*w-u*x-v*y-w*z))*(1-cos(theta))+L*y*cos(theta)+sqrt(L)*(c*u-a*w+w*x-u*z)*sin(theta)/L;
	z1=(c*(u*u+v*v)-w*(a*u+b*v-u*x-v*y-w*z))*(1-cos(theta))+L*z*cos(theta)+sqrt(L)*(-b*u+a*v-v*x+u*y)*sin(theta)/L;
}

/*void Rotate_about_ObjectX(Vector4* v){
	float a,b,c,u,v,w,x1,y1,z1;
	


}
*/
void normalize(float &x,float &y,float &z){
	float r=sqrt(x*x + y*y +z*z);
	x=x/r;
	y=y/r;
	z=z/r;	
}

void Rotate_about_look(float theta){
	float x1,y1,z1,a,b,c,u,v,w,x3,y3,z3,x4,y4,z4,x2,y2,z2;
	x1=eyeX+upX;
	y1=eyeY+upY;
	z1=eyeZ+upZ;
	u=centreX-eyeX;
	v=centreY-eyeY;
	w=centreZ-eyeZ;
	float r= sqrt(x1*x1+y1*y1+z1*z1);
	x1=x1/r;
	y1=y1/r;
	z1=z1/r;
	arbit_Rotation(eyeX,eyeY,eyeZ,u,v,w,x1,y1,z1,theta,x3,y3,z3);
	upX=x3-eyeX;
	upY=y3-eyeY;
	upZ=z3-eyeZ;
	r=sqrt(upX*upX+upY*upY+upZ*upZ);
	upX=upX/r;
	upY=upY/r;
	upZ=upZ/r;
	
	
}

void Rotate_about_up(float theta){
	float x1,y1,z1,a,b,c,u,v,w,x3,y3,z3,x4,y4,z4,x2,y2,z2;
	x1=centreX;
	y1=centreY;
	z1=centreZ;
	u=upX;
	v=upY;
	w=upZ;
	arbit_Rotation(eyeX,eyeY,eyeZ,u,v,w,x1,y1,z1,theta,x3,y3,z3);
	centreX=x3;
	centreY=y3;
	centreZ=z3;
	//cout<<"JKJ"<<centreX<<"  "<<centreY<<"  "<<centreZ<<endl;

}

void Rotate_about_side(float theta){
	Vector4 *look;
	Vector4 *up;
	Vector4 *side;
	look= new Vector4;
	up=new Vector4;
	side= new Vector4;
	look->x=centreX-eyeX;
	look->y=centreY-eyeY;
	look->z=centreZ-eyeZ;
	look->w=1;
	normalize(look->x,look->y,look->z);
	up->x=upX;
	up->y=upY;
	up->z=upZ;
	up->w=1;
	normalize(up->x,up->y,up->z);
	side= look->cross(up);
	normalize(side->x,side->y,side->z);
	float x1,y1,z1,a,b,c,u,v,w,x3,y3,z3,x4,y4,z4,x2,y2,z2;
	x1=eyeX+upX;
	y1=eyeY+upY;
	z1=eyeZ+upZ;
	u=side->x;
	v=side->y;
	w=side->z;
	arbit_Rotation(eyeX,eyeY,eyeZ,u,v,w,x1,y1,z1,theta,x3,y3,z3);
	x2=centreX;
	y2=centreY;
	z2=centreZ;
	arbit_Rotation(eyeX,eyeY,eyeZ,u,v,w,x2,y2,z2,theta,x4,y4,z4);
	upX=x3-eyeX;
	upY=y3-eyeY;
	upZ=z3-eyeZ;
	centreX=x4;
	centreY=y4;
	centreZ=z4;
}

void Scale(Vector4 *v,float Sx,float Sy,float Sz){
	Matrix4 *m= new Matrix4;
	m->M[0][0]=Sx;
	m->M[0][1]=0;
	m->M[0][2]=0;
	m->M[0][3]=0;
	m->M[1][0]=0;
	m->M[1][1]=Sy;
	m->M[1][2]=0;
	m->M[1][3]=0;
	m->M[2][0]=0;
	m->M[2][1]=0;
	m->M[2][2]=Sz;
	m->M[2][3]=0;
	m->M[3][0]=0;
	m->M[3][1]=0;
	m->M[3][2]=0;
	m->M[3][3]=1;
	v->homogenize();
	Vector4 *v1=m->vector_multiplication(v);
	v->x=v1->x;
	v->y=v1->y;
	v->z=v1->z;
	v->w=1;
	
	/*
	Vector4 *v1= new Vector4;
	v->x=v->x*Sx;
	v->y=v->y*Sy;
	v->z=v->z*Sz; */
}

void Rotate_about_objectX(float theta){
	float u,v,w,a,b,c,x,y,z,r;
	arbit_Rotation(X,Y,Z,X2-X,Y2-Y,Z2-Z,X1,Y1,Z1,theta,x,y,z);
	X1=x;
	Y1=y;
	Z1=z;
	x=X1-X;
	y=Y1-Y;
	z=Z1-z;
	r=sqrt(x*x+y*y+z*z);
	x=x/r;
	y=y/r;
	z=z/r;
	X1=X+x;
	Y1=Y+y;
	Z1=Z+z;
	cout<<X<<"  "<<Y<<"  "<<Z<<"   "<<X1<<"  "<<Y1<<"  "<<Z1<<"   "<<X2<<"  "<<Y2<<"  "<<Z2<<endl;
	for(int i=0;i<data->numVertices;i++){
		arbit_Rotation(X,Y,Z,X2-X,Y2-Y,Z2-Z,va[i].x,va[i].y,va[i].z,theta,x,y,z);
		va[i].x=x;
		va[i].y=y;
		va[i].z=z;
	
	}
}

void Rotate_about_objectY(float theta){
	float u,v,w,a,b,c,x,y,z,r;
	arbit_Rotation(X,Y,Z,X1-X,Y1-Y,Z1-Z,X2,Y2,Z2,theta,x,y,z);
	X2=x;
	Y2=y;
	Z2=z;
	
	x=X2-X;
	y=Y2-Y;
	z=Z2-z;
	r=sqrt(x*x+y*y+z*z);
	x=x/r;
	y=y/r;
	z=z/r;
	X2=X+x;
	Y2=Y+y;
	Z2=Z+z;
	
	cout<<X<<"  "<<Y<<"  "<<Z<<"   "<<X1<<"  "<<Y1<<"  "<<Z1<<"   "<<X2<<"  "<<Y2<<"  "<<Z2<<endl;
	for(int i=0;i<data->numVertices;i++){
		arbit_Rotation(X,Y,Z,X1-X,Y1-Y,Z1-Z,va[i].x,va[i].y,va[i].z,theta,x,y,z);
		va[i].x=x;
		va[i].y=y;
		va[i].z=z;
	
	}
}

void Rotate_about_objectZ(float theta){
	float u,v,w,a,b,c,x,y,z,r;
	Vector4 *v1= new Vector4;
	Vector4 *v2=new Vector4;
	v1->x=X1-X;
	v1->y=Y1-Y;
	v1->z=Z1-Z;
	v1->w=1;
	v2->x=X2-X;
	v2->y=Y2-Y;
	v2->z=Z2-Z;
	v2->w=1;
	Vector4 *v3=v1->cross(v2);
	normalize(v3->x,v3->y,v3->z);
	
	for(int i=0;i<data->numVertices;i++){
		arbit_Rotation(X,Y,Z,v3->x,v3->y,v3->z,va[i].x,va[i].y,va[i].z,theta,x,y,z);
		va[i].x=x;
		va[i].y=y;
		va[i].z=z;	
	}
	arbit_Rotation(X,Y,Z,v3->x,v3->y,v3->z,X1,Y1,Z1,theta,x,y,z);
	X1=x;
	Y1=y;
	Z1=z;
	
	x=X1-X;
	y=Y1-Y;
	z=Z1-z;
	r=sqrt(x*x+y*y+z*z);
	x=x/r;
	y=y/r;
	z=z/r;
	X1=X+x;
	Y1=Y+y;
	Z1=Z+z;
	
	arbit_Rotation(X,Y,Z,v3->x,v3->y,v3->z,X2,Y2,Z2,theta,x,y,z);
	X2=x;
	Y2=y;
	Z2=z;
	
	x=X2-X;
	y=Y2-Y;
	z=Z2-z;
	r=sqrt(x*x+y*y+z*z);
	x=x/r;
	y=y/r;
	z=z/r;
	X2=X+x;
	Y2=Y+y;
	Z2=Z+z;
}




void LookAt(/*Vector4 *v*/){
	Vector4 *look;
	Vector4 *up;
	Vector4 *side;
	look= new Vector4;
	up=new Vector4;
	side= new Vector4;
	look->x=centreX-eyeX;
	look->y=centreY-eyeY;
	look->z=centreZ-eyeZ;
	look->w=1;
	normalize(look->x,look->y,look->z);
	
	up->x=upX;
	up->y=upY;
	up->z=upZ;
	up->w=1;
	normalize(up->x,up->y,up->z);
	
	side= look->cross(up);
	normalize(side->x,side->y,side->z);
	
	Matrix4 *m;
	GLfloat d[16];
	m= new Matrix4;
	d[0]=m->M[0][0]=side->x;
	d[1]=m->M[1][0]=side->y;
	d[2]=m->M[2][0]=side->z;
	d[3]=m->M[3][0]=0;
	
	d[4]=m->M[0][1]=up->x;
	d[5]=m->M[1][1]=up->y;
	d[6]=m->M[2][1]=up->z;
	d[7]=m->M[3][1]=0;
	
	d[8]=m->M[0][2]=-look->x;
	d[9]=m->M[1][2]=-look->y;
	d[10]=m->M[2][2]=-look->z;
	d[11]=m->M[3][2]=0;
	
	d[12]=m->M[0][3]=0;
	d[13]=m->M[1][3]=0;
	d[14]=m->M[2][3]=0;
	d[15]=m->M[3][3]=1;
	
	//Vector4 *v1= m->vector_multiplication(v);
	/*v1->x=v1->x-eyeX;
	v1->y=v1->y-eyeX;
	v1->z=v1->z-eyeX;
	v1->w=1;
	return v1;*/
	glMultMatrixf(d);
	glTranslated(-eyeX, -eyeY, -eyeZ);
}

void init(void){
   	eyeX=0.0;
   	eyeY=0.0;
   	eyeZ=1.5;
   	centreX=0.0;
   	centreY=0.0;
   	centreZ=0.0;
   	upX=0.0;
   	upY=1.0;
   	upZ=0.0;
   	X=0;
   	Y=0;
   	Z=0;
   	X1=0;
   	Y1=1;
   	Z1=0;
   	X2=1;
   	Y2=0;
   	Y2=0;
   	glColor3f(1.0,1.0,0);
   GLfloat mat_specular[] = { 0.0, 0.5, 0.0, 0.0 };
   GLfloat mat_shininess[] = { 20.0 };
  // GLfloat light_position[] = { 0, 0, 5, 1.0 };
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel (GL_SMOOTH);
   GLfloat ambient[] = { 0.2, 0.7, 0.8, 1.0 };
GLfloat diffuse[] = { 0.5, 0.8, 1.0, 1.0 };
GLfloat specular[] = { 0.0, 0.0, 0.8, 1.0 };
GLfloat position[] = { 1.0, 1.0, 2.0, 0.0 };

glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
glLightfv(GL_LIGHT0, GL_POSITION, position);

   glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
   glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
  // glLightfv(GL_LIGHT0, GL_POSITION, light_position);

   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnable(GL_DEPTH_TEST);
   
   GLfloat light1_ambient[] = { 0.7, 0.2, 0.7, 1.0 };
GLfloat light1_diffuse[] = { 0.0, 1.0, 1.0, 1.0 };
GLfloat light1_specular[] = { 0.0, 0.0, 1.0, 1.0 };
GLfloat light1_position[] = { 2.0, 2.0, 1.0, 1.0 };
GLfloat spot_direction[] = { -1.0, -1.0, 0.0 };

glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);
glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.0);
glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.1);
glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.1);

glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 45.0);
glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction);
glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 2.0);

glEnable(GL_LIGHT1);
 //  glEnable(GL_COLOR_MATERIAL);
}

void display(){
	float x,y,z,X,Y,Z,t=45,id1,id2;
	glMatrixMode(GL_MODELVIEW);		
   	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
   	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
   	Vector4 *v,*v1,*v2,*v3,*v4,*v5;
   	glBegin(GL_LINES);
    	glVertex3f(0,0,0);
    	glVertex3f(1,0,0);
    	glEnd();
    	
    	glBegin(GL_LINES);
    	glVertex3f(0,0,0);
    	glVertex3f(0,1,0);
    	glEnd();
    	
    	glBegin(GL_LINES);
    	glVertex3f(0,0,0);
    	glVertex3f(0,0,1);
    	glEnd();
    	
   	 for (int i=0; i<data->numTriangles; ++i){
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
	 v = new Vector4;
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
	glFlush();
	
	//glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	glMatrixMode(GL_PROJECTION);
   	glLoadIdentity();
   	//glOrtho(-1.0, 1.0, -1.0, 1.0, 5.0, -5.0);
   	gluPerspective(90.0,1.0,1.0,-1.0);
   //	cout<<eyeX<<"  "<<eyeY<<"  "<<eyeZ<<"  "<<centreX<<"  "<<centreY<<"  "<<centreZ<<"  "<<upX<<"  "<<upY<<"  "<<upZ<<endl;
  	//gluLookAt(1.5, 0, 0,0, 0, 0,0, 1,0);
  	LookAt();
   	glFlush(); 
   	glMatrixMode(GL_MODELVIEW);
  	//glMatrixMode(GL_MODELVIEW);
  	//glLoadIdentity();
  	
  	//cout<<"Hello"<<endl;
}

void reshape(int w,int h){
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   	glMatrixMode(GL_PROJECTION);
   	glLoadIdentity();
   	//glOrtho(-1.0, 1.0, -1.0, 1.0, 5.0, -5.0);
   	gluPerspective(90,1.0,1.0,-1.0);

   	LookAt();
   	glMatrixMode(GL_MODELVIEW);
   	glLoadIdentity();
   	
}

Vector4 * makeVector(){
	Vector4 *v= new Vector4;
	v->x=X;
	v->y=Y;
	v->z=Z;
	v->w=1;
	return v;

}

void saveVector(Vector4 * v){
	X=v->x;
	Y=v->y;
	Z=v->z;

}

Vector4 * makeVector2(){
	Vector4 *v= new Vector4;
	v->x=X1;
	v->y=Y1;
	v->z=Z1;
	v->w=1;
	return v;

}

void saveVector2(Vector4 * v){
	X1=v->x;
	Y1=v->y;
	Z1=v->z;

}

Vector4 * makeVector3(){
	Vector4 *v= new Vector4;
	v->x=X2;
	v->y=Y2;
	v->z=Z2;
	v->w=1;
	return v;

}

void saveVector3(Vector4 * v){
	X2=v->x;
	Y2=v->y;
	Z2=v->z;

}

void keyboard (unsigned char key, int x1, int y1)
{
	Vector4 *v;
	float x,y,z,r;
   switch (key) {
      case 'x':
      	eyeX+=0.01;
      	centreX+=0.01;
         glutPostRedisplay();
         break;
      case 'X':
         eyeX-=0.01;
         centreX-=0.01;
         glutPostRedisplay();
         break;
         
      case 'y':
      	eyeY+=0.01;
      	centreY+=0.01;
         glutPostRedisplay();
         break;
      case 'Y':
         eyeY-=0.01;
         centreY-=0.01;
         glutPostRedisplay();
         break;
      case 'z':
         eyeZ+=0.01;
         centreZ+=0.01;
         glutPostRedisplay();
         break;
      case 'Z':
         eyeZ-=0.01;
         centreZ-=0.01;
         glutPostRedisplay();
         break;
      case 'a':
      	x=centreX-eyeX;
      	y=centreY-eyeY;
      	z=centreZ-eyeZ;
      	r=sqrt(x*x+y*y+z*z);
      	x/=r;
      	y/=r;
      	z/=r;
      	x/=10;
      	y/=10;
      	z/=10;
      	eyeX+=x;
      	eyeY+=y;
      	eyeZ+=z;
    //  	centreX+=x;
    //  	centreY+=y;
    //  	centreZ+=z;
         glutPostRedisplay();
         break;
      case 'A':
        x=centreX-eyeX;
      	y=centreY-eyeY;
      	z=centreZ-eyeZ;
      	r=sqrt(x*x+y*y+z*z);
      	x/=r;
      	y/=r;
      	z/=r;
      	x/=10;
      	y/=10;
      	z/=10;
      	eyeX-=x;
      	eyeY-=y;
      	eyeZ-=z;
     // 	centreX-=x;
     // 	centreY-=y;
     // 	centreZ-=z;
         glutPostRedisplay();
         break;
      case 'v':
      	v=makeVector();
      	RotateY(v,0.1);
      	saveVector(v);	
      	v=makeVector2();
      	RotateY(v,0.1);
      	saveVector2(v);	
      	v=makeVector3();
      	RotateY(v,0.1);
      	saveVector3(v);	
      	for (int i=0; i<data->numVertices; ++i) {
		RotateY(&(va[i]),0.1);
		}
         glutPostRedisplay();
         break;
      case 'V':
      	v=makeVector();
      	RotateY(v,-0.1);
      	saveVector(v);
      	v=makeVector2();
      	RotateY(v,-0.1);
      	saveVector2(v);
      	v=makeVector3();
      	RotateY(v,-0.1);
      	saveVector3(v);	
      	for (int i=0; i<data->numVertices; ++i) {
		RotateY(&(va[i]),-0.1);
		}
         glutPostRedisplay();
         break;
     case 'u':
     	v=makeVector();
      	RotateX(v,0.1);
      	saveVector(v);
      	v=makeVector2();
      	RotateX(v,0.1);
      	saveVector2(v);
      	v=makeVector3();
      	RotateX(v,0.1);
      	saveVector3(v);
      	for (int i=0; i<data->numVertices; ++i) {
		RotateX(&(va[i]),0.1);
		}
         glutPostRedisplay();
         break;
      case 'U':
      	v=makeVector();
      	RotateX(v,-0.1);
      	saveVector(v);
      	v=makeVector2();
      	RotateX(v,-0.1);
      	saveVector2(v);
      	v=makeVector3();
      	RotateX(v,-0.1);
      	saveVector3(v);
      	for (int i=0; i<data->numVertices; ++i) {
		RotateX(&(va[i]),-0.1);
		}
         glutPostRedisplay();
         break;
      case 'w':
      	v=makeVector();
      	RotateZ(v,0.1);
      	saveVector(v);
      	v=makeVector2();
      	RotateZ(v,0.1);
      	saveVector2(v);
      	v=makeVector3();
      	RotateZ(v,0.1);
      	saveVector3(v);
      	for (int i=0; i<data->numVertices; ++i) {
		RotateZ(&(va[i]),0.1);
		}
         glutPostRedisplay();
         break;
      case 'W':
      	v=makeVector();
      	RotateZ(v,-0.1);
      	saveVector(v);
      	v=makeVector2();
      	RotateZ(v,-0.1);
      	saveVector2(v);
      	v=makeVector3();
      	RotateZ(v,-0.1);
      	saveVector3(v);
      	for (int i=0; i<data->numVertices; ++i) {
		RotateZ(&(va[i]),-0.1);
		}
         glutPostRedisplay();
         break;
      case '+':
      	for (int i=0; i<data->numVertices; ++i) {
		Scale(&(va[i]),1.1,1.1,1.1);
		}
         glutPostRedisplay();
         break;
      case '-':
      	for (int i=0; i<data->numVertices; ++i) {
		Scale(&(va[i]),0.9,0.9,0.9);
		}
         glutPostRedisplay();
         break;
      case 'i':
      		Rotate_about_look(0.1);
         glutPostRedisplay();
         break;
      case 'j':
      		Rotate_about_up(0.1);
         glutPostRedisplay();
         break;
       case 'k':
      		Rotate_about_side(0.1);
         glutPostRedisplay();
         break;
         
         case 'I':
      		Rotate_about_look(-0.1);
         glutPostRedisplay();
         break;
      case 'J':
      		Rotate_about_up(-0.1);
         glutPostRedisplay();
         break;
       case 'K':
      		Rotate_about_side(-0.1);
         glutPostRedisplay();
         break;
         case 'e':
         	X+=0.01;
         	X1+=0.01;
         	X2+=0.01;
      		for (int i=0; i<data->numVertices; ++i) {
		Translate(0.01,0,0,&(va[i]));
		}
         glutPostRedisplay();
         break;
      case 'f':
      		Y+=0.01;
      		Y1+=0.01;
      		Y2+=0.01;
      		for (int i=0; i<data->numVertices; ++i) {
		Translate(0.0,0.01,0,&(va[i]));
		}
         glutPostRedisplay();
         break;
       case 'g':
       		Z+=0.01;
       		Z1+=0.01;
       		Z2+=0.01;
      		for (int i=0; i<data->numVertices; ++i) {
		Translate(0.0,0.0,0.01,&(va[i]));
		}
         glutPostRedisplay();
         break;
         
        case 'E':
         	X-=0.01;
         	X1-=0.01;
         	X2-=0.01;
      		for (int i=0; i<data->numVertices; ++i) {
		Translate(-0.01,0,0,&(va[i]));
		}
         glutPostRedisplay();
         break;
      case 'F':
      		Y-=0.01;
      		Y1-=0.01;
      		Y2-=0.01;
      		for (int i=0; i<data->numVertices; ++i) {
		Translate(0.0,-0.01,0,&(va[i]));
		}
         glutPostRedisplay();
         break;
       case 'G':
       		Z-=0.01;
       		Z1-=0.01;
       		Z2-=0.01;
      		for (int i=0; i<data->numVertices; ++i) {
		Translate(0.0,0.0,-0.01,&(va[i]));
		}
         glutPostRedisplay();
         break;
         
      case 'l':
         	Rotate_about_objectX(0.1);
         glutPostRedisplay();
         break;
      case 'm':
      		Rotate_about_objectY(0.1);
         glutPostRedisplay();
         break;
       case 'n':
       		Rotate_about_objectZ(0.1);
         glutPostRedisplay();
         break;
       case 'L':
         	Rotate_about_objectX(-0.1);
         glutPostRedisplay();
         break;
      case 'M':
      		Rotate_about_objectY(-0.1);
         glutPostRedisplay();
         break;
       case 'N':
       		Rotate_about_objectZ(-0.1);
         glutPostRedisplay();
         break; 
      default:
         break;
   }
}


int main(int argc, char ** argv){	
	if (argc<2){
	cout<<"please specify the name of ifs file"<<endl;
	return 0;
	}
	data=load_ifs_file (argv[1]);
	va = new Vector4[data->numVertices];
	 for (int i=0; i<data->numVertices; ++i) {
		va[i].x=data->vertices[i].x;
		va[i].y=data->vertices[i].y;
		va[i].z=data->vertices[i].z;
		va[i].w=1;
		}
			
	glutInit(&argc,argv);				//initialize glut
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);	//Setting the display modes
	glutInitWindowSize(800,800);			//Setting up the window size
	glutInitWindowPosition(0,0);		//Setting up the window position
	glutCreateWindow("Lab 2");			//Window title
	init();					//Initialise some opengl states
	glutDisplayFunc(display);		// display callback function
	glutReshapeFunc(reshape);		// reshape callback function
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}
