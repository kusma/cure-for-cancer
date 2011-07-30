#ifndef RENDERER_H
#define RENDERER_H

#include "kancr.h"
#include "../tool/sortlist.h"
//#include "../tyfuus/tyfuus.h"
#include <assert.h>

#include <math.h>
#ifndef M_PI
#define M_PI 3.14159265358979f
#endif

class Poly;

class Triangle{
protected:
	Vector normal;
	Vector center;
	Vertex* vertices;
	Material* material;

	unsigned int face_count;
	Vertex rotated_vertices[3*8];
	Vertex2D vertices2D[3];

public:
	Triangle(){};
	virtual inline bool Cull( Vector face_vector ){
		if( face_vector*normal > 0.0f) return true;
		return false;
	}

	virtual void Lighten( Light* light ){
		
	};

	void Triangulate(Poly* poly);

	virtual inline void Traslate( Matrix* matrix ){
		vertices[0].point = matrix->TransformVector( vertices[0].point );
		vertices[1].point = matrix->TransformVector( vertices[1].point );
		vertices[2].point = matrix->TransformVector( vertices[2].point );
	}

	virtual void Draw( Camera* cam ){

	};
};
/*
class Renderer{
private:
	Scene* owner;
	Vertex* vertex_list;
	Face* face_list;
	RadixSorter<Triangle>* sorter;
public:
	Renderer();
	~Renderer();
	void AddFace();
};
*/

// POLYFILLERS

class LineTriangle : public Triangle{
protected:
	unsigned int color;
public:
	LineTriangle(){};
	inline LineTriangle( Vertex vertices[3], Vector center, Material* material, Vector normal ){
		this->color = 0;
		this->vertices = vertices;
		this->material = material;
		this->normal = normal;
		this->center = center;
		this->face_count = 1;
	}
	virtual bool Cull( Vector face_vector ){
		return false;
	}

	inline void Lighten( Light* light ){
		color = material->diffuse|0xFF000000;
	}
	void Draw( Camera* cam );
};


class FlatTriangle : public Triangle{
protected:
	unsigned int color;
public:
	FlatTriangle(){};
	inline FlatTriangle( Vertex vertices[3], Vector center, Material* material, Vector normal ){
		assert(material);
		this->color = 0;
		this->vertices = vertices;
		this->material = material;
		this->normal = normal;
		this->center = center;
		this->face_count = 1;
	}

	virtual void Lighten( Light* light ){
		Vector m_light = light->pos-center;
		float dot = normal*m_light;
		if(dot>0.0f){
			unsigned char shade = (unsigned char)(((M_PI/2)-acos(dot))*(1.0f/M_PI*2*255.0f));
			unsigned int mask = (shade<<16)|(shade<<8)|shade;
			unsigned int d = material->diffuse;
			unsigned int c = color;
			unsigned int l = light->color;

			_asm{
				pxor mm5, mm5
				movd mm0, l
				punpcklbw mm0, mm5
				movd mm1, d
				punpcklbw mm1, mm5
				movd mm2, mask
				punpcklbw mm2, mm5
				pmullw mm0, mm2
				psrlw mm0, 8
				pmullw mm0, mm1
				psrlw mm0, 8
				packuswb mm0, mm0
				movd mm1, c
				paddusb mm0, mm1
				movd c, mm0
				emms
			}
			color = c|0xFF000000;


			// ut = inn / avstand^2
			//v[0].point.magnitude
			//float m = (light-((v[0].point+v[1].point+v[3].point)*0.333f)).magnitude();
			//printf("%f", m);
			//if(m>0.0f) shade = shade / (light-((v[0].point+v[1].point+v[3].point)*0.333f)).magnitude();
		}
		//else color = 0xFF;

	}

	void Draw( Camera* cam );
};

class AddflatTriangle:public FlatTriangle{
public:
	AddflatTriangle(){};
	AddflatTriangle( Vertex vertices[3], Vector center, Material* material, Vector normal ){
		assert(material);
		this->color = 0;
		this->vertices = vertices;
		this->material = material;
		this->normal = normal;
		this->center = center;
		this->face_count = 1;
	}

	void Draw( Camera* cam );

	inline bool Cull( Vector face_vector ){
		return false;
	}

};

class TextureTriangle : public FlatTriangle{
public:
	TextureTriangle(){};

	inline TextureTriangle( Vertex vertices[3], Vector center, Material* material, Vector normal ){
		this->color = 0;
		this->vertices = vertices;
		this->material = material;
		this->normal = normal;
		this->center = center;
		this->face_count = 1;
	}

	void Draw( Camera* cam );
};

class EnviromentTriangle : public TextureTriangle{
public:
	EnviromentTriangle(){};

	inline EnviromentTriangle( Vertex vertices[3], Vector center, Material* material, Vector normal ){
		this->color = 0;
		this->vertices = vertices;
		this->material = material;
		this->normal = normal;
		this->center = center;
		this->face_count = 1;
	}
	inline void Traslate( Matrix* matrix ){
		vertices[0].point = matrix->TransformVector( vertices[0].point );
		vertices[1].point = matrix->TransformVector( vertices[1].point );
		vertices[2].point = matrix->TransformVector( vertices[2].point );

		vertices[0].normal = matrix->RotateVector( vertices[0].normal ).normalize();
		vertices[1].normal = matrix->RotateVector( vertices[1].normal ).normalize();
		vertices[2].normal = matrix->RotateVector( vertices[2].normal ).normalize();
		vertices[0].u = 0.5f+(vertices[0].normal.x)*0.5f;
		vertices[0].v = 0.5f-(vertices[0].normal.y)*0.5f;
		vertices[1].u = 0.5f+(vertices[1].normal.x)*0.5f;
		vertices[1].v = 0.5f-(vertices[1].normal.y)*0.5f;
		vertices[2].u = 0.5f+(vertices[2].normal.x)*0.5f;
		vertices[2].v = 0.5f-(vertices[2].normal.y)*0.5f;

	}
	void Draw( Camera* cam );
};

#endif //RENDERER_H