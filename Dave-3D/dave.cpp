#include <iostream>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cmath>
#include "objects1.h"
#include "viewer.cpp"
using namespace std;
double eyeX,eyeY,eyeZ,centreX,centreY,centreZ,upX,upY,upZ,conX;
double seyeX,seyeY,seyeZ,scentreX,scentreY,scentreZ;

bool logo;
int logo_time;
wall wa[57];
floor2 myfloor;
enemy1 e1[4];
enemy2 e2[3];
treasure mykey;
bool door;
bool cheat;
bool treasure_found;

bool game_over;
int game_over_time;
int life;

bool new_game;
bool congratulations;
int congratulations_time;

int pointer;
int max_options;

int window_id;
ball ball_array[4];
Floor floor_array[7];
ceiling ceiling_array[2];
pivot pivot_array[50];
box box_array[4];
cup cup_array[4];
stick stick_array[2];
wedge mywedge;
balance mybalance;
box mybox;
pulley pulley_array[6];
key myposition;

void makeObjects(){
	ceiling_array[0].setColor(1,1,1);
	ceiling_array[0].theta=M_PI*45.0/180;
	ceiling_array[0].set(50,650,60,140);
	ceiling_array[1].setColor(1,1,1);
	ceiling_array[1].theta=-M_PI*45.0/180;
	ceiling_array[1].set(200,450,60,120);
	floor_array[0].setColor(1,1,1);
	floor_array[0].set(30,500,300);	
	floor_array[1].setColor(1,1,1);
	floor_array[1].set(300,300,100);	
	floor_array[2].setColor(1,1,1);
	floor_array[2].set(200,190,800);
	ceiling_array[0].thread_l=ceiling_array[0].y-floor_array[0].y-2*ceiling_array[0].myball.radius;
	ball_array[0].setColor(0,0,1);
	ball_array[0].radius=10;
	ball_array[0].set(ceiling_array[0].x+ceiling_array[0].length/2,floor_array[0].y + ball_array[0].radius);
	mywedge.setColor(0.4,0.9,1);
	mywedge.length=100;
	mywedge.set(floor_array[0].x+floor_array[0].length-mywedge.length,floor_array[0].y,mywedge.length,30);
	mybalance.setColor(0.7,0.3,0.3);
	mybalance.set(500,400);
	
	pivot_array[1].set(700,300,120,0);
	pivot_array[1].setColor(1,1,1);
	
	pivot_array[2].setColor(1,1,1);
	pivot_array[2].set(690,510,60,0);
	pivot_array[2].width=4;
	pivot_array[3].set(600,520,60,0);
	pivot_array[3].setColor(1,1,1);
	pivot_array[3].width=4;
	pivot_array[4].set(690,530,60,0);
	pivot_array[4].setColor(1,1,1);
	pivot_array[4].width=4;
	pivot_array[5].set(600,540,60,0);
	pivot_array[5].setColor(1,1,1);
	pivot_array[5].width=4;
	pivot_array[6].set(690,550,60,0);
	pivot_array[6].setColor(1,1,1);
	pivot_array[6].width=4;
	pivot_array[7].set(600,560,60,0);
	pivot_array[7].setColor(1,1,1);
	pivot_array[7].width=4;
	pivot_array[8].set(690,570,60,0);
	pivot_array[8].setColor(1,1,1);
	pivot_array[8].width=4;
	pivot_array[9].set(600,580,60,0);
	pivot_array[9].setColor(1,1,1);
	pivot_array[9].width=4;
	pivot_array[10].set(690,590,60,0);
	pivot_array[10].setColor(1,1,1);
	pivot_array[10].width=4;
	
	cup_array[0].setColor(0,1,0.5);
	cup_array[0].set(pivot_array[1].x-pivot_array[1].length,pivot_array[1].y+pivot_array[1].width/2,30,80);
	
	cup_array[2].setColor(0,1,0.5);
	cup_array[2].set(pivot_array[1].x+pivot_array[1].length,pivot_array[1].y+pivot_array[1].width/2,30,80);
	
	cup_array[1].setColor(0,1,0.5);
	cup_array[1].set(pivot_array[1].x+85,pivot_array[5].y+pivot_array[5].width/2,30,60);
	ball_array[1].radius=10;
	ball_array[1].setColor(0,1,0);
	ball_array[1].set(pivot_array[10].x,pivot_array[10].y+pivot_array[10].width/2+ball_array[1].radius);
	ball_array[2].radius=10;
	ball_array[2].setColor(0,1,0);
	ball_array[2].set(floor_array[2].x+30,floor_array[2].y+ball_array[2].radius);
	mybox.setColor(0.4,1,0.4);
	pivot_array[11].setColor(1,1,0.4);
	pivot_array[11].set(ceiling_array[1].x,300,100,90);
	pivot_array[12].setColor(1,1,1);
	pivot_array[12].set(1175,40,30,0);
	pivot_array[13].setColor(1,1,1);
	pivot_array[13].set(1175,40,30,90);
	pivot_array[14].setColor(1,1,1);
	pivot_array[14].set(25,40,30,0);
	pivot_array[15].setColor(1,1,1);
	pivot_array[15].set(25,40,30,90);
	
	mybox.set(floor_array[1].x,floor_array[1].y,-floor_array[1].y+ceiling_array[1].y-ceiling_array[1].thread_l*cos(M_PI*45.0/180)-20);
	pulley_array[0].set(cup_array[1].x+15,650,15);
	pulley_array[1].set(cup_array[1].x+100,650,15);
	pulley_array[2].set(pulley_array[1].myball.x+2*pulley_array[1].myball.radius,550,15);
	floor_array[3].setColor(1,1,1);
	floor_array[3].set(pulley_array[2].myball.x+100,pulley_array[2].myball.y-40,100);
	box_array[0].setColor(0.8,0.5,0.3);
	
	ball_array[3].setColor(.9,1,0);
	ball_array[3].radius=10;
	ball_array[3].set(floor_array[3].x+ball_array[3].radius,floor_array[3].y+ball_array[3].radius);
	box_array[0].set(floor_array[3].x+2*ball_array[3].radius,floor_array[3].y,40);
	floor_array[4].setColor(1,1,1);
	floor_array[4].set(920,420,80);
	floor_array[5].setColor(1,1,1);
	floor_array[5].set(860,350,80);
	
	
	floor_array[6].setColor(1,1,1);
	floor_array[6].set(0,pivot_array[14].y+pivot_array[14].length,1200);
	conX=0;
}

