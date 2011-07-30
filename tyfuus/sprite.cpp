#include "sprite.h"

Sprite::Sprite(){
};

Sprite::Sprite( Bitmap &source ){
	buffer = source.GetData();
	width = source.GetWidth();
	height = source.GetHeight();
	xpos = 0;
	ypos = 0;
}

void Sprite::Move( int x, int y ){
	this->xpos = x-(width>>1);
	this->ypos = y-(height>>1);
}
/*
void quad( unsigned int *buffer, int x1, int y1, int x2, int y2, int u1, int v1, int u2, int v2,  Bitmap& texture ){
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
*/
void Sprite::Blit( Bitmap &destination ){

	unsigned int* src = buffer;
	unsigned int* dst = destination.GetData();

	int line_offset=0;
	if(xpos<0){
		line_offset = abs(xpos);
		xpos = 0;
	}
	int line_clip = 0;
	if((xpos+width)>destination.GetWidth()){
		line_clip = (xpos+width)-destination.GetWidth();
		src -= line_clip;
		//line_offset -= line_clip;
	}
	int ycount = height;
	if(ypos<0){
		dst += abs(ypos)*destination.GetWidth();
		src += abs(ypos)*width;
		ycount -= abs(ypos);
	}
	if((ypos+height)>destination.GetHeight()){
		ycount -= (ypos+height)-destination.GetHeight();
	}

	dst += xpos+ypos*destination.GetWidth();
	__asm pxor mm5, mm5;

	int wcalc = (destination.GetWidth()-(width-line_offset));
	for(int y=ycount; y; y-- ){
		src += line_offset;
		src += line_clip;
		for(int x=width-line_offset-line_clip; x; x-- ){
			unsigned int old_color = *dst;
			unsigned int new_color = *src++;

			unsigned char a = new_color>>24;
			unsigned int alpha = (a<<24)|(a<<16)|(a<<8)|a;
			__asm{
				movd mm6, [alpha]
				punpcklbw mm6, mm5

				movd mm0, new_color
				punpcklbw mm0, mm5
				movd mm1, old_color
				punpcklbw mm1, mm5

				psubw mm0, mm1
				pmullw mm0, mm6
				psllw mm1, 8
				paddw mm0, mm1
				psrlw mm0, 8
				packuswb mm0, mm5
				
				movd new_color, mm0
			}
			*dst++ = new_color;
		}
		dst += line_clip;
		dst += wcalc;
	}
	_asm emms;
}

void Sprite::MulAddBlit( Bitmap &destination ){

	unsigned int* src = buffer;
	unsigned int* dst = destination.GetData();

	int line_offset=0;
	if(xpos<0){
		line_offset = abs(xpos);
		xpos = 0;
	}
	int line_clip = 0;
	if((xpos+width)>destination.GetWidth()){
		line_clip = (xpos+width)-destination.GetWidth();
		src -= line_clip;
		//line_offset -= line_clip;
	}
	int ycount = height;
	if(ypos<0){
		dst += abs(ypos)*destination.GetWidth();
		src += abs(ypos)*width;
		ycount -= abs(ypos);
	}
	if((ypos+height)>destination.GetHeight()){
		ycount -= (ypos+height)-destination.GetHeight();
	}

	dst += xpos+ypos*destination.GetWidth();
	__asm pxor mm5, mm5;

	int wcalc = (destination.GetWidth()-(width-line_offset));
	for(int y=ycount; y; y-- ){
		src += line_offset;
		src += line_clip;
		for(int x=width-line_offset-line_clip; x; x-- ){
			unsigned int old_color = *dst;
			unsigned int new_color = *src++;
			__asm{
				movd mm0, old_color
				punpcklbw mm0, mm5
				movd mm1, new_color
				punpcklbw mm1, mm5
				pmullw mm0, mm1
				psrlw mm0, 8
				packuswb mm0, mm0
				paddusb mm0, old_color
				movd new_color, mm0
			}
			*dst++ = new_color;
		}
		dst += line_clip;
		dst += wcalc;
	}
	_asm emms;
}


void Sprite::AddBlit( Bitmap &destination ){

	unsigned int* src = buffer;
	unsigned int* dst = destination.GetData();

	int line_offset=0;
	if(xpos<0){
		line_offset = abs(xpos);
		xpos = 0;
	}
	int line_clip = 0;
	if((xpos+width)>destination.GetWidth()){
		line_clip = (xpos+width)-destination.GetWidth();
		src -= line_clip;
		//line_offset -= line_clip;
	}
	int ycount = height;
	if(ypos<0){
		dst += abs(ypos)*destination.GetWidth();
		src += abs(ypos)*width;
		ycount -= abs(ypos);
	}
	if((ypos+height)>destination.GetHeight()){
		ycount -= (ypos+height)-destination.GetHeight();
	}

	dst += xpos+ypos*destination.GetWidth();
	__asm pxor mm5, mm5;

	int wcalc = (destination.GetWidth()-(width-line_offset));
	for(int y=ycount; y; y-- ){
		src += line_offset;
		src += line_clip;
		for(int x=width-line_offset-line_clip; x; x-- ){
			unsigned int old_color = *dst;
			unsigned int new_color = *src++;

			__asm{
				movd mm0, new_color
				paddusb mm0, old_color
				movd new_color, mm0
			}
			*dst++ = new_color;
		}
		dst += line_clip;
		dst += wcalc;
	}
	_asm emms;
}
