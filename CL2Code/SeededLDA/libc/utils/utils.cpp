#include <iostream>
#include <vector>
#include <stdlib.h>

using namespace std;
template <typename T> class utils{

	void swap(vector<T> &vList, int i, int j);
	int ReOrderAroundPivot(vector<T> &vList, int iStart, int iEnd, int iRand);
	T kthSmallest(vector<T> &vList, int k, int iStart, int iEnd);
	void QSort(vector<T> &vList, int iStart, int iEnd);

	public:
	T kthSmallest(vector<T> vList, int k);
	T median(vector<T> vList);
	void QSort(vector<T> &vList);

	/*
	void Print(vector<T> &vList){
		Print(vList,0,vList.size());
	}

	void Print(vector<T> &vList,int iStart, int iEnd){
		printf("List -- ");
		for(int i=iStart;i<iEnd;i++)
			printf("%d ", vList[i]);
		printf("\n");
	}
	*/
};

template <typename T> void utils<T>::swap(vector<T> &vList, int i, int j){
	T temp;
	temp = vList[i];
	vList[i] = vList[j];
	vList[j] = temp;
}

template <typename T> int utils<T>::ReOrderAroundPivot(vector<T> &vList, int iStart, int iEnd, int iRand){
	T pivot = vList[iRand];
	int iBegin = iStart;
	int iLowLimit = iBegin;
	int iUpperLimit = iEnd;
	//printf("\tarnd %d in [%d,%d] -- \n", pivot, iStart, iEnd);
	//Print(vList, iStart, iEnd+1);
	swap(vList, iStart, iRand);
	//Print(vList,iStart,iEnd+1);

	while(iStart < iEnd){
		while(iStart < iUpperLimit && vList[iStart] <= pivot)
			iStart++;
		while(iEnd > iLowLimit && vList[iEnd] > pivot)
			iEnd--;
		if(iStart < iEnd){
			//printf("Swap [%d]=%d and [%d]=%d\n", iStart, vList[iStart], iEnd, vList[iEnd]);
			swap(vList, iStart, iEnd);
			//iStart++;
			//iEnd--;
		}
	}
	//printf("\t\t%d , %d\n", iStart ,iEnd);
	int iReturn;
	/*
	if(iStart < iEnd){
		swap(vList, iStart, iBegin);
		iReturn = iStart;
	}else{
	*/
	if(vList[iEnd] <= vList[iBegin] && iBegin < iEnd){
		swap(vList, iEnd, iBegin);
		iReturn = iEnd;
	}else
		iReturn = iBegin;
	for(int i=iLowLimit;i<iReturn;i++)
		if(vList[i] > vList[iReturn]){
			fprintf(stderr, "** Order is broken\n");
			//fprintf(stderr, "Reorder arnd [%d]=%d in [%d,%d] -- \n", iReturn, pivot, iLowLimit, iUpperLimit+1);
			//Print(vList,iLowLimit,iUpperLimit+1);
			exit(1234);
		}
	for(int i=iReturn;i<=iUpperLimit;i++)
		if(vList[i] < vList[iReturn]){
			fprintf(stderr, "** Order is broken\n");
			//fprintf(stderr, "Reorder arnd [%d]=%d in [%d,%d] -- \n", iReturn, pivot, iLowLimit, iUpperLimit+1);
			//Print(vList,iLowLimit,iUpperLimit+1);
			exit(1234);
		}
	//printf("Returning %d\n", iReturn);
	//}
	//printf("\t\t");
	//Print(vList);
	return iReturn;
}

template <typename T> T utils<T>::kthSmallest(vector<T> &vList, int k, int iStart, int iEnd){
	int iNoElements = iEnd-iStart+1;	// including both ends
	if(k <= 0){
		fprintf(stderr, "Requesting invalid (%d) element\n", k);
		exit(1234);
	}

	if(k > iNoElements){
		fprintf(stderr, "Requesting for %d th element, while there are only %d = (%d-%d+1) elements\n", k, iNoElements, iEnd, iStart);
		exit(1234);
	}

	int iTemp = iStart + (int) (iNoElements * (rand() / (RAND_MAX + 1.0))); // 0 - iNoElements-1
	//Print(vList);
	//printf("%d in [%d,%d] -- reorder arnd %d \n", k, iStart, iEnd, vList[iTemp]);
	iTemp = ReOrderAroundPivot(vList, iStart, iEnd, iTemp);
	//printf("The reordered pivot at %d - (%d == %d)\n", iTemp, iTemp-iStart+1, k);

	if(iTemp-iStart+1 == k)
		return vList[iTemp];
	else if(iTemp-iStart < k)
		return kthSmallest(vList, k - (iTemp-iStart), iTemp, iEnd);
	else
		return kthSmallest(vList, k, iStart, iTemp-1);
}

template <typename T> void utils<T>::QSort(vector<T> &vList, int iStart, int iEnd){
	//printf("vList.size=%d iStart=%d iEnd=%d\n", vList.size(), iStart, iEnd);
	if(iStart >= iEnd)
		return;
	int iNoElements = iEnd-iStart+1;
	int iTemp = iStart + (int) (iNoElements * (rand() / (RAND_MAX + 1.0))); // 0 - iNoElements-1
	/*
	//int iTemp = 16; 
	//printf("[%d,%d] -- rand %d \n", iStart, iEnd, iTemp);
	T pivot = vList[iTemp];
	bool bBreak = false;
	for(int i=iStart;i<=iEnd;i++)
		printf("%d ", vList[i]);
	printf("\nChoose -- [%d]=%d\n",iTemp, vList[iTemp]);
	*/
	iTemp = ReOrderAroundPivot(vList, iStart, iEnd, iTemp);
	/*
	for(int i=iStart;i<iTemp;i++){
		printf("%d ", vList[i]);
		if(vList[i] > pivot){
			printf("*******");
			bBreak = true;
		}
	}
	printf("\n\t-- %d -- \n", vList[iTemp]);
	for(int i=iTemp+1;i<=iEnd;i++){
		printf("%d ", vList[i]);
		if(vList[i] < pivot){
			bBreak = true;
		}
	}
	printf("\ndone %d\n",bBreak);
	if(bBreak)
		exit(1234);
	*/
	QSort(vList, iStart, iTemp-1);
	QSort(vList, iTemp+1, iEnd);
}

template <typename T> void utils<T>::QSort(vector<T> &vList){
	QSort(vList, 0, vList.size()-1);
	for(int i=0;i<vList.size()-1;i++)
		if(vList[i] > vList[i+1]){
			fprintf(stderr, "List is not sorted\n");
			exit(1234);
		}
}

template <typename T> T utils<T>::kthSmallest(vector<T> vList, int k){
	return kthSmallest(vList, k, 0, vList.size()-1);
}

template <typename T> T utils<T>::median(vector<T> vList){
	int iSize = vList.size();
	if(iSize == 1)
		return vList[0];
	if(iSize % 2 ==0)
		return kthSmallest(vList, (int)iSize/2, 0, iSize-1);
	else
		return kthSmallest(vList, (int)iSize/2+1, 0, iSize-1);
}
