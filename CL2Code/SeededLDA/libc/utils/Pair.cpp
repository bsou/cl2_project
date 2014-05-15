#include <iostream>

#ifndef PAIR_H_ 
#define PAIR_H_

template <typename T, typename U> class Pair{
	public:
	T First;
	U Second;
	Pair<T,U> *Next;

	public:
	Pair(T aFirst,U aSecond){
		First = aFirst;
		Second = aSecond;
		Next = NULL;
	}

	Pair() {};
	Pair(const Pair<T,U> &);
	bool operator<(const Pair<T,U> &rhs){
		return Second < rhs.Second ? true : false;
	}

	bool operator>(const Pair<T,U> &rhs){
		return Second > rhs.Second ? true : false;
	}

	bool operator<=(const Pair<T,U> &rhs){
		return Second <= rhs.Second ? true : false;
	}

	bool operator>=(const Pair<T,U> &rhs){
		return Second >= rhs.Second ? true : false;
	}

	bool operator==(const Pair<T,U> &rhs){
		return Second == rhs.Second ? true : false;
	}

	class Compare{
		bool bDecreasing;

		public:
		Compare(bool bOrder){
			bDecreasing = bOrder;
		}

		bool operator()(const Pair<T,U> &lhs, const Pair<T,U> &rhs){
			if(bDecreasing)
				return lhs.Second < rhs.Second;
			else
				return lhs.Second > rhs.Second;
		}
	};
};

template <typename T, typename U> Pair<T,U>::Pair(const Pair<T,U> &aPair)
{
	First = aPair.First;
	Second = aPair.Second;
}

#endif

