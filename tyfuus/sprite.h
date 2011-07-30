#ifndef SPRITE_H
#define SPRITE_H

#include "tyfuus.h"

class Sprite{
	unsigned int* buffer;
	int width, height;
	int xpos, ypos;
public:
	Sprite( Bitmap &source );
	Sprite();
	//~Sprite();
	void Move( int x, int y );
	void Blit( Bitmap &destination );
	void AddBlit( Bitmap &destination );
	void MulAddBlit( Bitmap &destination );
};

#endif