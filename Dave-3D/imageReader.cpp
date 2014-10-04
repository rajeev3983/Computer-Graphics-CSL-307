#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string.h>
#include <stdio.h>
#include <GL/glut.h>
using namespace std;
fstream inf;  
typedef unsigned char  uchar;
typedef unsigned short ushort;
typedef unsigned long  ulong;
class Pix_c{ 
	    public: uchar r,g,b;
	      Pix_c(){r = g = b = 0;}
	      
	  };

class Image{
 private: 
  Pix_c* pixel;
    
  public:
    int nRows, nCols; 
      Image() {nRows = nCols = 0; pixel = 0;}
  Image(int rows, int cols) 
    {
      nRows = rows;
      nCols = cols;
      pixel = new Pix_c[rows*cols]; 
    }
  int readBMPFile(string fname); 
  void setTexture(GLuint textureName,bool repeat);
  
};

ushort getShort()
{
  char ic;
  ushort ip;
  inf.get(ic); ip = ic;  
  inf.get(ic);  ip |= ((ushort)ic << 8);
  return ip;
}

ulong getLong()
{  
  ulong ip = 0;
  char ic = 0;
  unsigned char uc = ic;
  inf.get(ic); uc = ic; ip = uc;
  inf.get(ic); uc = ic; ip |=((ulong)uc << 8);
  inf.get(ic); uc = ic; ip |=((ulong)uc << 16);
  inf.get(ic); uc = ic; ip |=((ulong)uc << 24);
  return ip;
}

int Image:: readBMPFile(string fname) 
{ 
  inf.open(fname.c_str(), ios::in|ios::binary); 
  if(!inf){ cout << " can't open file: " << fname << endl; return 0;}
  int k, row, col, numPadBytes, nBytesInRow;
  char ch1, ch2;
  inf.get(ch1); inf.get(ch2); 
  ulong fileSize = getLong();
  ushort reserved1 =  getShort();  
  ushort reserved2= getShort();    
  ulong offBits =getLong();  
  ulong headerSize =   getLong();     
  ulong numCols =getLong();   
  ulong numRows = getLong();  
  ushort planes=    getShort();    
  ushort bitsPerPix = getShort();    
  ulong compression = getLong();    
  ulong imageSize = getLong();   
  ulong xPels =    getLong();  
  ulong yPels =    getLong();     
  ulong numLUTentries = getLong();  
  ulong impColors = getLong();     

  if(bitsPerPix != 24) {cout << "not a 24 bit/pixelimage!\n"; inf.close(); return 0;}; 
 
  nBytesInRow = ((3 * numCols + 3)/4) * 4; 
  numPadBytes = nBytesInRow - 3 * numCols; 
  nRows = numRows;
   nCols = numCols;
  cout.flush();
  pixel = new Pix_c[nRows * nCols]; 
  if(!pixel) return 0; 
  long count = 0;
  char dum,r,g,b;
  
  for(row = 0; row < nRows; row++) 
    {
      for(col = 0; col < nCols; col++)
	{
	  inf.get(b); inf.get(g); inf.get(r);
	  pixel[count].r = r; pixel[count].g = g; pixel[count++].b = b;
	}
      for(k = 0; k < numPadBytes ; k++) 
	inf >> dum;
    }
  inf.close();
  return 1; 
}

void Image :: setTexture(GLuint textureName,bool repeat)
{
glBindTexture(GL_TEXTURE_2D, textureName);
	if(repeat){
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);
glTexImage2D(GL_TEXTURE_2D,0, GL_RGB, nCols, nRows, 0, GL_RGB, GL_UNSIGNED_BYTE, pixel);
}
