// CONFIG-HOR
#define WIDTH 320
#define HEIGHT 240
#define MUSIC

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <math.h>
#include <float.h>
#ifndef M_PI
#define M_PI 3.14159265358979f
#define M_PI2 M_PI*2
#define PI_SCALE 0.31830988618379067153776752674503f
#endif

#ifdef _DEBUG
#include <stdio.h>
#endif

#include "leepra.h"
#include "menu.h"
#include "kancr/kancr.h"
#include "arebare.h"
#include "effects/misc.h"
#include "tool/font.h"
#include "generators/sphere.h"
#include "tyfuus/tyfuus.h"
#include "file/file.h"
#include "kancr/lwo.h"

#ifdef MUSIC
#include "bassmod/bassmod.h"
#endif

float last_bass=-10;
int bass_sync;
float last_bassdrum=-10;
int bassdrum_sync;
float last_jalla=-10;
int jalla_sync;
float last_bassdrum2=-10;
int bassdrum2_sync;
float last_sweep=-10;
int sweep_sync;
float last_sash=-10;
int sash_sync;
float last_blur=-10;
int blur_sync;

bool added = false;

int active_sprite = 0;

#ifdef MUSIC
void CALLBACK BassSync(HSYNC handle, DWORD data, DWORD user) {
	int timei = BASSMOD_MusicGetPosition();
	int order = LOWORD(timei);
	float row = (float)HIWORD(timei)/256;
	float time = order*64 + row;

    last_bass = time;
}

void CALLBACK BassdrumSync(HSYNC handle, DWORD data, DWORD user) {
	int timei = BASSMOD_MusicGetPosition();
	int order = LOWORD(timei);
	float row = (float)HIWORD(timei)/256;
	float time = order*64 + row;

    last_bassdrum = time;
}

void CALLBACK JallaSync(HSYNC handle, DWORD data, DWORD user) {
	int timei = BASSMOD_MusicGetPosition();
	int order = LOWORD(timei);
	float row = (float)HIWORD(timei)/256;
	float time = order*64 + row;

    last_jalla = time;
}

void CALLBACK Bassdrum2Sync(HSYNC handle, DWORD data, DWORD user) {
	int timei = BASSMOD_MusicGetPosition();
	int order = LOWORD(timei);
	float row = (float)HIWORD(timei)/256;
	float time = order*64 + row;

    last_bassdrum2 = time;
}
void CALLBACK SweepSync(HSYNC handle, DWORD data, DWORD user) {
	int timei = BASSMOD_MusicGetPosition();
	int order = LOWORD(timei);
	float row = (float)HIWORD(timei)/256;
	float time = order*64 + row;

    last_sweep = time;
}
void CALLBACK SashSync(HSYNC handle, DWORD data, DWORD user) {
	int timei = BASSMOD_MusicGetPosition();
	int order = LOWORD(timei);
	float row = (float)HIWORD(timei)/256;
	float time = order*64 + row;

    last_sash = time;
}
int skip = 0;

void CALLBACK BlurSync(HSYNC handle, DWORD data, DWORD user ){
	int timei = BASSMOD_MusicGetPosition();
	int order = LOWORD(timei);
	float row = (float)HIWORD(timei)/256;
	float time = order*64 + row;
	switch(data){
	case 0:
		last_blur = time;
	break;

	case 1:
		skip++;
	break;

	default:
	break;

	}
}

#endif //MUSIC

inline float boost(float start, float end, float pos){
	return (float)sin(pos*M_PI);
}

inline float calc(float start, float end, float pos, int waves){
	float result = 0;
	if((pos>start)&&(pos<end)){
		float loog = log((start-pos)/(start-end));
		if( (loog>1.0f)||_isnan(loog) ) loog = 1.0f;
		result += boost(start, end, ((start-pos)/(start-end))*waves)
										*boost(start, end, (start-pos)/(start-end))
										*loog*loog;
	}
	return result;
}

inline float calc2(float start, float end, float pos, int waves){
	float result = 0;
	if((pos>start)&&(pos<end)){
		float loog = log((start-pos)/(start-end));
		if( (loog>1.0f)||_isnan(loog) ) loog = 1.0f;
		result += loog;
	}
	return result;
}

