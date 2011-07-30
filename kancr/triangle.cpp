
#include "kancr.h"
#include "../tool/misc.h"

#include "assert.h"

#define SHIFT 8
inline void hline( Bitmap* target, int x1, int x2, int y, int z1, int z2, unsigned int color, unsigned int* zbuffer ){
	//printf("line");
	assert(target);
	assert(zbuffer);

	if( x1==x2 ) return;

	if(y>239) return;
	assert(!(y>239));
	if(y<0) return;
	assert(!(y<0));

	if( x2 < x1 ){
		xchg( &x1, &x2 );
		xchg( &z1, &z2 );
	}

	if( x2<1 ) return;
	if( x1>target->GetWidth()-2 ) return;
	if( x1<1 ) x1 = 1;
	if( x2>target->GetWidth()-2 ) x2 = target->GetWidth()-2;
	if( x1==x2 ) return;

	int offset = y*target->GetWidth()+x1;

	int zdelta = (z2-z1)/(x2-x1);
	unsigned int z = z1+(zdelta>>1);
	unsigned int* bufferp = target->GetData()+offset;
	unsigned int* zbufferp = zbuffer+offset;
	for( int counter=x2-x1; counter; counter-- ){
		if( *zbufferp>(z>>SHIFT) ){
			*bufferp = color;
			*zbufferp = (z>>SHIFT);
		}
		z+=zdelta;
		zbufferp++;
		bufferp++;
	}
}

void DrawFlatTriangle( Bitmap* target, Vertex2D v[3], unsigned int color, unsigned int *zbuffer ){
	assert(target);

	int y, x1, x2, delta1, delta2;
	unsigned int z1, z2;
	int zdelta1, zdelta2;

	if(v[1].y > v[2].y) xchg<Vertex2D>(&v[1], &v[2]);
	if(v[0].y > v[2].y) xchg<Vertex2D>(&v[0], &v[2]);
	if(v[0].y > v[1].y) xchg<Vertex2D>(&v[0], &v[1]);

	if(v[0].y == v[2].y) return;

	if( (int)v[1].y > (int)v[0].y ){
		delta1 = (((int)v[2].x - (int)v[0].x)<<SHIFT) / ((int)v[2].y - (int)v[0].y);
		delta2 = (((int)v[1].x - (int)v[0].x)<<SHIFT) / ((int)v[1].y - (int)v[0].y);
		zdelta1 = ((int)(v[2].z - v[0].z)<<SHIFT) / ((int)v[2].y - (int)v[0].y);
		zdelta2 = ((int)(v[1].z - v[0].z)<<SHIFT) / ((int)v[1].y - (int)v[0].y);
		x1 = (int)v[0].x<<SHIFT;
		x2 = x1;
		z1 = (int)v[0].z<<SHIFT;
		z2 = z1;
		for( y=v[0].y; y<v[1].y; y++ ){
			hline(target, (x1>>SHIFT), (x2>>SHIFT), y, z1, z2, color, zbuffer);
			z1+=zdelta1;
			z2+=zdelta2;
			x1+=delta1;
			x2+=delta2;
		}
	}else{
		delta1 = (((int)v[2].x - (int)v[0].x)<<SHIFT) / ((int)v[2].y - (int)v[0].y);
		x1 = (int)v[0].x<<SHIFT;
		zdelta1 = ((int)(v[2].z - v[0].z)<<SHIFT) / ((int)v[2].y - (int)v[0].y);
		z1 = (int)v[0].z<<SHIFT;
	}

	if( (int)v[2].y > (int)v[1].y){
		delta2 = ((int)(v[2].x - (int)v[1].x)<<SHIFT) / ((int)v[2].y - (int)v[1].y);
		zdelta2 = (((int)v[2].z - (int)v[1].z)<<SHIFT) / ((int)v[2].y - (int)v[1].y);
		x2 = (int)v[1].x<<SHIFT;
		z2 = (int)v[1].z<<SHIFT;
		for( y=v[1].y; y<v[2].y; y++ ){
			hline(target, (x1>>SHIFT), (x2>>SHIFT), y, z1, z2, color, zbuffer);
			z1+=zdelta1;
			z2+=zdelta2;
			x1+=delta1;
			x2+=delta2;
		}
	}
}

