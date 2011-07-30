#include "kancr.h"

#ifdef WIN32
#include <windows.h>
#endif

Object::Object(){
	this->matrix.Identity();
	this->rotation.Identity(); // = Vector(0,0,0);
	this->translation.Identity();
	this->scale.Identity();
}

Object::~Object(){
}

void Object::Rotate( Vector rotation ){
	this->rotation.Rotate( rotation );
}

void Object::Scale( Vector scale ){
	this->scale.Scale( scale );
}

void Object::Move( Vector position ){
	this->translation.Translate( position );
	this->position = position;
}

void Object::Render(){
	this->matrix.Identity();

	matrix *= this->owner->active_camera->matrix;
	matrix *= translation;
	matrix *= rotation;
	matrix *= scale; 


//	printf("rendering object\n");
	this->mesh_list.Rewind();
	Mesh* current = this->mesh_list.GetCurrent();
//	if(current==NULL) printf("no meshes\n");
	while( current != NULL ){
//		printf("render mesh\n");
		current->Render();
		current = this->mesh_list.GetNext();
	}
}

void Object::AddMesh( Mesh& mesh ){
	assert(&mesh);
	mesh.owner = this;
	this->mesh_list.Add(mesh);
	mesh.GenerateNormals();
}