#include <windows.h>
#include <ddraw.h>

#include <stdio.h>

HWND win;
LPDIRECTDRAW ddraw;
LPDIRECTDRAWSURFACE primarybuffer;
LPDIRECTDRAWSURFACE backbuffer;
DDPIXELFORMAT pixelformat;
LPDIRECTDRAWCLIPPER clipper;


typedef HRESULT (WINAPI * DIRECTDRAWCREATE) (GUID FAR *lpGUID,LPDIRECTDRAW FAR *lplpDD,IUnknown FAR *pUnkOuter);
static HMODULE library = 0;

static LRESULT CALLBACK WndProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam);

//LPCLIPPER clipper;

int width, height;
int real_width, real_height;
bool fullscreen = true;
bool fakemode = false;

// Converter-stuff
typedef void* (*CONVERTER) (void *dst,const void *src, unsigned int count, int pitch);
CONVERTER converter;
CONVERTER get_converter( unsigned int bpp, unsigned int r_mask, unsigned int g_mask, unsigned int b_mask );

int leepra_open( char* title, int width_, int height_, bool fullscreen_){
    DIRECTDRAWCREATE DirectDrawCreate;
    library = (HMODULE) LoadLibrary("ddraw.dll");
    if (!library) return false;

    DirectDrawCreate = (DIRECTDRAWCREATE) GetProcAddress(library,"DirectDrawCreate");
    if (!DirectDrawCreate) return false;

	HRESULT result;
	HINSTANCE inst = GetModuleHandle(NULL);
	fullscreen = fullscreen_;
	width = width_;
	height = height_;
	real_width = width;
	real_height = height;

	result = DirectDrawCreate( 0, &ddraw, 0 );
	if(FAILED(result)) return false;

	if(fullscreen){

		result = ddraw->SetDisplayMode( real_width, real_height, 32 );
		if(FAILED(result)){
			result = ddraw->SetDisplayMode( real_width, real_height, 24 );
			if(FAILED(result)){
				result = ddraw->SetDisplayMode( real_width, real_height, 16 );
				fakemode = true;
				real_width = width*2;
				real_height = height*2;
				result = ddraw->SetDisplayMode( real_width, real_height, 32 );
				if(FAILED(result)){
					result = ddraw->SetDisplayMode( real_width, real_height, 24 );
					if(FAILED(result)){
						result = ddraw->SetDisplayMode( real_width, real_height, 16 );
						if(FAILED(result)){
							fakemode = false;
							fullscreen = false;
						}
					}
				}
			}
		}
	}

	RECT rect = {0,0,real_width,real_height};
	if(!fullscreen) AdjustWindowRect( &rect, WS_OVERLAPPEDWINDOW, false );

	WNDCLASS wc;
    wc.style = CS_VREDRAW | CS_HREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = 0;
    wc.hIcon = 0;
    wc.hCursor = LoadCursor(0,IDC_ARROW);
    wc.hbrBackground = 0;
    wc.lpszMenuName = 0;
    wc.lpszClassName = title;
    RegisterClass(&wc);

	win = CreateWindowEx(0, title, title, WS_VISIBLE | WS_OVERLAPPEDWINDOW, 0, 0, rect.right, rect.bottom, 0, 0, inst, 0);

	if( fullscreen ) result = ddraw->SetCooperativeLevel( win, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN );
	else result = ddraw->SetCooperativeLevel( win, DDSCL_NORMAL );
	if(FAILED(result)) return false;

	DDSURFACEDESC desc;
	DDSCAPS caps;

	memset( &desc, 0, sizeof(DDSURFACEDESC) );
	desc.dwSize = sizeof(DDSURFACEDESC);

	if(fullscreen){
		ShowCursor(0);
		desc.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
		desc.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_VIDEOMEMORY | DDSCAPS_FLIP | DDSCAPS_COMPLEX;
		desc.dwBackBufferCount = 1;
	}else{
		desc.dwFlags = DDSD_CAPS;
		desc.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
	}

	result = ddraw->CreateSurface( &desc, &primarybuffer, NULL );
	if( FAILED(result) ) return false;

	if(fullscreen){
		caps.dwCaps = DDSCAPS_BACKBUFFER;
		result = primarybuffer->GetAttachedSurface( &caps, &backbuffer );
		if( FAILED(result) ) return false;
	}else{
		memset( &desc, 0, sizeof(DDSURFACEDESC) );
		desc.dwSize = sizeof(DDSURFACEDESC);
		desc.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
		desc.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
		desc.dwWidth = width;
		desc.dwHeight = height;

		result = ddraw->CreateSurface( &desc, &backbuffer, NULL );
		if(FAILED(result)) return false;

		ddraw->CreateClipper( 0, &clipper, 0 );
		clipper->SetHWnd(0,win);
		primarybuffer->SetClipper( clipper );

	}

	pixelformat.dwSize = sizeof(DDPIXELFORMAT);
	primarybuffer->GetPixelFormat( &pixelformat );
	if (!(pixelformat.dwFlags & DDPF_RGB)) return false;

#ifdef _DEBUG
	printf("%u\n", pixelformat.dwRGBBitCount );
#endif

	converter = get_converter( pixelformat.dwRGBBitCount, pixelformat.dwRBitMask, pixelformat.dwGBitMask, pixelformat.dwBBitMask );
	if(converter == NULL) return false;

#ifndef _DEBUG
	SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);
