#include "kancr.h"
#include "camera.h"

#include "assert.h"

void Camera::SetupFustrum(){
	float angle_horizontal = (float)atan2( this->target->GetWidth()>>1,hpc)-0.0001f;
	float angle_vertical = (float)atan2( this->target->GetHeight()>>1,hpc)-0.0001f;
	float sh = (float)sin(angle_horizontal);
	float sv = (float)sin(angle_vertical);
	float ch = (float)cos(angle_horizontal);
	float cv = (float)cos(angle_vertical);

	fustrum.sides[0].normal = Vector(-ch,  0, -sh);
	fustrum.sides[0].distance = 0;
	fustrum.sides[1].normal = Vector( ch,  0, -sh);
	fustrum.sides[1].distance = 0;
	fustrum.sides[2].normal = Vector( 0, -cv, -sv);
	fustrum.sides[2].distance = 0;
	fustrum.sides[3].normal = Vector( 0,  cv, -sv);
	fustrum.sides[3].distance = 0;
	fustrum.znear.normal = Vector(0, 0, -1);
	fustrum.znear.distance = 0;//-(fustrum.znear.normal*Vector(1,1,-1));
}

Camera::Camera( Bitmap &target, Vector position, Vector look_at ){
	assert(&target);
	this->owner = NULL;

	this->target = &target;
	this->zbuffer = new unsigned int[target.GetWidth()*target.GetHeight()];
	assert(this->zbuffer);

	this->position = position;
	this->look_at = look_at;

	this->SetFov(90);

	translation.Identity();
	this->Move(position);
	rotation.Identity();
}

Camera::~Camera(){
	delete[] this->zbuffer;
};

void Camera::SetFov( float fov ){
	this->fov = fov;
	hpc = (target->GetWidth()>>1)/(float)tan((fov*0.5f)*(M_PI/180));
	this->SetupFustrum();
}

void Camera::Move( Vector position ){
	this->translation.Translate(position);
	this->position = position;
};

void Camera::UpdateMatrix(){
	matrix.Identity();
	matrix *= translation;
	matrix *= rotation;
}
