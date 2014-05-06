#include <iostream>
#include <string>
#include <math.h>
#include <stdlib.h>

using namespace std;

#ifndef GAMMA_DIST_H 
#define GAMMA_DIST_H 

#define PI 3.14159265358979323846264338328
#define E 2.71828182845904523536028747135


class GammaDistribution{

	string m_className;
	double m_Order;
	int m_iOrder;
	bool m_bOrderSet;

	void init(){
		m_className = "GammaDistribution";
		srand(3);
	}

	double _gamma_int(int iOrder); 
	double _gamma_frac(double dFrac);
	double _rand_(bool bNonZero);

	public:

	GammaDistribution(){
		init();
		m_bOrderSet = false;
	}

	GammaDistribution(double dOrder){
		init();
		SetOrder(dOrder);
	}
	void SetOrder(double dOrder);
	double Sample(double dScale);

};

double GammaDistribution::_rand_(bool bNonZero=false){
	string m_fName = m_className + ":_rand_";
	double dRand;
	if(bNonZero = false)
		dRand = rand()/RAND_MAX;
	else{
		do{
			dRand = (double)rand()/RAND_MAX;
		}while( dRand == 0);
	}
	if(dRand < 0 || dRand > 1)
		fprintf(stderr, "%s returning a random variable which is out of bounds\n", m_fName.c_str());
	return dRand;
}

double GammaDistribution::_gamma_int(int iOrder){
	string m_fName = m_className + ":_gamma_int";

	if(iOrder < 12){
		double dReturn = 0;
		for(int i=0;i<iOrder;i++)
			dReturn += -1 * log(_rand_(true));
		return dReturn;
	}else{
		double dSqrt = sqrt(2*m_iOrder-1);
		double x,y,v;
		do{
			do{
				y = tan(PI * _rand_()); //CORE::rand());
				x = dSqrt * y + m_iOrder-1;	//$x = $sqrt * $y + $order - 1;
			} while (x <= 0);
			v = _rand_(); //CORE::rand();
		} while (v > (1+y*y) * exp((m_iOrder-1) * log(x/(m_iOrder-1)) - dSqrt*y));
		return x;
	}
}

double GammaDistribution::_gamma_frac(double dFrac){
	string m_fName = m_className + ":_gamma_frac";

	double p = E/(E+dFrac), q, x, u, v;
	do{
		u = _rand_();
		v = _rand_(true);
		if(u < p){
			x = exp(log(v)/dFrac);
			q = exp(-1 * x);
		}else{
			x = 1 - log(v);
			q = exp((dFrac-1) * log(x));
		}
	}while(_rand_() >= q); 
	return x;
}

void GammaDistribution::SetOrder(double dOrder){
	string m_fName = m_className + ":"+ "SetOrder";
	if(dOrder <= 0){
		fprintf(stderr, "%s please input a positive order (not %lf)\n", m_fName.c_str(), dOrder);
		exit(1234);
	}
	m_Order = dOrder;
	m_iOrder = (int)floor(dOrder);
	m_bOrderSet = true;
}

double GammaDistribution::Sample(double dScale = 1.0){
	string m_fName = m_className + ":" + "Sample";
	if(m_bOrderSet == false){
		fprintf(stderr, "%s Please set the order first using SetOrder\n", m_fName.c_str());
		return -1.0;
	}

	//printf("%s Order:%lf iOrder:%d\n", m_fName.c_str(), m_Order, m_iOrder);
	if(m_Order == m_iOrder)
		return dScale * _gamma_int(m_iOrder);
	else if(m_iOrder == 0)
		return dScale * _gamma_frac(m_Order);
	else
		return dScale * (_gamma_int(m_iOrder) + _gamma_frac(m_Order-m_iOrder));
}

#endif

#ifndef DIRICHLET_DIST_H 
#define DIRICHLET_DIST_H 

class DirichletDistribution{

	string m_className;
	double *m_dOrder;
	int m_iDim;
	bool m_bOrderSet;
	GammaDistribution m_dGamma;

	void init(){
		m_className = "DirichletDistribution";
		m_bOrderSet = false;
		m_dOrder = NULL;
	}

	public:
	
	DirichletDistribution() {
		init();
	}

	// Uniform priors
	DirichletDistribution(double dOrder, int iSize){
		init();
		SetOrder(dOrder,iSize);
	}

	DirichletDistribution(double *dOrder, int iSize) {
		init();
		SetOrder(dOrder,iSize);
	}

	void SetOrder(double dOrder, int iSize);	// Uniform priors
	void SetOrder(double *dOrder, int iSize);
	double *Sample();

};

void DirichletDistribution::SetOrder(double dOrder, int iSize){
	string m_fName = m_className + ":SetOrder";
	if(iSize <= 0){
		fprintf(stderr, "%s No Dimensions (%d) is not valid\n", m_fName.c_str(), iSize);
		return;
	}
	m_iDim = iSize;
	if(m_dOrder != NULL)
		free(m_dOrder);
	m_dOrder = (double *)calloc(m_iDim,sizeof(double));
	for(int i=0;i<m_iDim;i++)
		m_dOrder[i] = dOrder;
	m_bOrderSet = true;
	return;
}

void DirichletDistribution::SetOrder(double *dOrder, int iSize){
	string m_fName = m_className + ":SetOrder";
	if(iSize <= 0){
		fprintf(stderr, "%s No Dimensions (%d) is not valid\n", m_fName.c_str(), iSize);
		return;
	}
	m_iDim = iSize;
	if(m_dOrder != NULL)
		free(m_dOrder);
	m_dOrder = (double *)calloc(m_iDim,sizeof(double));
	for(int i=0;i<m_iDim;i++)
		m_dOrder[i] = dOrder[i];
	m_bOrderSet = true;
	return;
}

double *DirichletDistribution::Sample(){
	string m_fName = m_className + ":Sample";
	if(m_bOrderSet == false){
		fprintf(stderr, "%s Please set the order using SetOrder\n", m_fName.c_str());
		return NULL;
	}else if(m_iDim <= 0){
		fprintf(stderr, "%s The dimensions became %d <= 0\n", m_fName.c_str(), m_iDim);
		return NULL;
	}
	double *dDist = (double *)calloc(m_iDim, sizeof(double));

	double dTotal = 0;
	for(int i=0;i<m_iDim;i++){
		m_dGamma.SetOrder(m_dOrder[i]);
		dDist[i] = m_dGamma.Sample();
		dTotal += dDist[i];
	}

	for(int i=0;i<m_iDim;i++)
		dDist[i] /= dTotal;
	return dDist;
}

#endif
