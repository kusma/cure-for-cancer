/*
#include <math.h>

#define	RGB(r,g,b)	((((r)&0xFF)<<16)|(((g)&0xFF)<<8)|((b)&0xFF))
#define	GETR(c)		(((c)&0xFF0000)>>16)
#define	GETG(c)		(((c)&0xFF00)>>8)
#define	GETB(c)		((c)&0xFF)

#include <stdio.h>

#ifndef M_PI
#define M_PI 3.14159265358979
#endif

#include "../kancr/vector.h"

float tunnel_radius = 1;
float tunnel_radius_squared = tunnel_radius*tunnel_radius;

inline float TunnelIntersect( Vector origin, Vector direction ){
	float a = (direction.x*direction.x) + (direction.y*direction.y);
	float b = 2*(origin.x*direction.x + origin.y*direction.y);
	float c = (origin.x*origin.x) + (origin.y*origin.y) - tunnel_radius_squared;

	float delta = (b*b) - 4*a*c;

	if (delta < 0) return 0;

	float sqrtd = (float)sqrt(delta);
	float amul = 2*a;
	float t1 = (-b + sqrtd)/amul;
	float t2 = (-b - sqrtd)/amul;
	if( t1>t2 ) return t2;
	else return t1;
}

Vector TraceRay( Vector origin, Vector direction )
{
	float t = 9999.0f;
	float t2 = TunnelIntersect( origin, direction );
	if( t2<t ){
		if (t2<0.0f) t=t2;	
	}
	Vector intersection = origin+(direction*t);
	return intersection;
}

#include "tunnel.h"
#include "../tool/misc.h"
#define SHIFT 8


inline void texture_hline( unsigned int* buffer, int size, int u1, int v1, int u2, int v2, unsigned int* texture ){
	int udelta = (u2-u1)/size;
	int u = u1;
	int vdelta = (v2-v1)/size;
	int v = v1;
	for( int counter=size; counter; counter-- ){
		*buffer++=texture[((u>>SHIFT)&0xFF) +(((v>>SHIFT)&0xFF)<<8)];
		u+=udelta;
		v+=vdelta;
	}
}

void fill_quad(unsigned int* buffer, int width, int x, int y, int size, int u1, int v1, int u2, int v2, int u3, int v3, int u4, int v4, unsigned int* texture){
	int u_delta1 = ((u3-u1)<<SHIFT)/size;
	int u_delta2 = ((u4-u2)<<SHIFT)/size;
	u1<<=SHIFT;
	u2<<=SHIFT;

	int v_delta1 = ((v3-v1)<<SHIFT)/size;
	int v_delta2 = ((v4-v2)<<SHIFT)/size;
	v1<<=SHIFT;
	v2<<=SHIFT;

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


Tunnel::Tunnel( int width, int height, unsigned int* texture, int lod ){
	this->lod = lod;
	this->width = width;
	this->height = height;

	this->grid_width = (width/lod);
	this->grid_height = (width/lod);

	this->texture = texture;
	this->direction = new Vector[(this->grid_width+1)*(this->grid_height+1)];
	this->grid = new grid_node[(this->grid_width+1)*(this->grid_height+1)];
	this->origin = Vector(0,0,0);
	this->Rotate(Vector(0,0,0));

	Vector* dir = this->direction;
	for(int y=0; y<(this->grid_height+1); y++)
		for(int x=0; x<(this->grid_width+1); x++){
			*dir = Vector(
				(((x*lod)-(width>>1))*(1.0f/((width)>>1))),
				(((y*lod)-(height>>1))*(1.0f/((width)>>1))),
				1
				);
			dir->normalize();
			*dir++;
		}
}

void Tunnel::Render( unsigned int* buffer, int time ){
	Vector* dir = this->direction;
	grid_node* grid=this->grid;
	for(int counter=((this->height/this->lod)+1)*((this->width/this->lod)+1); counter; counter--){
			Vector deform = *dir++;
			//deform.x *= sin(deform.x+time*0.001f)*sin(deform.y+time*0.0012f);
			//deform.y *= sin(deform.y+time*0.0013f)*sin(deform.x+time*0.012f);
			Vector direction = this->matrix.TransformVector( deform );
			//Vector direction = this->matrix.TransformVector( *dir++ );
			float deg = atan2(direction.x, direction.y)*5;
			direction.x *= 0.5f+(cos(deg)*0.3f)*sin(time*0.001f);
			direction.y *= 0.5f+(cos(deg)*0.3f)*sin(time*0.001f);
			//direction.z *= (sin(direction.x*direction.y)*0.01f);
			Vector intersection = TraceRay(this->origin, direction);
			grid->u = (int)(intersection.z*15);
			grid->v = (int)abs((atan2(intersection.x, intersection.y)*(256/M_PI)));
			if(intersection.z>255) intersection.z=255;
			grid->shade = intersection.z;
			*grid++;
		}
	grid = this->grid;
	int h = (this->height/this->lod);
	int w = (this->width/this->lod);
	for(int y=0; y<h; y++){
		for(int x=0; x<w; x++){
			fill_quad( buffer, this->width, x*this->lod, y*this->lod, this->lod,
				grid[x].u, grid[x].v,
				grid[x+1].u, grid[x+1].v,
				grid[x+w+1].u, grid[x+w+1].v,
				grid[x+w+2].u, grid[x+w+2].v,
				this->texture);
			
		}
		grid+=w+1;
	}

}

void Tunnel::Rotate( Vector rotation ){
	this->matrix.Identity();
	this->matrix.Rotate(rotation);
}

void Tunnel::Move( Vector position ){
	this->origin = position;
}

*/