/*
 * HIW (Hyper Ineffective World) Engine
 *
 * If you are not an excess coder, you should not have this file.
 * Not even our musicians are allowed to have this file, beacuse of
 * GayProtection(tm).
 *
 * Now, piss off!
 *
 */

#ifndef CAMERA_H
#define CAMERA_H

#include "kancr.h"
#include "../tyfuus/tyfuus.h"

#include <math.h>
#ifndef M_PI
#define M_PI 3.14159265358979f
#endif

class Scene;

class Camera {
	float hpc;
	float fov;
	Scene* owner;
public:
	Fustrum fustrum;
	Bitmap* target;
	unsigned int* zbuffer;
	Matrix matrix;
	Matrix rotation;
	Matrix translation;
	Vector position, look_at;

	Camera( Bitmap& target, Vector position, Vector look_at );
	~Camera();
	void Move( Vector position );

	void SetTarget( Bitmap& target ){
		this->target = &target;
	}

	void SetFov( float fov );
	void UpdateMatrix();
	void SetupFustrum();

	inline Vertex2D TransformVector( Vector v ){
		Vertex2D temp;
	
		float muller = 1.0f/(v.z);
		temp.x = (target->GetWidth()>>1)+(int)(hpc*v.x*muller);
		temp.y = (target->GetHeight()>>1)+(int)(hpc*v.y*muller);
		temp.z = (unsigned int)(0xFFFFFF*muller);

		return temp;
	}
};

#endif //CAMERA_H
