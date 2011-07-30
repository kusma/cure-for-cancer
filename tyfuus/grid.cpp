#include "tyfuus.h"
#include <windows.h>

#include <math.h>
#ifndef M_PI
#define M_PI 3.14159265358979f
#define M_PI2 M_PI*2
#define PI_SCALE 0.31830988618379067153776752674503f
#endif

#define SHIFT 8
inline void texture_hline( unsigned int* buffer, int size, int u1, int v1, int u2, int v2, int s1, int s2, unsigned int* texture ){
	int sizediv = 0xFF/size;
	int udelta = ((u2-u1)*sizediv)>>8;
	int u = u1;
	int vdelta = ((v2-v1)*sizediv)>>8;
	int v = v1;
	int sdelta = ((s2-s1)*sizediv)>>8;
	int s = s1;
	for( int counter=size; counter; counter-- ){
		unsigned int color = texture[((u>>SHIFT)&0xFF) +(((v>>SHIFT)&0xFF)<<8)];

		int shade = (s>>SHIFT)&0xff;
		shade |= (shade<<16)|(shade<<8);
		_asm{
			movd mm0, color
			punpcklbw mm0, mm5
			movd mm1, shade
			punpcklbw mm1, mm5

			pmullw mm0, mm1
			psrlw mm0, 8

			packuswb mm0, mm5
			movd color, mm0
		}

		*buffer++ = color;
		u+=udelta;
		v+=vdelta;
		s+=sdelta;
	}
}

void fill_quad(unsigned int* buffer, int width, int x, int y, int size, int u1, int v1, int u2, int v2, int u3, int v3, int u4, int v4, int s1, int s2, int s3, int s4, unsigned int* texture){

	int sizediv = 0xFF/size;
	int u_delta1 = (((u3-u1)<<SHIFT)*sizediv)>>8;
	int u_delta2 = (((u4-u2)<<SHIFT)*sizediv)>>8;
	u1<<=SHIFT;
	u2<<=SHIFT;

	int v_delta1 = (((v3-v1)<<SHIFT)*sizediv)>>8;
	int v_delta2 = (((v4-v2)<<SHIFT)*sizediv)>>8;
	v1<<=SHIFT;
	v2<<=SHIFT;

	int s_delta1 = (((s3-s1)<<SHIFT)*sizediv)>>8;
	int s_delta2 = (((s4-s2)<<SHIFT)*sizediv)>>8;
	s1<<=SHIFT;
	s2<<=SHIFT;

	_asm pxor mm5, mm5;

	buffer+=x+y*width;
	for(y=size; y; y--){
		u1+=u_delta1;
		u2+=u_delta2;
		v1+=v_delta1;
		v2+=v_delta2;
		s1+=s_delta1;
		s2+=s_delta2;
		texture_hline(buffer, size, u1, v1, u2, v2, s1, s2, texture);
		buffer+=width;
	}
	_asm emms;
}
/*
inline void texture_hline( unsigned int* buffer, int size, int u1, int v1, int u2, int v2, unsigned int* texture ){
	int udelta = (u2-u1)/size;
	int u = u1+0x7f;
	int vdelta = (v2-v1)/size;
	int v = v1+0x7f;
	for( int counter=size; counter; counter-- ){
		*buffer++=texture[((u>>SHIFT)&0xFF) +(((v>>SHIFT)&0xFF)<<8)];
		u+=udelta;
		v+=vdelta;
	}
}

void fill_quad(unsigned int* buffer, int width, unsigned int x, unsigned int y, unsigned int size,
			   unsigned int u1, unsigned int v1,
			   unsigned int u2, unsigned int v2,
			   unsigned int u3, unsigned int v3,
			   unsigned int u4, unsigned int v4,
			   unsigned int* texture){

	int u_delta1 = ((u3-u1)<<SHIFT)/size;
	int u_delta2 = ((u4-u2)<<SHIFT)/size;
	u1<<=SHIFT;
	u2<<=SHIFT;

	int v_delta1 = ((v3-v1)<<SHIFT)/size;
	int v_delta2 = ((v4-v2)<<SHIFT)/size;
	v1<<=SHIFT;
	v2<<=SHIFT;

	v1+=0.7f;
	v2+=0.7f;
	u1+=0.7f;
	u2+=0.7f;

	buffer+=x+y*width;
	for(y=size; y; y--){
		u1+=u_delta1;
		u2+=u_delta2;
		v1+=v_delta1;
		v2+=v_delta2;
		texture_hline(buffer, size, u1, v1, u2, v2, texture);
		buffer+=width;
	}
}
*/

