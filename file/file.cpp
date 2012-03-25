#include <windows.h>
#include "file.h"
#include "urarlib.h"
#include <stdlib.h>
#include <stdio.h>

File::File(unsigned char* data, int len){
	this->data = data;
	this->len = len;
	this->Rewind();
}

File::~File(){
	delete[] this->data;
	this->data = NULL;
	this->len = 0;
}

void File::Read(void* dest, int bytes){
	memcpy(dest, this->pos, bytes);
	this->pos+=bytes;
}

int File::GetSize(){
	return this->len;
}

void* File::GetFile(){
	return (void*)this->data;
}

void File::Rewind(){
	this->pos = this->data;
}

bool File::Eof(){
	if( !(this->pos<(this->data+this->len)) ) return true;
	return false;
}

FileLoader* FileLoader::instance(){
	static FileLoader instance;
	return &instance;
};


FileLoader::FileLoader(){
	this->archive = "";
	this->password = "";
}

#ifdef _DEBUG
void print_list(ArchiveList_struct *list){
  int  ch;
  printf("                   Name     Size  Packed   OS  FileTime    ");
  printf("CRC-32 Attr Ver Meth\n");
  printf("     ------------------------------------------------------");
  printf("--------------------\n");

  while(list != NULL){
    if(list->item.NameSize < 23)
    {
      printf("%23s", list->item.Name);
    } else
    {
      printf("%23s", "[name too long]");
    }
    printf("%9d",  list->item.UnpSize);
    printf("%8d",  list->item.PackSize);
    printf("%5d",  list->item.HostOS);
    printf("%10x", list->item.FileTime);
    printf("%10x", list->item.FileCRC);
    printf("%5d",  list->item.FileAttr);
    printf("%4d",  list->item.UnpVer);
    printf("%5d",  list->item.Method);
    printf("\n");

    (ArchiveList_struct*)list = (ArchiveList_struct*)list->next;
  }
  printf("\n");
}
#endif


void FileLoader::SetArchive(char* archive){
	this->archive = archive;
#ifdef _DEBUG
	ArchiveList_struct *List = NULL;
	int count;
	count = urarlib_list(archive, (ArchiveList_struct*)&List);
	printf("%d files(s) found.\n\n", count);
	print_list( List );
#endif
}

void FileLoader::SetPassword(char* password){
	this->password = password;
}

File* FileLoader::GetFile(char* filename){
	void* data;
	unsigned long size;

#ifdef _DEBUG
	if(archive==""){
		printf("no archive set\n");
	}
#endif

	if(	!urarlib_get(&data, &size, filename, archive, password) ){
#ifdef _DEBUG
		printf("file not found in archive, fallback to disk\n");
#endif
		char buffer[256];
		sprintf(buffer,"data/%s", filename);

		FILE* fp = fopen(buffer, "rb");
		if(fp==NULL){
#ifdef _DEBUG
			printf("file not on disk either\n");
#endif
			return NULL;
		}

		fseek(fp, 0, SEEK_END);
		size = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		data = new unsigned char[size];
		fread( data, sizeof(unsigned char), size, fp );
		
		fclose(fp);
	}else{
#ifdef _DEBUG
		printf("file found in archive: %s, %u bytes big\n", filename, size);
#endif
	}

	return new File((unsigned char*)data, size);	
}
