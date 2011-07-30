#ifndef __AREBARE_H__
#define __AREBARE_H__


class Bitmap;

class AreBare
{
protected:
	//small hack, to avoid too much retyping
	int XRES, YRES;
	unsigned char *tabell1;
	unsigned char *tabell2;
	unsigned char *xtabell1;
	unsigned char *xtabell3;
	unsigned char *ytabell2;
	unsigned char *ytabell3;
	Bitmap *vpage1;

	float param1[6], param2[6];

	void calcsphere2(unsigned char *where1,unsigned char *where2,int xres,int yres,float zz);

	
public:	
	AreBare(int xres, int yres);
	~AreBare();


	void setparam1(int i, float val);
	void setparam2(int i, float val);
	void setparam1(float *val);
	void setparam2(float *val);

	void prag_first(Bitmap &where, Bitmap &texture, float time);
	void yiha(Bitmap &where, Bitmap &texture1, Bitmap &texture2, float time);


	static void pc16(void *dst, void *src, int num);
	static void pc15(void *dst, void *src, int num);
	static void pc24(void *dst, void *src, int num);


};
#endif