#ifndef SORTLIST_H
#define SORTLIST_H
//template <class T> class SortList

#include <windows.h>


#define DEFAULT_MAX_NODES 10000

template <typename T> class RadixSorter{
public:
	typedef struct{
		unsigned int sort_key;
		T* data;
	}SortNode;
	
	T* data;
	SortNode* unsorted_list;
	SortNode* sorted_list;
	unsigned int max_nodes;
	unsigned int node_count;

	inline RadixSorter( int max_nodes = DEFAULT_MAX_NODES ){
		this->unsorted_list = NULL;
		this->sorted_list = NULL;
		this->data = NULL;
		this->SetMax(max_nodes);
	}

	inline ~RadixSorter(){
		delete[] this->unsorted_list;
		this->unsorted_list = NULL;
		delete[] this->sorted_list;
		this->sorted_list = NULL;
		delete[] this->data;
		this->data = NULL;
	}

	inline void SetMax(int max_nodes){
		delete[] this->unsorted_list;
		delete[] this->sorted_list;
		delete[] this->data;
		this->max_nodes = max_nodes;
		this->node_count = 0;
		data = new T[max_nodes];
		unsorted_list = new SortNode[max_nodes];
		sorted_list = new SortNode[max_nodes];
	}

	inline void AddNode( T* data, unsigned int sort_key ){
		if(this->node_count<this->max_nodes){
			//this->data[node_count] = data;
			this->unsorted_list[node_count].sort_key = sort_key;
			this->unsorted_list[node_count].data = data;//&this->data[node_count];
			this->node_count++;
		}
	}
	inline void Clear(){
		this->node_count = 0;
	}

	inline void Radix( int byte ){
		unsigned int count[256];
		unsigned int* current = count;
		int counter;

		for( counter=256; counter; counter--, current++) *current = 0;

		unsigned char* current_key = (unsigned char*)this->unsorted_list+byte;
		for( counter=this->node_count; counter; counter--, current_key+=sizeof(SortNode) ){
			count[*current_key]++;
		}

		int offset=0;
		int temp;
		current = count;
		for( counter=256; counter; counter--, current++){
			temp = *current;
			*current = offset;
			offset+= temp;
		}

		current_key = (unsigned char*)this->unsorted_list+byte;
		SortNode* current_data = this->unsorted_list;
		current = count;
		for( counter=this->node_count; counter; counter--, current_data++, current_key+=sizeof(SortNode) ){
			this->sorted_list[ count[*current_key]++ ] = *current_data;
		}
	}

	inline void FlipLists(){
		SortNode* temp = this->unsorted_list;
		this->unsorted_list = this->sorted_list;
		this->sorted_list = temp;
	}

	inline void SortList(){
		this->Radix(0);
		this->FlipLists();
		this->Radix(1);
		this->FlipLists();
		this->Radix(2);
		this->FlipLists();
		this->Radix(3);
/*
#ifdef _DEBUG
		int old=0;
		for(int i=0;i<this->node_count;i++){
			if( old>this->sorted_list[i].sort_key ) printf("sort error\n");
			old = this->sorted_list[i].sort_key;
		}
#endif
*/
	}
};

#endif //SORTLIST_H