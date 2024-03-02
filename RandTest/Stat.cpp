//#include "StdAfx.h"
#include "Stat.h"
#include "math.h"

CStat::CStat(void)
{
}

CStat::~CStat(void)
{
}

// Input z-score, returns cumulative normal distribution value
// Accuracy better than 1% to z=+/-7.5; .05% to z=+/-4.
double CStat::ZtoP( double zScore )
{
	double retval;

	// calculation variables
	double w;
	double y;
	double t;
	double num;
	double denom;

	// check
	if ( zScore > 8. )
	{
		zScore = 8.;
	}
	else 
	{
		if ( zScore < -8. )
			zScore = -8.;
	}


	// calculation constants
	double c[8];
	c[1] = 2.506628275;    c[2] = 0.31938153;     c[3] = -0.356563782;    c[4] = 1.781477937;
	c[5] = -1.821255978;   c[6] = 1.330274429;    c[7] = 0.2316419;

	w = (zScore>=0)? 1 : -1;
	t = 1. + (c[7]*w*zScore);
	y = 1./t;

	num = c[2] + (c[6] + (c[5]*t) + (c[4]*t*t) + (c[3]*t*t*t)) / (t*t*t*t) ;
	denom = c[1] * exp( .5*zScore*zScore ) * t;

	retval = 0.5 + w * ( .5 - (num/denom) );

	return retval;
}
