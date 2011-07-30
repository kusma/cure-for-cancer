#ifndef TUNNEL_H
#define TUNNEL_H

#include "../kancr/vector.h"
#include "../kancr/matrix.h"

Vector TraceRay( Vector origin, Vector direction );


class Tunnel{
private:

	class grid_node{
	public:
		int u;
		int v;
		unsigned char shade;
	};

	int width, height, lod;
	Vector* direction;
	Vector origin;
	Matrix matrix;
	/* må være 256x256 */
	int grid_width, grid_height;
	grid_node* grid;
	unsigned int* texture;

public:
	Tunnel( int width, int height, unsigned int* texture, int lod=8 );
	~Tunnel();
	void Rotate( Vector rotation );
	void Move( Vector position );
	void Render( unsigned int* buffer, int time );
};

#endif // TUNNEL_H
