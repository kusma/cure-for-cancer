#ifndef FILE_H
#define FILE_H

class File{
protected:
	unsigned char* data;
	int len;
	unsigned char* pos;
public:
	File(unsigned char* data, int len);
	~File();
	void* GetFile();
	void Read(void* dest, int bytes);
	void Rewind();
	int GetSize();
	bool Eof();
};

class FileLoader{
protected:
	char* archive;
	char* password;
private:
	FileLoader();//{};
public:
	//FileLoader(char* archive);
	static FileLoader* instance();
	SetArchive(char* archive);
	SetPassword(char* password);
	File* GetFile(char* filename);
};

#endif //FILE_H