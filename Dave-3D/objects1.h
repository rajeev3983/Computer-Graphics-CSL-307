#include <iostream>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cmath>
#include "initial.h"
#define HEIGHT 700
#define WIDTH 1200
#define segments 20
#define increment 5
#define dp 10
#define gr 9.8
#define tick 0.01
#define h 20
#define tock 1
using namespace std;

void normalise(double &a,double &b,double &c){
	double r=sqrt(a*a+b*b+c*c);
	a/=r;
	b/=r;
	c/=r;
}
class enemy1{
	public:
		double x,z,x_min,x_max,v_x;
		
		void set(double a, double b, double c){
			x_min=a;
			x_max=b;
			x=x_min;
			z=c;
			v_x=(x_max-x_min)/25;
			
		}
		
		
		void draw(){
			GLfloat mat_specular[] = { 0.0, 0.0, 0.0, 1.0 };
  			GLfloat light_diffuse[] = { 0.0, 0.0, 0.0, 1 };
  			GLfloat mat_shininess[] = { 50.0 };
  			glShadeModel(GL_SMOOTH);
   			GLfloat mat_amb_diff[] = { 0, 0, 0, 1.0 };
  			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
   			glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
   			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, light_diffuse);
			glPushMatrix();
			glTranslatef(x,5,z);
			glutSolidSphere( 5.0,32,32);
			glPopMatrix();
		}
		
		void next(){
			x+=v_x*tock;
			if(x>=x_max)
				v_x=-v_x;
			if(x<=x_min)
				v_x=-v_x;
		}
};

class enemy2{
	public:
		double x,z,z_min,z_max,v_z;
		
		void set(double a, double b, double c){
			x=a;
			z_min=b;
			z_max=c;
			z=c;
			v_z=(z_max-z_min)/25;
		}
		
		
		void draw(){
			GLfloat mat_specular[] = { 0.0, 0.0, 0.0, 1.0 };
  			GLfloat light_diffuse[] = { 0.0, 0.0, 0.0, 1 };
  			GLfloat mat_shininess[] = { 50.0 };
  			glShadeModel(GL_SMOOTH);
   			GLfloat mat_amb_diff[] = { 0, 0, 0, 1.0 };
  			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
   			glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
   			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, light_diffuse);
			glPushMatrix();
			glTranslatef(x,5,z);
			glutSolidSphere( 5.0,32,32);
			glPopMatrix();
		}
		
		void next(){
			z+=v_z*tock;
			if(z>=z_max)
				v_z=-v_z;
			if(z<=z_min)
				v_z=-v_z;
		}
};




class wall{
	public:
	double x1,z1,x2,z2;
	double nx,ny,nz;
	void set(double a,double b, double c,double d){
		x1=a;
		z1=b;
		x2=c;
		z2=d;
		nx=h*(z2-z1);
		ny=0;
		nz=-h*(x2-x1);
		normalise(nx,ny,nz);
	}
	
	void draw(){
	
   		glEnable(GL_TEXTURE_2D);
   		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
   		glBindTexture(GL_TEXTURE_2D, 2001);
		glBegin(GL_POLYGON);
		double t=max(fabs(x2-x1),fabs(z2-z1));
		glNormal3f(nx,ny,nz);
		glTexCoord2f(0.0, 0.0);glVertex3f(x1,0,z1);
		glTexCoord2f(0.0, 2.0); glVertex3f(x1,h,z1);
		glTexCoord2f(t/10, 2.0);glVertex3f(x2,h,z2);
		 glTexCoord2f(t/10, 0.0);glVertex3f(x2,0,z2);
		glEnd();
		glDisable(GL_TEXTURE_2D);
	
	}
};

class floor2{
	public:
	double x1,z1,x2,z2;
	double nx,ny,nz;
	void set(double a,double b, double c,double d){
		x1=a;
		z1=b;
		x2=c;
		z2=d;
		nx=0;
		ny=1;
		nz=0;
	}
	
	void draw(){
		glBegin(GL_QUADS);
		glNormal3f(nx,ny,nz);
		glVertex3f(x1,0,z1);
		glVertex3f(x2,0,z1);
		glVertex3f(x2,0,z2);
		glVertex3f(x1,0,z2);
		glEnd();
	
	}

};

class key{
	public:
		double x,z,side;
	void set(double a,double b){
		x=a;
		z=b;
	}
	
	void draw(){
		GLfloat mat_specular[] = { 1.0, 0.0, 0.0, 1.0 };
  		GLfloat light_diffuse[] = { 1.0, 0, 0.0, 1 };
  		GLfloat mat_shininess[] = { 50.0 };
  		glShadeModel(GL_SMOOTH);
   		GLfloat mat_amb_diff[] = { 0, 0, 0, 1.0 };
  		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
   		glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
   		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, light_diffuse);
		glPushMatrix();
		glTranslatef(x,10,z);
		glutSolidCube(2);
		glPopMatrix();
	}
};

