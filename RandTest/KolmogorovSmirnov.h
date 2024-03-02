#pragma once

#include <math.h>

class CKolmogorovSmirnov
{
public:
	CKolmogorovSmirnov();

public:
	double ZtoP( double zscore );
	double PtoZ( double p );
	void KSUP( double* pKn_pos, double* pKn_neg, double* U, unsigned long n );
	double lnf( double xx );
	double lnbin( double n, double k );
	double KSProb( unsigned long n, double Kn );

private:
	static int KSUPcompare( const void* elem1, const void* elem2 );
};