/*

  Tyfuus - Todimensjonale Yrkeshemmeligheter For Unikt Ultrabra Sammensetning (av bitkart)

  tilhører kusma/excess

  hvis du i det heletatt LESER dette, er du en kriminell jævel som bær stenges inne.

*/

#ifndef TYFUUS_H
#define TYFUUS_H

#include "grid.h"

#include <stdio.h>
#include <assert.h>
class Bitmap{
protected:
	int width, height;
	unsigned int* data;
public:
	inline unsigned int* GetData(){
		assert(data);
		return data;
	};

	inline unsigned int GetSubPixel( unsigned int x, unsigned int y ){
		unsigned int c1,c2,c3,c4;
		int b1,b2,b3,b4;
		int xblend, yblend;

		x %= width<<8;
		y %= height<<8;
		unsigned int* src = this->data+(x>>8)+((y>>8)*this->width);
		//src+=(x>>8)+((y>>8)*this->width);
		c1 = *src;
		c2 = *(src+1);
		c3 = *(src+1+this->width);
		c4 = *(src+this->width);

		xblend = x&0xFF;
		yblend = y&0xFF;

		b1 = ((255-xblend)*(255-yblend))>>8;
		b2 = ((    xblend)*(255-yblend))>>8;
		b3 = ((    xblend)*(    yblend))>>8;
		b4 = ((255-xblend)*(    yblend))>>8;

		b1 = ((b1<<24)|(b1<<16)|(b1<<8)|b1);
		b2 = ((b2<<24)|(b2<<16)|(b2<<8)|b2);
		b3 = ((b3<<24)|(b3<<16)|(b3<<8)|b3);
		b4 = ((b4<<24)|(b4<<16)|(b4<<8)|b4);

		__asm{
			movd mm7, b1
			punpcklbw mm7, mm5 //pakker ut alpha
			movd mm0, c1
			punpcklbw mm0, mm5 //pakker ut farge
			pmullw mm0, mm7 //ganger med alpha

			movd mm7, b2
			punpcklbw mm7, mm5 //pakker ut alpha
			movd mm1, c2
			punpcklbw mm1, mm5 //pakker ut farge
			pmullw mm1, mm7 //ganger med alpha
			paddusw mm0, mm1

			movd mm7, b3
			punpcklbw mm7, mm5 //pakker ut alpha
			movd mm1, c3
			punpcklbw mm1, mm5 //pakker ut farge
			pmullw mm1, mm7 //ganger med alpha
			paddusw mm0, mm1

			movd mm7, b4
			punpcklbw mm7, mm5 //pakker ut alpha
			movd mm1, c4
			punpcklbw mm1, mm5 //pakker ut farge
			pmullw mm1, mm7 //ganger med alpha
			paddusw mm0, mm1

			psrlw mm0, 8

			packuswb mm0, mm5
			
			movd c1, mm0
		};

		return c1;
	}

	inline int GetWidth(){
		return width;
	};

	inline int GetHeight(){
		return height;
	};

	Bitmap();
	Bitmap(int width, int height);
	~Bitmap();

	void Clear( unsigned int color );

	void Blit( Bitmap &destination );
	void MulBlit( Bitmap &destination );
	void AddBlit( Bitmap &destination );
	void SubBlit( Bitmap &destination );
	void Blend( Bitmap &destination, unsigned char alpha );
	void AlphaBlend( Bitmap &destination );
	void Treshold( unsigned char level );
	void Gamma( float gamma );
	void Invert();
	void HorisontalBlur( Bitmap &target, unsigned char amount );
	void VerticalBlur( Bitmap &target, unsigned char amount );
	void RadialBlur( int xpos, int ypos, float strengt );
	void AddRadialBlur( int xpos, int ypos, float strengt );
	void BammBlit( Bitmap &destination, int amount );
};

// BITMAP LOADERS

class JpegFile:public Bitmap{
public:
	JpegFile( char* filename );
	JpegFile::JpegFile( char* filename, char*alpha );
	void Load( char* filename );
};

class RawFile:public Bitmap{
public:
	RawFile(){};
	RawFile( char* filename, int width, int height, int bpp );
};

#include "sprite.h"


#endif //TYFUUS_H