class treasure{
	public:
		double x,z;
		void set(double a,double b){
		x=a;
		z=b;
		}
		void draw(){
		GLfloat mat_specular[] = { 1.0, 1.0, 0.0, 1.0 };
  		GLfloat light_diffuse[] = { 1.0, 1.0, 0.0, 1 };
  		GLfloat mat_shininess[] = { 50.0 };
  		glShadeModel(GL_SMOOTH);
   		GLfloat mat_amb_diff[] = { 0, 0, 0, 1.0 };
  		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
   		glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
   		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, light_diffuse);
		glPushMatrix();
		glTranslatef(x,5,z);
		glutWireTeapot(2);
		glPopMatrix();
	}
};


class ball{
	public:
		double x,y,z;
		double radius;
		double r,g,b;
		double vx;
		double vy;
		double ax;
		double ay;
		bool flag[5];
		void draw(){
			 GLfloat mat_specular[] = { r, g, b, 1.0 };
  			 GLfloat mat_shininess[] = { 50.0 };
   			 GLfloat light_diffuse[] = { r, g, b, 1.0 };
 
  				 glShadeModel (GL_SMOOTH);

  				 glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
  				 glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
   				glMaterialfv(GL_FRONT,GL_DIFFUSE,light_diffuse);
   
   
			//glColor3f(r,g,b);
			glPushMatrix();
			glTranslatef(x,y,0);
			glutSolidSphere(radius,10,10);
			glPopMatrix();
			
		/*	glBegin( GL_TRIANGLE_FAN );
			glColor4f(r,g,b,1);
       			glVertex3f(x, y,0);
        		for( int n = 0; n <= segments; ++n ) {
           			 float const t = 2*M_PI*(float)n/(float)segments;
            				glVertex3f(x + sin(t)*radius, y + cos(t)*radius,0);
       					 }
   				 glEnd();
   		*/		
		}
		
		void setColor(double a,double b1,double c){
			r=a;
			g=b1;
			b=c;
		}
		
		void set(double a,double b1){
			x=a;
			y=b1;
			vx=0;
			ax=0;
			vy=0;
			ay=0;
			for(int i=0;i<5;i++)
				flag[i]=false;
		}
		void next(){
			vx+=ax*tick;
			vy+=ay*tick;
			x+=vx*tick;
			y+=vy*tick;
			
		}
		
};

class wedge{
	public:
		double alpha;
		double x,y;
		double length;
		double r,g,b;
		
		void setColor(double a,double b1,double c){
			r=a;
			g=b1;
			b=c;
		}
		
		void set(double a,double b1,double c,double d){
			x=a;
			y=b1;
			length=c;
			alpha=(M_PI*d)/180;
		}
		
		void draw(){
			 GLfloat mat_specular[] = { r, g, b, 1.0 };
  			 GLfloat mat_shininess[] = { 50.0 };
   			 GLfloat light_diffuse[] = { r, g, b, 1.0 };
 
  				 glShadeModel (GL_SMOOTH);

  				 glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
  				 glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
   				glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,light_diffuse);
   
			glBegin(GL_POLYGON);
			glNormal3f(0,0,1);
			glVertex3f(x, y,dp);
			glVertex3f(x+length,y,dp);
			glVertex3f(x+length,y+length*tan(alpha),dp);
			glEnd();
			
			glBegin(GL_POLYGON);
			glNormal3f(0,0,-1);
			glVertex3f(x, y,-dp);
			glVertex3f(x+length,y,-dp);
			glVertex3f(x+length,y+length*tan(alpha),-dp);
			glEnd();
			
			glBegin(GL_POLYGON);
			glNormal3f(-sin(alpha),cos(alpha),0);
			glVertex3f(x, y,dp);
			glVertex3f(x, y,-dp);
			glVertex3f(x+length,y+length*tan(alpha),-dp);
			glVertex3f(x+length,y+length*tan(alpha),dp);
			glEnd();
			
			glBegin(GL_POLYGON);
			glNormal3f(1,0,0);
			glVertex3f(x+length, y,dp);
			glVertex3f(x+length, y,-dp);
			glVertex3f(x+length,y+length*tan(alpha),-dp);
			glVertex3f(x+length,y+length*tan(alpha),dp);
			glEnd();
			
			glBegin(GL_POLYGON);
			glNormal3f(0,-1,0);
			glVertex3f(x, y,dp);
			glVertex3f(x, y,-dp);
			glVertex3f(x+length,y,-dp);
			glVertex3f(x+length,y,dp);
			glEnd();
			
			
			
			
			
			
		}
};

class Floor{
	public:
		double x,y,z;
		double length;
		double r,g,b;
		void setColor(double a,double b1,double c){
			r=a;
			g=b1;
			b=c;
		}
		
		void set(double a,double b1,double c){
			x=a;
			y=b1;
			length=c;
		}
		void draw(){
				 GLfloat mat_specular[] = { r, g, b, 1.0 };
  			 GLfloat mat_shininess[] = { 50.0 };
   			 GLfloat light_diffuse[] = { r, g, b, 1.0 };
 
  				 glShadeModel (GL_SMOOTH);

  				 glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
  				 glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
   				glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,light_diffuse);
				glBegin(GL_POLYGON);
				glNormal3f(0,-1,0);
				glVertex3f(x,y,dp);
				glVertex3f(x+length,y,dp);
				glVertex3f(x+length,y,-dp);
				glVertex3f(x,y,-dp);
				