inline void addhline( Bitmap* target, int x1, int x2, int y, int z1, int z2, unsigned int color, unsigned int* zbuffer ){
	assert(target);
	assert(zbuffer);

	if( x1==x2 ) return;

	if(y>239) return;
	assert(!(y>239));
	if(y<0) return;
	assert(!(y<0));

	if( x1==x2 ) return;
	if( x2 < x1 ){
		xchg( &x1, &x2 );
	}

	if( x2<1 ) return;
	if( x1>target->GetWidth()-2 ) return;
	if( x1<0 ) x1 = 0;
	if( x2>target->GetWidth()-1 ) x2 = target->GetWidth()-1;
	if( x1==x2 ) return;

	int offset = y*target->GetWidth()+x1;

	unsigned int* bufferp = target->GetData()+offset;
	for( int counter=x2-x1; counter; counter-- ){
		unsigned int col1 = *bufferp;
		_asm{
			movd mm2, col1
			movd mm3, col1

			punpcklbw mm2, mm5
			punpcklbw mm3, mm5

			pmullw mm2, mm0
			pmullw mm3, mm1
			psrlw mm2, 8
			psrlw mm3, 8
			paddusw mm2, mm3
			packuswb mm2, mm5
			movd col1, mm2

		}
		*bufferp = col1;
		bufferp++;
	}
}

void DrawAddFlatTriangle( Bitmap* target, Vertex2D v[3], unsigned int color, unsigned int *zbuffer ){
	assert(target);

	int y, x1, x2, delta1, delta2;
	unsigned int z1, z2;
	int zdelta1, zdelta2;

	if(v[1].y > v[2].y) xchg<Vertex2D>(&v[1], &v[2]);
	if(v[0].y > v[2].y) xchg<Vertex2D>(&v[0], &v[2]);
	if(v[0].y > v[1].y) xchg<Vertex2D>(&v[0], &v[1]);

	if(v[0].y == v[2].y) return;

	static unsigned int jall = 0x7f7f7f7f;

	_asm{
		pxor mm5, mm5
		movd mm0, color
		paddusb mm0, jall
		movd mm1, color
		psubusb mm1, jall
		punpcklbw mm0, mm5
		punpcklbw mm1, mm5
		psllw mm1, 1
		//paddusw mm1, mm0
	}

	if( (int)v[1].y > (int)v[0].y ){
		delta1 = (((int)v[2].x - (int)v[0].x)<<SHIFT) / ((int)v[2].y - (int)v[0].y);
		delta2 = (((int)v[1].x - (int)v[0].x)<<SHIFT) / ((int)v[1].y - (int)v[0].y);
		zdelta1 = ((int)(v[2].z - v[0].z)<<SHIFT) / ((int)v[2].y - (int)v[0].y);
		zdelta2 = ((int)(v[1].z - v[0].z)<<SHIFT) / ((int)v[1].y - (int)v[0].y);
		x1 = (int)v[0].x<<SHIFT;
		x2 = x1;
		z1 = (int)v[0].z<<SHIFT;
		z2 = z1;
		for( y=v[0].y; y<v[1].y; y++ ){
			addhline(target, (x1>>SHIFT), (x2>>SHIFT), y, z1, z2, color, zbuffer);
			z1+=zdelta1;
			z2+=zdelta2;
			x1+=delta1;
			x2+=delta2;
		}
	}else{
		delta1 = (((int)v[2].x - (int)v[0].x)<<SHIFT) / ((int)v[2].y - (int)v[0].y);
		x1 = (int)v[0].x<<SHIFT;
		zdelta1 = ((int)(v[2].z - v[0].z)<<SHIFT) / ((int)v[2].y - (int)v[0].y);
		z1 = (int)v[0].z<<SHIFT;
	}

	if( (int)v[2].y > (int)v[1].y){
		delta2 = ((int)(v[2].x - (int)v[1].x)<<SHIFT) / ((int)v[2].y - (int)v[1].y);
		zdelta2 = (((int)v[2].z - (int)v[1].z)<<SHIFT) / ((int)v[2].y - (int)v[1].y);
		x2 = (int)v[1].x<<SHIFT;
		z2 = (int)v[1].z<<SHIFT;
		for( y=v[1].y; y<v[2].y; y++ ){
			addhline(target, (x1>>SHIFT), (x2>>SHIFT), y, z1, z2, color, zbuffer);
			z1+=zdelta1;
			z2+=zdelta2;
			x1+=delta1;
			x2+=delta2;
		}
	}
	_asm emms;
}

