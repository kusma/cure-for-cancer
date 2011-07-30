#include "tyfuus/tyfuus.h"
#include "arebare.h"
#include <math.h>

#define M_PI 3.1415926535f

AreBare::AreBare(int xres, int yres)
{
	XRES = xres;
	YRES = yres;

	tabell1 = new unsigned char[XRES * 2 * YRES * 2];
	tabell2 = new unsigned char[XRES * 2 * YRES * 2];
	xtabell1 = new unsigned char[XRES * 2 * YRES * 2];
	xtabell3 = new unsigned char[XRES * 2 * YRES * 2];
	ytabell2 = new unsigned char[XRES * 2 * YRES * 2];
	ytabell3 = new unsigned char[XRES * 2 * YRES * 2];

	calcsphere2(xtabell1, tabell1, XRES * 2, YRES * 2, 15);
	calcsphere2(tabell1, ytabell2, XRES * 2, YRES * 2, 175);
	calcsphere2(xtabell3, ytabell3, XRES * 2, YRES * 2, 0);

	
	calcsphere2(tabell1, tabell2, XRES * 2, YRES * 2, 30);

	vpage1 = new Bitmap(XRES, YRES);


	//defaults
	param1[0] = 50.0f * M_PI / 128.0f;	
	param1[1] = 50.0f * M_PI / 128.0f;
	param1[2] = 50.0f * M_PI / 248.0f;	
	param1[3] = 50.0f * M_PI / 168.0f;
	param1[4] = 50.0f * M_PI / 228.0f;	
	param1[5] = 50.0f * M_PI / 328.0f;

	param2[0] = 160.0f / 320.0f;
	param2[1] = 50.0f / 240.0f;
	param2[2] = 130.0f / 320.0f;
	param2[3] = 100.0f / 240.0f;
	param2[4] = 70.0f / 320.0f;
	param2[5] = 90.0f / 240.0f;

}


AreBare::~AreBare()
{
	delete[] tabell1;
	delete[] tabell2;
	delete[] xtabell1;
	delete[] xtabell3;
	delete[] ytabell2;
	delete[] ytabell3;	
}


void AreBare::calcsphere2(unsigned char *where1,unsigned char *where2,int xres,int yres,float zz)
{
	int xprime;
	int yprime;
	int x,y,offset;
	float du,dv;
	unsigned char burp,burp2;

	xprime=-(xres/2);
	yprime=-(yres/2);

    du=(float)sqrt((xprime*xprime)+(yprime*yprime)+(zz*zz))*(float)-128/(float)xprime;
    dv=(float)sqrt((xprime*xprime)+(yprime*yprime)+(zz*zz))*(float)-128/(float)yprime;

    offset=0;
    for ( y=0;y<yres;y++,yprime++,xprime=-(xres/2) )
	{
        for ( x=0;x<xres;x++,offset++,xprime++ )
        {
            burp=(unsigned char)((float)xprime*du/sqrt((xprime*xprime)+(yprime*yprime)+(zz*zz))+128);
            burp2=(unsigned char)((float)yprime*dv/sqrt((xprime*xprime)+(yprime*yprime)+(zz*zz))+128);
            where1[offset]=burp2;
            where2[offset]=burp;
        }
	}
}

void AreBare::setparam1(int i, float val)
{
	param1[i] = val == 0.0f ? 0.00001f : val;
}

void AreBare::setparam2(int i, float val)
{
	param2[i] = val;
}

void AreBare::setparam1(float *val)
{
	int i;
	for (i = 0; i < 6; i++)
		param1[i] = val[i] == 0.0f ? 0.00001f : val[i];
}

void AreBare::setparam2(float *val)
{
	int i;
	for (i = 0; i < 6; i++)
		param2[i] = val[i];
}


void AreBare::prag_first(Bitmap &where, Bitmap &texture, float time)
{
	int x,y;
	int zoomrot;
	int dif1,dif2;
	int xpos, ypos, xpos2, ypos2, xpos3, ypos3;
	int toffset, toffset2, toffset3;
	int zoom, rot;
	float xscale, yscale;

	xscale = (float)XRES / 2.1f;
	yscale = (float)YRES / 2.1f;

	xpos = (int)((XRES / 2) + (float)cos(time * param1[0]) * param2[0] * xscale);
	ypos = (int)((YRES / 2) + (float)sin(time * param1[1]) * param2[1] * yscale);

	xpos2 = (int)((XRES / 2)+ (float)sin(time * param1[2]) * param2[2] * xscale);
	ypos2 = (int)((YRES / 2) + (float)cos(time * param1[3]) * param2[3] * yscale);

	xpos3 = (int)((XRES / 2) + (float)sin(time * param1[4]) * param2[4] * xscale);
	ypos3 = (int)((YRES / 2) + (float)cos(time * param1[5]) * param2[5] * yscale);

//  ypos=100+sin(frames*pi/128)*90;

	toffset = (ypos * XRES * 2) + xpos;

	toffset2 = ((ypos2 * XRES * 2) + xpos2);

	toffset3 = ((ypos3 * XRES * 2) + xpos3);


	dif1 = toffset2 - toffset;
	dif2 = toffset3 - toffset2;

	zoom = (int)((float)cos(time * M_PI / 412.0f) * 256.0f);
	rot = (int)((float)sin(time * M_PI / 312.0f) * 256.0f);
	zoomrot = (zoom<<8) + rot;



//  tun3(tabell2, tabell1, toffset, where, texture);

	unsigned char *t1 = tabell2;
	unsigned char *t2 = tabell1;
	void *w = where.GetData();
	void *t = texture.GetData();
	
	int xres = XRES;
	int yres = YRES;


	_asm
	{
		mov edi, t1
		mov esi, t2
		mov ecx, toffset

		mov ebx, w
		mov eax, t

        xor edx,edx    
		push eax
		mov eax, yres
        mov y,eax
		pop eax
        ylup:
		push eax
		mov eax, xres
        mov x,eax
		pop eax
        xlup:          
        push ecx       
        mov dh,[edi+ecx]
        mov dl,[esi+ecx]
        add ecx,dif1    
        add dh,[edi+ecx]
        add dl,[esi+ecx]
        add ecx,dif2    
        sub dh,[edi+ecx]
        sub dl,[esi+ecx]
        add edx,zoomrot 
		and edx, 0ffffh
        mov edx,[eax+edx*4]
        mov [ebx],edx    
        pop ecx         
        inc ecx         
        add ebx, 4      
        dec x           
        jnz xlup        
        add ecx,320     
        dec y           
        jnz ylup
	}
}



void AreBare::yiha(Bitmap &where, Bitmap &texture1, Bitmap &texture2, float time)
{
	prag_first(where, texture1, time);
	prag_first(*vpage1, texture2, -time);
	vpage1->MulBlit(where);
}


