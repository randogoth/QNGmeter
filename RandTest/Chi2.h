#pragma once

class CChi2
{
public:
	CChi2(void);
	~CChi2(void);

	// Clear Chi2 test and set number of bins to use
	void Init(double nBins=32.);
	// Inserts a probability and calculates new chi^2
	double CalcChi2(double pValue);

protected:
	double Bin[65536];
	double SumBinsSquared;
	double pBin;	// Probability of falling into any given bin
	double nBins;	// Total number of bins
	double nCount;	// Number of pValues binned
};
