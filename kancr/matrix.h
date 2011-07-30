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

#ifndef MATRIX_H
#define MATRIX_H

class Matrix{
public:
	float matrix[16];
	void Identity();
	void Rotate( Vector rotation );
	void Translate( Vector translation );
	void Scale( Vector scale );
	void Perspective( float fov, float aspect, float znear, float zfar );

	inline Matrix &operator*=(const Matrix m){
		float temp[16];
		for( int i=0; i<4; i++ )
			for ( int j=0; j<4; j++ )
				temp[i+(j<<2)] = this->matrix[i] * m.matrix[(j<<2)] + this->matrix[i+(1<<2)] * m.matrix[1+(j<<2)] +
								 this->matrix[i+(2<<2)] * m.matrix[2+(j<<2)] + this->matrix[i+(3<<2)] * m.matrix[3+(j<<2)];

		for( int counter=0; counter<(4*4); counter++ ) this->matrix[counter] = temp[counter];

		return *this;
	}

	Vector ProjectVector(Vector v);
	Vector TransformVector(Vector v);
	Vector RotateVector(Vector v);
	Vector InverseRotateVector(Vector v);
};


#endif //MATRIX_H