#include <iostream>
#include <string>
#include <math.h>
#include <stdlib.h>
#include <assert.h>

using namespace std;

#ifndef GAMMA_H 
#define GAMMA_H 


class GammaFunc{

	public:

	static double lngamma(double x);
	static double digamma(double x);

};

double GammaFunc::lngamma(double x)
{
	assert(x > 0);
	static double c[6] = {76.18009172947146, -86.50532032941677, 24.01409824083091, -1.231739572450155, 0.1208650973866179e-2, -0.5395239384953e-5};
	double y = x;
	double tmp = x+5.5;
	tmp -= (x+0.5)*log(tmp);
	double ser = 1.000000000190015;
	for (int j=0; j<=5; j++) 
		ser += c[j]/++y;
	double loggamma = -tmp+log(2.5066282746310005*ser/x);
	assert(isfinite(loggamma));
	return loggamma;
}  // lngamma()



// digamma(x) returns the digamma(x) function, which is the derivative of log(Gamma(x))
double GammaFunc::digamma(double x) 
{
	assert(x > 0);
	double psix = 0;
	while (x <= 7) {   // bring x into a range we can compute accurately
		psix -= 1/x;
		++x;
	}
	double x1 = x-0.5;
	psix += log(x1);
	// add in correction
	double x2 = x1*x1;
	double x4 = x2*x2;
	double x8 = x4*x4;
	psix += (1.0/24.0)/x2-(7.0/960.0)/x4+(31.0/8064.0)/(x2*x4)-(127.0/30720.0)/x8+(511.0/67584.)/(x2*x8);
	assert(isfinite(psix));
	return psix;
}

#endif // GAMMA_H