void init() 
{
   glClearColor (0.0, 0.0, 0.0, 0.0);
   init2();
   door=false;
   cheat=false;
   treasure_found=false;
   logo=false;
   game_over=false;
   game_over_time=0;
   life=2;
   congratulations=false;
   congratulations_time=0;
   new_game=false;
   pointer=0;
   max_options=2;
   shift_x=5;
   shift_y=2.5;
   shift_z=-1.5;
   logo_time=0;
   zoomer=3;
   eyeX=seyeX=10;
   eyeY=seyeY=10;
   eyeZ=seyeZ=-5;
   centreX=scentreX=11;
   centreY=scentreY=10;
   centreZ=scentreZ=-5;
   mykey.set(70,-150);
   upX=0;
   upY=1;
   upZ=0;
   e1[0].set(70,170,-70);
   e1[1].set(110,190,-30);
   e1[2].set(10,110,-170);
   e1[3].set(150,190,-170);
   
   e2[0].set(10,-30,-150);
   e2[1].set(130,-110,-190);
   e2[2].set(190,-50,-150);
   wa[0].set(0,-20,40,-20);
	
	wa[1].set(40,-20,40,-60);
	
	wa[2].set(40,-60,20,-60);
	
	wa[3].set(20,-40,20,-80);
	
	wa[4].set(60,0,60,-20);
	
	wa[5].set(60,-20,80,-20);
	
	wa[6].set(80,-20,80,-40);
	
	wa[7].set(80,-40,60,-40);
	
	wa[8].set(60,-40,60,-80);
	
	wa[9].set(60,-80,100,-80);
	
	wa[10].set(80,-80,80,-100);
	
	wa[11].set(120,0,120,-20);
	
	wa[12].set(100,-20,120,-20);
	
	wa[13].set(100,-20,100,-60);
	
	wa[14].set(100,-60,80,-60);
	
	wa[15].set(160,0,160,-20);
	
	wa[16].set(200,-20,180,-20);
	
	wa[17].set(200,-40,140,-40);
	
	wa[18].set(140,-40,140,-60);
	
	wa[19].set(120,-40,180,-40);
	
	wa[20].set(180,-40,180,-80);
	
	wa[21].set(120,-60,160,-60);
	
	wa[22].set(0,-160,40,-160);
	
	wa[23].set(20,-160,20,-140);
	
	wa[24].set(20,-140,40,-140);
	
	wa[25].set(40,-140,40,-80);

	wa[26].set(20,-120,60,-120);
	
	wa[27].set(20,-100,40,-100);

	wa[28].set(60,-120,60,-100);
	
	wa[29].set(20,-180,20,-200);
	
	wa[30].set(40,-180,40,-200);
	
	wa[31].set(60,-180,60,-200);
	
	wa[32].set(120,-200,120,-140);
	
	wa[33].set(80,-180,120,-180);
	
	wa[34].set(120,-160,100,-160);
	
	wa[35].set(100,-160,100,-140);
	
	wa[36].set(100,-140,60,-140);
	
	wa[37].set(60,-140,60,-160);
	
	wa[38].set(60,-160,80,-160);
	
	wa[39].set(80,-140,80,-130);
	
	wa[40].set(180,-180,180,-200);
	
	wa[41].set(200,-160,180,-160);
	
	wa[42].set(180,-160,180,-120);
	
	wa[43].set(180,-140,160,-140);
	
	wa[44].set(160,-200,160,-180);
	
	wa[45].set(160,-180,140,-180);
	
	wa[46].set(140,-180,140,-100);
	
	wa[47].set(140,-160,160,-160);
	
	wa[48].set(140,-120,160,-120);
	
	wa[49].set(100,-100,160,-100);
	
	wa[50].set(100,-100,100,-120);
	
	wa[51].set(120,-100,120,-120);
	
	wa[52].set(0,0,200,0);
	
	
	wa[53].set(0,0,0,-200);
	
	wa[54].set(0,-200,180,-200);
	
	wa[55].set(200,-200,200,0);
	
	wa[56].set(180,-200,200,-200);
   glShadeModel (GL_SMOOTH);
   
   GLfloat light0_specular[] = { 1.0, 1.0, 1.0, 1.0 };
   GLfloat light_diffuse[] = { 0, 1.0, 1.0, 1.0 };
   GLfloat light_position[] = { 200.0, 20.0, -20.0, 0.0 };
   glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
   glLightfv(GL_LIGHT0, GL_POSITION, light_position);
   glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
 glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 2.0);
glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 1.0);
glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.5);
 
   GLfloat lmodel_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
  glEnable(GL_LIGHT0);
  
GLfloat light1_ambient[] = { 0.3, 0.6, 0, 1.0 };
GLfloat light1_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat light1_specular[] = { 0, 1.0, 1.0, 1.0 };
GLfloat light1_position[] = { 0.0, 20.0, -100.0, 1.0 };

glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);
glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
glEnable(GL_LIGHT1);


GLfloat light2_ambient[] = { 0, 0, 0, 1.0 };
GLfloat light2_diffuse[] = { 1, 1.0, 1, 1.0 };
GLfloat light2_specular[] = { 0.6, 0.2, 0.5, 1.0 };
GLfloat light2_position[] = { 0, 20.0, -180.0, 1.0 };


glLightfv(GL_LIGHT2, GL_AMBIENT, light2_ambient);
glLightfv(GL_LIGHT2, GL_DIFFUSE, light2_diffuse);
glLightfv(GL_LIGHT2, GL_SPECULAR, light2_specular);
glLightfv(GL_LIGHT2, GL_POSITION, light2_position);
glEnable(GL_LIGHT2);


GLfloat light3_ambient[] = { 0, 0, 0.6, 1.0 };
GLfloat light3_diffuse[] = { 1, 1.0, 1, 1.0 };
GLfloat light3_specular[] = { 0.6, 0.2, 0.5, 1.0 };
GLfloat light3_position[] = { 180, 20.0, -180.0, 1.0 };


glLightfv(GL_LIGHT3, GL_AMBIENT, light3_ambient);
glLightfv(GL_LIGHT3, GL_DIFFUSE, light3_diffuse);
glLightfv(GL_LIGHT3, GL_SPECULAR, light3_specular);
glLightfv(GL_LIGHT3, GL_POSITION, light3_position);
glEnable(GL_LIGHT3);


GLfloat light4_ambient[] = { 0.8, 0.6, 0, 1.0 };
GLfloat light4_diffuse[] = { 1, 1.0, 1, 1.0 };
GLfloat light4_specular[] = { 0.6, 0.1, 0.2, 1.0 };
GLfloat light4_position[] = { 200, 30.0, 0.0, 1.0 };


glLightfv(GL_LIGHT4, GL_AMBIENT, light4_ambient);
glLightfv(GL_LIGHT4, GL_DIFFUSE, light4_diffuse);
glLightfv(GL_LIGHT4, GL_SPECULAR, light4_specular);
glLightfv(GL_LIGHT4, GL_POSITION, light4_position);
glEnable(GL_LIGHT4);


GLfloat light5_ambient[] = { 0.8, 0.6, 0, 1.0 };
GLfloat light5_diffuse[] = { 0.8, 0.3, 1, 1.0 };
GLfloat light5_specular[] = { 0.1, 0.8, 0.2, 1.0 };
GLfloat light5_position[] = { 100, 30.0, -200.0, 1.0 };


glLightfv(GL_LIGHT5, GL_AMBIENT, light5_ambient);
glLightfv(GL_LIGHT5, GL_DIFFUSE, light5_diffuse);
glLightfv(GL_LIGHT5, GL_SPECULAR, light5_specular);
glLightfv(GL_LIGHT5, GL_POSITION, light5_position);
glEnable(GL_LIGHT5);



