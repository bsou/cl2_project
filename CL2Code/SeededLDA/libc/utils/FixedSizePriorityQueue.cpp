#include <iostream>
#include <queue>
#include <string>

#include "Pair.cpp"

using namespace std;

#ifndef FIXED_SIZE_PRIORITY_QUEUE_H_ 
#define FIXED_SIZE_PRIORITY_QUEUE_H_

template <typename T> class FixedSizePriorityQueue{
	int m_iSize;
	T pqSame;
	T pqReverse;

	public:
	FixedSizePriorityQueue(T qOrig, T qReverse, int iSize){

		pqSame = qOrig;
		pqReverse = qReverse;
		//Pair<string,int>::Compare pCompare(false);
		//Pair<T,U>::Compare givenOrder(bDecreasing);
		//*m_compare = compareObj;
		//pqSame = priority_queue<T,U,V>((*compareObj));
		//pqReverse = priority_queue<T,U,V>(-1*compareObj);
	}

	/*
	void push(const T &val){
		pqSame.push(val);
	}

	T top(){
		return pqSame.pop();
	}

	bool empty(){
		return pqSame.empty();
	}

	void pop(){
		pqSame.pop();
	}

	int size(){
		return pqSame.size();
	}
	*/
};

#endif