#include "tool/misc.h"

#ifdef _DEBUG
int main(){
#else
int APIENTRY WinMain (HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpCmd, int nShow){
#endif

#ifdef MUSIC

	FileLoader *loader = FileLoader::instance();
	loader->SetArchive("data.dat");
	loader->SetPassword("piss");
	Menu::instance();

#ifdef _DEBUG
//	freopen("debug.txt", "w", stdout);
	printf("starting\n");
#endif

	if(Menu::instance()->Sound()){
		if( BASSMOD_Init( -1, 44100, BASS_DEVICE_LEAVEVOL ) !=TRUE ){
			MessageBox(NULL, "Could not init audio, going nosound", "Error", MB_OK);
			BASSMOD_Init( -2, 44100, 0 );
		}
	}else{
			BASSMOD_Init( -2, 44100, 0 );
	}

	File* xm = loader->GetFile("musikk.xm");
	if( xm==NULL ){
		MessageBox(NULL, "Could not load audiofile", "Error", MB_OK);
		return 1;
	}

	if( BASSMOD_MusicLoad( TRUE, xm->GetFile(),0, 0, 0) != TRUE){
		MessageBox(NULL, "Could not load audiofile", "Error", MB_OK);
		return 1;
	}

#endif

	if(!leepra_open( "recnac rof eruc ehT", WIDTH, HEIGHT, Menu::instance()->Fullscreen() )){
		leepra_close();
		MessageBox(NULL, "Could not open window", "Error", MB_OK);
		return 1;
	}

	Bitmap screenbuffer = Bitmap(WIDTH, HEIGHT);
	Bitmap prevscreen = Bitmap(WIDTH, HEIGHT);
	Bitmap blurbuffer = Bitmap( 320, 240 );
	Bitmap renderbuffer = Bitmap( 320, 240 );

	Bitmap test_texture = Bitmap( 256, 256 );

	JpegFile tv = JpegFile("tv.bmp");
	Grid8x8 grid = Grid8x8(WIDTH, HEIGHT);
	JpegFile tunnel = JpegFile("tunnel.jpg");
	grid.SetTexture( tunnel );


	JpegFile jall = JpegFile("jall.jpg");
	JpegFile overlay = JpegFile("fucked.jpg","fucked-alpha.jpg");

	JpegFile aapning = JpegFile("aapning.gif");

	JpegFile murvegg = JpegFile("murvegg.jpg");
	JpegFile murvegg_env = JpegFile("murvegg-env.jpg");

	JpegFile music = JpegFile("music.gif","music-alpha.gif");
	JpegFile code = JpegFile("code.gif","code-alpha.gif");
	JpegFile modeling = JpegFile("modeling.gif","modeling-alpha.gif");
	JpegFile graphics = JpegFile("graphics.gif","graphics-alpha.gif");

	JpegFile sky = JpegFile("sky.jpg");
	JpegFile skylight = JpegFile("sky-light.jpg");
	JpegFile logo = JpegFile("logo.gif","logo-alpha.gif");
	Sprite logosprite = Sprite(logo);

	JpegFile cure_env = JpegFile("cure-env.jpg");

	JpegFile underscore_ = JpegFile("underscore.gif","underscore-alpha.gif");
	Sprite underscore = Sprite( underscore_ );

	JpegFile greets1 = JpegFile("greets1.gif", "greets1-alpha.jpg");
	JpegFile greets2 = JpegFile("greets2.gif", "greets2-alpha.jpg");

	JpegFile rusty = JpegFile("rusty.jpg");

	JpegFile light_ = JpegFile("light.jpg");
	Sprite lightsprite( light_ );

	JpegFile env = JpegFile("env.jpg");

	Sprite sprite[4];
	sprite[0] = Sprite( music );
	sprite[1] = Sprite( code );
	sprite[2] = Sprite( modeling );
	sprite[3] = Sprite( graphics );

	Scene test = Scene();
	Camera cam = Camera( screenbuffer, Vector(0,0,-10),Vector(0,0,0) );
	test.SetCamera(cam);
	LwoObject kule = LwoObject("kule.lwo");
	test.AddObject( kule );
	kule.mesh_list.Rewind();
	Mesh* m = kule.mesh_list.GetCurrent();
	if(m==NULL){
		MessageBox( NULL, "could not get meshlist", "error", MB_OK );
		return 1;
	}

	Light light = Light();
	test.AddLight( light );

	Blobb b = Blobb(3,3,3);
	m->AddPlugin( b );

	Blobb b2 = Blobb(11,12,11);
	b2.power = 0.1f;

	m->material->envmap = jall.GetData();
	m->material->diffuse = 0xFF000000;
	m->material->render_flag = RENDER_LINE;

	AreBare are = AreBare(WIDTH, HEIGHT);

#ifdef MUSIC
	bass_sync = BASSMOD_MusicSetSync(BASS_SYNC_MUSICINST, MAKELONG(7,-1), &BassSync, 0); 
	bassdrum_sync = BASSMOD_MusicSetSync(BASS_SYNC_MUSICINST, MAKELONG(14,-1), &BassdrumSync, 0); 
	jalla_sync = BASSMOD_MusicSetSync(BASS_SYNC_MUSICINST, MAKELONG(0x1d,-1), &JallaSync, 0); 
	bassdrum2_sync = BASSMOD_MusicSetSync(BASS_SYNC_MUSICINST, MAKELONG(0x21,-1), &Bassdrum2Sync, 0); 
	sweep_sync = BASSMOD_MusicSetSync(BASS_SYNC_MUSICINST, MAKELONG(5,-1), &SweepSync, 0); 
	sash_sync = BASSMOD_MusicSetSync(BASS_SYNC_MUSICINST, MAKELONG(0x25,-1), &SashSync, 0); 
	blur_sync = BASSMOD_MusicSetSync(BASS_SYNC_MUSICFX, 1, &BlurSync, 0); 
	BASSMOD_MusicSetPositionScaler(256);
	BASSMOD_MusicPlay();
#endif
	unsigned int order;
	float row;
	float time;
	int timei=0;
	int prevstate=0;

	while((!GetAsyncKeyState(VK_ESCAPE))&&BASSMOD_MusicIsActive()){
#ifdef MUSIC
		int timei = BASSMOD_MusicGetPosition();
#else
		timei++;
#endif
		order = LOWORD(timei);
		row = (float)HIWORD(timei)/256;
		time = order*64 + row;


//		screenbuffer.Clear(0);
/*
		//time *= 0.1f;
		unsigned int* buffer = screenbuffer.GetData();
		buffer[160+120*320] = 0xFF;
		for(float f=0; f<20; f+=0.1f){
			int x = 160+((xspline.GetValue(time))*10);
			int y = 120+((yspline.GetValue(time))*10);
			if(x>319) x = 319;
			if(x<0) x = 0;
			if(y>239) y = 239;
			if(y<0) y = 0;
			buffer[x+y*320] = 0xFFFFFF;

		}
*/

		/* part 1 */
		if(order<2){
			float f = calc2(last_bass, last_bass+8, time, 1 );
			aapning.Blit( blurbuffer );
			blurbuffer.HorisontalBlur( screenbuffer, abs(f*5) );
		}
		if((order>=2)&&(order<10)){
			m->material->envmap = murvegg_env.GetData();
			murvegg.Blit(screenbuffer);
			b.Wobble( Vector(time*0.31f,cos(time*0.313f),sin(time*0.13f)) );

			kule.Rotate( Vector( sin(time*0.1f)*0.9f , -time*0.133f, time*0.1f) );
			kule.Move( Vector(3,0,0) );
			kule.Scale(Vector(2.5,2.5,2.5 ));

			if(order<6){
				cam.SetTarget(renderbuffer);
				renderbuffer.Clear(0);
				m->material->render_flag = RENDER_LINE;
				test.Render(time);
				renderbuffer.HorisontalBlur( blurbuffer, 3 );
				blurbuffer.VerticalBlur( renderbuffer, 3 );
				renderbuffer.AlphaBlend( screenbuffer );

			}
			if(order==5){
				float s = calc( last_bassdrum, last_bassdrum+10, time, 1);
				cam.SetTarget(renderbuffer);
				renderbuffer.Clear(0);
				m->material->render_flag = RENDER_ENVMAP;
				test.Render(time);
				renderbuffer.Blend(screenbuffer,(int)(256*(row/64)*s));
			}
			if((order>=6)&&(order<10)){
				cam.SetTarget(screenbuffer);
				m->material->render_flag = RENDER_ENVMAP;
				test.Render(time);
			}

			if(order==9){
				blurbuffer.Clear(0);
				overlay.Blend( blurbuffer, 256*(row/64));
				blurbuffer.AlphaBlend(screenbuffer);
			}
		}
		if((order>=10)&&(order<18) ){
			if((order<14)||(int)time&10){
				grid.Reset();
				grid.Tunnel( Vector(time*0.0001f,time*0.01f,time*0.1f), Vector(0, calc(last_bassdrum, last_bassdrum+8, time, 3)*0.6f,time*0.1f), 0 );
				grid.Render( screenbuffer );
				overlay.HorisontalBlur( blurbuffer, (1+sin(time*100))*10);
				blurbuffer.AlphaBlend(screenbuffer);
			}else{
				are.setparam2(0, sin(time * 0.01f) * 0.815f);
				are.setparam2(1, sin(time * 0.02f) * 0.725f);
				are.setparam2(2, cos(time * 0.015f) * 0.635f);
				are.setparam2(3, sin(time * 0.013f) * 0.725f);
				are.setparam2(4, cos(time * 0.011f) * 0.615f);
				are.setparam2(5, sin(time * 0.019f) * 0.535f);
				are.yiha(screenbuffer, jall, jall, time * 0.1f);
				prevscreen.Blend(screenbuffer, 200);
				screenbuffer.Blit(prevscreen);
				b.Wobble( Vector(time*0.1f,cos(time*0.13f),sin(time*0.1f)) );
				b.power = 0.3f;
				kule.Rotate( Vector( sin(time*0.1f)*0.9f , -time*0.133f, time*0.1f) );
				kule.Move( Vector(-3,0,0) );
				float f = calc(last_bassdrum, last_bassdrum+15, time, 1 );
				float f2 = calc(last_bass, last_bass+5, time, 1 );
				kule.Scale(Vector((2.5+f2),(2.5+f2),(2.5+f2) ));
				cam.SetTarget(screenbuffer);
				m->material->envmap = jall.GetData();
				test.Render(time);
				screenbuffer.Gamma( 1+f*0.2f );
			}
		}
		if(order==18){
			if(row<64){
				screenbuffer.Clear(0);
				grid.Reset();
				grid.Tunnel( Vector(time*0.0001f,time*0.01f,time*0.1f), Vector(0, calc(last_bassdrum, last_bassdrum+8, time, 3)*0.6f,time*0.1f), 0 );
				grid.Render( renderbuffer );
				overlay.AlphaBlend( renderbuffer );
				renderbuffer.Blend( screenbuffer, 255*(1-(row/64)) );
			}else{
				renderbuffer.Clear(0);
				screenbuffer.Clear(0);
				logosprite.Move(120+sin(time*100)*5,160+sin(time*182)*5);
				logosprite.Blit( renderbuffer );
				renderbuffer.VerticalBlur( blurbuffer, ((row-64)/64)*20 );
				blurbuffer.Blend( screenbuffer, 255*(1-(row/64)) );
			}
		}


		/* part 2 */
		if(order>=19&&(order<31)){
			float f = calc2( last_jalla, last_jalla+6, time, 1 );
			skylight.Blit(blurbuffer);
			int x = (1+sin(time*0.03f))*160;
			int y = (1+sin(time*0.1f))*20;
			blurbuffer.AddRadialBlur( x, y, 0.02f );
			sky.Blit( renderbuffer );
			blurbuffer.AddBlit(renderbuffer);
			renderbuffer.BammBlit(screenbuffer, f*30);
			if((order>=27)&&(order<31)){
				//underscore.Move( 500-row,50 );
				//underscore.Blit( screenbuffer );
			}
		}
		if(order==19){
			renderbuffer.Clear( 0 );
			renderbuffer.Blend( screenbuffer, 255*(1-(row/64)) );
		}
		if(order==27){
			float f2 = calc2( last_bassdrum2, last_bassdrum2+10, time, 1 );
			sprite[0].Move( 100+(sin(time*100))*f2*10, 120+(sin(time*78.133))*f2*10 );
			sprite[0].Blit(screenbuffer);
		}
		if(order==28){
			float f2 = calc2( last_bassdrum2, last_bassdrum2+10, time, 1 );
			sprite[2].Move( 220+(sin(time*100))*f2*10, 120+(sin(time*78.133))*f2*10 );
			sprite[2].Blit(screenbuffer);
		}
		if(order==29){
			float f2 = calc2( last_bassdrum2, last_bassdrum2+10, time, 1 );
			sprite[1].Move( 100+(sin(time*100))*f2*10, 120+(sin(time*78.133))*f2*10 );
			sprite[1].Blit(screenbuffer);
		}
		if(order==30){
			float f2 = calc2( last_bassdrum2, last_bassdrum2+10, time, 1 );
			sprite[3].Move( 220+(sin(time*100))*f2*10, 120+(sin(time*78.133))*f2*10 );
			sprite[3].Blit(screenbuffer);
		}
		if((order>=31)&&(order<38)){
			float f = calc( last_jalla, last_jalla+5, time, 1 );
			are.setparam2(0, sin(time * 0.01f) * 0.815f);
			are.setparam2(1, sin(time * 0.02f) * 0.725f);
			are.setparam2(2, cos(time * 0.015f) * 0.635f);
			are.setparam2(3, sin(time * 0.013f) * 0.725f);
			are.setparam2(4, cos(time * 0.011f) * 0.615f);
			are.setparam2(5, sin(time * 0.019f) * 0.535f);
			are.yiha(screenbuffer, jall, jall, (time*0.1f));
			m->material->envmap = cure_env.GetData();
			b.power = (1+f)*0.1f;
			kule.Rotate( Vector( sin(time*0.05f)*0.9f , -time*0.0933f, time*0.08f) );
			cam.SetTarget(screenbuffer);
			test.Render(0);
			unsigned char blur = 0;
			//blur = abs(calc( last_blur, last_blur+10, time, 1)*255);
			if( ((time-last_blur)>0) && ((time-last_blur)<64)){
				blur = (255-(time-last_blur)*4);
			}
			prevscreen.Blend(screenbuffer, blur );
			//(1+cos(time*(M_PI2/64)))*127
			screenbuffer.Blit(prevscreen);
		}

		if(order==37){
			renderbuffer.Clear( 0 );
			renderbuffer.Blend( screenbuffer, 255*(row/64) );
		}



		/* part 3 */
		if((order>=38)&&(order<40)){
			grid.Reset();
			grid.Tunnel( Vector(time*0.0001f+(skip*3),time*0.01f,time*0.1f), Vector(0, 0,time*0.1f), 1 );
			grid.Render( screenbuffer );
			renderbuffer.Clear( 0 );
			renderbuffer.Blend( screenbuffer, 255*(row/32) );
		}
		if((order>=40)&&(order<50)){
			grid.Reset();
			grid.Tunnel( Vector(time*0.0001f+((skip+1)*3),time*0.01f,time*0.1f), Vector(0, calc(last_bassdrum, last_bassdrum+8, time, 3)*0.6f,time*0.1f), 1 );
			grid.Render( screenbuffer );
			if(order==41){
				renderbuffer.Clear(0);
				greets1.Blend( renderbuffer, 255*(row/64) );
				renderbuffer.AlphaBlend( screenbuffer );
			}
			if((order>=42)&&(order<44)) greets1.AlphaBlend( screenbuffer );
			if(order==44){
				renderbuffer.Clear(0);
				greets1.Blit( renderbuffer );
				greets2.Blend( renderbuffer, 255*(row/64) );
				renderbuffer.AlphaBlend( screenbuffer );

			}
			if((order>=45)&&(order<48)) greets2.AlphaBlend( screenbuffer );
			if(order==48){
				renderbuffer.Clear(0);
				greets2.Blend( renderbuffer, 255*(1-(row/64)) );
				renderbuffer.AlphaBlend( screenbuffer );
				prevscreen.Clear(0);
			}
		}

		if((order>=50) && (order<60)){
			float f = calc2( last_sweep, last_sweep+10, time, 1);
			float f2 = calc( last_sweep, last_sweep+20, time, 1);
			grid.SetTexture( rusty );
			grid.Reset();
			grid.SinusDistort(4, time*0.3f, 15 );
			grid.BlowupDistort(sin(time*0.1f)*10, sin(time*0.13f)*10, 8, 20 );
			grid.BlowupDistort(sin(time*0.1011f)*10, sin(time*0.132f)*10, 7, 15 );
			renderbuffer.Clear(0);
			grid.Render( renderbuffer );
			renderbuffer.BammBlit(screenbuffer, f*20);

			unsigned char blur = 0;
			if( ((time-last_blur)>0) && ((time-last_blur)<64)){
				blur = (255-(time-last_blur)*4);
			}
			prevscreen.Blend(screenbuffer, blur );
			screenbuffer.Blit(prevscreen);

			if(order==54){
				if(added==false){
					m->AddPlugin( b2 );
					added = true;
				}
				kule.Rotate( Vector( sin(time*0.1f)*0.9f , -time*0.133f, time*0.1f) );
				kule.Move( Vector(3,0,0) );
				kule.Scale(Vector(2.5,2.5,2.5));
				renderbuffer.Clear(0);
				cam.SetTarget(renderbuffer);
				m->material->envmap = env.GetData();
				b.power = (0.1f+f2*0.45f);
				b2.power = (0.1f+f2*0.15f);
				b2.Wobble( Vector(time*0.1f,cos(time*0.13f),sin(time*0.1f)) );

				test.Render(time);
				blurbuffer.Clear(0);
				renderbuffer.Blend(blurbuffer, 255*(row/64));
				blurbuffer.AlphaBlend(screenbuffer);
			}
			if(order>=55){
				kule.Rotate( Vector( sin(time*0.1f)*0.9f , -time*0.133f, time*0.1f) );
				kule.Move( Vector(3, calc2(last_sash, last_sash+10, time, 2), 0) );
				kule.Scale(Vector(2.5,2.5,2.5));
				cam.SetTarget(screenbuffer);
				m->material->envmap = env.GetData();
				b.power = (0.1f+f2*0.45f);
				b2.power = (0.1f+f2*0.15f);
				b2.Wobble( Vector(time*0.1f,cos(time*0.13f),sin(time*0.1f)) );

				test.Render(time);
			}
			screenbuffer.Gamma(1+(1+calc(last_sash,last_sash+10,time, 1))*0.05f);

		}

		if(order==59){
			renderbuffer.Clear( 0 );
			renderbuffer.Blend( screenbuffer, 255*(row/128) );
		}

#if _DEBUG
		static char textbuffer[200];
		sprintf( textbuffer, "%u %2.2f %2.2f",order, row, time);
		font_print( screenbuffer, 1, 1, "Arial", textbuffer, 12, 0xFF0000 );
#endif
		
		leepra_update( screenbuffer.GetData() );

#ifdef MUSIC && _DEBUG
		int keystate;
		if(GetAsyncKeyState(VK_RIGHT)) keystate = 1;
		else if(GetAsyncKeyState(VK_LEFT)) keystate = 2;
		else if(GetAsyncKeyState(VK_UP)) keystate = 3;
		else if(GetAsyncKeyState(VK_DOWN)) keystate = 4;
		else keystate = 0;
		if(keystate!=prevstate){
			if( keystate==1 ) BASSMOD_MusicSetPosition(MAKELONG(order+1,0));
			if( keystate==2 ) BASSMOD_MusicSetPosition(MAKELONG(order-1,0));
			if( keystate==3 ) BASSMOD_MusicPause();
			if( keystate==4 ) BASSMOD_MusicPlay();
		}
		prevstate = keystate;
#endif
	}


	leepra_close();
#ifdef MUSIC
	BASSMOD_Free();
#endif

	return 0;
}