#endif

	return true;
}

void leepra_update( void* data ){
	MSG message;
	while (PeekMessage(&message,win,0,0,PM_REMOVE)){
		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	DDSURFACEDESC descriptor;

	primarybuffer->Restore();
	if(!fullscreen) backbuffer->Restore();

	descriptor.dwSize = sizeof(DDSURFACEDESC);

	backbuffer->Lock(0,&descriptor,DDLOCK_WAIT,0);

	int pitch = descriptor.lPitch;

//	printf("pitch: %u\n", pitch>>2);

	if(pitch==width&&!fakemode){
		converter(data, descriptor.lpSurface, real_width*real_height*4, pitch);
	}else{
		char* dst = (char*)descriptor.lpSurface;
		char* src = (char*)data;

		if(fakemode) pitch *= 2;

		for(int y=height; y; y--){
			converter(dst, src, width*4, pitch);
			src += width*4;
			dst += pitch;
		}
	}

	backbuffer->Unlock(descriptor.lpSurface);

	if(!fullscreen){
		RECT srcrect;
		RECT destrect;
		POINT point;

		point.x = 0;
		point.y = 0;
		ClientToScreen(win, &point);
		GetClientRect(win, &destrect);
		OffsetRect(&destrect, point.x, point.y);
		SetRect(&srcrect, 0, 0, width, height);
		primarybuffer->Blt( &destrect, backbuffer, &srcrect, DDBLT_WAIT, NULL);
	}else{
		primarybuffer->Flip( NULL, DDFLIP_WAIT );
	}
}

void leepra_close(){
	if(primarybuffer!=NULL) primarybuffer->Release();
	ddraw->Release();
	ddraw = NULL;
	DestroyWindow( win );
	ShowCursor( true );
}


// pixelkonvertering
void* convert_argb32_argb888(void *dst, const void *src, unsigned int count, int pitch);
void* convert_argb32_rgb888(void *dst, const void *src, unsigned int count, int pitch);
void* convert_argb32_rgb565(void *dst, const void *src, unsigned int count, int pitch);
void* convert_argb32_rgb555(void *dst, const void *src, unsigned int count, int pitch);
void* convert_argb32_argb888_fakemode(void *dst, const void *src, unsigned int count, int pitch);
void* convert_argb32_rgb888_fakemode(void *dst, const void *src, unsigned int count, int pitch);
void* convert_argb32_rgb565_fakemode(void *dst, const void *src, unsigned int count, int pitch);
void* convert_argb32_rgb555_fakemode(void *dst, const void *src, unsigned int count, int pitch);

CONVERTER get_converter( unsigned int bpp, unsigned int r_mask, unsigned int g_mask, unsigned int b_mask ){
	if( (bpp==32)&&!fakemode ) return &convert_argb32_argb888;
	if( (bpp==24)&&!fakemode ) return &convert_argb32_rgb888;
	if( (bpp==16)&&!fakemode ) return &convert_argb32_rgb565;
	if( (bpp==15)&&!fakemode ) return &convert_argb32_rgb555;
	if( (bpp==32)&&fakemode ) return &convert_argb32_argb888_fakemode;
	if( (bpp==24)&&fakemode ) return &convert_argb32_rgb888_fakemode;
	if( (bpp==16)&&fakemode ) return &convert_argb32_rgb565_fakemode;
	if( (bpp==15)&&fakemode ) return &convert_argb32_rgb555_fakemode;

#ifdef _DEBUG
	printf("bpp:%u rmask:%x gmask:%x bmask:%x\n", bpp, r_mask, g_mask, b_mask);
#endif
	return NULL;
}
void* convert_argb32_argb888(void *dst, const void *src, unsigned int count, int pitch){
	unsigned int *in = (unsigned int*)src;
	unsigned int *out = (unsigned int*)dst;
	count >>= 2;
	for (;count;count--){
		*out++ = *in++;
	}
	return NULL;
}

void* convert_argb32_rgb565(void *dst, const void *src, unsigned int num, int pitch){
	_asm
	{
		push ebp
		mov esi,src
		mov edi,dst
		mov ecx,num
		xor edx,edx
	lup:
		mov eax,[esi+edx*4]
		mov ebx,eax
		shr ebx,8
		and ebx,1111100000000000b
		mov ebp,eax
		shr ebp,3
		and ebp,0000000000011111b
		add ebx,ebp
		shr eax,5
		and eax,0000011111100000b
		add ebx,eax
		mov [edi+edx*2],bx
		inc edx
		dec ecx
		jnz lup
		pop ebp
	}
	return NULL;
}

void* convert_argb32_rgb555(void *dst, const void *src, unsigned int num, int pitch){
	_asm
	{
		push ebp
		mov esi,src
		mov edi,dst
		mov ecx,num
		xor edx,edx
	lup:
		mov eax,[esi+edx*4]
		mov ebx,eax
		shr ebx,9
		and ebx,0111110000000000b
		mov ebp,eax
		shr ebp,3
		and ebp,0000000000011111b
		add ebx,ebp
		shr eax,6
		and eax,0000001111100000b
		add ebx,eax
		mov [edi+edx*2],bx
		inc edx
		dec ecx
		jnz lup
		pop ebp
	}
	return NULL;
}


void* convert_argb32_rgb888(void *dst, const void *src, unsigned int num, int pitch){
	_asm
	{
		push ebp
		mov esi,src
		mov edi,dst
		mov ecx,num
		xor edx,edx
	lup:
		mov eax,[esi]
		mov [edi],eax
		add esi,4
		add edi,3
		dec ecx
		jnz lup
		pop ebp
	}
	return NULL;
}

void* convert_argb32_argb888_fakemode(void *dst, const void *src, unsigned int count, int pitch){
	unsigned int *in = (unsigned int*)src;
	unsigned int *out = (unsigned int*)dst;
	count >>= 2;
	pitch >>= 3;
	for (;count;count--){
		unsigned int pixel = *in++;
		*out = pixel;
		*(out+1) = pixel;
		*(out+pitch) = pixel;
		*(out+pitch+1) = pixel;
		out += 2;
	}
	return NULL;
}

void* convert_argb32_rgb565_fakemode(void *dst, const void *src, unsigned int num, int pitch){
	_asm
	{
		push ebp
		mov esi,src
		mov edi,dst
		mov ecx,num
		xor edx,edx
	lup:
		mov eax,[esi+edx*4]
		mov ebx,eax
		shr ebx,8
		and ebx,1111100000000000b
		mov ebp,eax
		shr ebp,3
		and ebp,0000000000011111b
		add ebx,ebp
		shr eax,5
		and eax,0000011111100000b
		add ebx,eax
		mov [edi+edx*2],bx
		inc edx
		dec ecx
		jnz lup
		pop ebp
	}
	return NULL;
}

void* convert_argb32_rgb555_fakemode(void *dst, const void *src, unsigned int num, int pitch){
	_asm
	{
		push ebp
		mov esi,src
		mov edi,dst
		mov ecx,num
		xor edx,edx
	lup:
		mov eax,[esi+edx*4]
		mov ebx,eax
		shr ebx,9
		and ebx,0111110000000000b
		mov ebp,eax
		shr ebp,3
		and ebp,0000000000011111b
		add ebx,ebp
		shr eax,6
		and eax,0000001111100000b
		add ebx,eax
		mov [edi+edx*2],bx
		inc edx
		dec ecx
		jnz lup
		pop ebp
	}
	return NULL;
}


void* convert_argb32_rgb888_fakemode(void *dst, const void *src, unsigned int num, int pitch){
	_asm
	{
		push ebp
		mov esi,src
		mov edi,dst
		mov ecx,num
		xor edx,edx
	lup:
		mov eax,[esi]
		mov [edi],eax
		add esi,4
		add edi,3
		dec ecx
		jnz lup
		pop ebp
	}
	return NULL;
}


static LRESULT CALLBACK WndProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam){
	switch (message){
		case WM_PAINT:
			if(!fullscreen){
			RECT srcrect;
			RECT destrect;
			POINT point;

			point.x = 0;
			point.y = 0;
			ClientToScreen(win, &point);
			GetClientRect(win, &destrect);
			OffsetRect(&destrect, point.x, point.y);
			SetRect(&srcrect, 0, 0, width, height);
			primarybuffer->Blt( &destrect, backbuffer, &srcrect, DDBLT_WAIT, NULL);
		}else{
			primarybuffer->Flip( NULL, DDFLIP_WAIT );
		}
	}
    return DefWindowProc(hWnd,message,wParam,lParam);
}