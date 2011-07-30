#include "kancr.h"
#include "mesh.h"

#include <math.h>
#ifndef M_PI
#define M_PI 3.14159265358979f
#endif

Mesh::Mesh(){
	this->face_count = 0;
	this->face_list = NULL;

	this->vertex_count = 0;
	this->current_vertex_list = this->vertex_list = NULL;

	this->material = NULL;
}

Mesh::~Mesh(){
	delete[] this->face_list;
	this->face_list = NULL;
	this->face_count = 0;

	delete[] this->vertex_list;
	this->current_vertex_list = this->vertex_list = NULL;
	this->vertex_count = 0;

	if(this->material!=NULL) delete this->material;
}

void Mesh::AddPlugin( MeshPlugin& plugin ){
	this->plugins.Add( plugin );
	plugin.Attatch(this);
}

void Mesh::SetMaterial( Material& material ){
	this->material = &material;
}

/*
// m = object matrix: 

// subtract object position first: 
x = camera->position.x - m[TX]; 
y = camera->position.y - m[TY]; 
z = camera->position.z - m[TZ]; 

// multiply by inverse of object matrix by doing a matrix/vector multiply in transpose order: 
pos.x = x*m[XX] + y*m[YX] + z*m[ZX]; 
pos.y = x*m[XY] + y*m[YY] + z*m[ZY]; 
pos.z = x*m[XZ] + y*m[YZ] + z*m[ZZ]; 
  
for(i=0; i<obj->num_faces; i++) { 
        v1 = face->v1; // index to vertex 1 on face 
        ex = pos.x - vertex[v1].x; 
        ey = pos.y - vertex[v1].y; 
        ez = pos.z - vertex[v1].z; 
        d = ex*normalF->x + ey*normalF->y + ez*normalF->z; 
        if(d>0.0) { 
            // flag face as visable: 
        } 
        face++; 
        normalF++; 
}
*/
/*
speile en vektor om en normal

t=-2.0f*vecDot(normal, origDir); 
direction.x = t*normal.x + origDir.x; 
direction.y = t*normal.y + origDir.y;
direction.z = t*normal.z + origDir.z;
*/

#define MAX_LIGHTS 20
void Mesh::Render(){
	this->current_vertex_list = this->vertex_list;
	this->plugins.Rewind();
	MeshPlugin* current = this->plugins.GetCurrent();
	while( current != NULL ){
		current->Apply();
		current = this->plugins.GetNext();
	}

	Scene* scene = this->owner->owner;

	assert(scene);
	assert(scene->active_camera);
	assert(this->material);

	Camera* cam = scene->active_camera;
	Matrix* object_matrix = &this->owner->matrix;

	Vector cam_pos = this->owner->matrix.InverseRotateVector( this->owner->owner->active_camera->position+this->owner->position );

	Light obj_lights[MAX_LIGHTS];
	int light_count=0;

	scene->light_list.Rewind();
	Light* current_light = scene->light_list.GetCurrent();
	while( (current_light!=NULL)||(light_count>=MAX_LIGHTS) ){
		obj_lights[light_count].color = current_light->color;
		obj_lights[light_count++].pos = this->owner->matrix.InverseRotateVector( current_light->pos );//.normalize();
		current_light = scene->light_list.GetNext();
	}
	
	Face* face = this->face_list;

	for(int i=this->face_count; i; i--){
		Vertex vertices[3];
		vertices[0] = this->current_vertex_list[face->a];
		vertices[1] = this->current_vertex_list[face->b];
		vertices[2] = this->current_vertex_list[face->c];

		Triangle* tri = NULL;
		Vector center = (this->current_vertex_list[face->a].point+this->current_vertex_list[face->b].point+this->current_vertex_list[face->c].point)*0.3333333f;

		static FlatTriangle flat_tri;
		static LineTriangle line_tri;
		static TextureTriangle texture_tri;
		static EnviromentTriangle env_tri;
		static AddflatTriangle addflat_tri;

		switch( this->material->render_flag ){
		case RENDER_FLAT:
			flat_tri = FlatTriangle( (Vertex*)vertices, center, this->material, face->normal );
			tri = &flat_tri;
		break;
		case RENDER_TEXTURE:
			texture_tri = TextureTriangle( (Vertex*)vertices, center, this->material, face->normal );
			tri = &texture_tri;
		break;
		case RENDER_ENVMAP:
			env_tri = EnviromentTriangle( (Vertex*)vertices, center, this->material, face->normal );
			tri = &env_tri;
		break;
		case RENDER_LINE:
			line_tri = LineTriangle( (Vertex*)vertices, center, this->material, face->normal );
			tri = &line_tri;
		break;
		case RENDER_ADDFLAT:
			addflat_tri = AddflatTriangle( (Vertex*)vertices, center, this->material, face->normal );
			tri = &addflat_tri;
		break;
		default:
			return;
		break;
		}

		Vector face_vector = cam_pos - center;

		if( (this->material->double_sided) || !tri->Cull( face_vector ) ){
			tri->Traslate( object_matrix );

			for(int j=0; j<light_count; j++){
				//printf("light: %x\n", obj_lights[j].color );
				tri->Lighten( &obj_lights[j] );
			}
			tri->Draw( cam );
		}
		*face++;
	}
}

void Mesh::GenerateNormals(){

	int i;
	Face* face = this->face_list;

	for(i=this->face_count; i; i--){
		Vertex v[3];
		v[0] = this->current_vertex_list[face->a];
		v[1] = this->current_vertex_list[face->b];
		v[2] = this->current_vertex_list[face->c];

		face->normal = Vector(
			(v[0].point.y-v[1].point.y)*(v[0].point.z-v[2].point.z)-(v[0].point.z-v[1].point.z)*(v[0].point.y-v[2].point.y),
			(v[0].point.z-v[1].point.z)*(v[0].point.x-v[2].point.x)-(v[0].point.x-v[1].point.x)*(v[0].point.z-v[2].point.z),
			(v[0].point.x-v[1].point.x)*(v[0].point.y-v[2].point.y)-(v[0].point.y-v[1].point.y)*(v[0].point.x-v[2].point.x)
			).normalize();

		*face++;
	}

	unsigned int pols[1024];
	memset(pols, 0, 1024*sizeof(unsigned int));

	Vertex* vertex = this->current_vertex_list;

	for(i=this->vertex_count; i; i--){
		vertex->normal = Vector(0,0,0);
		*vertex++;
	}

	//printf("%u\n", this->face_count);
	face = this->face_list;
	for(i=this->face_count; i; i--){
		this->current_vertex_list[face->a].normal += face->normal;
		this->current_vertex_list[face->b].normal += face->normal;
		this->current_vertex_list[face->c].normal += face->normal;
		pols[face->a]++;
		pols[face->b]++;
		pols[face->c]++;
		*face++;
	}

	vertex = this->current_vertex_list;
	for(i=0;i<this->vertex_count; i++){
		vertex->normal/=pols[i];
		*vertex++;
	}

}