void texture_hline( Bitmap* target, int x1, int x2, int y, int z1, int z2, int u1, int u2, int v1, int v2, unsigned int color, unsigned int* zbuffer, unsigned int* texture ){
	assert(target);
	assert(zbuffer);

	if( x1==x2 ) return;

	if(y>239) return;
	assert(!(y>239));
	if(y<0) return;
	assert(!(y<0));

	if( x2 < x1 ){
		xchg( &x1, &x2 );
		xchg( &z1, &z2 );
		xchg( &u1, &u2 );
		xchg( &v1, &v2 );
	}

	if( x2<1 ) return;
	if( x1>target->GetWidth()-2 ) return;
	if( x1<0 ) x1 = 0;
	if( x2>target->GetWidth()-1 ) x2 = target->GetWidth()-1;
	if( x1==x2 ) return;

	int offset = y*target->GetWidth()+x1;

	int precalc = 65535/(x2-x1);
	int zdelta = (z2-z1)/(x2-x1);
	int udelta = ((u2-u1)*precalc)>>16;
	int vdelta = ((v2-v1)*precalc)>>16;

	unsigned int z = z1+(zdelta>>1);
	unsigned int* bufferp = target->GetData()+offset;
	unsigned int* zbufferp = zbuffer+offset;

//	printf("y: %i, x1: %i, x2: %i, offset: %i\r", y, x1, x2, offset);
//	printf("\n");
//	assert(offset<(320*240));

	for( int counter=x2-x1; counter; counter-- ){
		if( *zbufferp>(z>>SHIFT) ){
			*bufferp = texture[((u1>>8)&0xFF)+(v1&0xFF00)]|0xFF000000;//u1>>SHIFT;
			*zbufferp = (z>>SHIFT);
		}
		u1+=udelta;
		v1+=vdelta;
		z+=zdelta;
		zbufferp++;
		bufferp++;
	}
}

