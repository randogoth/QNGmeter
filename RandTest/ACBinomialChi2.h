#pragma once
#include "BinomialChi2.h"

class CACBinomialChi2 :
	public CBinomialChi2
{
public:
	CACBinomialChi2(void);
	~CACBinomialChi2(void);
	// Overridden to initialze pTable for auto-correlation
	virtual void InitPTable(void);
	// Oberridden to calc index for ac binning
	virtual void BinIt(double BinVal);

    virtual double Calc();
    virtual void ResetAll();

private:
    double pBin[54];
    double pTable[54];
};
