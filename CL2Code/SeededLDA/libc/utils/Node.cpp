#include <iostream>

#ifndef NODE_H_ 
#define NODE_H_ 

template <typename T> class Node{
	public:
	T First;
	Node<T> *Next;

	public:
	Node(T aFirst){
		First = aFirst;
		Next = NULL;
	}

	Node() { Next = NULL;};
};

#endif
