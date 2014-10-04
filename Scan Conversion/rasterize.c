#include <GL/glut.h>
#include <stdio.h>
#define maxline 1002	//maximum number of vertices forming line
#define maxtri  1002	//maximum number of vertices forming triangle
int x0,Y0,x1,Y1,x,y;
float r1,b1,g1,r2,b2,g2,a1,a2;
int dx,dy,d,ie,ine,xinc,yinc;
float m,dr,dg,db,da,r,g,b,a;
int linex[maxline],liney[maxline],numline;
float liner[maxline],lineg[maxline],lineb[maxline],linea[maxline];
int trix[maxtri],triy[maxtri],numtri;
float trir[maxtri],trig[maxtri],trib[maxtri],tria[maxtri];

//Intialising some states in opengl
void init(void){
	glClearColor(0.0, 0.0, 0.0, 0.0);
   	glShadeModel(GL_FLAT);
   	glPointSize(1);
}
max(int i1,int i2){		//finding the max
	if(i1>i2)
		return i1;
	else
		return i2;
}

min(int i1,int i2){		//finding the min
	if(i1>i2)
		return i2;
	else
		return i1;
}
void swap(int *i1, int *i2)		//swap for integers
{
   int temp;
   temp = *i2;
   *i2   = *i1;
   *i1   = temp;   
}

void swapf(float *i1,float *i2){	//swap for float
	float temp;
   temp = *i2;
   *i2   = *i1;
   *i1   = temp;  

}


//Swapping the vertices to reduce the number of cases for drawing the line
void reduce_case(){
	if(Y1<Y0 || (Y1==Y0 && x1<x0)){
		swap(&x0,&x1);
		swap(&Y0,&Y1);
		swapf(&r1,&r2);
		swapf(&g1,&g2);
		swapf(&b1,&b2);
		swapf(&a1,&a2);
	}
}


//Selecting the appropriate color for the pixel and drawing the pixel
void putpixel(int flag){
	if(flag==0){
			r=r1+(x-x0)*dr/(x1-x0);
			g=g1+(x-x0)*dg/(x1-x0);
			b=b1+(x-x0)*db/(x1-x0);
			a=a1+(x-x0)*da/(x1-x0);
			glColor4f(r,g,b,a);
	}
	else{
			r=r1+(y-Y0)*dr/(Y1-Y0);
			g=g1+(y-Y0)*dg/(Y1-Y0);
			b=b1+(y-Y0)*db/(Y1-Y0);
			a=a1+(y-Y0)*da/(Y1-Y0);
			glColor4f(r,g,b,a);
	}
	glVertex2i(x,y);
}


void drawline(){
	glColor4f(r1,g1,b1,a1);
	glVertex2i(x,y);
	m=(1.0*(Y1-Y0))/(x1-x0);	//now based on the magnitude and sign of m, there are four cases for drawing line
	if(m>=-1 && m<=1){
		if(m<0){			//CASE 1: when m<0 and |m| <=1
			xinc=-1;
			ine=-2*dy;
			ie=-2*(dy+dx);	
			d=-2*dy-dx;
		}
		else{				//CASE 2: when m>=0 and |m| <=1
			xinc=1;
			ie=2*dy;
			ine=2*(dy-dx);
			d=2*dy-dx;
		}
		while(x!=x1){		//In both these cases we increase/decrease along x axis and find whether we need to increase y
			if(d<=0){	//depending on the value of d
			d+=ie;
			x+=xinc;
			if(m<0)
				y++;
			}
			else{
			d+=ine;
			x+=xinc;
			if(m>=0)
				y++;
			}
			putpixel(0);
		}	
		}
	
	else{
		if(m>0){			//CASE 3: when m>0 and |m|>1
			xinc=1;
			d=dy-2*dx;
			ie=2*(dy-dx);
			ine=-2*dx;
		}
		else{				//CASE 4: when m<0 and |m|>1
			xinc=-1;
			d=dy-2*dx;
			ine=-2*(dy+dx);
			ie=-2*dx;
		}	
		while(y!=Y1){
			if(d<=0){
				d+=ie;				
				y++;	//In both these cases we increase along y axis and find whether we need to increase/decrease x
				if(m>0)	//depending on the value of d
					x+=xinc;
			}
			else{	
				d+=ine;
				y++;
				if (m<=0)
					x+=xinc;
			}
			putpixel(1);
		}
		}
	
}

void drawtriangle(int i){
		int c=1,j;
	for(j=0;j<3;j++){
		if(j==2)
			c=-2;
		x0=trix[i];		// Copying the vertices and rgba values from arrays to our variables
		x1=trix[i+c];
		Y0=triy[i];
		Y1=triy[i+c];		// Copying the vertices and rgba values from arrays to our variables
		r1=trir[i];
		g1=trig[i];
		b1=trib[i];
		a1=tria[i];		// Copying the vertices and rgba values from arrays to our variables
		r2=trir[i+c];
		g2=trig[i+c];
		b2=trib[i+c];
		a2=tria[i+c];		// Copying the vertices and rgba values from arrays to our variables
		reduce_case();			//Function to reduce the cases for drawing line
		dr=r2-r1;
		dg=g2-g1;
		db=b2-b1;
		dx=x1-x0;
		dy=Y1-Y0;
		x=x0;
		y=Y0;
		r=r1;
		g=g1;
		b=b1;
		a=a1;
		drawline();		//drawing the edges of the triangle
		i++;
		}

}

