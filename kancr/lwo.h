#ifndef LWO_H
#define LWO_H


// flytt funksjonene så de ikke blir inline..


#define CHUNK_FORM 0x4d524f46
#define CHUNK_TAGS 0x53474154
#define CHUNK_LAYR 0x5259414c // parser ikke denne nå
#define CHUNK_PNTS 0x53544e50
#define CHUNK_BBOX 0x584f4242 // parser ikke denne nå
#define CHUNK_POLS 0x534c4f50
#define CHUNK_PTAG 0x47415450 // parser ikke ennå
#define CHUNK_SURF 0x46525553 // parser ikke ennå

#define PARAMETER_LWO2 0x324f574c
#define PARAMETER_FACE 0x46414345
#define PARAMETER_SURF 0x46525553

class LwoObject:public Object{
private:
	File* file;
	Vector* points;
	unsigned int point_count;
	Face* polygons;
	unsigned int polygon_count;
	unsigned int* surf_indices;

	class Chunk{
	public:
		unsigned int tag;
		unsigned int len;
	};

	class SubChunk{
	public:
		unsigned int tag;
		unsigned short len;
	};

public:
	// endian-swap a 4byte integer
	void Swap(unsigned int& integer){
		integer =	((integer&0xFF000000)>>24)|
					((integer&0x00FF0000)>>8 )|
					((integer&0x0000FF00)<<8 )|
					((integer&0x000000FF)<<24);
	}

	void Swap(unsigned short& integer){
		integer =	((integer&0xFF00)>>8)|
					((integer&0x00FF)<<8 );
	}

	void ReadFormChunk( int len ){
		unsigned int filetype;
		file->Read(&filetype, 4);

#ifdef _DEBUG
		if( filetype != PARAMETER_LWO2 ){
			printf("Format is not supported\n");
		}else{
			printf("Format is supported\n");
		}
#endif

	}

	void ReadTagsChunk( int len ){
		char *string = new char[len+1];
		file->Read(string, len);
		string[len] = '\0';
#ifdef _DEBUG
		printf("Found material-name: %s\n", string);
#endif
		delete[] string;
	}

	void ReadPointsChunk( int len ){
		if(this->points!=NULL) delete this->points;
		this->points = new Vector[len/12];
		this->point_count = len/12;
		file->Read(this->points, len);

		for(int i=0; i< len/12; i++){
			Swap(*(unsigned int*)&this->points[i].x);
			Swap(*(unsigned int*)&this->points[i].y);
			Swap(*(unsigned int*)&this->points[i].z);
		}
#ifdef _DEBUG
		printf("Found vertices\n");
#endif
	}

	void ReadPtagChunk( int len ){
		unsigned int type;
		file->Read(&type, 4);
		len-=4;
		//unsigned char* temp = new unsigned char[len];
//		if( type != PARAMETER_SURF ){
			unsigned char string[5];
			memcpy( string, &type, 4);
			string[4]='\0';
#ifdef _DEBUG
			printf("Ptag not supported: \"%s\" (0x%x)\n", string, type);
#endif

			unsigned char* temp = new unsigned char[len];
			file->Read(temp, len);
			delete[] temp;
//		}else{
//			printf("ptag supported\n");
//			unsigned short* temp = new unsigned short[len];
//			file->Read(temp, len);
//			for(int i=0;i<len/4;i++){
//				printf("%u, %u\n", temp[i*2], temp[i*2+1]);
//			}
//			delete[] temp;
//		}
	}

	void ReadPolygonsChunk( int len ){
		unsigned int filetype;
		file->Read(&filetype, 4);
		len -= 4;
		if( filetype != PARAMETER_FACE ){
			if(this->polygons!=NULL) delete this->polygons;
			this->polygons = new Face[len/8];
			this->polygon_count = 0;
			for(int i=0; i< len/8; i++){
				unsigned short flags; //oops, ignored!
				file->Read(&flags, 2);
				unsigned short indices[3];
				Swap(flags);
				flags &= 0x03ff;
#ifdef _DEBUG
				if(flags!=3) printf("\nERROR: Object not triangulized!\n\n");
#endif

				file->Read(indices, 6);
				Swap(indices[0]);
				Swap(indices[1]);
				Swap(indices[2]);
				this->polygons[this->polygon_count].a = indices[0];
				this->polygons[this->polygon_count].b = indices[1];
				this->polygons[this->polygon_count].c = indices[2];
				if( this->polygons[this->polygon_count].a > this->point_count ||
					this->polygons[this->polygon_count].b > this->point_count ||
					this->polygons[this->polygon_count].c > this->point_count ){
#ifdef _DEBUG
					printf("\nERROR: Corrupted mesh!\n\n");
#endif
				}
				this->polygon_count++;
			}
#ifdef _DEBUG
			printf("Found polygons\n");
#endif
		}else{
#ifdef _DEBUG
			printf("only FACEs are supported\n");
#endif
		}
	}

	void HandleChunk( Chunk chunk ){
		switch((unsigned int)chunk.tag){
			case CHUNK_FORM:
				ReadFormChunk( chunk.len );
			break;

//			case CHUNK_TAGS:
//				ReadTagsChunk( chunk.len );
//			break;

//			case CHUNK_SURF:
//				ReadTagsChunk( chunk.len );
//			break;
//			case CHUNK_PTAG:
//				ReadPtagChunk( chunk.len );
//			break;

			case CHUNK_PNTS:
				ReadPointsChunk( chunk.len );
			break;
			case CHUNK_POLS:
				ReadPolygonsChunk( chunk.len );
			break;
			default:
				unsigned char* temp = new unsigned char[chunk.len];
				unsigned char string[5];
				memcpy( string, &chunk.tag, 4);
				string[4]='\0';
#ifdef _DEBUG
				printf("Unhandled chunk \"%s\" (0x%x) with a size of %u bytes\n",
							string, (unsigned int)chunk.tag, chunk.len);
#endif
				file->Read(temp, chunk.len);
				delete[] temp;
			break;
		}
	}

	void ReadChunk(){
		Chunk chunk;
		file->Read(&chunk, sizeof(Chunk));
		Swap( (unsigned int)chunk.len );

		HandleChunk(chunk);
	};

	LwoObject(char* filename){
		this->points = NULL;
		this->point_count = 0;
		this->polygons = NULL;
		this->polygon_count = 0;

		file = FileLoader::instance()->GetFile(filename);
		if( file==NULL ){
#ifdef _DEBUG
			printf("file not found: %s\n", filename);
#endif
			return;
		}

		while(!file->Eof()) ReadChunk();

		Mesh* mesh = new Mesh();
		mesh->vertex_list = mesh->current_vertex_list = new Vertex[this->polygon_count];

		for(int i=0; i<this->point_count; i++){
			mesh->vertex_list[i].point = this->points[i];
			mesh->vertex_list[i].u = 0;
			mesh->vertex_list[i].v = 0;
		}
		mesh->vertex_count = this->point_count;

		mesh->face_list = this->polygons;
		mesh->face_count = this->polygon_count;
		this->AddMesh( *mesh );

		Material* m=new Material;
		m->diffuse = 0xFFFFFFFF;
		m->render_flag = RENDER_FLAT;
		m->double_sided  = false;// true;
		mesh->SetMaterial( *m );
		delete file;
	}
};


#endif // LWO_H