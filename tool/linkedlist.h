#ifndef LINKEDLIST_H
#define LINKEDLIST_H

/*
template <typename T> class ListNode{
public:
	T* data;
	int pos;
	ListNode<T>* next;
	ListNode<T>* prev;
};
*/

template <typename T> class LinkedList{
private:
	typedef struct __link{
	public:
		T* data;
		int pos;
		__link* next;
		__link* prev;
	}ListNode;

	ListNode* first;
	ListNode* last;
	ListNode* current;
	int node_count;
public:

	inline LinkedList(){
		this->node_count = 0;
		this->last = NULL;
		this->first = NULL;
		this->current = NULL;
	}

	inline ~LinkedList(){

	}

	inline void Add( T& link ){
		ListNode* temp = new ListNode;
		temp->data = &link;
		if( this->last==NULL )
			this->last = temp;
		else
			this->first->prev = temp;


		temp->pos = this->node_count++;
		temp->next = this->first;
		temp->prev = NULL;
		this->first = temp;
	}

	inline void AddBack( T& link ){
		ListNode* temp = new ListNode;
		temp->data = &link;
		if( this->first==NULL )
			this->first = temp;
		else
			this->last->next = temp;


		temp->pos = this->node_count++;
		temp->next = NULL;
		temp->prev = this->last;
		this->last = temp;
	}

	void Rewind(){
		this->current = this->first;
	}

	inline T* operator[](const int index) const{
		ListNode* temp = this->first;
		if( index > this->node_count ) return NULL;

		while ( temp!=NULL ){
			if( temp->pos == index ) return temp->data;
			temp=temp->next;
		}
		return NULL;
	}

	inline T* GetCurrent(){
		if(this->current!=NULL)	return (T*)this->current->data;
		else return NULL;
	}

	inline T* GetNext(){
		this->current=this->current->next;
		if(this->current!=NULL) return (T*)this->current->data;
		else{
			this->Rewind();
			return NULL;
		}
	}

	inline T* GetLast(){
		current = last;
		return last->data;
	}

	inline T* GetPrev(){
		this->current=this->current->prev;
		if(this->current!=NULL) return (T*)this->current->data;
		else{
			this->Rewind();
			return NULL;
		}
	}

	inline void RemoveCurrent(){
		if( this->current->next != NULL ) this->current->next->prev = this->current->prev;
		if( this->current->prev != NULL ) this->current->prev->next = this->current->next;
	}

	inline int GetNodeCount(){
		return node_count;
	}
};




#endif //LINKEDLIST_H
