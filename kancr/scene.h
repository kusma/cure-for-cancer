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

#ifndef SCENE_H
#define SCENE_H

#include "kancr.h"
#include "../tool/linkedlist.h"

class Scene{
public:
	LinkedList<Camera> camera_list;
	Camera *active_camera;

	LinkedList<Object> object_list;
	LinkedList<Light> light_list;
	Scene();
	~Scene();

	void AddObject( Object &object );
	void AddLight( Light &light );
	void SetCamera( Camera &camera );
	void Render( float time );
};

#endif //SCENE_H