GLfloat light6_ambient[] = { 0.3, 0.1, 0, 1.0 };
GLfloat light6_diffuse[] = { 0.2, 0.9, 0.1, 1.0 };
GLfloat light6_specular[] = { 0.1, 0.8, 0.2, 1.0 };
GLfloat light6_position[] = { 100, 30.0, -100.0, 1.0 };


glLightfv(GL_LIGHT6, GL_AMBIENT, light6_ambient);
glLightfv(GL_LIGHT6, GL_DIFFUSE, light6_diffuse);
glLightfv(GL_LIGHT6, GL_SPECULAR, light6_specular);
glLightfv(GL_LIGHT6, GL_POSITION, light6_position);
glEnable(GL_LIGHT6);



/*
glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 0);
glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0);
glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0); */

/*
glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 45.0);
glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction);
glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 2.0);*/
glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

glEnable(GL_POINT_SMOOTH);
glEnable(GL_LINE_SMOOTH);
glEnable(GL_POLYGON_SMOOTH);  
glEnable(GL_COLOR_MATERIAL);
glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
glColorMaterial(GL_FRONT_AND_BACK, GL_SPECULAR);
}



void wall_c(){
   //GLfloat light_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
   GLfloat mat_specular[] = { 1, 0.3, 0.1, 1.0 };
   GLfloat light_diffuse[] = { 0.1, 0.1, 0.9, 1 };
   GLfloat mat_shininess[] = { 50.0 };
  
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel (GL_SMOOTH);
   GLfloat mat_amb_diff[] = { 1, 0.1, 0.1, 1.0 };
  
   glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
   glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
   glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, light_diffuse);
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnable(GL_DEPTH_TEST);
//   glEnable(GL_COLOR_MATERIAL);
}


void wall_c(int i){
   //GLfloat light_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
   if (i==0){
   GLfloat mat_specular[] = { 1, 0.3, 0.1, 1.0 };
   GLfloat light_diffuse[] = { 0.1, 0.1, 0.9, 1 };
   GLfloat mat_shininess[] = { 50.0 };
  
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel (GL_SMOOTH);
   GLfloat mat_amb_diff[] = { 1, 0.1, 0.1, 1.0 };
  
   glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
   glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
   glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, light_diffuse);
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnable(GL_DEPTH_TEST);
   return;
   }
   if(i==1){
   	GLfloat mat_specular[] = { 0.1, 0.3, 1, 1.0 };
   GLfloat light_diffuse[] = { 0.1, 1, 0.3, 1 };
   GLfloat mat_shininess[] = { 50.0 };
  
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel (GL_SMOOTH);
   GLfloat mat_amb_diff[] = { 1, 0.1, 0.1, 1.0 };
  
   glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
   glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
   glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, light_diffuse);
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnable(GL_DEPTH_TEST);
   return;
   }
   
   if(i==2){
   	GLfloat mat_specular[] = { 0.5, 0.8, 0.1, 1.0 };
   GLfloat light_diffuse[] = { 0.1, 0.7, 0.2, 1 };
   GLfloat mat_shininess[] = { 50.0 };
  
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel (GL_SMOOTH);
   GLfloat mat_amb_diff[] = { 1, 0.1, 0.1, 1.0 };
  
   glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
   glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
   glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, light_diffuse);
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnable(GL_DEPTH_TEST);
   return;
   
   }
   
   if(i==3){
   	 GLfloat mat_specular[] = { 0.4, 0.3, 0.1, 1.0 };
   GLfloat light_diffuse[] = { 0.8, 0.2, 0.1, 1 };
   GLfloat mat_shininess[] = { 50.0 };
  
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel (GL_SMOOTH);
   GLfloat mat_amb_diff[] = { 1, 0.1, 0.1, 1.0 };
  
   glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
   glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
   glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, light_diffuse);
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnable(GL_DEPTH_TEST);
   return;
   }
   
   if(i==4){
   	 GLfloat mat_specular[] = { 0.4, 0.3, 0.1, 1.0 };
   GLfloat light_diffuse[] = { 0.1, 0.3, 0.6, 1 };
   GLfloat mat_shininess[] = { 50.0 };
  
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel (GL_SMOOTH);
   GLfloat mat_amb_diff[] = { 1, 0.1, 0.1, 1.0 };
  
   glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
   glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
   glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, light_diffuse);
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnable(GL_DEPTH_TEST);
   return;
   }
   
   if(i==5){
   	 GLfloat mat_specular[] = { 0.1, 0.3, 0.1, 1.0 };
   GLfloat light_diffuse[] = { 0.6, 0.8, 0.2, 1 };
   GLfloat mat_shininess[] = { 50.0 };
  
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel (GL_SMOOTH);
   GLfloat mat_amb_diff[] = { 1, 0.1, 0.1, 1.0 };
  
   glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
   glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
   glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, light_diffuse);
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnable(GL_DEPTH_TEST);
   return;
   }
   
   if(i==6){
   	 GLfloat mat_specular[] = { 0.1, 0.3, 0.1, 1.0 };
   GLfloat light_diffuse[] = { 0.2, 0.6, 0.8, 1 };
   GLfloat mat_shininess[] = { 50.0 };
  
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel (GL_SMOOTH);
   GLfloat mat_amb_diff[] = { 1, 0.1, 0.1, 1.0 };
  
   glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
   glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
   glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, light_diffuse);
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnable(GL_DEPTH_TEST);
   return;
   }



}




void floor_c(){
   GLfloat mat_specular[] = { 0, 0, 0, 0.0 };
   GLfloat light_diffuse[] = { 0.1, 0.1, 0.1, 1.0 };
   GLfloat mat_shininess[] = { 50.0 };
  
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel (GL_SMOOTH);
   GLfloat mat_amb_diff[] = { 0.3, 0.2, 0.1, 1.0 };
  
   glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
   glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
   glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, light_diffuse);
   glEnable(GL_LIGHTING);
   glEnable(GL_DEPTH_TEST);
   glEnable(GL_COLOR_MATERIAL);
}

void reshape(int w , int h2){
	glViewport (0, 0, (GLsizei) w, (GLsizei) h2);
   	glMatrixMode(GL_PROJECTION);
   	glLoadIdentity();
   	gluPerspective(60,1,0,-200);
   	glMatrixMode(GL_MODELVIEW);
   	glLoadIdentity();
}