				glEnd();
			for(int i=x+increment;i<x+length;i+=increment){
				glBegin(GL_LINES);
				glVertex3f(i,y,0);
				glVertex3f(i-increment,y-increment,0);
				glEnd();
			}
			
		}
};

class ceiling{
	public:
		double x,y,z;
		double length;
		double r,g,b;
		double thread_l;
		double theta;
		double velocity;
		double acceleration;
		ball myball;
		void setColor(double a,double b1,double c){
			r=a;
			g=b1;
			b=c;
		}
		
		void set(double a,double b1,double c,double d){
			x=a;
			y=b1;
			length=c;
			thread_l=d;
			velocity=0;
			myball.setColor(1,0,0);
			myball.radius=10;
			
		}
		
		void draw(){
				
				 GLfloat mat_specular[] = { r, g, b, 1.0 };
  			 GLfloat mat_shininess[] = { 50.0 };
   			 GLfloat light_diffuse[] = { r, g, b, 1.0 };
 
  				 glShadeModel (GL_SMOOTH);

  				 glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
  				 glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
   				glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,light_diffuse);
				glBegin(GL_POLYGON);
				glNormal3f(0,-1,0);
				glVertex3f(x,y,dp);
				glVertex3f(x+length,y,dp);
				glVertex3f(x+length,y,-dp);
				glVertex3f(x,y,-dp);
				
				glEnd();
				
			for(int i=x+increment;i<x+length;i+=increment){
				glBegin(GL_LINES);
				glVertex3f(i,y,0);
				glVertex3f(i-increment,y+increment,0);
				glEnd();
			}
			myball.x=x+length/2-thread_l*sin(theta);
			myball.y=y-thread_l*cos(theta)-myball.radius;
			glBegin(GL_LINES);
   			glColor3f(1,1,1);
   			glVertex3f(x+length/2,y,0);
   			glVertex3f(myball.x,myball.y+myball.radius,0);
   			glEnd();
   			myball.draw();
		}
		
		void next(){
			acceleration=gr*sin(theta);
			velocity+=acceleration*tick;
			theta-=velocity*tick;
			myball.x=x+length/2-thread_l*sin(theta);
			myball.y=y-thread_l*cos(theta);
			//myball.set(x+length/2-thread_l*sin(theta),y-thread_l*cos(theta));
			
		
		}
		
};

class pivot{
	public:
		double x,y;
		double length;
		double r,g,b;
		double theta;
		double width;
		bool flag[5];
		void setColor(double a,double b1,double c){
			r=a;
			g=b1;
			b=c;
		}
		
		void set(double a,double b1,double c,double d){
			x=a;
			y=b1;
			length=c;
			theta=d;
			width=10;
			for(int i=0;i<5;i++)
				flag[i]=0;
		}
		
		void draw(){
			
		
			
			double t=M_PI*theta/180;
			GLfloat mat_specular[] = { r, g, b, 1.0 };
  			 GLfloat mat_shininess[] = { 50.0 };
   			 GLfloat light_diffuse[] = { r, g, b, 1.0 };
 
  				 glShadeModel (GL_SMOOTH);

  				 glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
  				 glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
   				glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,light_diffuse);
   				
			
			glBegin(GL_POLYGON);
			glNormal3f(0,0,1);
			glVertex3f(x-length*cos(t)-width*sin(t)/2,y+length*sin(t)-width*cos(t)/2,dp);
			glVertex3f(x-length*cos(t)+width*sin(t)/2,y+length*sin(t)+width*cos(t)/2,dp);
			glVertex3f(x+length*cos(t)+width*sin(t)/2,y-length*sin(t)+width*cos(t)/2,dp);
			glVertex3f(x+length*cos(t)-width*sin(t)/2,y-length*sin(t)-width*cos(t)/2,dp);
			glEnd();
			
			glBegin(GL_POLYGON);
			glNormal3f(0,0,-1);
			glVertex3f(x-length*cos(t)-width*sin(t)/2,y+length*sin(t)-width*cos(t)/2,-dp);
			glVertex3f(x-length*cos(t)+width*sin(t)/2,y+length*sin(t)+width*cos(t)/2,-dp);
			glVertex3f(x+length*cos(t)+width*sin(t)/2,y-length*sin(t)+width*cos(t)/2,-dp);
			glVertex3f(x+length*cos(t)-width*sin(t)/2,y-length*sin(t)-width*cos(t)/2,-dp);
			glEnd();
			
			glBegin(GL_POLYGON);
			glNormal3f(-cos(t),sin(t),0);
			glVertex3f(x-length*cos(t)-width*sin(t)/2,y+length*sin(t)-width*cos(t)/2,-dp);
			glVertex3f(x-length*cos(t)+width*sin(t)/2,y+length*sin(t)+width*cos(t)/2,-dp);
			glVertex3f(x-length*cos(t)-width*sin(t)/2,y+length*sin(t)-width*cos(t)/2,dp);
			glVertex3f(x-length*cos(t)+width*sin(t)/2,y+length*sin(t)+width*cos(t)/2,dp);
			glEnd();
			
