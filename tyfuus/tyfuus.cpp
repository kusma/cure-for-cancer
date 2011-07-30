#include <windows.h>
#include "tyfuus.h"

Bitmap::Bitmap(){
	this->data = NULL;
	this->width = 0;
	this->height = 0;
}

Bitmap::Bitmap( int width, int height ){
	this->width = width;
	this->height = height;
	this->data = new unsigned int[width*height];
}

Bitmap::~Bitmap(){
	delete[] this->data;
}

void Bitmap::Blit( Bitmap &destination ){
	unsigned int* src = data;
	unsigned int* dst = destination.data;
	memcpy( dst, src, width*height*sizeof(unsigned int));
}


void Bitmap::BammBlit( Bitmap &destination, int amount ){
	if((amount+1)==0){
		Bitmap::Blit( destination );
		return;
	}
	unsigned int* src = data;
	unsigned int* dst = destination.data;
	//int offset = -10+sin(time);
	for( int i=height; i; i-- ){
		int offset = (rand()%(amount+1))+(amount>>1);
		if(offset<0){
			memcpy( dst, (src-offset), (width+offset)*sizeof(unsigned int));
			dst += width+offset;
			memset( dst, 0, -(offset*4));
			dst -= offset;
		}else{
			memset( dst, 0, offset*4 );
			dst += offset;
			memcpy( dst, src, (width-offset)*sizeof(unsigned int));
			dst += width-offset;
		}
		src += width;
	}
}


void Bitmap::MulBlit( Bitmap &destination ){
	unsigned int* src = data;
	unsigned int* dst = destination.data;
	int max_x = (width<destination.width) ? width : destination.width;
	int max_y = (height<destination.height) ? height : destination.height;
	int count = max_x*max_y;
	_asm{
		pxor mm3, mm3
		mov ecx, count
		mov edi, dst
		mov esi, src
ALIGN 16
jump_here:
		lodsd
		movd mm0, eax
		movd mm1, [edi]
		punpcklbw mm0, mm3
		punpcklbw mm1, mm3
		pmullw mm0, mm1
		psrlw mm0, 8
		packuswb mm0, mm0
		movd eax, mm0
		stosd
		loop jump_here

	}
	__asm emms;
}

void Bitmap::AddBlit( Bitmap &destination ){
	unsigned int* src = data;
	unsigned int* dst = destination.data;
	int max_x = (width<destination.width) ? width : destination.width;
	int max_y = (height<destination.height) ? height : destination.height;
	for( int counter=max_y*max_x; counter; counter-- ){
		int old_color = *src++;
		int new_color = *dst;
		_asm{
			movd mm0, old_color
			paddusb mm0, new_color
			movd old_color, mm0
		}
		*dst++ = old_color;
	}
	__asm emms;
}

void Bitmap::SubBlit( Bitmap &destination ){
	unsigned int* src = data;
	unsigned int* dst = destination.data;
	int max_x = (width<destination.width) ? width : destination.width;
	int max_y = (height<destination.height) ? height : destination.height;
	for( int counter=max_y*max_x; counter; counter-- ){
		int old_color = *src++;
		int new_color = *dst;
		_asm{
			movd mm0, old_color
			psubusb mm0, new_color
			movd old_color, mm0
		}
		*dst++ = old_color;
	}
	__asm emms;
}