void rubeinit();
void display(){

	
	if(!logo){
		glMatrixMode(GL_MODELVIEW);
   	glLoadIdentity();
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1,1,1);
	glEnable(GL_TEXTURE_2D);
   		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
   		glBindTexture(GL_TEXTURE_2D, 2004);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0);glVertex3f(-1,-0.5,0);
	glTexCoord2f(0, 1.0);glVertex3f(-1,0.5,0);
	glTexCoord2f(1.0, 1.0);glVertex3f(1,0.5,0);
	glTexCoord2f(1.0, 0.0);glVertex3f(1,-0.5,0);
	glEnd();
   	glDisable(GL_TEXTURE_2D);
   	glMatrixMode(GL_PROJECTION);
   	glLoadIdentity();
   	glOrtho(-1,1,-1,1,-1,1);
	glFlush();
	logo_time++;
	if(logo_time>25)
		logo=true;
	return;
	
	}
	if(!new_game){
		glMatrixMode(GL_MODELVIEW);
   	glLoadIdentity();
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1,1,1);
	glEnable(GL_TEXTURE_2D);
   	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
   	glBindTexture(GL_TEXTURE_2D, 2007);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0);glVertex3f(-1,-0.7,0);
	glTexCoord2f(0, 1.0);glVertex3f(-1,0.7,0);
	glTexCoord2f(1.0, 1.0);glVertex3f(1,0.7,0);
	glTexCoord2f(1.0, 0.0);glVertex3f(1,-0.7,0);
	glEnd();
   	glDisable(GL_TEXTURE_2D);
   	//glPolygonMode( GL_FRONT, GL_LINE );
   	if(pointer==0){
   	glColor3f(1,0,0);
   	glBegin(GL_QUADS);
   	glVertex2f(-0.4,0.33);
   	glVertex2f(0.4,0.33);
   	glVertex2f(0.4,0.35);
   	glVertex2f(-0.4,0.35);
   	
   	glVertex2f(-0.4,0.1);
   	glVertex2f(0.4,0.1);
   	glVertex2f(0.4,0.12);
   	glVertex2f(-0.4,0.12);
   	glEnd();
   	}
   	else if(pointer==1){
   	glColor3f(1,0,0);
   	glBegin(GL_QUADS);
   	glVertex2f(-0.4,0.1);
   	glVertex2f(0.4,0.1);
   	glVertex2f(0.4,0.12);
   	glVertex2f(-0.4,0.12);
   	
   	glVertex2f(-0.4,-0.12);
   	glVertex2f(0.4,-0.12);
   	glVertex2f(0.4,-0.14);
   	glVertex2f(-0.4,-0.14);
   	glEnd();
   	}
   	
   	else if(pointer==2){
   	glColor3f(1,0,0);
   	glBegin(GL_QUADS);
   	glVertex2f(-0.4,-0.12);
   	glVertex2f(0.4,-0.12);
   	glVertex2f(0.4,-0.14);
   	glVertex2f(-0.4,-0.14);
   	
   	glVertex2f(-0.4,-0.34);
   	glVertex2f(0.4,-0.34);
   	glVertex2f(0.4,-0.36);
   	glVertex2f(-0.4,-0.36);
   	glEnd();
   	
   	
   	}
   	
   	//glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
   	glMatrixMode(GL_PROJECTION);
   	glLoadIdentity();
   	glOrtho(-1,1,-1,1,-1,1);
	glFlush();
	return;
	
	
	} 
	if(game_over && game_over_time<25){
		glMatrixMode(GL_MODELVIEW);
   	glLoadIdentity();
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1,1,1);
	glEnable(GL_TEXTURE_2D);
   		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
   		glBindTexture(GL_TEXTURE_2D, 2005);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0);glVertex3f(-1,-0.5,0);
	glTexCoord2f(0, 1.0);glVertex3f(-1,0.5,0);
	glTexCoord2f(1.0, 1.0);glVertex3f(1,0.5,0);
	glTexCoord2f(1.0, 0.0);glVertex3f(1,-0.5,0);
	glEnd();
   	glDisable(GL_TEXTURE_2D);
   	glMatrixMode(GL_PROJECTION);
   	glLoadIdentity();
   	glOrtho(-1,1,-1,1,-1,1);
	glFlush();
	game_over_time++;
	if(game_over_time==25){
		door=true;
		makeObjects();
		rubeinit();
		}
	return;
	
	}
	
	if(congratulations && congratulations_time<25){
		glMatrixMode(GL_MODELVIEW);
   	glLoadIdentity();
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1,1,1);
	glEnable(GL_TEXTURE_2D);
   		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
   		glBindTexture(GL_TEXTURE_2D, 2006);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0);glVertex3f(-1,-0.5,0);
	glTexCoord2f(0, 1.0);glVertex3f(-1,0.5,0);
	glTexCoord2f(1.0, 1.0);glVertex3f(1,0.5,0);
	glTexCoord2f(1.0, 0.0);glVertex3f(1,-0.5,0);
	glEnd();
   	glDisable(GL_TEXTURE_2D);
   	glMatrixMode(GL_PROJECTION);
   	glLoadIdentity();
   	glOrtho(-1,1,-1,1,-1,1);
	glFlush();
	congratulations_time++;
	if(congratulations_time==25){
		door=true;
		makeObjects();
		rubeinit();
		}
	return;
	
	}
	if(!door){
	glMatrixMode(GL_MODELVIEW);
   	glLoadIdentity();
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1,1,1);
	
	
	for(int i=0;i<56;i++){
		wa[i].draw();
		}
	if(!treasure_found){
	wa[56].draw();
	}

	for(int i=0;i<4;i++)
		e1[i].draw();
	for(int i=0;i<3;i++)
		e2[i].draw();
		
	myfloor.set(0,0,200,-200);
	floor_c();
	myfloor.draw();
	if(!treasure_found){
	mykey.draw();
	}
	
   	glEnable(GL_TEXTURE_2D);
   		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
   		glBindTexture(GL_TEXTURE_2D, 2003);
	glBegin(GL_POLYGON);
	glTexCoord2f(4.0, 0.0);glVertex3f(20,0.1,0);
	glTexCoord2f(0, 0);glVertex3f(20,0.1,-20);
	glTexCoord2f(0, 4.0);glVertex3f(30,0.1,-20);
	glTexCoord2f(4.0, 4.0);glVertex3f(30,0.1,0);
	glEnd();
   	glDisable(GL_TEXTURE_2D);
   	
   	/*
   	glEnable(GL_TEXTURE_2D);
   	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
   	glBindTexture(GL_TEXTURE_2D, 2004);
	glBegin(GL_POLYGON);
	glTexCoord2f(1.0, 0.0);glVertex3f(180,0,-200);
	glTexCoord2f(0, 0);glVertex3f(180,h,-200);
	glTexCoord2f(0, 1.0);glVertex3f(200,h,-200);
	glTexCoord2f(1.0, 1.0);glVertex3f(200,0,-200);
	glEnd();
	
	glEnable(GL_TEXTURE_2D);
   	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
   	glBindTexture(GL_TEXTURE_2D, 2005);
	glBegin(GL_POLYGON);
	glTexCoord2f(0, 0.0);glVertex3f(180,h,-200);
	glTexCoord2f(0, 1);glVertex3f(180,h+10,-200);
	glTexCoord2f(1, 1.0);glVertex3f(200,h+10,-200);
	glTexCoord2f(1.0, 0);glVertex3f(200,h,-200);
	glEnd();
	*/
	
	wa[56].set(180,-200,200,-200);
   	glDisable(GL_TEXTURE_2D);
	display_child();
	glMatrixMode(GL_PROJECTION);
   	glLoadIdentity();
   	gluPerspective(60,1200.0/700.0,2,-200);
   	if(!cheat)
	gluLookAt (eyeX, eyeY, eyeZ,centreX, centreY, centreZ, upX, upY, upZ);
	else
		gluLookAt(105,200,-105,105,150,-105,0,0,-1);
	glFlush();
	}
	else{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
   	glLoadIdentity();
   	glColor3f(1,1,1);
   	
	
   	for(int i=0;i<7;i++)
   		floor_array[i].draw();
   	for(int i=0;i<2;i++)
   		ceiling_array[i].draw();	
   	ball_array[0].draw();
   	ball_array[1].draw();
   	ball_array[2].draw();
   	ball_array[3].draw();
   	mywedge.draw();
   	mybalance.draw();
   	for(int i=0;i<16;i++)
   		pivot_array[i].draw();
   	
   	cup_array[0].draw();
   	cup_array[1].draw();
   	cup_array[2].draw();
   	mybox.draw();
   	pulley_array[0].draw();
   	pulley_array[1].draw();
   	pulley_array[2].draw();
   	box_array[0].draw();
   	glColor3f(1,1,1);
   	glBegin(GL_LINES);
   	glVertex3f(pulley_array[0].myball.x,pulley_array[0].myball.y+pulley_array[0].myball.radius,0);
   	glVertex3f(pulley_array[1].myball.x,pulley_array[1].myball.y+pulley_array[1].myball.radius,0);
   	glVertex3f(pulley_array[1].myball.x+pulley_array[1].myball.radius,pulley_array[1].myball.y,0);
   	glVertex3f(pulley_array[1].myball.x+pulley_array[1].myball.radius,pulley_array[2].myball.y,0);
   	glVertex3f(pulley_array[2].myball.x,pulley_array[2].myball.y-pulley_array[2].myball.radius,0);
   	glVertex3f(box_array[0].x,pulley_array[2].myball.y-pulley_array[2].myball.radius,0);
   	glVertex3f(cup_array[1].x-cup_array[1].length/2,cup_array[1].y+cup_array[1].height,0);
   	glVertex3f(cup_array[1].x,cup_array[1].y+cup_array[1].height+10,0);
   	glVertex3f(cup_array[1].x,cup_array[1].y+cup_array[1].height+10,0);
   	glVertex3f(cup_array[1].x+cup_array[1].length/2,cup_array[1].y+cup_array[1].height,0);
   	glVertex3f(cup_array[1].x,cup_array[1].y+cup_array[1].height+10,0);
   	glVertex3f(pulley_array[0].myball.x-pulley_array[0].myball.radius,pulley_array[0].myball.y,0);
   	glVertex3f(mybox.x+mybox.length,mybox.y+mybox.length/2,0);
   	double t=M_PI*cup_array[0].theta/180;
   	glVertex3f(cup_array[0].x-cup_array[0].length*cos(t)/2+cup_array[0].height*sin(t),cup_array[0].y+cup_array[0].length*sin(t)/2+cup_array[0].height*cos(t),0);
	glEnd();
	glColor3f(1,1,1);
	glEnable(GL_TEXTURE_2D);
   		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
   		glBindTexture(GL_TEXTURE_2D, 2002);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0);glVertex3f(conX,pivot_array[14].y+pivot_array[14].length,0);
	glTexCoord2f(0.0, 1.0);glVertex3f(conX,pivot_array[14].y+pivot_array[14].length+100,0);
	glTexCoord2f(1.0, 1.0);glVertex3f(conX+3200,pivot_array[14].y+pivot_array[14].length+100,0);
	glTexCoord2f(1.0, 0.0);glVertex3f(conX+3200,pivot_array[14].y+pivot_array[14].length,0);
	glEnd();
   	glDisable(GL_TEXTURE_2D);
   	
   	glMatrixMode(GL_PROJECTION);
   	glLoadIdentity();
   	gluPerspective(60,1200.0/700.0,1,-2);
       gluLookAt (eyeX+WIDTH/2, eyeY+HEIGHT/2, 600,centreX+WIDTH/2, centreY+HEIGHT/2,0, upX, upY, upZ);
     //   gluLookAt (eyeX, eyeY, 200,centreX, centreY, 0, upX, upY, upZ);
	glFlush();	
	}
	
}

