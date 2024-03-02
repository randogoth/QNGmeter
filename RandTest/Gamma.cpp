//#include "StdAfx.h"
#include "Gamma.h"
#include "math.h"
#include <stdlib.h>

CGamma::CGamma()
{

}

CGamma::~CGamma()
{

}

double CGamma::gammln(double xx)
{
	double	RetVal;

	double	y;
	double	x;
	double	tmp;
	double	ser;
	int		j;

	double	cof[6];

	cof[0] = 76.18009172947146;  cof[1] = -86.50532032941677;  cof[2] = 24.01409824083091;
	cof[3] = -1.231739572450155; cof[4] = 1.208650973866179e-3; cof[5] = -5.395239384953e-6;

	y = x = xx;

	tmp = (x+5.5) - (x+0.5)*(log(x+5.5));
	ser = 1.000000000190015;

	for ( j=0; j<=5; j++ )
	{
		y += 1.0;
		ser += cof[j]/y;
	}

	RetVal = log( 2.506628274631*ser/x ) - tmp;

	return RetVal;
}

double CGamma::gamser(double a, double x)
{
	double RetVal;

	double gln;
	double ap;
	double del;
	double sm;
	int ITMAX;
	int m;

	if ( x!=0 )
	{
		ITMAX = 31;
		gln = gammln( a );
		ap = a;
		del = sm = 1.0/a;
		
		for( m=1; m<=ITMAX; m++ )
		{
			ap += 1;
			del *= x/ap;
			sm += del;
		}

		RetVal = sm * exp( a*log(x) - x - gln );
	}
	else
		RetVal = 0;

	return RetVal;
}

double CGamma::gcf(double a, double x)
{
	double RetVal;

	double gln;
	double b;
	double c;
	double d;
	double h;
	double an;
	double del;
	double FPMIN;
	int	ITMAX;
	int i;

	gln = gammln( a );
	b = x + 1.0 - a;
	FPMIN = pow( 10., -30 );
	c = 1.0 / FPMIN;
	d = 1.0 / b;
	h = d;

	ITMAX = 30;
	for ( i=1; i<=ITMAX; i++ )
	{
		an = (-i) * (i-a);
		b += 2.0;
		d = an*d + b;
		if ( abs((int)d) < FPMIN ) d = FPMIN;
		c = b + an/c;
		if ( abs((int)c) < FPMIN ) c = FPMIN;
		d = 1.0/d;
		del = d * c;
		h *= del;
	}

	RetVal = 1.0 - (exp( a*log(x) - x - gln ) * h);

	return RetVal;
}

double CGamma::Gamma(double a, double chi2)
{
	double RetVal;

	double x;

	a = a / 2;
	x = chi2 / 2;

	if ( x<=a )
		RetVal = gamser( a, x );
	else
		RetVal = gcf( a, x );

	return 1-RetVal;
}

double CGamma::Beta(double z, double a, double b)
{
	double bt;

	if (z < 0.0)
		z = 0.0;
	if (z > 1.0)
		z = 1.0;

	if (z == 0.0 || z == 1.0)
		bt=0.0;
	else
		// Factors in front of the continued fraction.
		bt = exp( gammln(a+b) - gammln(a) - gammln(b) + a*log(z) + b*log(1.0-z) );

	if (z < (a+1.0)/(a+b+2.0)) // Use continued fraction directly.
		return bt*betacf(a, b, z)/a;
	else // Use continued fraction after making the symmetry transformation.
		return 1.0-bt*betacf(b, a, 1.0-z)/b;	
}

double CGamma::betacf(double a, double b, double x)
{
	int m,m2;
	double aa,c,d,del,h,qab,qam,qap;

	double MAXIT = 100;
	double EPS = 3.0e-7;
	double FPMIN = 1.0e-30;

	qab = a+b;
	qap = a+1.0;
	qam = a-1.0;

	c = 1.0; // First step of Lentz's method.
	d = 1.0 - qab*x/qap;

	if (fabs(d) < FPMIN)
		d = FPMIN;

	d = 1.0/d;
	h = d;

	for (m=1; m<=MAXIT; m++)
//	while (true)
	{
		m2 = 2*m;
		aa = m*(b-m)*x / ((qam+m2)*(a+m2));
		d = 1.0 + aa*d; // One step (the even one) of the recurrence.

		if (fabs(d) < FPMIN)
			d=FPMIN;

		c = 1.0 + aa/c;

		if (fabs(c) < FPMIN)
			c = FPMIN;

		d = 1.0/d;
		h *= d*c;
		aa = -(a+m)*(qab+m)*x / ((a+m2)*(qap+m2));
		d = 1.0 + aa*d; // Next step of the recurrence (the odd one).

		if (fabs(d) < FPMIN)
			d = FPMIN;

		c = 1.0 + aa/c;

		if (fabs(c) < FPMIN)
			c = FPMIN;

		d = 1.0/d;
		del = d*c;
		h *= del;

		if (fabs(del-1.0) < EPS) // Are we done?
			break;
	}

	return h;
}