#define SHIFT 8
inline void texture_subpixel_hline( unsigned int* buffer, int size, int u1, int v1, int u2, int v2, unsigned int* texture ){
	int udelta = (u2-u1)/(size);
	int vdelta = (v2-v1)/(size);
//	int u = u1+0x7f;
//	int v = v1+0x7f;
	int u = u1;
	int v = v1;

	_asm{
//		emms
		pxor mm5,mm5	
	}
	for( int counter=size; counter; counter-- ){
		int color1 = texture[(((u)>>SHIFT)&0xFF)	+((((v)>>SHIFT)&0xFF)<<8)];
		int color2 = texture[(((u+0xFF)>>SHIFT)&0xFF)	+((((v)>>SHIFT)&0xFF)<<8)];
		int color3 = texture[(((u+0xFF)>>SHIFT)&0xFF)	+((((v+0xFF)>>SHIFT)&0xFF)<<8)];
		int color4 = texture[(((u)>>SHIFT)&0xFF)	+((((v+0xFF)>>SHIFT)&0xFF)<<8)];

		int ublend = u&0xFF;
		int vblend = v&0xFF;

		int b1 = ((255-ublend)*(255-vblend))>>8;
		int b2 = ((    ublend)*(255-vblend))>>8;
		int b3 = ((    ublend)*(    vblend))>>8;
		int b4 = ((255-ublend)*(    vblend))>>8;

		b1 = ((b1<<24)|(b1<<16)|(b1<<8)|b1);
		b2 = ((b2<<24)|(b2<<16)|(b2<<8)|b2);
		b3 = ((b3<<24)|(b3<<16)|(b3<<8)|b3);
		b4 = ((b4<<24)|(b4<<16)|(b4<<8)|b4);

		__asm{
			movd mm7, b1
			punpcklbw mm7, mm5 //pakker ut alpha
			movd mm0, color1
			punpcklbw mm0, mm5 //pakker ut farge
			pmullw mm0, mm7 //ganger med alpha

			movd mm7, b2
			punpcklbw mm7, mm5 //pakker ut alpha
			movd mm1, color2
			punpcklbw mm1, mm5 //pakker ut farge
			pmullw mm1, mm7 //ganger med alpha
			paddusw mm0, mm1

			movd mm7, b3
			punpcklbw mm7, mm5 //pakker ut alpha
			movd mm1, color3
			punpcklbw mm1, mm5 //pakker ut farge
			pmullw mm1, mm7 //ganger med alpha
			paddusw mm0, mm1

			movd mm7, b4
			punpcklbw mm7, mm5 //pakker ut alpha
			movd mm1, color4
			punpcklbw mm1, mm5 //pakker ut farge
			pmullw mm1, mm7 //ganger med alpha
			paddusw mm0, mm1

			psrlw mm0, 8

			psrlw mm5, 0xF
			packuswb mm0, mm5
			
			movd color1, mm0
		};

		*buffer++=color1;
		u+=udelta;
		v+=vdelta;
	}
	_asm emms;
}


void fill_subpixel_quad(unsigned int* buffer, int width, unsigned int x, unsigned int y, unsigned int size,
			   unsigned int u1, unsigned int v1,
			   unsigned int u2, unsigned int v2,
			   unsigned int u3, unsigned int v3,
			   unsigned int u4, unsigned int v4,
			   unsigned int* texture){

	int u_delta1 = (u3-u1)/size;
	int u_delta2 = (u4-u2)/size;
	int v_delta1 = (v3-v1)/size;
	int v_delta2 = (v4-v2)/size;

	v1+=0.7f;
	v2+=0.7f;
	u1+=0.7f;
	u2+=0.7f;

	buffer+=x+y*width;
	for(y=size; y; y--){
		u1+=u_delta1;
		u2+=u_delta2;
		v1+=v_delta1;
		v2+=v_delta2;
		texture_subpixel_hline(buffer, size, u1, v1, u2, v2, texture);
		buffer+=width;
	}
}