			glBegin(GL_POLYGON);
			glNormal3f(cos(t),-sin(t),0);
			glVertex3f(x+length*cos(t)+width*sin(t)/2,y-length*sin(t)+width*cos(t)/2,-dp);
			glVertex3f(x+length*cos(t)-width*sin(t)/2,y-length*sin(t)-width*cos(t)/2,-dp);
			glVertex3f(x+length*cos(t)+width*sin(t)/2,y-length*sin(t)+width*cos(t)/2,dp);
			glVertex3f(x+length*cos(t)-width*sin(t)/2,y-length*sin(t)-width*cos(t)/2,dp);
			glEnd();
			
			glBegin(GL_POLYGON);
			glNormal3f(sin(t),cos(t),0);
			glVertex3f(x-length*cos(t)+width*sin(t)/2,y+length*sin(t)+width*cos(t)/2,-dp);
			glVertex3f(x+length*cos(t)+width*sin(t)/2,y-length*sin(t)+width*cos(t)/2,-dp);
			glVertex3f(x+length*cos(t)+width*sin(t)/2,y-length*sin(t)+width*cos(t)/2,dp);
			glVertex3f(x-length*cos(t)+width*sin(t)/2,y+length*sin(t)+width*cos(t)/2,dp);
			
			glEnd();
			
			glBegin(GL_POLYGON);
			glNormal3f(-sin(t),-cos(t),0);
			glVertex3f(x-length*cos(t)+width*sin(t)/2,y+length*sin(t)-width*cos(t)/2,-dp);
			glVertex3f(x+length*cos(t)+width*sin(t)/2,y-length*sin(t)-width*cos(t)/2,-dp);
			glVertex3f(x+length*cos(t)+width*sin(t)/2,y-length*sin(t)-width*cos(t)/2,dp);
			glVertex3f(x-length*cos(t)+width*sin(t)/2,y+length*sin(t)-width*cos(t)/2,dp);
			glEnd();
			
			ball temp_ball;
			temp_ball.radius=5;
			temp_ball.setColor(1,0,0);
			temp_ball.set(x,y);
			
			
		//	temp_ball.draw();
			
			
			GLfloat mat_specular1[] = { 0, 0, 0, 1.0 };
  			GLfloat mat_shininess1[] = { 1.0 };
   			GLfloat light_diffuse1[] = { 1, 0, 0, 1.0 };
  			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular1);
  			glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess1);
   			glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,light_diffuse1);
   				
   			glBegin(GL_POLYGON);
   			glVertex3f(x-2,y-2,dp+0.1);
   			glVertex3f(x+2,y-2,dp+0.1);
   			glVertex3f(x+2,y+2,dp+0.1);
   			glVertex3f(x-2,y+2,dp+0.1);
   			glEnd();	
			
			
			}
			
};

class cup{
	public:
		double x,y;
		double length;
		double width;
		double height;
		double r,g,b;
		double theta;
		void setColor(double a,double b1,double c){
			r=a;
			g=b1;
			b=c;
		}
		
		void set(double a,double b1,double c,double d){
			x=a;
			y=b1;
			width=6;
			height=c;
			length=d;
			theta=0;
		}
		
		void draw(){
			GLfloat mat_specular[] = { r, g, b, 1.0 };
  			 GLfloat mat_shininess[] = { 50.0 };
   			 GLfloat light_diffuse[] = { r, g, b, 1.0 };
 
  				 glShadeModel (GL_SMOOTH);

  				 glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
  				 glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
   				glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,light_diffuse);
			double t=M_PI*theta/180;
			
			glBegin(GL_POLYGON);
			glNormal3f(0,0,1);
			glVertex3f(x-length*cos(t)/2,y+length*sin(t)/2,dp);
			glVertex3f(x-length*cos(t)/2+height*sin(t),y+length*sin(t)/2+height*cos(t),dp);
			glVertex3f(x-length*cos(t)/2+height*sin(t)+width*cos(t),y+length*sin(t)/2+height*cos(t)-width*sin(t),dp);
			glVertex3f(x-length*cos(t)/2+width*cos(t),y+length*sin(t)/2-width*sin(t),dp);
			glEnd();
			
			glBegin(GL_POLYGON);
			glNormal3f(0,0,-1);
			glVertex3f(x-length*cos(t)/2,y+length*sin(t)/2,-dp);
			glVertex3f(x-length*cos(t)/2+height*sin(t),y+length*sin(t)/2+height*cos(t),-dp);
			glVertex3f(x-length*cos(t)/2+height*sin(t)+width*cos(t),y+length*sin(t)/2+height*cos(t)-width*sin(t),-dp);
			glVertex3f(x-length*cos(t)/2+width*cos(t),y+length*sin(t)/2-width*sin(t),-dp);
			glEnd();
			
