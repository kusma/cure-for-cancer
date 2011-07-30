#include "../tyfuus/tyfuus.h"
#include <windows.h>

void font_print( Bitmap& target, int xpos, int ypos, char *name, char* text, int size, int color ){
	HDC hdc_mem;
	HBITMAP bitmap;
	BITMAPINFO bi;
	LOGFONT lf;
	HFONT font;
	HDC hdc;

	unsigned int* buffer = target.GetData();
	int width = target.GetWidth();
	int height = target.GetHeight();

	unsigned int* temp = new unsigned int[width*height];
	int x, y;

	hdc = GetDC( NULL );
	bitmap = CreateCompatibleBitmap(hdc, width, height );
	hdc_mem = CreateCompatibleDC(hdc);
	SelectObject(hdc_mem, bitmap);

	for(y=0;y<target.GetHeight();y++){
		int y_stash = ((height-1)-y)*width;
		for(x=0;x<width;x++){
			temp[x+y*width] = buffer[x+y_stash];
		}
	}

	lf.lfHeight         = size;
	lf.lfWidth          = 0;
	lf.lfEscapement     = 0;
	lf.lfOrientation    = 0;
	lf.lfWeight         = 0;
	lf.lfItalic         = 0;
	lf.lfUnderline      = 0;
	lf.lfStrikeOut      = 0;
	lf.lfCharSet        = 0;
	lf.lfOutPrecision   = 0;
	lf.lfClipPrecision  = 0;
	lf.lfQuality        = ANTIALIASED_QUALITY;
	lf.lfPitchAndFamily = 0;
	strcpy( lf.lfFaceName, name );
	font = CreateFontIndirect( &lf );

	memset( &bi, 0, sizeof(BITMAPINFO) );
	bi.bmiHeader.biSize=sizeof(bi.bmiHeader);
	bi.bmiHeader.biWidth = width;
	bi.bmiHeader.biHeight = height;
	bi.bmiHeader.biPlanes = 1;
	bi.bmiHeader.biBitCount = 32;
	bi.bmiHeader.biCompression = BI_RGB;

	SetDIBits( hdc_mem, bitmap, 0, height, temp, &bi, DIB_RGB_COLORS );

	SetBkMode( hdc_mem, TRANSPARENT ); 
	SetTextColor( hdc_mem, ((color&0xFF)<<16)|((color&0xFF0000)>>16)|(color&0xFF00) );
	SelectObject(hdc_mem, font);
	TextOut(hdc_mem, xpos, ypos, text, strlen(text));

	GetDIBits( hdc_mem, bitmap, 0, height, temp, &bi, DIB_RGB_COLORS );

	DeleteDC(hdc_mem);
	DeleteObject(bitmap);
	DeleteObject(font);

	for(y=0;y<height;y++){
		for(x=0;x<width;x++){
			buffer[x+y*width] = temp[x+((height-1)-y)*width];
		}
	}

	delete temp;
}
