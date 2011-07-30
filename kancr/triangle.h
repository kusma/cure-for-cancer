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

#ifndef TRIANGLE_H
#define TRIANGLE_H

typedef struct{
	int u, v;
	int x, y;
	unsigned int z;
	unsigned int color;
}Vertex2D;

class Bitmap;

//void DrawFlatTriangle( unsigned int* buffer, int width, int height, Vertex2D v[3], unsigned int color );
void DrawFlatTriangle( Bitmap* target, Vertex2D v[3], unsigned int color, unsigned int *zbffer );
void DrawAddFlatTriangle( Bitmap* target, Vertex2D v[3], unsigned int color, unsigned int *zbffer );
void DrawTextureTriangle( Bitmap* target, Vertex2D v[3], unsigned int color, unsigned int* zbuffer, unsigned int* texture );
//void DrawLine( unsigned int* buffer, unsigned int* zbuffer, int x1, int y1, int x2, int y2, unsigned int color );
void DrawLine( unsigned int* buffer, unsigned int* zbuffer, Vertex2D v1, Vertex2D v2, unsigned int color );

#endif //TRIANGLE_H