//Function for finding the area of the triangle with given vertices
float area(int x01, int y01, int x02, int y02, int x03, int y03)
{
   return abs((x01*(y02-y03) + x02*(y03-y01)+ x03*(y01-y02))/2.0);
}

//Function for color interpolation 
void colortriangle(int num){
	int i,j,k,xmin,xmax,ymin,ymax;
	xmin=min(trix[num],min(trix[num+1],trix[num+2]));
	xmax=max(trix[num],max(trix[num+1],trix[num+2]));
	ymin=min(triy[num],min(triy[num+1],triy[num+2]));
	ymax=max(triy[num],max(triy[num+1],triy[num+2]));
	float A = area (trix[num], triy[num], trix[num+1], triy[num+1], trix[num+2], triy[num+2]); //Area of the triangle
	j=ymin+1;
	while(j!=ymax){
	for(i=xmin+1;i<xmax;i++){
   		float A1 =area (i,j , trix[num+1], triy[num+1], trix[num+2], triy[num+2]);//Areas of the subtriangles 
   		float A2 = area (trix[num], triy[num], i, j, trix[num+2], triy[num+2]);	//formed with two vertices of triangle and
   		float A3 =area (trix[num], triy[num], trix[num+1], triy[num+1], i,j);	// a given point
   		if (A == A1 + A2 + A3){						//Check if the point is interior
   			r=(A1*trir[num]+A2*trir[num+1]+A3*trir[num+2])/A;	//Calculating the RGBA values based on
   			g=(A1*trig[num]+A2*trig[num+1]+A3*trig[num+2])/A;	//the magnitude ratios of three areas
   			b=(A1*trib[num]+A2*trib[num+1]+A3*trib[num+2])/A;
   			a=(A1*tria[num]+A2*tria[num+1]+A3*tria[num+2])/A;
   			glColor3f(r,g,b);				//Setting the color of the pixel
   			glVertex2i(i,j);			//Drawing the pixel
   		}
		}
		j++;
	}
}


void display(void){
	glClear(GL_COLOR_BUFFER_BIT);		//Clearing the background 
	glMatrixMode(GL_MODELVIEW);		//Selecting the GL_MODELVIEW matrix mode
   	glLoadIdentity();			
	glBegin(GL_POINTS);		//First drawing the lines	
	int i;		
	for(i=0;i<2*numline;i+=2){		
		x0=linex[i];		// Copying the vertices and rgba values from arrays to our variables
		x1=linex[i+1];
		Y0=liney[i];
		Y1=liney[i+1];
		r1=liner[i];		// Copying the vertices and rgba values from arrays to our variables
		g1=lineg[i];
		b1=lineb[i];
		a1=linea[i];
		r2=liner[i+1];
		g2=lineg[i+1];	// Copying the vertices and rgba values from arrays to our variables
		b2=lineb[i+1];
		a2=linea[i+1];
		reduce_case();
		dr=r2-r1;	
		dg=g2-g1;
		db=b2-b1;
		da=a2-a1;
		dx=x1-x0;
		dy=Y1-Y0;
		x=x0;
		y=Y0;
		drawline();		// Now drawing the line 
	}
					//Now drawing the triangles
	for(i=0;i<3*numtri;i+=3){
		drawtriangle(i);	// First drawing the edges of the triangle
		colortriangle(i);	//Coloring the inner points of triangle
	}
	glEnd();
   	glFlush();
}

void reshape(int w,int h){
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   	glMatrixMode(GL_PROJECTION);
   	glLoadIdentity();
   	glOrtho(-w/2, w/2, -h/2, h/2, -1.0, 1.0);
   	glMatrixMode(GL_MODELVIEW);
   	glLoadIdentity();
}

int main(int argc, char ** argv){	
	int i;
	if(argc<2){
	printf("Please specify the file name\n");
	return 0;
	}
	FILE *f;
	f=fopen(argv[1],"r");
	fscanf(f,"%d",&numline);		//number of lines		
	for(i=0;i<2*numline;i++){
		fscanf(f,"%d%d%f%f%f%f",linex+i,liney+i,liner+i,lineg+i,lineb+i,linea+i);
		liner[i]/=255;
		lineg[i]/=255;
		lineb[i]/=255;
		linea[i]/=255;
		}
	fscanf(f,"%d",&numtri);		//number of triangles		
	for(i=0;i<3*numtri;i++){
		fscanf(f,"%d%d%f%f%f%f",trix+i,triy+i,trir+i,trig+i,trib+i,tria+i);
		trir[i]/=255;
		trig[i]/=255;
		trib[i]/=255;
		tria[i]/=255;
		}
	fclose(f);
	glutInit(&argc,argv);				//initialize glut
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);	//Setting the display modes
	glutInitWindowSize(800,600);			//Setting up the window size
	glutInitWindowPosition(0,0);		//Setting up the window position
	glutCreateWindow("Lab 1");			//Window title
	init();					//Initialise some opengl states
	glutDisplayFunc(display);		// display callback function
	glutReshapeFunc(reshape);		// reshape callback function
	glutMainLoop();
	return 0;
}