			glBegin(GL_POLYGON);
			glNormal3f(-cos(t),sin(t),0);
			glVertex3f(x-length*cos(t)/2+height*sin(t),y+length*sin(t)/2+height*cos(t),dp);
			glVertex3f(x-length*cos(t)/2+height*sin(t)+width*cos(t),y+length*sin(t)/2+height*cos(t)-width*sin(t),dp);
			glVertex3f(x-length*cos(t)/2+height*sin(t)+width*cos(t),y+length*sin(t)/2+height*cos(t)-width*sin(t),-dp);
			glVertex3f(x-length*cos(t)/2+height*sin(t),y+length*sin(t)/2+height*cos(t),-dp);
			glEnd();
			
			glBegin(GL_POLYGON);
			glNormal3f(sin(t),cos(t),0);
			glVertex3f(x-length*cos(t)/2,y+length*sin(t)/2,dp);
			glVertex3f(x-length*cos(t)/2+height*sin(t)+width*cos(t),y+length*sin(t)/2+height*cos(t)-width*sin(t),dp);
			glVertex3f(x-length*cos(t)/2+height*sin(t)+width*cos(t),y+length*sin(t)/2+height*cos(t)-width*sin(t),-dp);
			glVertex3f(x-length*cos(t)/2,y+length*sin(t)/2,-dp);
			glEnd();
			
			glBegin(GL_POLYGON);
			glNormal3f(cos(t),-sin(t),0);
			glVertex3f(x-length*cos(t)/2+height*sin(t)+width*cos(t),y+length*sin(t)/2+height*cos(t)-width*sin(t),dp);
			glVertex3f(x-length*cos(t)/2+width*cos(t),y+length*sin(t)/2-width*sin(t),dp);
			glVertex3f(x-length*cos(t)/2+width*cos(t),y+length*sin(t)/2-width*sin(t),-dp);
			glVertex3f(x-length*cos(t)/2+height*sin(t)+width*cos(t),y+length*sin(t)/2+height*cos(t)-width*sin(t),-dp);
			glEnd();
			
			
			glBegin(GL_POLYGON);
			glNormal3f(-sin(t),-cos(t),0);
			glVertex3f(x-length*cos(t)/2+width*cos(t),y+length*sin(t)/2-width*sin(t),dp);
			glVertex3f(x-length*cos(t)/2,y+length*sin(t)/2,dp);
			glVertex3f(x-length*cos(t)/2,y+length*sin(t)/2,-dp);
			glVertex3f(x-length*cos(t)/2+width*cos(t),y+length*sin(t)/2-width*sin(t),-dp);
			glEnd();
			
			
			
			
			
			
			
			
			
			glBegin(GL_POLYGON);
			glNormal3f(0,0,1);
			glVertex3f(x-length*cos(t)/2+width*cos(t),y+length*sin(t)/2-width*sin(t),dp);
			glVertex3f(x+length*cos(t)/2-width*cos(t),y-length*sin(t)/2+width*sin(t),dp);
			glVertex3f(x+length*cos(t)/2-width*cos(t)+width*sin(t),y-length*sin(t)/2+width*sin(t)+width*cos(t),dp);
			glVertex3f(x-length*cos(t)/2+width*cos(t)+width*sin(t),y+length*sin(t)/2-width*sin(t)+width*cos(t),dp);
			glEnd();
			
			glBegin(GL_POLYGON);
			glNormal3f(0,0,-1);
			glVertex3f(x-length*cos(t)/2+width*cos(t),y+length*sin(t)/2-width*sin(t),-dp);
			glVertex3f(x+length*cos(t)/2-width*cos(t),y-length*sin(t)/2+width*sin(t),-dp);
			glVertex3f(x+length*cos(t)/2-width*cos(t)+width*sin(t),y-length*sin(t)/2+width*sin(t)+width*cos(t),-dp);
			glVertex3f(x-length*cos(t)/2+width*cos(t)+width*sin(t),y+length*sin(t)/2-width*sin(t)+width*cos(t),-dp);
			glEnd();
			
			glBegin(GL_POLYGON);
			glNormal3f(-cos(t),sin(t),0);
			glVertex3f(x-length*cos(t)/2+width*cos(t),y+length*sin(t)/2-width*sin(t),dp);
			glVertex3f(x+length*cos(t)/2-width*cos(t),y-length*sin(t)/2+width*sin(t),dp);
			glVertex3f(x+length*cos(t)/2-width*cos(t),y-length*sin(t)/2+width*sin(t),-dp);
			glVertex3f(x-length*cos(t)/2+width*cos(t),y+length*sin(t)/2-width*sin(t),-dp);
			glEnd();
			
			glBegin(GL_POLYGON);
			glNormal3f(sin(t),cos(t),0);
			glVertex3f(x+length*cos(t)/2-width*cos(t),y-length*sin(t)/2+width*sin(t),dp);
			glVertex3f(x+length*cos(t)/2-width*cos(t)+width*sin(t),y-length*sin(t)/2+width*sin(t)+width*cos(t),dp);
			glVertex3f(x+length*cos(t)/2-width*cos(t)+width*sin(t),y-length*sin(t)/2+width*sin(t)+width*cos(t),-dp);
			glVertex3f(x+length*cos(t)/2-width*cos(t),y-length*sin(t)/2+width*sin(t),-dp);
			glEnd();
			
