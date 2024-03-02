#include "StdAfx.h"
#include "acbinomialchi2.h"
#include "math.h"

CACBinomialChi2::CACBinomialChi2(void)
{
	InitPTable();
}

CACBinomialChi2::~CACBinomialChi2(void)
{
}

// Overridden to initialze pTable for auto-correlation
VOID CACBinomialChi2::InitPTable(void)
{
	// 2^16 auto-correlation 
	pTable[0] = 0.0436123510244955321;
	pTable[1] = 0.0494945236349887629;
	pTable[2] = 0.0483966799920995590;
	pTable[3] = 0.0465904838376547746;
	pTable[4] = 0.0441572550221125783;
	pTable[5] = 0.0412031166704026490;
	pTable[6] = 0.0378513315493016954;
	pTable[7] = 0.0342338133652554904;
	pTable[8] = 0.0304826229422669920;
	pTable[9] = 0.0267222012692252800;
	pTable[10] = 0.0230629500401081457;
	pTable[11] = 0.0195965713435074457;
	pTable[12] = 0.0163933532864769463;
	pTable[13] = 0.0135013695454263963;
	pTable[14] = 0.0109473753452615719;
	pTable[15] = 0.0087390490108419405;
	pTable[16] = 0.0068681557366127721;
	pTable[17] = 0.0053141974233424296;
	pTable[18] = 0.0040481501288843967;
	pTable[19] = 0.0030359644605488801;
	pTable[20] = 0.0022415976474860848;
	pTable[21] = 0.0016294434474942837;
	pTable[22] = 0.0011661147954655219;
	pTable[23] = 0.0008216056587517001;
	pTable[24] = 0.0016958983342359457;
}

// Oberridden to calc index for ac binning
VOID CACBinomialChi2::BinIt(double BinVal)
{
	// center bin (7 slots for ac)
	int BinIndex = 0;
	int Side = 1;	// middle bin and right side

	// calc for autocorrelation
	if ((BinVal<-3.) || (BinVal>3.))
		BinIndex = 1 + (abs((int)BinVal)-4)/8;	// 8 slots per bin

	if (BinVal<-3.)
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
