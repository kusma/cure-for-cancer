#ifndef M_PI
#define M_PI 3.1415396
#endif

// nyttige makroer

#define MAKEY(y) (((y)<<8)+((y)<<6)) // best hvis WIDTH == 320 
#define put_pixel(buffer,x,y,color) buffer[(x)+MAKEY((y))] = (color)
#define RGB(r,g,b) ((((r)&0xFF)<<16)|(((g)&0xFF)<<8)|((b)&0xFF))
#define GETR(c) (((c)&0xFF0000)>>16)
#define GETG(c) (((c)&0xFF00)>>8)
#define GETB(c) ((c)&0xFF)

void alphablend_layers(unsigned int* dest, unsigned int* source, int size){
	int counter;
	unsigned int old_color, new_color;
	while(size--){
		old_color = *dest;
		new_color = *source++;
		unsigned char alpha = new_color>>24;
		unsigned int rb = (
			(old_color&0x00FF00FF)*(255-alpha)
			+(new_color&0x00FF00FF)*(alpha)
			)>>8;
		unsigned int g = (
			(old_color&0x0000FF00)*(255-alpha)
			+(new_color&0x0000FF00)*(alpha)
			)>>8;
		*dest++ = (rb&0xff00ff)|(g&0xff00);
	}
}

void colorscale_layer(unsigned int* dest, int size, unsigned char alpha){
	int counter;
	unsigned int old_color;
	unsigned short blend[4]={alpha,alpha,alpha,alpha};
	
	__asm{
		movq mm7, [blend]
		pxor mm5, mm5
	};

	for(counter=0;counter<size;counter++){
		old_color = dest[counter];
		__asm{
			movd mm0, old_color
			punpcklbw mm0, mm5

			pmullw mm0, mm7
			psrlw mm0, 8

			psrlw mm5, 0xF
			packuswb mm0, mm5

			movd old_color, mm0
		}

		dest[counter] = old_color;
	}
	__asm emms;

}

void colorize_layer(unsigned int* dest, int size, unsigned int color){
	int counter;
	unsigned int old_color, new_color;
	
	__asm{
		pxor mm5, mm5
		movd mm7, [color]
		punpcklbw mm7, mm5
	};

	for(counter=0;counter<size;counter++){
		old_color = dest[counter];
		__asm{
			movd mm0, old_color
			punpcklbw mm0, mm5

			pmullw mm0, mm7
			psrlw mm0, 8

			psrlw mm5, 0xF
			packuswb mm0, mm5

			movd old_color, mm0
		}

		dest[counter] = old_color;
	}
	__asm emms;

}

#define pos_sine(deg, rot) ((1+sin((float)(deg)))*(float)(rot))
#define sine(deg, rot) ((sin((float)(deg)))*(float)(rot))

__inline float catmull_rom( float p1, float p2, float p3, float p4, float t)
{
	 float result = (float)(0.5 * ((-p1 + 3*p2 -3*p3 + p4)*t*t*t
			+ (2*p1 -5*p2 + 4*p3 - p4)*t*t
			+ (-p1+p3)*t
			+ 2*p2));
			
	return result;
}

// her kan jeg låne clipping-saker til sprites
void quad(unsigned int *buffer, int x1, int y1, int x2, int y2, int u1, int v1, int u2, int v2,  unsigned int *texture){
	int xstart, ystart, xend, yend, ustep, vstep, x, y;
	unsigned int u, v, ustart, vstart;
	xstart = x1;
	xend = x2;
	ystart = y1;
	yend = y2;
	ustart = u1<<8;
	vstart = v1<<8;

	if(x1==x2) return;
	if(y1==y2) return;

	if(x2<0) return;
	if(y2<0) return;
	if(x1>WIDTH) return;
	if(y1>HEIGHT) return;

	ustep = ((u2-u1)<<8)/(x2-x1);
	vstep = ((v2-v1)<<8)/(y2-y1);

	if(xstart<0){
		xstart=0;
		ustart = ustep*(-x1);
	}
	if(ystart<0){
		ystart=0;
		vstart = vstep*(-y1);
	}
	if(xend>WIDTH)xend = WIDTH;
	if(yend>HEIGHT)yend = HEIGHT;

	v = vstart;
	for(y=ystart; y<yend; y++ ){
		u = ustart;
		for(x=xstart; x<xend; x++ ){
			buffer[x+MAKEY(y)] = texture[((u>>8)&0xFF)+(v&0xFF00)];
			u+=ustep;
		}
		v+=vstep;
	}
}
