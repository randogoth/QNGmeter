#pragma once

class CBinomialChi2
{
public:
	CBinomialChi2(void);
	~CBinomialChi2(void);

	// Bins incomming values for future Chi2 test
	virtual void BinIt(double BinVal);
	// Calculate cumulative chi^2
	double Calc(void);

	// Resets the cumulative test completely
	void ResetAll(void);

protected:
	// Positive side bins
	double pBinPositive[30];
	// Negative side bins
	double pBinNegative[30];
	// Total values binned
	double Total;

	// Table with bin probabilities
	double pTable[25];
	// Initializes pTable
	virtual void InitPTable(void);
};
