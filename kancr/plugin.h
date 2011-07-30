#ifndef PLUGIN_H
#define PLUGIN_H

#include <windows.h>

class Mesh;

class MeshPlugin{
public:
	MeshPlugin(){
		vertex_list = NULL;
		owner = NULL;
	}
	~MeshPlugin(){
		if(vertex_list) delete[] vertex_list;
	}
	Attatch( Mesh* mesh );
	Mesh *owner;
	virtual void Apply()=0;
	Vertex *vertex_list;
};

#endif //PLUGIN_H