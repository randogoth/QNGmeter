#include "StdAfx.h"
#include "chi2.h"

CChi2::CChi2(void)
{
	Init();
}

CChi2::~CChi2(void)
{
}

// Clear Chi2 test and set number of bins to use
void CChi2::Init(double nBins)
{
	nCount = 0;					// nCount
	this->nBins = nBins;		// nBins
	pBin = 1./nBins;			// pBin = 1/nBins
	SumBinsSquared = 0;			// SumBinsSquared
	memset( Bin, 0, 65536*4 );	// Zero all Bins
}

// Inserts a probability and calculates new chi^2
double CChi2::CalcChi2(double pValue)
{
	double np;
	double BinAdd;
	UINT BinIndex;

	nCount++;
	np = nCount*pBin;
	BinIndex = (UINT)(nBins * pValue);

	BinAdd = 2 * Bin[BinIndex] + 1;
	Bin[BinIndex]++;
	SumBinsSquared += BinAdd;

	return (SumBinsSquared/np - nCount);
}
