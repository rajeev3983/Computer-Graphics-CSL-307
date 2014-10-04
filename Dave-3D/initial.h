#include <iostream>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cmath>
#include "imageReader.cpp"
Image pix[7]; 
using namespace std;

void init2(void)
{    
   pix[0].readBMPFile("brk.bmp");  
   pix[0].setTexture(2001,true);
   pix[1].readBMPFile("credits.bmp");  
   pix[1].setTexture(2002,true);
   pix[2].readBMPFile("start.bmp");
   pix[2].setTexture(2003,false);
    pix[3].readBMPFile("dave.bmp");
   pix[3].setTexture(2004,false);
  pix[4].readBMPFile("gameOver.bmp");
   pix[4].setTexture(2005,false);
   pix[5].readBMPFile("congratulations.bmp");
   pix[5].setTexture(2006,false);
    pix[6].readBMPFile("new_game.bmp");
   pix[6].setTexture(2007,false);

}