			glBegin(GL_POLYGON);
			glNormal3f(cos(t),-sin(t),0);
			glVertex3f(x+length*cos(t)/2-width*cos(t)+width*sin(t),y-length*sin(t)/2+width*sin(t)+width*cos(t),dp);
			glVertex3f(x-length*cos(t)/2+width*cos(t)+width*sin(t),y+length*sin(t)/2-width*sin(t)+width*cos(t),dp);
			glVertex3f(x-length*cos(t)/2+width*cos(t)+width*sin(t),y+length*sin(t)/2-width*sin(t)+width*cos(t),-dp);
			glVertex3f(x+length*cos(t)/2-width*cos(t)+width*sin(t),y-length*sin(t)/2+width*sin(t)+width*cos(t),-dp);
			glEnd();
			
			
			glBegin(GL_POLYGON);
			glNormal3f(-cos(t),-sin(t),0);
			glVertex3f(x-length*cos(t)/2+width*cos(t),y+length*sin(t)/2-width*sin(t),dp);
			glVertex3f(x-length*cos(t)/2+width*cos(t)+width*sin(t),y+length*sin(t)/2-width*sin(t)+width*cos(t),dp);
			glVertex3f(x-length*cos(t)/2+width*cos(t)+width*sin(t),y+length*sin(t)/2-width*sin(t)+width*cos(t),-dp);
			glVertex3f(x-length*cos(t)/2+width*cos(t),y+length*sin(t)/2-width*sin(t),-dp);
			glEnd();
			
			
			
			
			
			
			glBegin(GL_POLYGON);
			glNormal3f(0,0,1);
			glVertex3f(x+length*cos(t)/2,y-length*sin(t)/2,dp);
			glVertex3f(x+length*cos(t)/2+height*sin(t),y-length*sin(t)/2+height*cos(t),dp);
			glVertex3f(x+length*cos(t)/2+height*sin(t)-width*cos(t),y-length*sin(t)/2+height*cos(t)-width*sin(t),dp);
			glVertex3f(x+length*cos(t)/2-width*cos(t),y-length*sin(t)/2+width*sin(t),dp);
			glEnd();
			
			glBegin(GL_POLYGON);
			glNormal3f(0,0,-1);
			glVertex3f(x+length*cos(t)/2,y-length*sin(t)/2,-dp);
			glVertex3f(x+length*cos(t)/2+height*sin(t),y-length*sin(t)/2+height*cos(t),-dp);
			glVertex3f(x+length*cos(t)/2+height*sin(t)-width*cos(t),y-length*sin(t)/2+height*cos(t)-width*sin(t),-dp);
			glVertex3f(x+length*cos(t)/2-width*cos(t),y-length*sin(t)/2+width*sin(t),-dp);
			glEnd();
			
			glBegin(GL_POLYGON);
			glNormal3f(-cos(t),sin(t),0);
			glVertex3f(x+length*cos(t)/2,y-length*sin(t)/2,dp);
			glVertex3f(x+length*cos(t)/2+height*sin(t),y-length*sin(t)/2+height*cos(t),dp);
			glVertex3f(x+length*cos(t)/2+height*sin(t),y-length*sin(t)/2+height*cos(t),-dp);
			glVertex3f(x+length*cos(t)/2,y-length*sin(t)/2,-dp);
			glEnd();
			
			glBegin(GL_POLYGON);
			glNormal3f(sin(t),cos(t),0);
			glVertex3f(x+length*cos(t)/2+height*sin(t),y-length*sin(t)/2+height*cos(t),dp);
			glVertex3f(x+length*cos(t)/2+height*sin(t)-width*cos(t),y-length*sin(t)/2+height*cos(t)-width*sin(t),dp);
			glVertex3f(x+length*cos(t)/2+height*sin(t)-width*cos(t),y-length*sin(t)/2+height*cos(t)-width*sin(t),-dp);
			glVertex3f(x+length*cos(t)/2+height*sin(t),y-length*sin(t)/2+height*cos(t),-dp);
			glEnd();
			
			glBegin(GL_POLYGON);
			glNormal3f(cos(t),-sin(t),0);
			glVertex3f(x+length*cos(t)/2+height*sin(t)-width*cos(t),y-length*sin(t)/2+height*cos(t)-width*sin(t),dp);
			glVertex3f(x+length*cos(t)/2-width*cos(t),y-length*sin(t)/2+width*sin(t),dp);
			glVertex3f(x+length*cos(t)/2-width*cos(t),y-length*sin(t)/2+width*sin(t),-dp);
			glVertex3f(x+length*cos(t)/2+height*sin(t)-width*cos(t),y-length*sin(t)/2+height*cos(t)-width*sin(t),-dp);
			glEnd();
			