void rubeinit(void){
	glClearColor(0.0, 0.0, 0.0, 0.0);
	eyeX=0;
   eyeY=0;
   eyeZ=0;
   centreX=0;
   centreY=0;
   centreZ=-1;
   upX=0;
   upY=1;
   upZ=0;
  
    GLfloat light_position[] = { 0, 0, 600.0, 0.0 };
   glLightfv(GL_LIGHT0, GL_POSITION, light_position);
   
GLfloat light1_ambient[] = { 0.1, 0.1, 0, 1.0 };
GLfloat light1_diffuse[] = { 0.2, 0.2, 0.2, 1.0 };
GLfloat light1_specular[] = { 0, 0.3, 0.3, 1.0 };
GLfloat light1_position[] = { 0.0, 0, -10.0, 1.0 };

glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);
glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
glEnable(GL_LIGHT1);

GLfloat light2_ambient[] = { 0.1, 0.1, 0, 1.0 };
GLfloat light2_diffuse[] = { 0.2, 0.2, 0.2, 1.0 };
GLfloat light2_specular[] = { 0, 0.3, 0.3, 1.0 };
GLfloat light2_position[] = { 600.0, 700, -1.0, 1.0 };

glLightfv(GL_LIGHT2, GL_AMBIENT, light2_ambient);
glLightfv(GL_LIGHT2, GL_DIFFUSE, light2_diffuse);
glLightfv(GL_LIGHT2, GL_SPECULAR, light2_specular);
glLightfv(GL_LIGHT2, GL_POSITION, light2_position);
glEnable(GL_LIGHT2);
glEnable(GL_LIGHTING);
glEnable(GL_LIGHT0);


 glDisable(GL_LIGHT3);
   glDisable(GL_LIGHT4);
  glDisable(GL_LIGHT5);
 // glDisable(GL_LIGHT6);
//   glDisable(GL_LIGHT7); 
   glEnable(GL_DEPTH_TEST);
   	}
   	


bool check2(double a1,double b1,double c1,double a2,double b2,double c2){
	double D,Dx,Dy,t1,t2;
	D=a1*b2-a2*b1;
	Dx=c1*b2-c2*b1;
	Dy=a1*c2-a2*c1;
	t1=-Dx/D;
	t2=-Dy/D;
	if( t1>=0 && t2 >=0 && t1<=1 && t2<=1){
		return true;
		}
	return false;

}

bool check(double a,double c){
	double x0,y0,X0,Y0,x1,y1,X1,Y1,t1,t2,D,Dx,Dy;
	x0=eyeX;
	y0=eyeZ;
	x1=eyeX+6*a;
	y1=eyeZ+6*c;
	for(int i=0;i<57;i++){
		
		X0=wa[i].x1;
		Y0=wa[i].z1;
		X1=wa[i].x2;
		Y1=wa[i].z2;
		if(check2(x1-x0,X0-X1,x0-X0,y1-y0,Y0-Y1,y0-Y0)){
			if(i==56 && treasure_found){
				congratulations=true;
			}
			return true;
			}
		}
			
	return false;

}