void DrawTextureTriangle( Bitmap* target, Vertex2D v[3], unsigned int color, unsigned int* zbuffer, unsigned int* texture ){
	assert(target);

	int y, x1, x2, delta1, delta2;
	unsigned int z1, z2;
	int zdelta1, zdelta2;

	unsigned int u1, u2;
	int udelta1, udelta2;

	unsigned int v1, v2;
	int vdelta1, vdelta2;

	if(v[1].y > v[2].y) xchg<Vertex2D>(&v[1], &v[2]);
	if(v[0].y > v[2].y) xchg<Vertex2D>(&v[0], &v[2]);
	if(v[0].y > v[1].y) xchg<Vertex2D>(&v[0], &v[1]);

	if(v[0].y == v[2].y) return;

	if( (int)v[1].y > (int)v[0].y ){
		int precalc1 = 65535/((int)v[2].y - (int)v[0].y);
		int precalc2 = 65535/((int)v[1].y - (int)v[0].y);
		delta1 =  (((v[2].x - v[0].x)<<SHIFT) * precalc1)>>16;
		delta2 =  (((v[1].x - v[0].x)<<SHIFT) * precalc2)>>16;
		zdelta1 = ((int)(v[2].z - v[0].z)<<SHIFT) / ((int)v[2].y - (int)v[0].y);
		zdelta2 = ((int)(v[1].z - v[0].z)<<SHIFT) / ((int)v[1].y - (int)v[0].y);
		udelta1 = (((v[2].u - v[0].u)<<SHIFT) * precalc1)>>16;
		udelta2 = (((v[1].u - v[0].u)<<SHIFT) * precalc2)>>16;
		vdelta1 = (((v[2].v - v[0].v)<<SHIFT) * precalc1)>>16;
		vdelta2 = (((v[1].v - v[0].v)<<SHIFT) * precalc2)>>16;

		x1 = (int)v[0].x<<SHIFT;
		x2 = x1;
		z1 = (int)v[0].z<<SHIFT;
		z2 = z1;
		u1 = (int)v[0].u<<SHIFT;
		u2 = u1;
		v1 = (int)v[0].v<<SHIFT;
		v2 = v1;

		for( y=v[0].y; y<v[1].y; y++ ){
			texture_hline(target, (x1>>SHIFT), (x2>>SHIFT), y, z1, z2,u1,u2,v1,v2, color, zbuffer, texture);
			u1+=udelta1;
			u2+=udelta2;
			v1+=vdelta1;
			v2+=vdelta2;
			z1+=zdelta1;
			z2+=zdelta2;
			x1+=delta1;
			x2+=delta2;
		}
	}else{
		int precalc1 = 65535/((int)v[2].y - (int)v[0].y);
		delta1 =  (((v[2].x - v[0].x)<<SHIFT) * precalc1)>>16;
		zdelta1 = ((int)(v[2].z - v[0].z)<<SHIFT) / ((int)v[2].y - (int)v[0].y);
		udelta1 = (((v[2].u - v[0].u)<<SHIFT) * precalc1)>>16;
		vdelta1 = (((v[2].v - v[0].v)<<SHIFT) * precalc1)>>16;
		x1 = (int)v[0].x<<SHIFT;
		z1 = (int)v[0].z<<SHIFT;
		u1 = (int)v[0].u<<SHIFT;
		v1 = (int)v[0].v<<SHIFT;
	}

	if( (int)v[2].y > (int)v[1].y){
		int precalc2 = 65535/((int)v[2].y - (int)v[1].y);
		delta2 =  (((v[2].x - v[1].x)<<SHIFT)*precalc2)>>16;
		zdelta2 = (((int)v[2].z - (int)v[1].z)<<SHIFT) / ((int)v[2].y - (int)v[1].y);
		udelta2 = (((v[2].u - v[1].u)<<SHIFT)*precalc2)>>16;
		vdelta2 = (((v[2].v - v[1].v)<<SHIFT)*precalc2)>>16;

		x2 = (int)v[1].x<<SHIFT;
		z2 = (int)v[1].z<<SHIFT;
		u2 = (int)v[1].u<<SHIFT;
		v2 = (int)v[1].v<<SHIFT;
		for( y=v[1].y; y<v[2].y; y++ ){
			texture_hline(target, (x1>>SHIFT), (x2>>SHIFT), y, z1, z2,u1,u2,v1,v2, color, zbuffer, texture);
			u1+=udelta1;
			u2+=udelta2;
			v1+=vdelta1;
			v2+=vdelta2;
			z1+=zdelta1;
			z2+=zdelta2;
			x1+=delta1;
			x2+=delta2;
		}
	}
}