Grid8x8::Grid8x8(int width, int height){
	this->width = width;
	this->height = height;
	this->grid_width = width>>3;
	this->grid_height = height>>3;
	this->texture = NULL;
	this->data = new GridNode[ (grid_width+1)*(grid_height+1) ];
	this->direction = new Vector[(grid_width+1)*(grid_height+1)];
	Vector* dir = direction;
	for(int y=0; y<(grid_height+1); y++)
		for(int x=0; x<(grid_width+1); x++){
			*dir = Vector(
				(((x*8)-(width>>1))*(1.0f/((width)>>1))),
				(((y*8)-(height>>1))*(1.0f/((width)>>1))),
				-0.7f
				);
			dir->normalize();
			*dir++;
		}
	Reset();
	matrix.Identity();
}
Grid8x8::~Grid8x8(){
	delete[] this->data;
}

void Grid8x8::Reset(){
	GridNode* pointer = this->data;
	for( int y=0; y<grid_height+1; y++ ){
		for( int x=0; x<grid_width+1; x++ ){
			pointer->u = (255/(grid_width+1))*x;
			pointer->v = (255/(grid_height+1))*y;
			pointer->shade = 255;
			pointer++;
		}
	}
}

void Grid8x8::Move(float dx, float dy){
	GridNode* pointer = this->data;
	for( int y=0; y<grid_height+1; y++ ){
		for( int x=0; x<grid_width+1; x++ ){
			pointer->u += dx;
			pointer->v += dy;
			pointer++;
		}
	}
}

void Grid8x8::SetTexture( Bitmap& texture ){
	this->texture = &texture;
}

void Grid8x8::Render( Bitmap& target ){
	GridNode* pointer = data;
	unsigned int* dst = target.GetData();
	unsigned int* src = texture->GetData();

	for( int y=0; y<grid_height; y++ ){
		for( int x=0; x<grid_width; x++ ){
			//target[(x<<3)+((y<<3)*width)] = 0xFFFFFF;
			fill_quad( dst, width, (x<<3), (y<<3), 8,
						(pointer)->u, (pointer)->v,
						(pointer+1)->u, (pointer+1)->v,
						(pointer+1+grid_width)->u, (pointer+1+grid_width)->v,
						(pointer+2+grid_width)->u, (pointer+2+grid_width)->v,
						(pointer)->shade, (pointer+1)->shade,
						(pointer+1+grid_width)->shade, (pointer+2+grid_width)->shade,
						src );

			pointer++;
		}
		pointer++;
	}
}

void Grid8x8::RenderSubpixel( Bitmap& target ){
	GridNode* pointer = data;
	unsigned int* dst = target.GetData();
	unsigned int* src = texture->GetData();

	for( int y=0; y<grid_height; y++ ){
		for( int x=0; x<grid_width; x++ ){
			fill_subpixel_quad( dst, width, (x<<3), (y<<3), 8,
						(pointer)->u*256, (pointer)->v*256,
						(pointer+1)->u*256, (pointer+1)->v*256,
						(pointer+1+grid_width)->u*256, (pointer+1+grid_width)->v*256,
						(pointer+2+grid_width)->u*256, (pointer+2+grid_width)->v*256,
						src );

			pointer++;
		}
		pointer++;
	}
}


// Distortions

void Grid8x8::SinusDistort(float wavelen, float time, float strength){
	GridNode* pointer = this->data;
	wavelen = 1/wavelen;
	for( int y=0; y<grid_height+1; y++ ){
		for( int x=0; x<grid_width+1; x++ ){
			pointer->u += sin(time+(y*wavelen))*strength;
			pointer->v += sin(time+(x*wavelen))*strength;
			pointer++;
		}
	}
}

