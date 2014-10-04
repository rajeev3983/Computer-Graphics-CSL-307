/*
Author: Frank Pfenning
*/

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <string.h>
#include <stdio.h>
#include <cmath>
#include <iostream>
using namespace std;
#define MAX_TRIANGLES 10000
#define MAX_SPHERES 10000
#define MAX_LIGHTS 10000
#define infinity 100000000
#define rootthree 1.7320508
char *filename=0;

//different display modes
#define MODE_DISPLAY 1
#define MODE_JPEG 2
int mode=MODE_DISPLAY;

//you may want to make these smaller for debugging purposes
#define WIDTH 640
#define HEIGHT 480

//the field of view of the camera
#define fov 60.0

double buffer[WIDTH+1][HEIGHT+1][3];

struct Vertex
{
  double position[3];
  double color_diffuse[3];
  double color_specular[3];
  double normal[3];
  double shininess;
};

typedef struct _Triangle
{
  struct Vertex v[3];
} Triangle;

typedef struct _Sphere
{
  double position[3];
  double color_diffuse[3];
  double color_specular[3];
  double shininess;
  double radius;
} Sphere;

typedef struct _Light
{
  double position[3];
  double color[3];
} Light;

Triangle triangles[MAX_TRIANGLES];
Sphere spheres[MAX_SPHERES];
Light lights[MAX_LIGHTS];
double ambient_light[3];

int num_triangles=0;
int num_spheres=0;
int num_lights=0;

void plot_pixel_display(int x,int y,unsigned char r,unsigned char g,unsigned char b);
void plot_pixel_jpeg(int x,int y,unsigned char r,unsigned char g,unsigned char b);
void plot_pixel(int x,int y,unsigned char r,unsigned char g,unsigned char b);

//MODIFY THIS FUNCTION
void draw_scene()
{
  unsigned int x,y;
  //simple output
  for(x=0;x < WIDTH;x++)
  {
    glBegin(GL_POINTS);
    for(y=0;y < HEIGHT;y++)
    {
      plot_pixel(x,y,x%256,y%256,(x+y)%256);
    }
    glEnd();
    glFlush();
  }
  printf("Done!\n"); fflush(stdout);
}

void plot_pixel_display(int x,int y,unsigned char r,unsigned char g,unsigned char b)
{
  glColor3f(((double)r)/256.f,((double)g)/256.f,((double)b)/256.f);
  glVertex2i(x,y);
}

void plot_pixel_jpeg(int x,int y,unsigned char r,unsigned char g,unsigned char b)
{
  buffer[HEIGHT-y-1][x][0]=r;
  buffer[HEIGHT-y-1][x][1]=g;
  buffer[HEIGHT-y-1][x][2]=b;
}

void plot_pixel(int x,int y,unsigned char r,unsigned char g, unsigned char b)
{
  plot_pixel_display(x,y,r,g,b);
  if(mode == MODE_JPEG)
      plot_pixel_jpeg(x,y,r,g,b);
}

/*void save_jpg()
{
  Pic *in = NULL;

  in = pic_alloc(640, 480, 3, NULL);
  printf("Saving JPEG file: %s\n", filename);

  memcpy(in->pix,buffer,3*WIDTH*HEIGHT);
  if (jpeg_write(filename, in))
    printf("File saved Successfully\n");
  else
    printf("Error in Saving\n");

  pic_free(in);      

} */

//checking parse
void parse_check(const char *expected,string found)
{
  if(strcasecmp(expected,found.c_str()))
    {
      char error[100];
      printf("Expected '%s ' found '%s '\n",expected,found.c_str());
      printf("Parse error, abnormal abortion\n");
      exit(0);
    }

}

//parsing doubles
void parse_doubles(FILE*file,const char *check, double p[3])
{
  char str[100];
  fscanf(file,"%s",str);
  parse_check(check,str);
  fscanf(file,"%lf %lf %lf",&p[0],&p[1],&p[2]);
  printf("%s %lf %lf %lf\n",check,p[0],p[1],p[2]);
}