			glBegin(GL_POLYGON);
			glNormal3f(-cos(t),-sin(t),0);
			glVertex3f(x+length*cos(t)/2,y-length*sin(t)/2,dp);
			glVertex3f(x+length*cos(t)/2-width*cos(t),y-length*sin(t)/2+width*sin(t),dp);
			glVertex3f(x+length*cos(t)/2-width*cos(t),y-length*sin(t)/2+width*sin(t),-dp);
			glVertex3f(x+length*cos(t)/2,y-length*sin(t)/2,-dp);
			glEnd();
			
			
		}
};

class box{
	public:
		double r,g,b;
		double x,y;
		double length;
		bool flag[5];
		void setColor(double a,double b1, double c){
			r=a;
			g=b1;
			b=c;
		}
		
		void set(double a,double b1,double l){
			x=a;
			y=b1;
			length=l;
			for(int i=0;i<5;i++)
				flag[i]=false;
		}
		void draw(){
			GLfloat mat_specular[] = { r, g, b, 1.0 };
  			 GLfloat mat_shininess[] = { 50.0 };
   			 GLfloat light_diffuse[] = { r, g, b, 1.0 };
 
  				 glShadeModel (GL_SMOOTH);

  				 glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
  				 glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
   				glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,light_diffuse);
   				
			glBegin(GL_POLYGON);
			glNormal3f(0,0,1);
			glVertex3f(x,y,dp);
			glVertex3f(x,y+length,dp);
			glVertex3f(x+length,y+length,dp);
			glVertex3f(x+length,y,dp);
			glEnd();
			
			glBegin(GL_POLYGON);
			glNormal3f(0,0,-1);
			glVertex3f(x,y,-dp);
			glVertex3f(x,y+length,-dp);
			glVertex3f(x+length,y+length,-dp);
			glVertex3f(x+length,y,-dp);
			glEnd();
			
			glBegin(GL_POLYGON);
			glNormal3f(-1,0,0);
			glVertex3f(x,y,dp);
			glVertex3f(x,y+length,dp);
			glVertex3f(x,y+length,-dp);
			glVertex3f(x,y,-dp);
			glEnd();
			
			glBegin(GL_POLYGON);
			glNormal3f(0,-1,0);
			glVertex3f(x,y,dp);
			glVertex3f(x,y+length,dp);
			glVertex3f(x,y+length,-dp);
			glVertex3f(x,y,-dp);
			glEnd();
			
			glBegin(GL_POLYGON);
			glNormal3f(1,0,0);
			glVertex3f(x+length,y,dp);
			glVertex3f(x+length,y+length,dp);
			glVertex3f(x+length,y+length,-dp);
			glVertex3f(x+length,y,-dp);
			glEnd();
			
			glBegin(GL_POLYGON);
			glNormal3f(0,1,0);
			glVertex3f(x,y+length,dp);
			glVertex3f(x+length,y+length,dp);
			glVertex3f(x+length,y+length,-dp);
			glVertex3f(x,y+length,-dp);
			glEnd();
		
		}
};


class balance{
	public:
		double x,y,base_side;
		double length;
		double height;
		double width;
		double s_height;
		double s_length;
		double theta;
		bool flag[5];
		double r,g,b;
		cup temp_cup;
		
		void setColor(double a,double b1, double c){
			r=a;
			g=b1;
			b=c;
		}
		
		void set(double a,double b){
			x=a;
			y=b;
			length=100;
			height=100;
			s_height=30;
			width=8;
			s_length=80;
			base_side=30;
			theta=0;
			r=0.1;
			g=0;
			b=0.5;
			for(int i=0;i<5;i++)
				flag[i]=false;
		}
		void draw(){
			GLfloat mat_specular[] = { r, g, b, 1.0 };
  			 GLfloat mat_shininess[] = { 50.0 };
   			 GLfloat light_diffuse[] = { r, g, b, 1.0 };
 
  				 glShadeModel (GL_SMOOTH);

  				 glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
  				 glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
   				glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,light_diffuse);
   				
			glBegin(GL_POLYGON);
			glNormal3f(0,0,1);
			glVertex3f(x,y,dp);
			glVertex3f(x-base_side*sin(M_PI*30/180),y-base_side*cos(M_PI*30/180),dp);
			glVertex3f(x+base_side*sin(M_PI*30/180),y-base_side*cos(M_PI*30/180),dp);
			glEnd();
			
			glBegin(GL_POLYGON);
			glNormal3f(0,0,-1);
			glVertex3f(x,y,-dp);
			glVertex3f(x-base_side*sin(M_PI*30/180),y-base_side*cos(M_PI*30/180),-dp);
			glVertex3f(x+base_side*sin(M_PI*30/180),y-base_side*cos(M_PI*30/180),-dp);
			glEnd();
			