void check_treasure(){
	if((mykey.x-eyeX)*(mykey.x-eyeX) + (mykey.z-eyeZ)*(mykey.z-eyeZ) < 9){
		treasure_found=true;
	}
	
}

void upkey(){
	double lookX,lookY,lookZ;
	lookX=centreX-eyeX;
	lookY=centreY-eyeY;
	lookZ=centreZ-eyeZ;
	double t=1;
	
	normalise(lookX,lookY,lookZ);
	if(check(lookX,lookZ))
		return;
	eyeX+=t*lookX;
	eyeY+=t*lookY;
	eyeZ+=t*lookZ;
	centreX+=t*lookX;
	centreY+=t*lookY;
	centreZ+=t*lookZ;
	for (int i=0; i<data->numVertices; ++i) {
		va[i].x+=(t*lookX);
		va[i].z+=(t*lookZ);
		}
	if(!treasure_found)
		check_treasure();
}

void downkey(){
	double lookX,lookY,lookZ;
	lookX=centreX-eyeX;
	lookY=centreY-eyeY;
	lookZ=centreZ-eyeZ;
	
	normalise(lookX,lookY,lookZ);
	if(check(-lookX,-lookZ))
		return;
	double t=1;
	eyeX-=t*lookX;
	eyeY-=t*lookY;
	eyeZ-=t*lookZ;
	centreX-=t*lookX;
	centreY-=t*lookY;
	centreZ-=t*lookZ;
	for (int i=0; i<data->numVertices; ++i) {
		va[i].x-=(t*lookX);
		va[i].z-=(t*lookZ);
		}
	
}


void arbit_Rotation(double  a,double b,double c,double u,double v,double w,double x,double y, double z,double theta,double &x1,double &y1,double &z1){
	double L=sqrt(u*u+v*v+w*w);
	x1=(a*(v*v +w*w)-u*(b*v+c*w-u*x-v*y-w*z))*(1-cos(theta))+L*x*cos(theta)+sqrt(L)*(-c*v+b*w-w*y+v*z)*sin(theta)/L;
	y1=(b*(u*u+w*w)-v*(a*u+c*w-u*x-v*y-w*z))*(1-cos(theta))+L*y*cos(theta)+sqrt(L)*(c*u-a*w+w*x-u*z)*sin(theta)/L;
	z1=(c*(u*u+v*v)-w*(a*u+b*v-u*x-v*y-w*z))*(1-cos(theta))+L*z*cos(theta)+sqrt(L)*(-b*u+a*v-v*x+u*y)*sin(theta)/L;
}

void leftkey(){
	
	double t,t1,t2,t3,x1,y1,z1;
	t=0.04;
	double lookX=centreX-eyeX;
	double lookY=centreY-eyeY;
	double lookZ=centreZ-eyeZ;
	normalise(lookX,lookY,lookZ);
	arbit_Rotation(eyeX,eyeY,eyeZ,0,1,0,eyeX+4*lookX,eyeY+4*lookY,eyeZ+4*lookZ,t,x1,y1,z1);
	lookX=x1;
	lookY=y1;
	lookZ=z1;
	lookX=lookX-eyeX;
	lookZ=lookZ-eyeZ;
	lookY=lookY-eyeY;
	normalise(lookX,lookY,lookZ);
	if(check(lookX,lookZ))
		return;
	arbit_Rotation(eyeX,eyeY,eyeZ,0,1,0,centreX,centreY,centreZ,t,x1,y1,z1);
	centreX=x1;
	centreY=y1;
	centreZ=z1;
	for (int i=0; i<data->numVertices; ++i) {
		arbit_Rotation(eyeX,eyeY,eyeZ,0,1,0,va[i].x,va[i].y,va[i].z,t,x1,y1,z1);
		va[i].x=x1;
		va[i].y=y1;
		va[i].z=z1;
		}
	//normalise(centreX,centreY,centreZ);

}

void rightkey(){
	
	double t,t1,t2,t3,x1,y1,z1;
	t=-0.04;
	double lookX=centreX-eyeX;
	double lookY=centreY-eyeY;
	double lookZ=centreZ-eyeZ;
	normalise(lookX,lookY,lookZ);
	arbit_Rotation(eyeX,eyeY,eyeZ,0,1,0,eyeX+4*lookX,eyeY+4*lookY,eyeZ+4*lookZ,t,x1,y1,z1);
	lookX=x1;
	lookY=y1;
	lookZ=z1;
	lookX=lookX-eyeX;
	lookZ=lookZ-eyeZ;
	lookY=lookY-eyeY;
	normalise(lookX,lookY,lookZ);
	if(check(lookX,lookZ))
		return;
		
	arbit_Rotation(eyeX,eyeY,eyeZ,0,1,0,centreX,centreY,centreZ,t,x1,y1,z1);
	centreX=x1;
	centreY=y1;
	centreZ=z1;
	for (int i=0; i<data->numVertices; ++i) {
		arbit_Rotation(eyeX,eyeY,eyeZ,0,1,0,va[i].x,va[i].y,va[i].z,t,x1,y1,z1);
		va[i].x=x1;
		va[i].y=y1;
		va[i].z=z1;
		}
}


void keyboard (unsigned char key, int x, int y)
{
   switch (key) {
   	
      case 'w':
      	if(!door && !cheat){
         upkey();
         glutPostRedisplay();
         }
         break;
      case 's':
      	if(!door && !cheat){
      	downkey();
         glutPostRedisplay();
         }
         break;
      case 'a':
      	if(!door && !cheat){
      		leftkey();
         glutPostRedisplay();
         }
         break;
      case 'd':
      	if(!door && !cheat){
      		rightkey();
         glutPostRedisplay();
         }
         break;
      case 13:
      	if(!new_game){
      		if(pointer==0)	
      		new_game=true;
      		else if (pointer==2)
      			glutDestroyWindow(window_id); 
      		else if(pointer==1){
      			door=true;
      			new_game=true;
		makeObjects();
		rubeinit();
      		}
      	}
      	break;
       case 'c':
       if(!door){
       		cheat=!cheat;
       }
         break;
      default:
         break;
   }
}


void keyboard2 (int key, int x, int y)
{
   switch (key) {
   	
      case GLUT_KEY_UP:
      	if(!new_game){
      	pointer=max(0,pointer-1);
      	 glutPostRedisplay();
      	
      	}
      	else if(!door && !cheat){
         upkey();
         glutPostRedisplay();
         }
         break;
      case GLUT_KEY_DOWN:
      if(!new_game){
      	pointer=min(max_options,pointer+1);
      	 glutPostRedisplay();
      	}
      	else if(!door && !cheat){
      	downkey();
         glutPostRedisplay();
         }
         break;
      case GLUT_KEY_LEFT:
      	if(!door && !cheat){
      		leftkey();
         glutPostRedisplay();
         }
         break;
      case GLUT_KEY_RIGHT:
      	if(!door && !cheat){
      		rightkey();
         glutPostRedisplay();
         }
         break;
      default:
         break;
   }
}

