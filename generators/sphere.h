#include <math.h>
#ifndef M_PI
#define M_PI 3.14159265358979f
#endif

class Sphere : public Object{
public:
	Sphere(int size, int lod){
		lod <<=1;
		Mesh* mesh = new Mesh();
		mesh->vertex_count = (lod+1)*(lod+1);
		mesh->vertex_list = new Vertex[mesh->vertex_count];
		mesh->current_vertex_list = mesh->vertex_list;
		mesh->face_count = (lod*lod)*2;
		mesh->face_list = new Face[mesh->face_count];

		int x,y;
		int index=0;
		for(y=0; y<(lod+1); y++)
			for(x=0; x<(lod+1); x++){
				mesh->vertex_list[index].point.x =	(float)sin(x*(M_PI/((lod+1)/2)))*(float)sin(y*(M_PI/lod))*size;
				mesh->vertex_list[index].point.y =	(float)cos(y*(M_PI/(lod)))*size;
				mesh->vertex_list[index].point.z =(float)cos(x*(M_PI/((lod+1)/2)))*(float)sin(y*(M_PI/lod))*size;
				mesh->vertex_list[index].u = (1.0f/(lod))*x;
				mesh->vertex_list[index].v = (1.0f/(lod))*y;
				index++;
			}
		index = 0;
		for(y=0; y<lod; y++)
			for(x=0; x<lod; x++){
				mesh->face_list[index].a = x+(y*(lod+1));
				mesh->face_list[index].b = x+((y+1)*(lod+1));
				mesh->face_list[index++].c = x+(y*(lod+1))+1;
				mesh->face_list[index].a = x+(y*(lod+1))+1;
				mesh->face_list[index].b = x+((y+1)*(lod+1));
				mesh->face_list[index++].c = x+((y+1)*(lod+1))+1;
			}
		this->AddMesh( *mesh );
		mesh->GenerateNormals();
		this->position= Vector(0, 0, 0);
		mesh->material = new Material;
		mesh->material->diffuse = 0xFF00FF;
		mesh->material->render_flag = RENDER_FLAT;
		mesh->material->double_sided  = true;

	}
};

