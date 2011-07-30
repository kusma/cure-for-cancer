#include "kancr.h"

#ifdef WIN32
#include <windows.h>
#endif

Scene::Scene(){
	this->active_camera = NULL;
}

Scene::~Scene(){
}

void Scene::SetCamera( Camera &camera ){
	assert(&camera);
	this->active_camera = &camera;
}

void Scene::AddObject( Object& object ){
	assert(&object);
	this->object_list.Add( object );
	object.owner = this;
}

void Scene::AddLight( Light& light ){
	assert(&light);
	this->light_list.Add( light );
}

void Scene::Render( float time ){
	if(this->active_camera==NULL) return;
	assert(this->active_camera);
	this->active_camera->UpdateMatrix();

	for( int i=0; i<( this->active_camera->target->GetWidth()*this->active_camera->target->GetHeight() ); i++ ) this->active_camera->zbuffer[i]=0xFFFFFFFF;

	this->object_list.Rewind();
	Object* current = this->object_list.GetCurrent();
	while( current != NULL ){
		current->Render();
		current = this->object_list.GetNext();
	}
}