//parsing radius
void parse_rad(FILE*file,double *r)
{
  char str[100];
  fscanf(file,"%s",str);
  parse_check("rad:",str);
  fscanf(file,"%lf",r);
  printf("rad: %f\n",*r);
}

//parsing shininess
void parse_shi(FILE*file,double *shi)
{
  char s[100];
  fscanf(file,"%s",s);
  parse_check("shi:",s);
  fscanf(file,"%lf",shi);
  printf("shi: %f\n",*shi);
}

//function for reading from the file 
int loadScene(char *argv)
{
  FILE *file = fopen(argv,"r");
  if(file<=0){
  	cout<<"No such file"<<endl;
  	exit(0);
  	}
  string st;
  int number_of_objects;
  char type[50];
  int i;
  Triangle t;
  Sphere s;
  Light l;
  fscanf(file,"%i",&number_of_objects);

  printf("number of objects: %i\n",number_of_objects);
  char str[200];
	st="amb:";
  parse_doubles(file,st.c_str(),ambient_light);

  for(i=0;i < number_of_objects;i++)
    {
      fscanf(file,"%s\n",type);
      printf("%s\n",type);
      if(strcasecmp(type,"triangle")==0)
	{

	  printf("found triangle\n");
	  int j;

	  for(j=0;j < 3;j++)
	    {
	    st="pos:";
	      parse_doubles(file,st.c_str(),t.v[j].position);
	      st="nor:";
	      parse_doubles(file,st.c_str(),t.v[j].normal);
	      st="dif:";
	      parse_doubles(file,st.c_str(),t.v[j].color_diffuse);
	      st="spe:";
	      parse_doubles(file,st.c_str(),t.v[j].color_specular);
	      parse_shi(file,&t.v[j].shininess);
	    }

	  if(num_triangles == MAX_TRIANGLES)
	    {
	      printf("too many triangles, you should increase MAX_TRIANGLES!\n");
	      exit(0);
	    }
	  triangles[num_triangles++] = t;
	}
      else if(strcasecmp(type,"sphere")==0)
	{
	  printf("found sphere\n");

	  parse_doubles(file,"pos:",s.position);
	  parse_rad(file,&s.radius);
	  parse_doubles(file,"dif:",s.color_diffuse);
	  parse_doubles(file,"spe:",s.color_specular);
	  parse_shi(file,&s.shininess);

	  if(num_spheres == MAX_SPHERES)
	    {
	      printf("too many spheres, you should increase MAX_SPHERES!\n");
	      exit(0);
	    }
	  spheres[num_spheres++] = s;
	}
      else if(strcasecmp(type,"light")==0)
	{
	  printf("found light\n");
	  parse_doubles(file,"pos:",l.position);
	  parse_doubles(file,"col:",l.color);

	  if(num_lights == MAX_LIGHTS)
	    {
	      printf("too many lights, you should increase MAX_LIGHTS!\n");
	      exit(0);
	    }
	  lights[num_lights++] = l;
	}
      else
	{
	  printf("unknown type in scene description:\n%s\n",type);
	  exit(0);
	}
    }
  return 0;
}

//function for normalising the vectors
void normalize(double &x,double &y,double &z){
	double r=sqrt(x*x+y*y+z*z);
	x=x/r;
	y=y/r;
	z=z/r;
}


//fuction for checking intersection with sphere
double intersection_with_sphere(double start_x, double start_y,double start_z,double x,double y,double z,int i,double &t){
	double radius, pos_x,pos_y,pos_z,A,B,C,k,t0,t1;
	radius=spheres[i].radius;
	pos_x=spheres[i].position[0];
	pos_y=spheres[i].position[1];
	pos_z=spheres[i].position[2];
	A=1;
	B=2*(x*(start_x-pos_x)+y*(start_y-pos_y)+z*(start_z-pos_z));
	C=(pow(start_x-pos_x,2)+pow(start_y-pos_y,2)+pow(start_z-pos_z,2)) - radius*radius;
	k=B*B-4*A*C;
	if(k<0)
		return infinity;
	t0=(-B-sqrt(k))/2;	//points of itersection
	t1=(-B+sqrt(k))/2;
	if(t0<0)
		t0=infinity;	//intesection behind the camera
	if(t1<0)
		t1=infinity;
	t=min(t0,t1);  //smallest positive intersection
	return t;
}


