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

#ifndef OBJECT_H
#define OBJECT_H

#include "kancr.h"

class Scene;

class Object{
public:
	Scene* owner;
	Vector position;
	//Vector rotation;
	int meshcount;
	int polycount;
//	Mesh* mesh_list;
	LinkedList<Mesh> mesh_list;

	Matrix rotation;
	Matrix matrix;
	Matrix translation;
	Matrix scale;

	Object();
	~Object();
	void AddMesh( Mesh& mesh );
	void Rotate( Vector rotation );
	void Move( Vector position );
	void Scale( Vector scale );
	void Render();
};

#endif //OBJECT_H