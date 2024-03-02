//#include "StdAfx.h"
#include "KolmogorovSmirnov.h"
#include "memory.h"
#include "stdlib.h"
#include "math.h"
#include <float.h>

CKolmogorovSmirnov::CKolmogorovSmirnov()
{
}


///////////////////////////////////////////////////////////////////////////////
// double CKolmogorovSmirnov::ZtoP
//
// Returns: cumulative normal distribution value based on input z-score
// Accuracy better than 1% for zscore<=±7.5; better than 0.05% for zscore=±4
double CKolmogorovSmirnov::ZtoP( double zscore )
{
	double retval;

	// calculation variables
	double w;
	double y;
	double t;
	double num;
	double denom;

	// calculation constants
	double c[8];
	c[1] = 2.506628275;    c[2] = 0.31938153;     c[3] = -0.356563782;    c[4] = 1.781477937;
	c[5] = -1.821255978;   c[6] = 1.330274429;    c[7] = 0.2316419;

	w = (zscore>=0)? 1 : -1;
	y = 1./( 1. + (c[7]*w*zscore) );
	t = 1. + (c[7]*w*zscore);

	num = w * ( 0.5 - c[2] + ((c[6] + (c[5]*t) + (c[4]*t*t) + (c[3]*t*t*t))/(t*t*t*t)) );
	denom = c[1] * pow( 10, pow(0.5*zscore, 2) );

	retval = 0.5 + (num/denom);

	return retval;
}


///////////////////////////////////////////////////////////////////////////////
// double CKolmogorovSmirnov::PtoZ
//
// Returns: z-score based on input cumulative normal distribution value
// Accuracy better than 0.1% for zscore<=±7.5; 6 digits for zscore=±6
double CKolmogorovSmirnov::PtoZ( double p )
{
	double retval = -8.2;
    if (p <= DBL_EPSILON)
        return retval;

	// calculation variables
	double pp;
	double y;
	double num;
	double denom;

	// calculation constants
	double P[5];
	P[0] = -0.322232431088;    P[1] = -1.0;    P[2] = -0.342242088547;
	P[3] = -0.0204231210245;    P[4] = -0.453642210148e-4;
	double q[5];
	q[0] = 0.099348462606;    q[1] = 0.588581570495;    q[2] = 0.531103462366;
	q[3] = 0.10353775285;    q[4] = 0.38560700634e-2;

	pp = (p<0.5)? p : (1.-p);
	y = sqrt( log(1./(pp*pp)) );
	num   = y*(y*(y*(y*P[4]+P[3]) + P[2]) + P[1]) + P[0];
	denom = y*(y*(y*(y*q[4]+q[3]) + q[2]) + q[1]) + q[0];

	retval = y + (num/denom);
	retval = (p<0.5)? -retval : retval;

	return retval;
}


void CKolmogorovSmirnov::KSUP( double* pKn_pos, double* pKn_neg, double* U, unsigned long n )
{
	unsigned long i;	// universal index
	double val;			// calc value
	double* Us = new double[n];
	double Kn_pos;
	double Kn_neg;
	double* cump1 = new double[n+2];
	double* cump2 = new double[n+2];
	double* kn = new double[n+2];
	double max;
	double min;
	
	memcpy( (void*)Us, (void*)U, n * sizeof(double) );
	qsort( Us, n, sizeof(double), KSUPcompare );

	cump1[1] = Us[0];
	cump2[1] = 0.;
	for ( i=1; i<=n; i++ )
	{
		cump1[i+1] = Us[i-1];
		val = (double)i/n;
		cump2[i+1] = val;
	}

	min = cump2[2] - cump1[2];
	max = min;
	for ( i=2; i<=(n+1); i++ )
	{
		kn[i-1] = cump2[i] - cump1[i];
		if ( kn[i-1] > max )
		{
			max = kn[i-1];
		}
		if ( kn[i-1] < min )
		{
			min = kn[i-1];
		}
	}

	Kn_pos = sqrt((double)n) * (double)max;
	Kn_neg = (-sqrt((double)n)) * ((double)min - (1/(double)n));

	*pKn_pos = KSProb( n, Kn_pos );
	*pKn_neg = KSProb( n, Kn_neg );

	if (Us!=NULL)
	{
		delete Us;
		Us = NULL;
	}

	if (kn!=NULL)
	{
		delete kn;
		kn = NULL;
	}

	if (cump1!=NULL)
	{
		delete cump1;
		cump1 = NULL;
	}

	if (cump2!=NULL)
	{
		delete cump2;
		cump2 = NULL;
	}
}


double CKolmogorovSmirnov::KSProb( unsigned long n, double Kn )
{
	double retval;
	double e;
	double las;
	unsigned long j;
	double cof[7];

	cof[1] = 76.18009172947146;  cof[2] = -86.50532032941677;  cof[3] = 24.01409824083091;
	cof[4] = -1.231739572450155;  cof[5] = 1.208650973866179e-3;  cof[6] = -5.395239384953e-6;

	if ( Kn<=0 )
	{
		retval = 0;
	}
	else if ( Kn>=sqrt((double)n) )
	{
		retval = 1;
	}
	else
	{
		e = Kn / sqrt((double)n);
		las = floor( (double)n - ((double)n*e) );

		retval = 0;
		for ( j=0; j<=las; j++ )
		{
			double ee = exp(lnbin(n, j));
			double p1 = pow((e+(double)j/(double)n),((double)j-1.));
			double p2 = pow((1.0-e-(double)j/(double)n),(double)(n-j));
			retval +=  ee * p1 * p2;
		}

		retval = 1.-e*retval;
	}

	return retval;
}

double	CKolmogorovSmirnov::lnbin( double n, double k )
{
	double retval = 0;
	if (k==0)
		retval = 0;
	else
	{
		retval = lnf(n) - lnf(k) - lnf(n-k);
	}

	return retval;
}


double	CKolmogorovSmirnov::lnf( double xx )
{
	double retval = 0;
	double x1 = xx + 1.;

	double cof[7];

	cof[1] = 76.18009172947146;  cof[2] = -86.50532032941677;  cof[3] = 24.01409824083091;
	cof[4] = -1.231739572450155;  cof[5] = 1.208650973866179e-3;  cof[6] = -5.395239384953e-6;

	if (x1<=1.)
		retval = 0;
	else
	{
		double x = x1;
		double y = x1;

		double tmp = x + 5.5 - (x+.5)*log(x+5.5);
		double ser = 1.000000000190015;

		for (int j=0; j<=5; j++)
		{
			y = y+1.;
			ser += cof[j+1]/y;
		}

		retval = log(2.506628274631 * ser/x) - tmp;
	}

	return retval;
}

int CKolmogorovSmirnov::KSUPcompare( const void* elem1, const void* elem2 )
{
	if ((*(double*)elem1)==(*(double*)elem2))
		return 0;
	if ((*(double*)elem1)>(*(double*)elem2))
		return 1;
	else
		return -1;
}