//function for checking if a point is inside the triangle
bool PointInTriangle(int i,double  x,double y,double z)
{
	double v0[3],v1[3],v2[3],dot00,dot01,dot02,dot11,dot12,invDenom,u,v;
	v0[0]=triangles[i].v[2].position[0]-triangles[i].v[0].position[0];
	v0[1]=triangles[i].v[2].position[1]-triangles[i].v[0].position[1];
	v0[2]=triangles[i].v[2].position[2]-triangles[i].v[0].position[2];
	
	v1[0]=triangles[i].v[1].position[0]-triangles[i].v[0].position[0];
	v1[1]=triangles[i].v[1].position[1]-triangles[i].v[0].position[1];
	v1[2]=triangles[i].v[1].position[2]-triangles[i].v[0].position[2];
	
	v2[0]=x-triangles[i].v[0].position[0];
	v2[1]=y-triangles[i].v[0].position[1];
	v2[2]=z-triangles[i].v[0].position[2];
	
	dot00=(v0[0]*v0[0]+v0[1]*v0[1]+v0[2]*v0[2]);
	dot01=(v0[0]*v1[0]+v0[1]*v1[1]+v0[2]*v1[2]);
	dot02=(v0[0]*v2[0]+v0[1]*v2[1]+v0[2]*v2[2]);
	dot11=(v1[0]*v1[0]+v1[1]*v1[1]+v1[2]*v1[2]);
	dot12=(v1[0]*v2[0]+v1[1]*v2[1]+v1[2]*v2[2]);
	
	invDenom=1/(dot00*dot11-dot01*dot01);
	u=(dot11*dot02-dot01*dot12)*invDenom;
	v = (dot00 * dot12 - dot01 * dot02) * invDenom;
	return (u >= 0) && (v >= 0) && (u + v <= 1);
 
}


double det(double x11,double x12,double x13,double x21,double x22,double x23,double x31,double x32,double x33){
	return x11*x22*x33+x21*x32*x13+x31*x12*x23-x11*x32*x23-x31*x22*x13-x21*x12*x33;
}


//fuction for checking intersection with the triangle
double intersection_with_triangle(double start_x,double start_y,double start_z,double x,double y, double z,int i,double &t){
	double nor_x,nor_y,nor_z,x1,y1,z1,d,den;
	x1=triangles[i].v[0].position[0];
	y1=triangles[i].v[0].position[1];
	z1=triangles[i].v[0].position[2];
	double temp11=triangles[i].v[0].position[0]-triangles[i].v[1].position[0];
	double temp12=triangles[i].v[0].position[1]-triangles[i].v[1].position[1];
	double temp13=triangles[i].v[0].position[2]-triangles[i].v[1].position[2]; //finding normal to the triangle
	double temp21=triangles[i].v[2].position[0]-triangles[i].v[1].position[0];
	double temp22=triangles[i].v[2].position[1]-triangles[i].v[1].position[1];
	double temp23=triangles[i].v[2].position[2]-triangles[i].v[1].position[2];
	nor_x=temp12*temp23-temp22*temp13;
	nor_y=temp21*temp13-temp11*temp23;
	nor_z=temp11*temp22-temp21*temp12;
	normalize(nor_x,nor_y,nor_z);
	
	d=nor_x*(-x1)+nor_y*(-y1)+ nor_z*(-z1); //from the equation of plane
	den=nor_x*x+nor_y*y+nor_z*z;
	if(den==0){
		//cout<<i;
		return infinity;
		}
	t=-(nor_x*start_x+nor_y*start_y+nor_z*start_z+d)/den; //intersection
	if(t<0){
		return infinity;
	}
	x1=start_x+t*x; 
	y1=start_y+t*y;	//points of intersection
	z1=start_z+t*z;
	if(PointInTriangle(i,x1,y1,z1)){ //checking if the point of iteresection lies inside the triangle
		
		return t;
		}
	return infinity;
}