void checkColision(){
	double lookX,lookY,lookZ;
	lookX=centreX-eyeX;
	lookY=centreY-eyeY;
	lookZ=centreZ-eyeZ;
	normalise(lookX,lookY,lookZ);
	double t=6;
	double tempe1=eyeX+t*lookX;
	double tempe2=eyeZ+t*lookZ;
	for(int i=0;i<4;i++){
		if((e1[i].x-tempe1)*(e1[i].x-tempe1) + (e1[i].z-tempe2)*(e1[i].z-tempe2) < 40){
			eyeX=seyeX;
			eyeY=seyeY;
			eyeZ=seyeZ;
			centreX=scentreX;
			centreY=scentreY;
			centreZ=scentreZ;
			life--;
			if(life<1){
				game_over=true;
				return;
			}
			 for (int j=0; j<data->numVertices; ++j) {
	 	float temp1=data->vertices[j].x;
	 	float temp2=data->vertices[j].y;
	 	float temp3=data->vertices[j].z;
	 	double t=-M_PI/2;
	 	va[j].x=temp1*cos(t) +temp3*sin(t);
	 	va[j].y=temp2;
	 	va[j].z=-temp1*sin(t)+temp3*cos(t);
	 	
		va[j].x=(va[j].x+shift_x)*zoomer;
		va[j].y=(va[j].y+ shift_y)*zoomer;
		va[j].z=(va[j].z+shift_z)*zoomer;
		va[j].w=1;
		}
		
			treasure_found=false;
			return;
		}
		}
	for(int i=0;i<3;i++){
		if((e2[i].x-tempe1)*(e2[i].x-tempe1) + (e2[i].z-tempe2)*(e2[i].z-tempe2) < 40){
			eyeX=seyeX;
			eyeY=seyeY;
			eyeZ=seyeZ;
			centreX=scentreX;
			centreY=scentreY;
			centreZ=scentreZ;
			life--;
			if(life<1){
				game_over=true;
				return;
			}
			  for (int j=0; j<data->numVertices; ++j) {
	 	float temp1=data->vertices[j].x;
	 	float temp2=data->vertices[j].y;
	 	float temp3=data->vertices[j].z;
	 	double t=-M_PI/2;
	 	va[j].x=temp1*cos(t) +temp3*sin(t);
	 	va[j].y=temp2;
	 	va[j].z=-temp1*sin(t)+temp3*cos(t);
	 	
		va[j].x=(va[j].x+shift_x)*zoomer;
		va[j].y=(va[j].y+ shift_y)*zoomer;
		va[j].z=(va[j].z+shift_z)*zoomer;
		va[j].w=1;
		}
			treasure_found=false;
			return;
		}
		}

}

void update_ceil(int i){
	if(i==0){
		if(ceiling_array[i].theta<=0){
			ceiling_array[0].theta=0;
			ceiling_array[0].velocity=0;
			ceiling_array[0].acceleration=0;
			return;
		}
		ceiling_array[i].next();
		
	}
}

void update_ball(int i){
	if(i==0){
		if(!ball_array[0].flag[0]){
			int x,y,r;
			x=ceiling_array[0].myball.x-ball_array[0].x;
			y=ceiling_array[0].myball.y-ball_array[0].y;
			r=ball_array[0].radius;
			if(4*r*r>=x*x+y*y){
				ball_array[0].flag[0]=true;
				ball_array[0].vx=400;
				}
			return;
		}
		if(!ball_array[0].flag[1]){
			
			ball_array[0].next();
			if(ball_array[0].x+2>=mywedge.x){
				ball_array[0].flag[1]=true;
				ball_array[0].vy=ball_array[0].vx*sin(mywedge.alpha);
				ball_array[0].vx=ball_array[0].vx*cos(mywedge.alpha);
				
				
				}
			return;
		
		}
		if(!ball_array[0].flag[2]){
			ball_array[0].next();
			if(ball_array[0].x>=mywedge.x+mywedge.length){
				ball_array[0].flag[2]=true;
				ball_array[0].ax=0;
				ball_array[0].ay=-4000;
				ball_array[0].vx=280;
			}
			return;
		}
		
		if(!ball_array[0].flag[3]){
			ball_array[0].next();
			if(mybalance.temp_cup.y+mybalance.temp_cup.width+ball_array[0].radius>=ball_array[0].y){
			ball_array[0].y=mybalance.temp_cup.y+mybalance.temp_cup.width+ball_array[0].radius;
			ball_array[0].vx=0;
			ball_array[0].vy=0;
			ball_array[0].ax=0;
			ball_array[0].ay=0;
			ball_array[0].flag[3]=true;
			mybalance.flag[0]=true;
			}
			return;
		}
		if(!ball_array[0].flag[4]){
			ball_array[0].y=mybalance.temp_cup.y+mybalance.temp_cup.width+ball_array[0].radius;
		
		}
		
		
		return;
	}
	return;
}

void update_balance(){
	if(!mybalance.flag[0])
		return;
	if(!mybalance.flag[1]){
	mybalance.theta+=0.1;
		if(mybalance.theta>=3){
			mybalance.flag[1]=true;
		}
		}
	return;
	}

void update_pivot(int i){
	if(i==0){
		if(!pivot_array[0].flag[0] && mybalance.flag[1]){
			pivot_array[0].theta+=0.2;
			if(pivot_array[0].theta>10){
				pivot_array[0].flag[0]=true;
				ball_array[1].flag[0]=true;
				double t=M_PI*10/180;
				ball_array[1].ax=1000*sin(t)*cos(t);
				ball_array[1].ay=-1000*sin(t)*sin(t);
				}
		}
		return;
	
	}
}


void update_ball1(){
	if(!pivot_array[10].flag[0])
		return;
	if(!ball_array[1].flag[0]){	
		double t=M_PI*7/180;
		ball_array[1].ax=2000*sin(t)*cos(t);
		ball_array[1].ay=-2000*sin(t)*sin(t);
		ball_array[1].next();
		if(ball_array[1].x>=pivot_array[10].x+pivot_array[10].length*cos(t)){
			ball_array[1].flag[0]=true;
			ball_array[1].ax=0;
			ball_array[1].ay=-1000;
		}
		return;
	}
	if(!ball_array[1].flag[2]){
		ball_array[1].next();
		if(ball_array[1].y<=cup_array[1].y+cup_array[1].width+ball_array[1].radius){
			ball_array[1].y=cup_array[1].y+cup_array[1].width+ball_array[1].radius;
			ball_array[1].ax=0;
			ball_array[1].ay=0;
			ball_array[1].vx=0;
			ball_array[1].vy=0;
			ball_array[1].flag[2]=true;
		}
		return;
	}
	if(!ball_array[1].flag[3]){
		cup_array[1].y-=50*tick;
		ball_array[1].y-=50*tick;
		ball_array[3].x-=50*tick;
		box_array[0].x-=50*tick;
		if(ball_array[3].x+3<=floor_array[3].x){
			ball_array[1].flag[3]=true;
			ball_array[3].vx=-90;
			ball_array[3].ay=-800;
		}
		return;
	}
	
	return;
	
}