void Grid8x8::AttractDistort( float cx, float cy, float strength, float size ){
	GridNode* pointer = this->data;
	cx += (grid_width>>1);
	cy += (grid_height>>1);
	float precalc = (M_PI/size*0.5f);
	for( int y=0; y<grid_height+1; y++ ){
		for( int x=0; x<grid_width+1; x++ ){
			float deg = atan2(y-cy,x-cx);
			float dist = sqrt((x-cx)*(x-cx)+(y-cy)*(y-cy));
			if( dist<size ){
				dist *= 1-cos(M_PI-dist*precalc);
			}
			pointer->u += (cx+cos(deg)*dist)*strength;
			pointer->v += (cy+sin(deg)*dist)*strength;
			pointer++;
		}
	}
}

void Grid8x8::BlowupDistort( float cx, float cy, float strength, float size ){
	GridNode* pointer = this->data;
	cx += (grid_width>>1);
	cy += (grid_height>>1);
	float precalc = (M_PI/size);
	for( int y=0; y<grid_height+1; y++ ){
		for( int x=0; x<grid_width+1; x++ ){
			float deg = atan2(y-cy,x-cx);
			float dist = sqrt((x-cx)*(x-cx)+(y-cy)*(y-cy));
			if( dist<size ){
				dist*= (cos(M_PI+dist*precalc)-0.9f)*strength;
			}
			pointer->u += (cx+cos(deg)*dist);
			pointer->v += (cy+sin(deg)*dist);
			pointer++;
		}
	}
}

void Grid8x8::WaveDistort(float cx, float cy, float wobble, float wavelen, float strength){
	GridNode* pointer = this->data;
	cx += (grid_width>>1);
	cy += (grid_height>>1);
	wavelen = 1/wavelen;
	for( int y=0; y<grid_height+1; y++ ){
		for( int x=0; x<grid_width+1; x++ ){
			float deg = atan2(y-cy,x-cx);
			float dist = sqrt((x-cx)*(x-cx)+(y-cy)*(y-cy))*wavelen;
			pointer->u += cx+cos(deg)*(dist+cos(dist-wobble)*strength );
			pointer->v += cy+sin(deg)*(dist+cos(dist-wobble)*strength );
			pointer++;
		}
	}
}

#include "../kancr/vector.h"
float tunnel_radius = 2;
float tunnel_radius_squared = tunnel_radius*tunnel_radius;

inline float TunnelIntersect( Vector origin, Vector direction ){
	float a = (direction.x*direction.x) + (direction.y*direction.y);
	float b = 2*(origin.x*direction.x + origin.y*direction.y);
	float c = (origin.x*origin.x) + (origin.y*origin.y) - tunnel_radius_squared;

	float delta = (b*b) - 4*a*c;

	float sqrtd = (float)sqrt(delta);
	float amul = 2*a;
	float t = (-b + sqrtd)/amul;
	return t;
}

inline Vector TraceRay( Vector origin, Vector direction )
{
	float t = TunnelIntersect( origin, direction );
	Vector intersection = origin+(direction*t);
	return intersection;
}

void Grid8x8::Tunnel( Vector rotation, Vector position, float deform ){
	matrix.Identity();
	matrix.Rotate( rotation );
	origin = position;
	Vector* dir = direction;
	GridNode* grid=this->data;
	for( int counter = (grid_height+1)*(grid_width+1); counter; counter-- ){
			Vector direction = this->matrix.TransformVector( *dir++ );
			Matrix temp;
			temp.Identity();
			float d = deform;
			d *= direction.z;
			temp.Rotate(Vector(d, d*0.89, 0));
			direction = temp.TransformVector( direction );

			Vector intersection = TraceRay(this->origin, direction);

			grid->u = (int)(intersection.z*64);
			grid->v = (int)abs((atan2(intersection.y, intersection.x)*(512/M_PI)));

			int shade = fabs((this->origin.z-intersection.z))*32;
			if(shade>255) shade=255;
			if(shade<0) shade=0;
			grid->shade = ~(char)shade;

			*grid++;
		}
}