//function for checking intersection with shadow ray
bool checkshadow(double x, double y, double z, int i){
	double l_x,l_y,l_z,s_x,s_y,s_z,t,closest,j,t1,t2,d;
	l_x=lights[i].position[0];
	l_y=lights[i].position[1];
	l_z=lights[i].position[2];
	s_x=x-l_x;
	s_y=y-l_y;
	s_z=z-l_z;
	t=sqrt(s_x*s_x+s_y*s_y+s_z*s_z);
	normalize(s_x,s_y,s_z);
	closest=infinity;
	t2=infinity;
				
	for(int j=0;j<num_triangles;j++){	//checking intersection with spheres
				d=intersection_with_triangle(l_x,l_y,l_z,s_x,s_y,s_z,j,t1);
				if(d<closest){
				closest=d;	//closest point of intersection
				t2=d;
				}
			}
			
	for(int j=0;j<num_spheres;j++){		//checking intersection with triangles
				d=intersection_with_sphere(l_x,l_y,l_z,s_x,s_y,s_z,j,t1);
				if(d<closest){
				closest=d;	//closest point of intersection
				t2=t1;
				}
				}
	
		
	return t>t2+0.001; //condition of shadow

}

//finding area of a triangle with given sides
double area(double a,double b,double c){
	double s=(a+b+c)/2;
	return sqrt(s*(s-a)*(s-b)*(s-c));
	
}

//function for recursive interpolation
void recursive(double start_x,double start_y,double start_z,double dir_x,double dir_y,double dir_z,int depth,double &a1,double &a2,double &a3){
		if(depth==3){
			a1=ambient_light[0];
			a2=ambient_light[1];
			a3=ambient_light[2];
			a1=1;
			a2=1;
			a3=1;
			return;
			}
		double i,x1,y1,z1,t,d,closest,t1;
		int closest_index;
		bool tri;
		closest=infinity;
		for(i=0;i<num_spheres;i++){
				d=intersection_with_sphere(start_x,start_y,start_z,x1,y1,z1,i,t);
				if(d<closest && d>0.01){
				closest=d;
				closest_index=i;
				tri=false;
				t1=t;
				}
				}
			for(i=0;i<num_triangles;i++){
				d=intersection_with_triangle(start_x,start_y,start_z,x1,y1,z1,i,t);
				if(d<closest && d>0.01){
				closest=d;
				closest_index=i;
				tri=true;
				t1=t;
				}
				}
		if(closest==infinity){
		a1=ambient_light[0];
		a2=ambient_light[1];
		a3=ambient_light[2];
		a1=1;
		a2=1;
		a3=1;
		return;
		}
		
		if(tri){
		double s1,s2,s3,R[3],N[3],a11=0,a12=0,a13=0;
		s1=start_x+dir_x*t1;
		s2=start_y+dir_y*t1;
		s3=start_z+dir_z*t1;
		N[0]=triangles[closest_index].v[0].normal[0];
		N[1]=triangles[closest_index].v[0].normal[1];
		N[2]=triangles[closest_index].v[0].normal[2];
		double k2=(dir_x*N[0]+dir_y*N[1]+dir_z*N[2]);
		R[0]=2*N[0]*k2-dir_x;
		R[1]=2*N[1]*k2-dir_y;
		R[2]=2*N[2]*k2-dir_z;
		normalize(R[0],R[1],R[2]);
		recursive(s1,s2,s3,R[0],R[1],R[2],depth+1,a11,a12,a13);
		a1=triangles[closest_index].v[0].color_specular[0]*a11;
		a2=triangles[closest_index].v[0].color_specular[1]*a12;
		a2=triangles[closest_index].v[0].color_specular[2]*a12;
		return;
		}
		else{
		double ks[3],R[3],a11=0,a12=0,a13=0,s1,s2,s3,N[3];
		s1=start_x+dir_x*t1;
		s2=start_y+dir_y*t1;
		s3=start_z+dir_z*t1;
		N[0]=(x1*t1-spheres[(int)(closest_index)].position[0])/spheres[(int)(closest_index)].radius;
		N[1]=(y1*t1-spheres[(int)(closest_index)].position[1])/spheres[(int)(closest_index)].radius;
		N[2]=(z1*t1-spheres[(int)(closest_index)].position[2])/spheres[(int)(closest_index)].radius;
		normalize(N[0],N[1],N[2]);
		double k2=(dir_x*N[0]+dir_y*N[1]+dir_z*N[2]);
		R[0]=2*N[0]*k2-dir_x;
		R[1]=2*N[1]*k2-dir_y;
		R[2]=2*N[2]*k2-dir_z;
		recursive(s1,s2,s3,R[0],R[1],R[2],depth+1,a11,a12,a13);
		a1=spheres[closest_index].color_specular[0]*a11;
		a2=spheres[closest_index].color_specular[1]*a12;
		a2=spheres[closest_index].color_specular[2]*a12;
		return;
		
		}
				
		


}


