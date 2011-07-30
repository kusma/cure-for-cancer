/*

  Tyfuus - Todimensjonale Yrkeshemmeligheter For Unikt Ultrabra Sammensetning (av bitkart)

  tilhører kusma/excess

  hvis du i det heletatt LESER dette, er du en kriminell jævel som bær stenges inne.

*/


#ifndef GRID_H
#define GRID_H


#include <math.h>
#include "../kancr/vector.h"
#include "../kancr/matrix.h"
class Bitmap;

class Grid8x8{
protected:
	class GridNode{
	public:
		float u, v;
		unsigned char shade;
	};


	Vector* direction;
	Vector origin;
	Matrix matrix;

	int width, height;
	int grid_width, grid_height;
	GridNode *data;
	Bitmap* texture;

public:
	Grid8x8(int width, int height);
	~Grid8x8();

	void SetTexture( Bitmap& texture );
	void Reset();

	void SinusDistort(float wavelen, float time, float strength);
	void AttractDistort( float cx, float cy, float strength, float size );
	void BlowupDistort( float cx, float cy, float strength, float size );
	void WaveDistort( float cx, float cy, float wobble, float wavelen, float strength );
	void Move(float dx, float dy);

	void Tunnel( Vector rotation, Vector position, float time );


	inline void TestDistort(float cx, float cy, int time, float strength){
		GridNode* pointer = this->data;
		cx = (grid_width>>1) + cx;
		cy = (grid_height>>1) + cy;
		for( int y=0; y<grid_height+1; y++ ){
			for( int x=0; x<grid_width+1; x++ ){
				float fx = x-cx;
				float fy = y-cy;
				float deg = (float)atan2(fx,fy);
				float dist = (float)sqrt(fx*fx+fy*fy);
				pointer->u += (float)sin(dist*0.7f+time*0.003f)*10;//sin(time*0.001f+(y*0.1f))*strength;
				pointer->v += (float)cos(dist*0.7f+time*0.003f)*10;//sin(time*0.001f+(x*0.1f))*strength;
				pointer++;
			}
		}		
	}

	void Render( Bitmap& target );
	void RenderSubpixel( Bitmap& target );

};


#endif //GRID_H