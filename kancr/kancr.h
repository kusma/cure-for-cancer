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

#ifndef KANCR_H
#define KANCR_H

#define TARGET_SOFTWARE

#include "vector.h"

typedef struct{
	Vector point;
	Vector normal;
	float u, v;
	unsigned int color;
}Vertex;

typedef struct{
	int a, b, c;
	Vector normal;
}Face;

typedef struct{
	int width, height;
	unsigned int *data;
}Texture;

class Plane{
public:
	Vector normal;
	float distance;
};

class Fustrum{
public:
	Plane sides[4];
	Plane znear;
};

#include "plugin.h"
#include "vector.h"
#include "matrix.h"
#include "triangle.h"
#include "mesh.h"
#include "camera.h"
#include "object.h"
#include "light.h"
#include "scene.h"
#include "renderer.h"

#endif //KANCR_H
