/* inline all this into matrix.h later */

#include "kancr.h"

void Matrix::Identity(){
	this->matrix[0 ] = 1; this->matrix[1 ] = 0; this->matrix[2 ] = 0; this->matrix[3 ] = 0;
	this->matrix[4 ] = 0; this->matrix[5 ] = 1; this->matrix[6 ] = 0; this->matrix[7 ] = 0;
	this->matrix[8 ] = 0; this->matrix[9 ] = 0; this->matrix[10] = 1; this->matrix[11] = 0;
	this->matrix[12] = 0; this->matrix[13] = 0; this->matrix[14] = 0; this->matrix[15] = 1;
}

void Matrix::Rotate( Vector rotation ){
	float cx, cy, cz, sx, sy, sz;
	cx = (float)cos(rotation.x);
	cy = (float)cos(rotation.y);
	cz = (float)cos(rotation.z);
	sx = (float)sin(rotation.x);
	sy = (float)sin(rotation.y);
	sz = (float)sin(rotation.z);
	this->matrix[0 ] = cy*cz;			this->matrix[1 ] = cy*sz;			this->matrix[2 ] = -sy;		this->matrix[3 ] = 0;
	this->matrix[4 ] = sx*sy*cz-cx*sz;	this->matrix[5 ] = sx*sy*sz+cx*cz;	this->matrix[6 ] = sx*cy;	this->matrix[7 ] = 0;
	this->matrix[8 ] = cx*sy*cz+sx*sz;	this->matrix[9 ] = cx*sy*sz-sx*cz;	this->matrix[10] = cx*cy;	this->matrix[11] = 0;
	this->matrix[12] = 0;				this->matrix[13] = 0;				this->matrix[14] = 0;		this->matrix[15] = 1;
}

void Matrix::Translate( Vector translation ){
	this->matrix[0 ] = 1;				this->matrix[1 ] = 0;				this->matrix[2 ] = 0;				this->matrix[3 ] = 0;
	this->matrix[4 ] = 0;				this->matrix[5 ] = 1;				this->matrix[6 ] = 0;				this->matrix[7 ] = 0;
	this->matrix[8 ] = 0;				this->matrix[9 ] = 0;				this->matrix[10] = 1;				this->matrix[11] = 0;
	this->matrix[12] = translation.x;	this->matrix[13] = translation.y;	this->matrix[14] = translation.z;	this->matrix[15] = 1;
}

void Matrix::Scale( Vector scale ){
	this->matrix[0 ] = scale.x;	this->matrix[1 ] = 0;		this->matrix[2 ] = 0;		this->matrix[3 ] = 0;
	this->matrix[4 ] = 0;		this->matrix[5 ] = scale.y;	this->matrix[6 ] = 0;		this->matrix[7 ] = 0;
	this->matrix[8 ] = 0;		this->matrix[9 ] = 0;		this->matrix[10] = scale.z;	this->matrix[11] = 0;
	this->matrix[12] = 0;		this->matrix[13] = 0;		this->matrix[14] = 0;		this->matrix[15] = 1;
}

void Matrix::Perspective( float fov, float aspect, float znear, float zfar ){
	this->matrix[0 ] = 1; this->matrix[1 ] = 0; this->matrix[2 ] = 0; this->matrix[3 ] = 0;
	this->matrix[4 ] = 0; this->matrix[5 ] = 1; this->matrix[6 ] = 0; this->matrix[7 ] = 0;
	this->matrix[8 ] = 0; this->matrix[9 ] = 0; this->matrix[10] = 1; this->matrix[11] = -1;
	this->matrix[12] = 0; this->matrix[13] = 0; this->matrix[14] = 0; this->matrix[15] = 0;
}

// annet stash

Vector Matrix::ProjectVector(Vector v){
	Vector temp;
	float w = (v.x*this->matrix[ 3] + v.y*this->matrix[ 7] + v.z*this->matrix[11]);

	temp.x = (v.x*this->matrix[ 0] + v.y*this->matrix[ 4] + v.z*this->matrix[ 8])/w;
	temp.y = (v.x*this->matrix[ 1] + v.y*this->matrix[ 5] + v.z*this->matrix[ 9])/w;
	temp.z = (v.x*this->matrix[ 2] + v.y*this->matrix[ 6] + v.z*this->matrix[10])/w;
	return temp;
}

Vector Matrix::TransformVector(Vector v){
	Vector temp;
	temp.x = v.x*this->matrix[ 0] + v.y*this->matrix[ 4] + v.z*this->matrix[ 8] + this->matrix[12];
	temp.y = v.x*this->matrix[ 1] + v.y*this->matrix[ 5] + v.z*this->matrix[ 9] + this->matrix[13];
	temp.z = v.x*this->matrix[ 2] + v.y*this->matrix[ 6] + v.z*this->matrix[10] + this->matrix[14];
	return temp;
}

Vector Matrix::RotateVector(Vector v){
	Vector temp;
	temp.x = v.x*this->matrix[ 0] + v.y*this->matrix[ 4] + v.z*this->matrix[ 8];
	temp.y = v.x*this->matrix[ 1] + v.y*this->matrix[ 5] + v.z*this->matrix[ 9];
	temp.z = v.x*this->matrix[ 2] + v.y*this->matrix[ 6] + v.z*this->matrix[10];
	return temp;
}

Vector Matrix::InverseRotateVector(Vector v){
	Vector temp;
	temp.x = v.x*this->matrix[ 0] + v.y*this->matrix[ 1] + v.z*this->matrix[ 2];
	temp.y = v.x*this->matrix[ 4] + v.y*this->matrix[ 5] + v.z*this->matrix[ 6];
	temp.z = v.x*this->matrix[ 8] + v.y*this->matrix[ 9] + v.z*this->matrix[10];
	return temp;
}

