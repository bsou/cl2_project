#include <stdio.h>
#include <vector>
#include <stdlib.h>

#include "utils/utils.cpp"
#include "utils/Pair.cpp"

using namespace std;

void Temp(vector<int> &vList){
	for(int i=0;i<vList.size();i++)
		vList[i] /= 2;
}

int main(int argc,char *argv[]){
	
	vector<Pair<int,double> > vList;
	for(int i=0;i<10000;i++){
		double Temp = (1000 * (rand() / (RAND_MAX + 1.0)));
		if(i% 2 == 0)
			vList.push_back(Pair<int,double>(i,Temp));
		else
			vList.push_back(Pair<int,double>(i,-1*Temp));
	}
	printf("List has %d elements\n", vList.size());

	utils<Pair<int,double> > u;
	u.QSort(vList);
	
	for(int i=0;i<vList.size();i++){ 
		printf("%f ", vList[i].Second);
		if(i > 0 && vList[i].Second < vList[i-1].Second)
			printf("****\n");
	}
	printf("\n");
	//Temp(vList);
	//for(int i=0;i<vList.size();i++) printf("%d ", vList[i]);

	/*
	cout << endl;
	for(int i=0;i<vList.size();i++) printf("%d ", vList[i]);
	cout << endl;
	cout << "Smallest element is " << u.kthSmallest(vList, 1) << endl;
	
	cout << "List " ; 
	for(int i=0;i<vList.size();i++) printf("%d ", vList[i]);
	cout << endl;
	cout << "Median is " << u.median(vList) << endl;
	for(int i=0;i<vList.size();i++) printf("%d ", vList[i]);
		
	cout << "\n\n\nObserve from now\n\n\n" << endl;
	u.QSort(vList);
	for(int i=0;i<vList.size();i++) printf("%d ", vList[i]);
	cout << endl;
	*/

}