void update_sticks(){
	if(!mybalance.flag[1])
		return;
	for(int i=2;i<=10;i++){
		if(!pivot_array[i].flag[0]){
			double t;
			if(i%2==0)
				t=0.5;
			else
				t=-0.5;
			pivot_array[i].theta+=t;
			if(fabs(pivot_array[i].theta)>=7){
				pivot_array[i].flag[0]=true;
				if(t<0)
				pivot_array[i].theta=-7;
				else
					pivot_array[i].theta=7;
			}
			return;
		}
	}	

}

void update_ball3(){
	if(!ball_array[1].flag[3])
		return;
	if(!ball_array[3].flag[0]){
		ball_array[3].next();
		if(ball_array[3].y<=floor_array[4].y+ball_array[3].radius){
			ball_array[3].y=floor_array[4].y+ball_array[3].radius;
			ball_array[3].flag[0]=true;
			ball_array[3].vy=300;
		}
		return;
	}
	if(!ball_array[3].flag[1]){
		ball_array[3].next();
		if(ball_array[3].y<=floor_array[5].y+ball_array[3].radius){
			ball_array[3].y=floor_array[5].y+ball_array[3].radius;
			ball_array[3].flag[1]=true;
			ball_array[3].vy=200;
			
		}
		return;
	}
	if(!ball_array[3].flag[2]){
		ball_array[3].next();
		if(ball_array[3].y<=cup_array[2].y+cup_array[2].width+ball_array[3].radius){
			ball_array[3].y=cup_array[2].y+cup_array[2].width+ball_array[3].radius;
			ball_array[3].flag[2]=true;
			ball_array[3].vy=100;
			
		}
		return;
	}
	
	return;
}

void update_balance1(){
	if(!ball_array[3].flag[2])
		return;
	if(!pivot_array[1].flag[0]){
		pivot_array[1].theta+=0.1;
		double t=pivot_array[1].theta*M_PI/180;
		cup_array[0].x=pivot_array[1].x-pivot_array[1].length*cos(t)+pivot_array[1].width*sin(t)/2;
		cup_array[0].y=pivot_array[1].y+pivot_array[1].length*sin(t)+pivot_array[1].width*cos(t)/2;
		cup_array[2].x=pivot_array[1].x+pivot_array[1].length*cos(t)+pivot_array[1].width*sin(t)/2;
		cup_array[2].y=pivot_array[1].y-pivot_array[1].length*sin(t)+pivot_array[1].width*cos(t)/2;
		ball_array[3].y=cup_array[2].y+cup_array[2].width*cos(t)+ball_array[3].radius-(cup_array[2].length/2-ball_array[3].radius)*sin(t);
		if(ball_array[3].x+ball_array[3].radius<cup_array[2].x+cup_array[2].length*cos(t)/2-cup_array[2].width*cos(t)){
			ball_array[3].x+=0.5;
		}
		cup_array[0].theta+=0.1;
		cup_array[2].theta+=0.1;
		mybox.x+=0.2;
		if(pivot_array[1].theta>=10){
			pivot_array[1].theta=10;
			cup_array[0].theta=10;
			cup_array[2].theta=10;
			pivot_array[1].flag[0]=true;
		}
		return;
	}
	return;
}

void update_ceil1(){
	if(!pivot_array[1].flag[0])
		return;
	if(!ceiling_array[1].myball.flag[0]){
	if(ceiling_array[1].myball.x<=pivot_array[11].x+pivot_array[11].width/2+ceiling_array[1].myball.radius){
			ceiling_array[1].velocity=0;
			ceiling_array[1].acceleration=0;
			ceiling_array[1].myball.flag[0]=true;
			return;
		}
		ceiling_array[1].next();
		}
	return;
}

void update_pivot11(){
	if(!ceiling_array[1].myball.flag[0]){
		return;
		}
	if(!pivot_array[11].flag[0]){
		pivot_array[11].theta-=0.6;
		if(pivot_array[11].theta<=80){
			pivot_array[11].flag[0]=true;
			ball_array[2].vx=200;
			
		}
		ceiling_array[1].next();
		return;
	
	}
	ceiling_array[1].next();
	
}

void update_ball2(){
	if(!pivot_array[11].flag[0])
		return;
	if(!ball_array[2].flag[0]){
		ball_array[2].next();
		if(ball_array[2].x>=floor_array[2].x+floor_array[2].length){
			ball_array[2].flag[0]=true;
			ball_array[2].ay=-500;
		}
		return;
		}
	if(!ball_array[2].flag[1]){
		ball_array[2].next();
		if(ball_array[2].y<=pivot_array[14].y+pivot_array[14].length+ball_array[2].radius){
			ball_array[2].flag[1]=true;
		}
		return;
		}
	ball_array[2].x-=1.2;
	
	
}



void update(int t){
	if(!door){
	for(int i=0;i<4;i++)
		e1[i].next();
	for(int i=0;i<3;i++)
		e2[i].next();
	checkColision();
	glutPostRedisplay();
	glutTimerFunc(100,update,0);
	}
	else{
		update_ceil(0);
	update_ball(0);
	update_balance();
	update_sticks();
	update_ball1();
	update_ball3();
	update_balance1();
	update_ceil1();
	update_pivot11();
	update_ball2();
	//update_box1();
	pivot_array[12].theta+=-4;
	pivot_array[13].theta+=-4;
	pivot_array[14].theta+=-4;
	pivot_array[15].theta+=-4;
	conX-=1.2;
	glutPostRedisplay();
	if(!ball_array[2].flag[1])
	glutTimerFunc(25,update,0);
	}
}

int main(int argc, char ** argv){
	
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);	//Setting the display modes
	glutInitWindowSize(WIDTH,HEIGHT);			//Setting up the window size
	glutInitWindowPosition(0,0);		//Setting up the window position
	window_id=glutCreateWindow("Dave 3D");			//Window title
	init();					//Initialise some opengl states
	data=load_ifs_file ("legoman.ifs");
	va = new Vector4[data->numVertices];
	 for (int i=0; i<data->numVertices; ++i) {
	 	float temp1=data->vertices[i].x;
	 	float temp2=data->vertices[i].y;
	 	float temp3=data->vertices[i].z;
	 	double t=-M_PI/2;
	 	va[i].x=temp1*cos(t) +temp3*sin(t);
	 	va[i].y=temp2;
	 	va[i].z=-temp1*sin(t)+temp3*cos(t);
	 	
		va[i].x=(va[i].x+shift_x)*zoomer;
		va[i].y=(va[i].y+ shift_y)*zoomer;
		va[i].z=(va[i].z+shift_z)*zoomer;
		va[i].w=1;
		}		
	glutDisplayFunc(display);		// display callback function
	glutReshapeFunc(reshape);		// reshape callback function
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(keyboard2);
	glutTimerFunc(25,update,0);
	glutMainLoop();
	return 0;
}
