#include <iostream>

#ifndef TRIPPLE_H_
#define TRIPPLE_H_

template <typename T, typename U, typename V> class Tripple{
	public:
	T First;
	U Second;
	V Third;

	public:
	Tripple() {};

	Tripple(T aFirst,U aSecond,V aThird){
		First = aFirst;
		Second = aSecond;
		Third = aThird;
	}

	Tripple(const Tripple<T,U,V> &);
	class Compare{
		bool bDecreasing;

		public:
		Compare(bool bOrder){
			bDecreasing = bOrder;
		}

		bool operator()(const Tripple<T,U,V> &lhs, const Tripple<T,U,V> &rhs){
			if(bDecreasing)
				return lhs.Third < rhs.Third;
			else
				return lhs.Third > rhs.Third;
		}
	};
};

template <typename T, typename U, typename V> Tripple<T,U,V>::Tripple(const Tripple<T,U,V> &aTripple)
{
	First = aTripple.First;
	Second = aTripple.Second;
	Third = aTripple.Third;
}

#endif