void DrawLine( unsigned int* buffer, unsigned int* zbuffer, Vertex2D v1, Vertex2D v2, unsigned int color ){
	assert(buffer);

	unsigned short white[4]={0xFF,0xFF,0xFF,0xFF};
	unsigned int mmx_shade[2] = {1,1};

	int color1, color2;
	int delta, zdelta;
	int x, y;
	int xlen = abs(v2.x-v1.x);
	int ylen = abs(v2.y-v1.y);

	if(v1.x>318)v1.x=318;
	if(v1.x<2)	v1.x=2;
	if(v2.x>318)v2.x=318;
	if(v2.x<2)	v2.x=2;
	if(v1.y>238)v1.y=238;
	if(v1.y<2)	v1.y=2;
	if(v2.y>238)v2.y=238;
	if(v2.y<2)	v2.y=2;

	_asm{
		emms;
		pxor mm5, mm5;
	}

	if(xlen>ylen){
		if(v1.x>v2.x) xchg<Vertex2D>(&v1, &v2);
		xlen = v2.x-v1.x;
		ylen = v2.y-v1.y;

		delta = 0;
		zdelta = 0;
		if(xlen!=0){
			delta = ((ylen<<8)/xlen);
			zdelta = ((ylen<<8)/xlen);
		}

		buffer += (v1.x-1)+v1.y*320;
		y=0;

		for(int i=xlen+1; i; i-- ){
			int shade = y&0xFF;
			int neg_shade = 255-shade;
			color1 = *buffer;
			color2 = *(buffer+320);

			_asm{
				mov eax, [shade]
				movzx bl, al
				shl ebx, 16
				add eax, ebx
				mov [mmx_shade], eax
				mov [mmx_shade+4], eax
				movq mm3, [mmx_shade]
				movq mm4, [white]
				psubusw mm4, mm3

				movd mm0, [color]
				punpcklbw mm0, mm5
				movd mm1, [color1]
				punpcklbw mm1, mm5

				psubw mm1, mm0
				pmullw mm1, mm3
				psllw mm0, 8
				paddw mm1, mm0
				psrlw mm0, 8
				psrlw mm1, 8
				packuswb mm1, mm1
				movd [color1], mm1

				movd mm1, [color2]
				punpcklbw mm1, mm5

				psubw mm1, mm0
				pmullw mm1, mm4
				psllw mm0, 8
				paddw mm1, mm0
				psrlw mm0, 8
				psrlw mm1, 8
				packuswb mm1, mm1
				movd [color2], mm1
			}

			*buffer = color1;
			*(buffer+320) = color2;

			buffer-=(y>>8)*320;
			y += delta;
			buffer+=(y>>8)*320+1;
		}

	}else{
		if(v1.y>v2.y) xchg<Vertex2D>(&v1, &v2);

		xlen = v2.x-v1.x;
		ylen = v2.y-v1.y;
		delta = 0;
		if(ylen!=0) delta = ((xlen<<8)/ylen);

		buffer += (v1.x-1)+v1.y*320;
		x=0;

		for(int i=ylen+1; i; i-- ){
			int shade = x&0xFF;
			int neg_shade = 255-shade;

			color1 = *buffer;
			color2 = *(buffer+1);
			_asm{
				mov eax, [shade]
				movzx bl, al
				shl ebx, 16
				add eax, ebx
				mov [mmx_shade], eax
				mov [mmx_shade+4], eax
				movq mm3, [mmx_shade]
				movq mm4, [white]
				psubusw mm4, mm3

				movd mm0, [color]
				punpcklbw mm0, mm5
				movd mm1, [color1]
				punpcklbw mm1, mm5

				psubw mm1, mm0
				pmullw mm1, mm3
				psllw mm0, 8
				paddw mm1, mm0
				psrlw mm0, 8
				psrlw mm1, 8
				packuswb mm1, mm1
				movd [color1], mm1

				movd mm1, [color2]
				punpcklbw mm1, mm5

				psubw mm1, mm0
				pmullw mm1, mm4
				psllw mm0, 8
				paddw mm1, mm0
				psrlw mm0, 8
				psrlw mm1, 8
				packuswb mm1, mm1
				movd [color2], mm1
			}

			*buffer = color1;
			*(buffer+1) = color2;

			buffer += 320;
			buffer -= x>>8;
			x += delta;
			buffer += x>>8;
		}
	}
	_asm emms;
}
