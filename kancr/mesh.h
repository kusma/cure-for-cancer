#ifndef MESH_H
#define MESH_H

#include "kancr.h"
#include <stdio.h>

#include "../tool/linkedlist.h"


class Object;

#define RENDER_FLAT 1
#define RENDER_LINE 2
#define RENDER_GOURARD 3
#define RENDER_TEXTURE 4
#define RENDER_ENVMAP 5
#define RENDER_ADDFLAT 6

class Material{
public:
	unsigned int render_flag;
	bool double_sided;

	unsigned int color;

	unsigned int diffuse;
	unsigned int diffuse_color;

	unsigned int specular;
	unsigned int ambient;
	unsigned int* texture;
	unsigned int* envmap;
};

class Mesh{
public:
	Mesh();
	~Mesh();
	void Render();
	void GenerateNormals();
	void AddPlugin( MeshPlugin& plugin );
	void SetMaterial( Material& material );

	Object* owner;

	Material* material;

	LinkedList<MeshPlugin> plugins;
	int vertex_count;
	Vertex *vertex_list;
	Vertex *current_vertex_list;
	int face_count;
	Face *face_list;
};

#endif //SCENE_H