/*
class Box : public Object
{
public:
	Box(float size){
		Mesh* mesh = new Mesh();
		mesh->vertex_count = 8;
		mesh->vertex_list = new Vertex[8];
		mesh->face_count = 12;
		mesh->face_list = new Face[12];
		mesh->vertex_list[0].point.x = -size;
		mesh->vertex_list[0].point.y = -size;
		mesh->vertex_list[0].point.z = -size;
		mesh->vertex_list[1].point.x =  size;
		mesh->vertex_list[1].point.y = -size;
		mesh->vertex_list[1].point.z = -size;
		mesh->vertex_list[2].point.x =  size;
		mesh->vertex_list[2].point.y =  size;
		mesh->vertex_list[2].point.z = -size;
		mesh->vertex_list[3].point.x = -size;
		mesh->vertex_list[3].point.y =  size;
		mesh->vertex_list[3].point.z = -size;
		mesh->vertex_list[4].point.x = -size;
		mesh->vertex_list[4].point.y = -size;
		mesh->vertex_list[4].point.z =  size;
		mesh->vertex_list[5].point.x =  size;
		mesh->vertex_list[5].point.y = -size;
		mesh->vertex_list[5].point.z =  size;
		mesh->vertex_list[6].point.x =  size;
		mesh->vertex_list[6].point.y =  size;
		mesh->vertex_list[6].point.z =  size;
		mesh->vertex_list[7].point.x = -size;
		mesh->vertex_list[7].point.y =  size;
		mesh->vertex_list[7].point.z =  size;
		mesh->face_list[0].a = 0;
		mesh->face_list[0].b = 3;
		mesh->face_list[0].c = 1;
		mesh->face_list[1].a = 1;
		mesh->face_list[1].b = 3;
		mesh->face_list[1].c = 2;
		mesh->face_list[2].a = 0;
		mesh->face_list[2].b = 4;
		mesh->face_list[2].c = 3;
		mesh->face_list[3].a = 7;
		mesh->face_list[3].b = 3;
		mesh->face_list[3].c = 4;
		mesh->face_list[4].a = 1;
		mesh->face_list[4].b = 2;
		mesh->face_list[4].c = 6;
		mesh->face_list[5].a = 1;
		mesh->face_list[5].b = 6;
		mesh->face_list[5].c = 5;
		mesh->face_list[6].a = 0;
		mesh->face_list[6].b = 1;
		mesh->face_list[6].c = 4;
		mesh->face_list[7].a = 1;
		mesh->face_list[7].b = 5;
		mesh->face_list[7].c = 4;
		mesh->face_list[8].a = 3;
		mesh->face_list[8].b = 7;
		mesh->face_list[8].c = 6;
		mesh->face_list[9].a = 2;
		mesh->face_list[9].b = 3;
		mesh->face_list[9].c = 6;
		mesh->face_list[10].a = 4;
		mesh->face_list[10].b = 6;
		mesh->face_list[10].c = 7;
		mesh->face_list[11].a = 4;
		mesh->face_list[11].b = 5;
		mesh->face_list[11].c = 6;
		mesh->current_vertex_list = mesh->vertex_list;
		this->AddMesh( mesh );
		this->position= Vector( 0, 0, 0 );
	}
};
*/
/*
class Plane : public Object{
public:
	Plane(int size){
		Mesh* mesh = new Mesh();
		mesh->vertex_count = (size+1)*(size+1);
		mesh->vertex_list = new Vertex[mesh->vertex_count];
		mesh->face_count = (size*size)*2;
		mesh->face_list = new Face[mesh->face_count];

		int x,y;
		int index=0;
		for(y=0; y<(size+1); y++)
			for(x=0; x<(size+1); x++){
				mesh->vertex_list[index].point.x =  x-((size+1)/2);
				mesh->vertex_list[index].point.y =  y-((size+1)/2);
				mesh->vertex_list[index++].point.z = 0;
			}
		index = 0;
		for(y=0; y<size; y++)
			for(x=0; x<size; x++){
				mesh->face_list[index].color.r = 0xFF;
				mesh->face_list[index].color.g = 0xFF*sin(x*0.1);
				mesh->face_list[index].color.b = 0xFF*sin(y*0.1);
				mesh->face_list[index].a = x+(y*(size+1));
				mesh->face_list[index].b = x+((y+1)*(size+1));
				mesh->face_list[index++].c = x+(y*(size+1))+1;

				mesh->face_list[index].color.r = 0xFF;
				mesh->face_list[index].color.g = 0xFF*sin(x*0.1);
				mesh->face_list[index].color.b = 0xFF*sin(y*0.1);
				mesh->face_list[index].a = x+(y*(size+1))+1;
				mesh->face_list[index].b = x+((y+1)*(size+1));
				mesh->face_list[index++].c = x+((y+1)*(size+1))+1;

			}
		this->position= Vector( 0, 0, -18.0f );
	}
};
*/
/*
class Plane : public Object{
public:
	Plane(int size){
		Mesh* mesh = new Mesh();
		mesh->vertex_count = (size+1)*(size+1);
		mesh->vertex_list = new Vertex[mesh->vertex_count];
		mesh->current_vertex_list = mesh->vertex_list;
		mesh->face_count = (size*size)*2;
		mesh->face_list = new Face[mesh->face_count];

		int x,y;
		int index=0;
		for(y=0; y<(size+1); y++)
			for(x=0; x<(size+1); x++){
				mesh->vertex_list[index].point.x = (float)(x-((size+1)>>1));
				mesh->vertex_list[index].point.y = (float)(y-((size+1)>>1));
				mesh->vertex_list[index++].point.z = 0;
			}
		index = 0;
		for(y=0; y<size; y++)
			for(x=0; x<size; x++){
				mesh->face_list[index].a = x+(y*(size+1));
				mesh->face_list[index].b = x+((y+1)*(size+1));
				mesh->face_list[index++].c = x+(y*(size+1))+1;

				mesh->face_list[index].a = x+(y*(size+1))+1;
				mesh->face_list[index].b = x+((y+1)*(size+1));
				mesh->face_list[index++].c = x+((y+1)*(size+1))+1;
			}
		this->AddMesh( mesh );
		this->position= Vector( 0, 0, -18.0f );
	}
};
*/
/*
class Plane : public Object{
public:
	Plane(int size){
		Mesh* mesh = new Mesh();
		mesh->vertex_count = (size+1)*(size+1);
		mesh->vertex_list = new Vertex[mesh->vertex_count];
		mesh->face_count = (size*size)*2;
		mesh->face_list = new Face[mesh->face_count];

		int x,y;
		int index=0;
		for(y=0; y<(size+1); y++)
			for(x=0; x<(size+1); x++){
				mesh->vertex_list[index].point.x =  x-((size+1)/2);
				mesh->vertex_list[index].point.y =  y-((size+1)/2);
				mesh->vertex_list[index++].point.z = 0;
			}
		index = 0;
		for(y=0; y<size; y++)
			for(x=0; x<size; x++){
				mesh->face_list[index].color.r = 0xFF;
				mesh->face_list[index].color.g = 0xFF*sin(x*0.1);
				mesh->face_list[index].color.b = 0xFF*sin(y*0.1);
				mesh->face_list[index].a = x+(y*(size+1));
				mesh->face_list[index].b = x+((y+1)*(size+1));
				mesh->face_list[index++].c = x+(y*(size+1))+1;

				mesh->face_list[index].color.r = 0xFF;
				mesh->face_list[index].color.g = 0xFF*sin(x*0.1);
				mesh->face_list[index].color.b = 0xFF*sin(y*0.1);
				mesh->face_list[index].a = x+(y*(size+1))+1;
				mesh->face_list[index].b = x+((y+1)*(size+1));
				mesh->face_list[index++].c = x+((y+1)*(size+1))+1;

			}
		this->position= Vector( 0, 0, -18.0f );
	}
};
*/