//function for drawing the whole scene
void calculate(){
	double x,y,z,Z,closest,i,j,k,d,x1,y1,z1,t1,closest_index,kd[3],ks[3],n,I[3],t,L[3],N[3],R[3],V[3];
	Z=-(HEIGHT)/(2*tan(fov*3.1415926/360));
	//Z=-240*rootthree;
	bool tri;
	for(x=-WIDTH/2;x<=WIDTH/2;x++){
		
		for(y=-HEIGHT/2;y<=HEIGHT/2;y++){ //X and Y for each pixel
			I[0]=0;
			I[1]=0;
			I[2]=0;
			z=Z;
			x1=x;
			y1=y;
			z1=z;
			normalize(x1,y1,z1);
			closest=infinity;
			//cout<<y<<endl;
			for(i=0;i<num_spheres;i++){
				d=intersection_with_sphere(0,0,0,x1,y1,z1,i,t); // checking intersection with sphere
				if(d<closest){
				closest=d;	//closest point till now
				closest_index=i;
				tri=false;
				t1=t;
				}
				}
			for(i=0;i<num_triangles;i++){
				d=intersection_with_triangle(0,0,0,x1,y1,z1,i,t); // checking intersection with triangle
				if(d<closest){
				closest=d; //closest point till now
				closest_index=i; 
				tri=true;
				t1=t;
				}
				}
			if(closest==infinity){
				buffer[(int)x+WIDTH/2][(int)y+HEIGHT/2][0]=I[0];
				buffer[(int)x+WIDTH/2][(int)y+HEIGHT/2][1]=I[1];
				buffer[(int)x+WIDTH/2][(int)y+HEIGHT/2][2]=I[2];
				continue;
			}
			
		
			else if(tri){
				double a,b,c,t0,t2,t3;
				
			//Area wise interpolation 
				a=sqrt(pow(triangles[(int)closest_index].v[0].position[0]-triangles[(int)closest_index].v[1].position[0],2)+pow(triangles[(int)closest_index].v[0].position[1]-triangles[(int)closest_index].v[1].position[1],2)+pow(triangles[(int)closest_index].v[0].position[2]-triangles[(int)closest_index].v[1].position[2],2));
				//side of triangle
				b=sqrt(pow(triangles[(int)closest_index].v[1].position[0]-triangles[(int)closest_index].v[2].position[0],2)+pow(triangles[(int)closest_index].v[1].position[1]-triangles[(int)closest_index].v[2].position[1],2)+pow(triangles[(int)closest_index].v[1].position[2]-triangles[(int)closest_index].v[2].position[2],2));
				//side of triangle
				c=sqrt(pow(triangles[(int)closest_index].v[2].position[0]-triangles[(int)closest_index].v[0].position[0],2)+pow(triangles[(int)closest_index].v[2].position[1]-triangles[(int)closest_index].v[0].position[1],2)+pow(triangles[(int)closest_index].v[2].position[2]-triangles[(int)closest_index].v[0].position[2],2));
				double A=area(a,b,c);
				//Area of complete triangle
				t0=sqrt(pow(triangles[(int)closest_index].v[0].position[0]-x1*t1,2)+pow(triangles[(int)closest_index].v[0].position[1]-y1*t1,2)+pow(triangles[(int)closest_index].v[0].position[2]-z1*t1,2));				
				t2=sqrt(pow(triangles[(int)closest_index].v[1].position[0]-x1*t1,2)+pow(triangles[(int)closest_index].v[1].position[1]-y1*t1,2)+pow(triangles[(int)closest_index].v[1].position[2]-z1*t1,2));
				t3=sqrt(pow(triangles[(int)closest_index].v[2].position[0]-x1*t1,2)+pow(triangles[(int)closest_index].v[2].position[1]-y1*t1,2)+pow(triangles[(int)closest_index].v[2].position[2]-z1*t1,2));
				double A3=area(a,t0,t2);
				double A1=area(b,t2,t3);
				double A2=area(c,t3,t0);
				A1/=A;
				A2/=A;
				A3/=A;   
				
				//interpolation for diffuse colour
				kd[0]=(triangles[(int)closest_index].v[0].color_diffuse[0]*A1+triangles[(int)closest_index].v[1].color_diffuse[0]*A2+triangles[(int)closest_index].v[2].color_diffuse[0]*A3);
				kd[1]=(triangles[(int)closest_index].v[0].color_diffuse[1]*A1+triangles[(int)closest_index].v[1].color_diffuse[1]*A2+triangles[(int)closest_index].v[2].color_diffuse[1]*A3);
				kd[2]=(triangles[(int)closest_index].v[0].color_diffuse[2]*A1+triangles[(int)closest_index].v[1].color_diffuse[2]*A2+triangles[(int)closest_index].v[2].color_diffuse[2]*A3);
				//interpolation for specular colour
				ks[0]=(triangles[(int)closest_index].v[0].color_specular[0]*A1+triangles[(int)closest_index].v[1].color_specular[0]*A2+triangles[(int)closest_index].v[2].color_specular[0]*A3);
				ks[1]=(triangles[(int)closest_index].v[0].color_specular[1]*A1+triangles[(int)closest_index].v[1].color_specular[1]*A2+triangles[(int)closest_index].v[2].color_specular[1]*A3);
				ks[2]=(triangles[(int)closest_index].v[0].color_specular[2]*A1+triangles[(int)closest_index].v[1].color_specular[2]*A2+triangles[(int)closest_index].v[2].color_specular[2]*A3);
				//interpolation for shininess
				n=(triangles[(int)closest_index].v[0].shininess*A1)+(triangles[(int)closest_index].v[1].shininess*A2)+(triangles[(int)closest_index].v[2].shininess*A3);
				n=(int)(n+0.1);
				//interpolation for normals
				N[0]=(triangles[(int)closest_index].v[0].normal[0]*A1+triangles[(int)closest_index].v[1].normal[0]*A2+triangles[(int)closest_index].v[2].normal[0]*A3);
				N[1]=(triangles[(int)closest_index].v[0].normal[1]*A1+triangles[(int)closest_index].v[1].normal[1]*A2+triangles[(int)closest_index].v[2].normal[1]*A3);
				N[2]=(triangles[(int)closest_index].v[0].normal[2]*A1+triangles[(int)closest_index].v[1].normal[2]*A2+triangles[(int)closest_index].v[2].normal[2]*A3);
				
				normalize(N[0],N[1],N[2]);
			}
			else{
				//diffuse colour of sphere
				kd[0]=spheres[(int)closest_index].color_diffuse[0];
				kd[1]=spheres[(int)closest_index].color_diffuse[1];
				kd[2]=spheres[(int)closest_index].color_diffuse[2];
				//specular constant for sphere
				ks[0]=spheres[(int)closest_index].color_specular[0];
				ks[1]=spheres[(int)closest_index].color_specular[1];
				ks[2]=spheres[(int)closest_index].color_specular[2];
				n=spheres[(int)closest_index].shininess;
				//shininess for spheres
				N[0]=(x1*t1-spheres[(int)(closest_index)].position[0])/spheres[(int)(closest_index)].radius;
				N[1]=(y1*t1-spheres[(int)(closest_index)].position[1])/spheres[(int)(closest_index)].radius;
				N[2]=(z1*t1-spheres[(int)(closest_index)].position[2])/spheres[(int)(closest_index)].radius;
				//normal to the sphere
				normalize(N[0],N[1],N[2]);
				
				
			}
			x1=x1*t1;
			y1=y1*t1;	//point of intersections
			z1=z1*t1;
			
			
			for(i=0;i<num_lights;i++){
				L[0]=x1-lights[(int)i].position[0];
				L[1]=y1-lights[(int)i].position[1];
				L[2]=z1-lights[(int)i].position[2];
				normalize(L[0],L[1],L[2]);
				
					
			k=(-L[0]*N[0]+(-L[1])*N[1]+(-L[2])*N[2]);// dot product of light vector and normal vector
			
			if(k<0)
				k=0;
			
			
			
			double k2=(L[0]*N[0]+L[1]*N[1]+L[2]*N[2]);
			double x3,y3,z3,x2,y2,z2;
			x3=x1;
			y3=y1;
			z3=z1;
			x2=2*N[0]*k2-L[0];	//reflected ray
			y2=2*N[1]*k2-L[1];
			z2=2*N[2]*k2-L[2];
			normalize(x3,y3,z3);
			normalize(x2,y2,z2);
			double k4=(x2*x3+y2*y3+z2*z3); //dot product of reflected ray with viewing vector
			//k4=abs(k4);
			k4=(pow(k4,n));
			if(k4<0)
				k4=0;
			/*double a1=0,a2=0,a3=0;
			recursive(x3,y3,z3,x2,y2,z2,1,a1,a2,a3);
			I[0]+=ks[0]*a1;
			I[1]+=ks[1]*a2;
			I[2]+=ks[2]*a3; */
			if(checkshadow(x1,y1,z1,(int)i)) //checking for shadow ray
					continue;
				
				for(j=0;j<3;j++){
				I[(int)j]+=lights[(int)i].color[(int)j]*(kd[(int)j]*k + ks[(int)j]*k4); //diffuse and specular colour
				}	
			}
			for(i=0;i<3;i++){
				I[(int)i]+=ambient_light[(int)i];	//ambient illumination
			}
			
			
				buffer[(int)x+WIDTH/2][(int)y+HEIGHT/2][0]=I[0];
				buffer[(int)x+WIDTH/2][(int)y+HEIGHT/2][1]=I[1];
				buffer[(int)x+WIDTH/2][(int)y+HEIGHT/2][2]=I[2];
			glBegin(GL_POINTS);
			glColor3f(buffer[(int)x+WIDTH/2][(int)y+HEIGHT/2][0],buffer[(int)x+WIDTH/2][(int)y+HEIGHT/2][1],buffer[(int)x+WIDTH/2][(int)y+HEIGHT/2][2]);
   			glVertex2f(x+WIDTH/2,y+HEIGHT/2);	//drawing the pixel
   			glEnd();
			
			
		}
	glFlush();
	}
	
}

void display()
{	
	
}

void init()
{
  glMatrixMode(GL_PROJECTION);
  glOrtho(0,WIDTH,0,HEIGHT,1,-1);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glClearColor(0,0,0,0);
  glClear(GL_COLOR_BUFFER_BIT);
}


//idle function for drawing only once
void idle()
{
  //hack to make it only draw once
  static int once=0;
  if(!once)
  {
      calculate(); //function for redering the objects
    }
  once=1;
}


int main (int argc, char ** argv)
{
  if (argc<2)
  {  
    printf ("Please specify the input file\n");
    exit(0);
  }
  if(argc == 3)
    {
      mode = MODE_JPEG;
      filename = argv[2];
    }
  else if(argc == 2)
    mode = MODE_DISPLAY;

  glutInit(&argc,argv);
  loadScene(argv[1]);

  glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
  glutInitWindowPosition(0,0);
  glutInitWindowSize(WIDTH,HEIGHT);
  int window = glutCreateWindow("Ray Tracer");
   
  glutDisplayFunc(display);
   glutIdleFunc(idle);
  init();
  glutMainLoop();
}