void Bitmap::Blend( Bitmap &destination, unsigned char alpha ){
	unsigned int* src = data;
	unsigned int* dst = destination.data;
	int max_x = (width<destination.width) ? width : destination.width;
	int max_y = (height<destination.height) ? height : destination.height;

	unsigned short blend[4]={0xFF-alpha,0xFF-alpha,0xFF-alpha,0xFF-alpha};

	__asm{
		movq mm6, [blend]
		pxor mm5, mm5
	};

	for( int counter=max_y*max_x; counter; counter-- ){
		int old_color = *src++;
		int new_color = *dst;
		__asm{
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
	__asm emms;
}

#include <stdio.h>

void Bitmap::AlphaBlend( Bitmap &destination ){
	unsigned int* src = data;
	unsigned int* dst = destination.data;
	int max_x = (width<destination.width) ? width : destination.width;
	int max_y = (height<destination.height) ? height : destination.height;

	static unsigned short white[4]={0xFF,0xFF,0xFF,0xFF};

	__asm{
		pxor mm5, mm5
	};

	for( int counter=max_y*max_x; counter; counter-- ){
		int old_color = *src++;
		int new_color = *dst;

		unsigned char a = old_color>>24;
		unsigned int alpha = (a<<24)|(a<<16)|(a<<8)|a;

		__asm{
			movd mm6, [alpha]
			punpcklbw mm6, mm5

			movd mm0, old_color
			punpcklbw mm0, mm5
			movd mm1, new_color
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
	__asm emms;
}


void Bitmap::Treshold(unsigned char level){
	unsigned int* ptr = data;
	int count = width*height;
	while(count--){
		int shade = *ptr;
		shade = ((shade&0xFF0000)>>16)+((shade&0xFF00)>>8)+(shade&0xFF);
		shade /= 3;
		if( shade<level ) *ptr++ = 0;
		else *ptr++ = 0xFFFFFF;
	}
}

void Bitmap::Invert(){
	unsigned int* ptr = data;
	int count = width*height;
	while(count--){
		*ptr = ~*ptr;
		ptr++;
	}
}

void Bitmap::Clear( unsigned int color ){
	memset( data, 0, sizeof(unsigned int)*width*height);
}

void Bitmap::HorisontalBlur( Bitmap &target, unsigned char amount ){
	if(amount==0){
		memcpy( target.data, data, sizeof(unsigned int)*width*height );
		return;
	}

	unsigned int* head = data;
	unsigned int* tail = data;
	unsigned int* dst = target.data;
	//short mul = (int)((1.0f/(amount*2))*255.0f);
	short mul = 0xFF/(amount*2);
	unsigned short muller[4]={mul,mul,mul,mul};
	unsigned int color;
	int x;
	int w = this->width;
	int h = this->height;

	_asm{
		pxor mm2, mm2;
		movq mm4, muller
	}
	for(int y=h; y; y--){
		// cleare bufferet
		_asm pxor mm0, mm0;
		// lese hodet
		for( x=amount; x; x-- ){
			color = *head++;
			_asm{
				movd mm1, color
				punpcklbw mm1, mm2
				paddusw mm0, mm1
			}
		}
		//lese kvasi-halen (første pixel repitert)
		for( x=amount; x; x-- ){
			color = *tail;
			_asm{
				movd mm1, color
				punpcklbw mm1, mm2
				paddusw mm0, mm1
			}
		}
		//tegn halen
		for( x=amount; x; x-- ){
			_asm{
				movq mm1, mm0
				pmullw mm1, mm4
				psrlw mm1, 8
				packuswb mm1, mm2
				movd color, mm1
			}
			*dst++ = color;
			color = *head++;
			_asm{
				movq mm1, color
				punpcklbw mm1, mm2
				paddusw mm0, mm1
			}
			color = *tail;
			_asm{
				movq mm1, color
				punpcklbw mm1, mm2
				psubusw mm0, mm1
			}
		}
		//tegn "kroppen"
		for( x=(w-(amount<<1)); x; x-- ){
			_asm{
				movq mm1, mm0
				pmullw mm1, mm4
				psrlw mm1, 8
				packuswb mm1, mm2
				movd color, mm1
			}
			*dst++ = color;
			color = *head++;
			_asm{
				movq mm1, color
				punpcklbw mm1, mm2
				paddusw mm0, mm1
			}
			color = *tail++;
			_asm{
				movq mm1, color
				punpcklbw mm1, mm2
				psubusw mm0, mm1
			}
		}
		//tegn hodet
		*head--;
		for( x=amount; x; x-- ){
			_asm{
				movq mm1, mm0
				pmullw mm1, mm4
				psrlw mm1, 8
				packuswb mm1, mm2
				movd color, mm1
			}
			*dst++ = color;
			color = *head;
			_asm{
				movq mm1, color
				punpcklbw mm1, mm2
				paddusw mm0, mm1
			}
			color = *tail++;
			_asm{
				movq mm1, color
				punpcklbw mm1, mm2
				psubusw mm0, mm1
			}
		}
		*head++;
		tail+=amount;
	}
	_asm emms;
}

void Bitmap::VerticalBlur( Bitmap &target, unsigned char amount ){
	if(amount==0){
		memcpy( target.data, data, sizeof(unsigned int)*width*height );
		return;
	}

	unsigned int* head = data;
	unsigned int* tail = data;
	unsigned int* dst = target.data;

	short mul = (int)((1.0f/(amount*2))*255.0f);
	unsigned short muller[4]={mul,mul,mul,mul};
	unsigned int color;
	register int w = width;
	int h = this->height;
	int y;

	_asm{
		pxor mm2, mm2;
		movq mm4, muller
	}
	for(int x=w; x; x--){
		dst = &target.data[w-x];
		head = &this->data[w-x];
		tail = &this->data[w-x];

		// cleare bufferet
		_asm pxor mm0, mm0;
		// lese hodet
		for( y=amount; y; y-- ){
			color = *head;
			head += w;
			_asm{
				movd mm1, color
				punpcklbw mm1, mm2
				paddusw mm0, mm1
			}
		}
		//lese kvasi-halen (første pixel repitert)
		for( y=amount; y; y-- ){
			color = *tail;
			_asm{
				movd mm1, color
				punpcklbw mm1, mm2
				paddusw mm0, mm1
			}
		}
		//tegn halen
		for( y=amount; y; y-- ){
			_asm{
				movq mm1, mm0
				pmullw mm1, mm4
				psrlw mm1, 8
				packuswb mm1, mm2
				movd color, mm1
			}
			*dst = color;
			dst += w;
			color = *head;
			head += w;
			_asm{
				movq mm1, color
				punpcklbw mm1, mm2
				paddusw mm0, mm1
			}
			color = *tail;
			_asm{
				movq mm1, color
				punpcklbw mm1, mm2
				psubusw mm0, mm1
			}
		}
		//tegn "kroppen"
		for( y=(h-(amount<<1)); y; y-- ){
			_asm{
				movq mm1, mm0
				pmullw mm1, mm4
				psrlw mm1, 8
				packuswb mm1, mm2
				movd color, mm1
			}
			*dst = color;
			dst += w;
			color = *head;
			head += w;
			_asm{
				movq mm1, color
				punpcklbw mm1, mm2
				paddusw mm0, mm1
			}
			color = *tail;
			tail += w;
			_asm{
				movq mm1, color
				punpcklbw mm1, mm2
				psubusw mm0, mm1
			}
		}
		head-=w;
		//tegn hodet
		for( y=amount; y; y-- ){
			_asm{
				movq mm1, mm0
				pmullw mm1, mm4
				psrlw mm1, 8
				packuswb mm1, mm2
				movd color, mm1
			}
			*dst = color;
			dst += w;
			color = *head;
			_asm{
				movq mm1, color
				punpcklbw mm1, mm2
				paddusw mm0, mm1
			}
			color = *tail;
			tail += w;
			_asm{
				movq mm1, color
				punpcklbw mm1, mm2
				psubusw mm0, mm1
			}
		}
		head += w;
	}
	_asm emms;
}

#define BLUR_OPACITY 230
void Bitmap::AddRadialBlur( int xpos, int ypos, float strength ){
	int x,y;
	unsigned int old_pixel;
	unsigned int new_pixel;
	int newx, newy;
	unsigned int prediv = 4096/(1.0f+strength);
	unsigned int* buffer = data;
	int yoffset = ypos*width;
	unsigned short blend[4]={BLUR_OPACITY,BLUR_OPACITY,BLUR_OPACITY,BLUR_OPACITY};
	static unsigned short white[4] = {0xFF,0xFF,0xFF,0xFF};

	__asm{
		movq mm6, [blend]
		pxor mm5, mm5
	};
	for(y=ypos; y>-1; y--){
		newy = (((ypos-y)<<8)*prediv)>>12;
		for(x=xpos; x>-1; x--){
			newx = (((xpos-x)<<8)*prediv)>>12;
			old_pixel = buffer[x+yoffset];
			new_pixel = GetSubPixel( (xpos<<8)-newx , (ypos<<8)-newy );
			__asm{
				movd mm0, new_pixel
				punpcklbw mm0, mm5
				movd mm1, old_pixel
				punpcklbw mm1, mm5
				pmullw mm0, mm6
				psrlw mm0, 8
				paddw mm0, mm1
				packuswb mm0, mm0
				movd new_pixel, mm0
			}
			buffer[x+yoffset] = new_pixel;
		}
		for(x=xpos+1; x<width; x++){
			newx = (((xpos-x)<<8)*prediv)>>12;
			old_pixel = buffer[x+yoffset];
			new_pixel = GetSubPixel( (xpos<<8)-newx , (ypos<<8)-newy );
			__asm{
				movd mm0, new_pixel
				punpcklbw mm0, mm5
				movd mm1, old_pixel
				punpcklbw mm1, mm5
				pmullw mm0, mm6
				psrlw mm0, 8
				paddw mm0, mm1
				packuswb mm0, mm0
				movd new_pixel, mm0
			}
			buffer[x+yoffset] = new_pixel;
		}
		yoffset -= width;
	}

	yoffset = (ypos+1)*width;
	for(y=ypos+1; y<height; y++){
		newy = (((ypos-y)<<8)*prediv)>>12;
		for(x=xpos; x>-1; x--){
			newx = (((xpos-x)<<8)*prediv)>>12;
			old_pixel = buffer[x+yoffset];
			new_pixel = GetSubPixel( (xpos<<8)-newx , (ypos<<8)-newy );
			__asm{
				movd mm0, new_pixel
				punpcklbw mm0, mm5
				movd mm1, old_pixel
				punpcklbw mm1, mm5
				pmullw mm0, mm6
				psrlw mm0, 8
				paddw mm0, mm1
				packuswb mm0, mm0
				movd new_pixel, mm0
			}
			buffer[x+yoffset] = new_pixel;
		}
		for(x=xpos+1; x<width; x++){
			newx = (((xpos-x)<<8)*prediv)>>12;
			old_pixel = buffer[x+yoffset];
			new_pixel = GetSubPixel( (xpos<<8)-newx , (ypos<<8)-newy );
			__asm{
				movd mm0, new_pixel
				punpcklbw mm0, mm5
				movd mm1, old_pixel
				punpcklbw mm1, mm5
				pmullw mm0, mm6
				psrlw mm0, 8
				paddw mm0, mm1
				packuswb mm0, mm0
				movd new_pixel, mm0
			}
			buffer[x+yoffset] = new_pixel;
		}
		yoffset += width;
	}
	_asm emms;
}

#define BLUR_OPACITY 230
void Bitmap::RadialBlur( int xpos, int ypos, float strength ){
	int x,y;
	unsigned int old_pixel;
	unsigned int new_pixel;
	int newx, newy;
	unsigned int prediv = 4096/(1.0f+strength);
	unsigned int* buffer = data;
	int yoffset = ypos*width;
	unsigned short blend[4]={BLUR_OPACITY,BLUR_OPACITY,BLUR_OPACITY,BLUR_OPACITY};
	static unsigned short white[4] = {0xFF,0xFF,0xFF,0xFF};

	__asm{
		movq mm6, [blend]
		pxor mm5, mm5
	};
	for(y=ypos; y>-1; y--){
		newy = (((ypos-y)<<8)*prediv)>>12;
		for(x=xpos; x>-1; x--){
			newx = (((xpos-x)<<8)*prediv)>>12;
			old_pixel = buffer[x+yoffset];
			new_pixel = GetSubPixel( (xpos<<8)-newx , (ypos<<8)-newy );
			__asm{
				movd mm0, new_pixel
				punpcklbw mm0, mm5
				movd mm1, old_pixel
				punpcklbw mm1, mm5
				psubw mm0, mm1
				pmullw mm0, mm6
				psllw mm1, 8
				paddw mm0, mm1
				psrlw mm0, 8
				packuswb mm0, mm0
				movd new_pixel, mm0
			}
			buffer[x+yoffset] = new_pixel;
		}
		for(x=xpos+1; x<width; x++){
			newx = (((xpos-x)<<8)*prediv)>>12;
			old_pixel = buffer[x+yoffset];
			new_pixel = GetSubPixel( (xpos<<8)-newx , (ypos<<8)-newy );
			__asm{
				movd mm0, new_pixel
				punpcklbw mm0, mm5
				movd mm1, old_pixel
				punpcklbw mm1, mm5
				psubw mm0, mm1
				pmullw mm0, mm6
				psllw mm1, 8
				paddw mm0, mm1
				psrlw mm0, 8
				packuswb mm0, mm0
				movd new_pixel, mm0
			}
			buffer[x+yoffset] = new_pixel;
		}
		yoffset -= width;
	}

	yoffset = (ypos+1)*width;
	for(y=ypos+1; y<height; y++){
		newy = (((ypos-y)<<8)*prediv)>>12;
		for(x=xpos; x>-1; x--){
			newx = (((xpos-x)<<8)*prediv)>>12;
			old_pixel = buffer[x+yoffset];
			new_pixel = GetSubPixel( (xpos<<8)-newx , (ypos<<8)-newy );
			__asm{
				movd mm0, new_pixel
				punpcklbw mm0, mm5
				movd mm1, old_pixel
				punpcklbw mm1, mm5
				psubw mm0, mm1
				pmullw mm0, mm6
				psllw mm1, 8
				paddw mm0, mm1
				psrlw mm0, 8
				packuswb mm0, mm0
				movd new_pixel, mm0
			}
			buffer[x+yoffset] = new_pixel;
		}
		for(x=xpos+1; x<width; x++){
			newx = (((xpos-x)<<8)*prediv)>>12;
			old_pixel = buffer[x+yoffset];
			new_pixel = GetSubPixel( (xpos<<8)-newx , (ypos<<8)-newy );
			__asm{
				movd mm0, new_pixel
				punpcklbw mm0, mm5
				movd mm1, old_pixel
				punpcklbw mm1, mm5
				psubw mm0, mm1
				pmullw mm0, mm6
				psllw mm1, 8
				paddw mm0, mm1
				psrlw mm0, 8
				packuswb mm0, mm0
				movd new_pixel, mm0
			}
			buffer[x+yoffset] = new_pixel;
		}
		yoffset += width;
	}
	_asm emms;
}


void Bitmap::Gamma( float gamma ){
	static unsigned char gammatable[256];
	for( int i =0; i<256; i++ ){
		int val = pow(i,gamma);
		if(val<0) val=0;
		if(val>255) val=255;
		gammatable[i] = val;
	}

	unsigned char* src = (unsigned char*)data;
	for( int counter=width*height*4; counter; counter-- ){
		*src++ = gammatable[*src];
	}

}

// Bitmap Loaders
#include "../file/file.h"

#include <commdlg.h>
#include <ocidl.h>
#include <olectl.h>
#include <crtdbg.h>
#define HIMETRIC_INCH	2540

JpegFile::JpegFile( char* filename, char*alpha ){
	Load( filename );
	JpegFile temp = JpegFile( alpha );
	if( (width!=temp.GetWidth())||(height!=temp.GetHeight()) ){
		return;
	}
	
	unsigned int* dst = data;
	unsigned int* src = temp.GetData();

	for(int c=height*width; c; c-- ){
		*dst++ |= (*src++&0xFF)<<24;
	}
}

JpegFile::JpegFile( char* filename ){
	Load(filename);
}

void JpegFile::Load( char* filename ){
	FileLoader* loader = FileLoader::instance();
	File* file = loader->GetFile(filename);
	if(file==NULL){
		return;
	}

	HGLOBAL global = GlobalAlloc( GMEM_MOVEABLE, file->GetSize() );
	void* temp_data = GlobalLock( global );

	//kopier inn
	memcpy( temp_data, file->GetFile(), file->GetSize() );

	GlobalUnlock( global );
	LPSTREAM stream = NULL;
	HRESULT hr = CreateStreamOnHGlobal( global, TRUE, &stream );

	LPPICTURE picture;
	hr = ::OleLoadPicture( stream, file->GetSize(), FALSE, IID_IPicture, (LPVOID *)&picture);

	stream->Release();
	delete file;
	
	/*
	låd
	*/
	long width, height;
	picture->get_Width( &width );
	picture->get_Height( &height );

	HDC hdc = GetDC( NULL );
	int width_ = MulDiv(width, GetDeviceCaps(hdc, LOGPIXELSX), HIMETRIC_INCH);
	int height_ = MulDiv(height, GetDeviceCaps(hdc, LOGPIXELSY), HIMETRIC_INCH);

	HBITMAP bitmap = CreateCompatibleBitmap(hdc, width_, height_ );
	HDC hdc_mem = CreateCompatibleDC(hdc);
	SelectObject(hdc_mem, bitmap);

#ifdef _DEBUG
	printf("width: %u, height: %u\n", width_, height_);
#endif

	picture->Render( hdc_mem, 0, 0, width_, height_, 0, height, width, -height, NULL );

	unsigned int* temp = new unsigned int[width_*height_];
	data = new unsigned int[width_*height_];

	BITMAPINFO bi;
	memset( &bi, 0, sizeof(BITMAPINFO) );
	bi.bmiHeader.biSize=sizeof(bi.bmiHeader);
	bi.bmiHeader.biWidth = width_;
	bi.bmiHeader.biHeight = height_;
	bi.bmiHeader.biPlanes = 1;
	bi.bmiHeader.biBitCount = 32;
	bi.bmiHeader.biCompression = BI_RGB;

	GetDIBits( hdc_mem, bitmap, 0, height_, temp, &bi, DIB_RGB_COLORS );
	DeleteDC(hdc_mem);
	DeleteObject(bitmap);

	for(int y=0;y<height_;y++){
		for(int x=0;x<width_;x++){
			data[x+y*width_] = temp[x+((height_-1)-y)*width_];
		}
	}

	delete[] temp;

	//this->data = new unsigned int[width*height];
	this->width = width_;
	this->height = height_;

}


RawFile::RawFile( char* filename, int width, int height, int bpp ){
	this->data = new unsigned int[width*height];
	this->width = width;
	this->height = height;

	FileLoader* loader = FileLoader::instance();
	File* file = FileLoader::instance()->GetFile(filename);

	if(file==NULL){
		return;
	}

	unsigned int* dest = this->data;
	if((bpp==32)||(bpp==24)){
		for(int i=0; i<(width*height); i++){
			int color = 0;
			file->Read(&this->data[i], (bpp>>3));

			*dest++= (this->data[i]&0xFF00FF00)|
					((this->data[i]&0x000000FF)<<16)|
					((this->data[i]&0x00FF0000)>>16);

			if(bpp==24) *(dest-1) = (*(dest-1)&0xFFFFFF)|0xFF000000;
		}
	}
	delete file;
}

