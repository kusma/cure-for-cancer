#include "kancr.h"
#include "../tool/misc.h"

#include <math.h>
#ifndef M_PI
#define M_PI 3.14159265358979f
#endif

class ClipFace{
public:
	int a, b, c;
};

class Poly{
public:
	int vertex_count;
	Vertex vertices[8];
	int face_count;
	ClipFace faces[6];
	void Triangulate();
};

void Clip( Poly* in, Poly* out, Plane* plane ){
	double curr_dot, next_dot;
	bool curr_in, next_in;

	out->vertex_count = 0;

	curr_dot = in->vertices[0].point * plane->normal;
	curr_in = (curr_dot>=plane->distance);

	for(int i=0; i<in->vertex_count; i++){
		Vertex* curr_vert = &in->vertices[i];
		Vertex* next_vert = &in->vertices[(i+1)%in->vertex_count];

		//legg bare til vertexen, den er inne
		if( curr_in ){
			out->vertices[out->vertex_count++] = *curr_vert;
		}

		next_dot = (next_vert->point*plane->normal);
		next_in = (next_dot>=plane->distance);

		// hvis en ute og en inne
		if( curr_in != next_in ){
			double scale =(plane->distance-curr_dot)/(next_dot-curr_dot);
			Vector vec = next_vert->point - curr_vert->point;
			vec *= scale;
			vec += curr_vert->point;

			Vertex v2;
			v2.point = vec;
			v2.u = curr_vert->u+(next_vert->u-curr_vert->u)*scale;
			v2.v = curr_vert->v+(next_vert->v-curr_vert->v)*scale;
			out->vertices[out->vertex_count++] = v2;
		}

		curr_dot = next_dot;
		curr_in = next_in;
	}
}

void Poly::Triangulate(){
	this->face_count=0;
	if(this->vertex_count==3){
		this->face_count=1;
		this->faces[0].a = 0;
		this->faces[0].b = 1;
		this->faces[0].c = 2;
	}

	else if(this->vertex_count>3){
		for(int i=1; i<(this->vertex_count-1); i++){
			this->faces[this->face_count].a = 0;
			this->faces[this->face_count].b = i;
			this->faces[this->face_count++].c = i+1;
		}
	}

}

void FlatTriangle::Draw( Camera* cam ){
	int i;
	unsigned int* target = cam->target->GetData();
	unsigned int* zbuffer = cam->zbuffer;
	int width = cam->target->GetWidth();
	int height = cam->target->GetHeight();

	Poly p_in, p_out;
	p_in.vertex_count = 3;
	p_in.vertices[0] = vertices[0];
	p_in.vertices[1] = vertices[1];
	p_in.vertices[2] = vertices[2];

	Clip( &p_in, &p_out, &cam->fustrum.sides[0] );
	Clip( &p_out, &p_in, &cam->fustrum.sides[1] );
	Clip( &p_in, &p_out, &cam->fustrum.sides[2] );
	Clip( &p_out, &p_in, &cam->fustrum.sides[3] );

	for(i=0; i<p_in.vertex_count; i++) p_in.vertices[i].point.z += 1.0f;
	Clip( &p_in, &p_out, &cam->fustrum.znear );
	for(i=0; i<p_in.vertex_count; i++) p_in.vertices[i].point.z -= 1.0f;

	Poly* pol = &p_out;
	pol->Triangulate();

	for(i=0; i<pol->face_count; i++){
		vertices2D[0] = cam->TransformVector( pol->vertices[pol->faces[i].a].point );
		vertices2D[1] = cam->TransformVector( pol->vertices[pol->faces[i].b].point );
		vertices2D[2] = cam->TransformVector( pol->vertices[pol->faces[i].c].point );
		DrawFlatTriangle( cam->target, vertices2D, color, zbuffer );
	}
}

void TextureTriangle::Draw( Camera* cam ){
	int i;
	unsigned int* target = cam->target->GetData();
	unsigned int* zbuffer = cam->zbuffer;
	int width = cam->target->GetWidth();
	int height = cam->target->GetHeight();

	Poly p_in, p_out;
	p_in.vertex_count = 3;
	p_in.vertices[0] = vertices[0];
	p_in.vertices[1] = vertices[1];
	p_in.vertices[2] = vertices[2];

	Clip( &p_in, &p_out, &cam->fustrum.sides[0] );
	Clip( &p_out, &p_in, &cam->fustrum.sides[1] );
	Clip( &p_in, &p_out, &cam->fustrum.sides[2] );
	Clip( &p_out, &p_in, &cam->fustrum.sides[3] );

	for(i=0; i<p_in.vertex_count; i++) p_in.vertices[i].point.z += 1.0f;
	Clip( &p_in, &p_out, &cam->fustrum.znear );
	for(i=0; i<p_in.vertex_count; i++) p_in.vertices[i].point.z -= 1.0f;

	Poly* pol = &p_out;
	pol->Triangulate();

	for(i=0; i<pol->face_count; i++){
		vertices2D[0] = cam->TransformVector( pol->vertices[pol->faces[i].a].point );
		vertices2D[0].u = pol->vertices[pol->faces[i].a].u*255;
		vertices2D[0].v = pol->vertices[pol->faces[i].a].v*255;
		vertices2D[1] = cam->TransformVector( pol->vertices[pol->faces[i].b].point );
		vertices2D[1].u = pol->vertices[pol->faces[i].b].u*255;
		vertices2D[1].v = pol->vertices[pol->faces[i].b].v*255;
		vertices2D[2] = cam->TransformVector( pol->vertices[pol->faces[i].c].point );
		vertices2D[2].u = pol->vertices[pol->faces[i].c].u*255;
		vertices2D[2].v = pol->vertices[pol->faces[i].c].v*255;
		DrawTextureTriangle( cam->target, vertices2D, color, zbuffer, material->texture );
	}
}