			glBegin(GL_POLYGON);
			glNormal3f(-sin(M_PI*60/180),cos((M_PI*60/180)),0);
			glVertex3f(x,y,-dp);
			glVertex3f(x-base_side*sin(M_PI*30/180),y-base_side*cos(M_PI*30/180),-dp);
			glVertex3f(x-base_side*sin(M_PI*30/180),y-base_side*cos(M_PI*30/180),dp);
			glVertex3f(x,y,dp);
			glEnd();
			
			
			glBegin(GL_POLYGON);
			glNormal3f(0,-1,0);
			glVertex3f(x-base_side*sin(M_PI*30/180),y-base_side*cos(M_PI*30/180),dp);
			glVertex3f(x+base_side*sin(M_PI*30/180),y-base_side*cos(M_PI*30/180),dp);
			glVertex3f(x+base_side*sin(M_PI*30/180),y-base_side*cos(M_PI*30/180),-dp);
			glVertex3f(x-base_side*sin(M_PI*30/180),y-base_side*cos(M_PI*30/180),-dp);
			glEnd();
			
			glBegin(GL_POLYGON);
			glNormal3f(sin(M_PI*60/180),cos((M_PI*60/180)),0);
			glVertex3f(x,y,dp);
			glVertex3f(x+base_side*sin(M_PI*30/180),y-base_side*cos(M_PI*30/180),dp);
			glVertex3f(x+base_side*sin(M_PI*30/180),y-base_side*cos(M_PI*30/180),-dp);
			glVertex3f(x,y,-dp);
			glEnd();
			
			
			
			GLfloat mat_specular1[] = { 1, 1, 1, 1.0 };
  			 GLfloat mat_shininess1[] = { 50.0 };
   			 GLfloat light_diffuse1[] = { 1, 1, 1, 1.0 };
 
  				 glShadeModel (GL_SMOOTH);

  				 glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular1);
  				 glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess1);
   				glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,light_diffuse1);
   				
			glBegin(GL_LINES);
			glVertex3f(x-length*cos(M_PI*theta/180),y-length*sin(M_PI*theta/180),0);
			glVertex3f(x+length*cos(M_PI*theta/180),y+length*sin(M_PI*theta/180),0);
			glEnd();
			glBegin(GL_LINES);
			glVertex3f(x-length*cos(M_PI*theta/180),y-length*sin(M_PI*theta/180),0);
			glVertex3f(x-length*cos(M_PI*theta/180),y-length*sin(M_PI*theta/180)+s_height,0);
			glEnd();
			glBegin(GL_LINES);
			glVertex3f(x+length*cos(M_PI*theta/180),y+length*sin(M_PI*theta/180),0);
			glVertex3f(x+length*cos(M_PI*theta/180),y+length*sin(M_PI*theta/180)+height,0);
			glEnd();
			glColor3f(1,1,0);
			
			temp_cup.setColor(1,1,0);
			temp_cup.set(x-length*cos(M_PI*theta/180),y-length*sin(M_PI*theta/180)+s_height,30,80);
			temp_cup.draw();
			
			glColor3f(1,1,1);
			glBegin(GL_LINES);
			glVertex3f(x+length*cos(M_PI*theta/180)-s_length/2,y+length*sin(M_PI*theta/180)+height,0);
			glVertex3f(x+length*cos(M_PI*theta/180)+s_length/2,y+length*sin(M_PI*theta/180)+height,0);
			glEnd();
			
		}

};

class pulley{
	public:
		ball myball;
		ball nut;
		void set(double x,double y,double r){
			myball.x=x;
			myball.y=y;
			nut.x=x;
			nut.y=y;
			myball.radius=r;
			nut.radius=3;
			nut.r=1;
			nut.g=1;
			nut.b=1;
			myball.r=1;
			myball.g=0;
			myball.b=1;
		}
		
		void draw(){
			GLfloat mat_specular[] = { myball.r, myball.g, myball.b, 1.0 };
  			 GLfloat mat_shininess[] = { 50.0 };
   			 GLfloat light_diffuse[] = { myball.r, myball.g, myball.b, 1.0 };
 
  				 glShadeModel (GL_SMOOTH);

  				 glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
  				 glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
   				glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,light_diffuse);
			GLUquadricObj *quadratic;
			glPushMatrix();
			glTranslatef(myball.x,myball.y,-dp);
			quadratic = gluNewQuadric();
			gluCylinder(quadratic,myball.radius,myball.radius,20,32,32);
			glPopMatrix();
			
			glPushMatrix();
			glTranslatef(myball.x,myball.y,dp);
			
			GLUquadricObj *disk;
    			disk = gluNewQuadric();
   			gluDisk(disk, nut.radius, myball.radius, 32, 32);
			glPopMatrix();
		}
		
};

class stick{
	public:
		double x,y;
		double width;
		double length;
		double r,g,b;
		void setColor(double a,double b1,double c){
			r=a;
			g=b1;
			b=c;
		}
		
		void set(double a,double b1,double l){
			x=a;
			y=b1;
			length=l;
			width=4;
		}
		void draw(){
			glColor3f(r,g,b);
			glBegin(GL_POLYGON);
			glVertex3f(x,y,0);
			glVertex3f(x,y+width,0);
			glVertex3f(x+length,y+width,0);
			glVertex3f(x+length,y,0);
			glEnd();
		}
		
};

