#include "StdAfx.h"
#include "binomialchi2.h"
#include "math.h"

CBinomialChi2::CBinomialChi2(void)
{
	InitPTable();
}

CBinomialChi2::~CBinomialChi2(void)
{
}

// Bins incomming values for future Chi2 test
void CBinomialChi2::BinIt(double BinVal)
{
	// center bin (15 slots for 1/0)
	int BinIndex = 0;
	int Side = 1;	// middle bin and right side

	// calc Index for 1/0 balance
	if ((BinVal>32775.) || (BinVal<32761.))
		BinIndex = 1 + (abs((int)BinVal-32768)-8)/16;	// 16 slots per bin

	if (BinVal<32761)
		Side = -1;	// left side

	// tails of distribution fall in one bin 
	if (BinIndex>24)
		BinIndex = 24;

	if (Side>0)
	{	// fill in positive side
		pBinPositive[BinIndex]+=1.;
	}
	else
	{	// fill in negative side
		pBinNegative[BinIndex]+=1.;
	}

	// update Total count
	Total += 1.;
}

// Calculate cumulative chi^2
double CBinomialChi2::Calc()
{
	double Sum = (pBinPositive[0] * pBinPositive[0])/(Total * pTable[0]);

	for (int i=1; i<25; i++)
	{
		Sum += (pBinPositive[i] * pBinPositive[i])/(Total * pTable[i]);
		Sum += (pBinNegative[i] * pBinNegative[i])/(Total * pTable[i]);
	}

	return Sum-Total;
}

// Resets the cumulative test completely
void CBinomialChi2::ResetAll(void)
{
	ZeroMemory(pBinPositive, 30*sizeof(double));
	ZeroMemory(pBinNegative, 30*sizeof(double));

	Total = 0;
}

// Initializes pTable
void CBinomialChi2::InitPTable(void)
{
	// 2^16 1/0 bias distribution table
	pTable[0] = 0.0467242519481981447;
	pTable[1] = 0.0494717022548410683;
	pTable[2] = 0.0483507488411394182;
	pTable[3] = 0.0465235232774633227;
	pTable[4] = 0.0440722296391575050;
	pTable[5] = 0.0411036575708872835;
	pTable[6] = 0.0377414795440626524;
	pTable[7] = 0.0341177483099464753;
	pTable[8] = 0.0303644043131267252;
	pTable[9] = 0.0266055419211793493;
	pTable[10] = 0.0229510396552503468;
	pTable[11] = 0.0194919592594017396;
	pTable[12] = 0.0162978932289932796;
	pTable[13] = 0.0134162222471676280;
	pTable[14] = 0.0108730598645528967;
	pTable[15] = 0.0086755302334023398;
	pTable[16] = 0.0068149541705240074;
	pTable[17] = 0.0052705078207208266;
	pTable[18] = 0.0040129572987985991;
	pTable[19] = 0.0030081473713346360;
	pTable[20] = 0.0022200160266961827;
	pTable[21] = 0.0016130041110183372;
	pTable[22] = 0.0011538175281394167;
	pTable[23] = 0.0008125704223141780;
	pTable[24] = 0.0016751591157826984;
}