void AddflatTriangle::Draw( Camera* cam ){
	unsigned int* target = cam->target->GetData();
	unsigned int* zbuffer = cam->zbuffer;
	int width = cam->target->GetWidth();
	int height = cam->target->GetHeight();

	vertices2D[0] = cam->TransformVector( vertices[0].point );
	vertices2D[1] = cam->TransformVector( vertices[1].point );
	vertices2D[2] = cam->TransformVector( vertices[2].point );
	DrawAddFlatTriangle( cam->target, vertices2D, color, zbuffer );
}
/*

void AddflatTriangle::Draw( Camera* cam ){
	int i;
	unsigned int* target = cam->target->GetData();
	unsigned int* zbuffer = cam->zbuffer;
	int width = cam->target->GetWidth();
	int height = cam->target->GetHeight();

	Poly p_in, p_out;
	p_in.vertex_count = 3;
	p_in.vertices[0] = vertices[0];
	p_in.vertices[1] = vertices[1];
	p_in.vertices[2] = vertices[2];


	Poly* pol = &p_in;
	pol->Triangulate();

	for(i=0; i<pol->face_count; i++){
		vertices2D[0] = cam->TransformVector( pol->vertices[pol->faces[i].a].point );
		vertices2D[1] = cam->TransformVector( pol->vertices[pol->faces[i].b].point );
		vertices2D[2] = cam->TransformVector( pol->vertices[pol->faces[i].c].point );
		DrawAddFlatTriangle( cam->target, vertices2D, color, zbuffer );
	}
}
*/

void EnviromentTriangle::Draw( Camera* cam ){
	int i;
	unsigned int* target = cam->target->GetData();
	unsigned int* zbuffer = cam->zbuffer;
	int width = cam->target->GetWidth();
	int height = cam->target->GetHeight();

	Poly p_in, p_out;
	p_in.vertex_count = 3;
	p_in.vertices[0] = vertices[0];
	p_in.vertices[1] = vertices[1];
	p_in.vertices[2] = vertices[2];

	Clip( &p_in, &p_out, &cam->fustrum.sides[0] );
	Clip( &p_out, &p_in, &cam->fustrum.sides[1] );
	Clip( &p_in, &p_out, &cam->fustrum.sides[2] );
	Clip( &p_out, &p_in, &cam->fustrum.sides[3] );

	for(i=0; i<p_in.vertex_count; i++) p_in.vertices[i].point.z += 1.0f;
	Clip( &p_in, &p_out, &cam->fustrum.znear );
	for(i=0; i<p_in.vertex_count; i++) p_in.vertices[i].point.z -= 1.0f;

	Poly* pol = &p_out;
	pol->Triangulate();

	for(i=0; i<pol->face_count; i++){
		vertices2D[0] = cam->TransformVector( pol->vertices[pol->faces[i].a].point );
		vertices2D[0].u = pol->vertices[pol->faces[i].a].u*255;
		vertices2D[0].v = pol->vertices[pol->faces[i].a].v*255;
		vertices2D[1] = cam->TransformVector( pol->vertices[pol->faces[i].b].point );
		vertices2D[1].u = pol->vertices[pol->faces[i].b].u*255;
		vertices2D[1].v = pol->vertices[pol->faces[i].b].v*255;
		vertices2D[2] = cam->TransformVector( pol->vertices[pol->faces[i].c].point );
		vertices2D[2].u = pol->vertices[pol->faces[i].c].u*255;
		vertices2D[2].v = pol->vertices[pol->faces[i].c].v*255;

		DrawTextureTriangle( cam->target, vertices2D, color, zbuffer, material->envmap );
	}
}

void LineTriangle::Draw( Camera* cam ){
	int i;
	unsigned int* target = cam->target->GetData();
	unsigned int* zbuffer = cam->zbuffer;
	int width = cam->target->GetWidth();
	int height = cam->target->GetHeight();

	Poly p_in, p_out;
	p_in.vertex_count = 3;
	p_in.vertices[0] = vertices[0];
	p_in.vertices[1] = vertices[1];
	p_in.vertices[2] = vertices[2];

	Clip( &p_in, &p_out, &cam->fustrum.sides[0] );
	Clip( &p_out, &p_in, &cam->fustrum.sides[1] );
	Clip( &p_in, &p_out, &cam->fustrum.sides[2] );
	Clip( &p_out, &p_in, &cam->fustrum.sides[3] );

	for(i=0; i<p_in.vertex_count; i++) p_in.vertices[i].point.z += 1.0f;
	Clip( &p_in, &p_out, &cam->fustrum.znear );
	for(i=0; i<p_in.vertex_count; i++) p_in.vertices[i].point.z -= 1.0f;

	Poly* pol = &p_out;
	pol->Triangulate();
	for(i=0; i<pol->face_count; i++){
		vertices2D[0] = cam->TransformVector( pol->vertices[pol->faces[i].a].point );
		vertices2D[1] = cam->TransformVector( pol->vertices[pol->faces[i].b].point );
		vertices2D[2] = cam->TransformVector( pol->vertices[pol->faces[i].c].point );
		DrawLine( target, zbuffer, vertices2D[0], vertices2D[1], color );
		DrawLine( target, zbuffer, vertices2D[2], vertices2D[1], color );
		DrawLine( target, zbuffer, vertices2D[0], vertices2D[2], color );
	}
}
