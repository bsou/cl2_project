#include <stdio.h>

#include "stat/GammaFunc.cpp"

int main(int argc, char *argv[]){
	
	double x = atof(argv[1]);

	printf("x:%lf T(x):%lf psi(x):%lf\n", x, GammaFunc::lngamma(x), GammaFunc::digamma(x));
}
