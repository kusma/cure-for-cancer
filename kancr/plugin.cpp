#include "kancr.h"

MeshPlugin::Attatch( Mesh* mesh ){
	assert(mesh);
	this->owner=mesh;
	//if(this->vertex_list)
	delete[] this->vertex_list;
	this->vertex_list = new Vertex[mesh->vertex_count];
	assert(this->vertex_list);
}