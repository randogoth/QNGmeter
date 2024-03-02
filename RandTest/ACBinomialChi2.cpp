//#include "StdAfx.h"
#include "ACBinomialChi2.h"
#include "math.h"

CACBinomialChi2::CACBinomialChi2(void)
{
	InitPTable();
}

CACBinomialChi2::~CACBinomialChi2(void)
{
}

// Bins incomming values for future Chi2 test
void CACBinomialChi2::BinIt(double BinVal)
{
    int BinIndex = -1;
    if (BinVal <= 16090)
        BinIndex = 0;
    else if (BinVal >= 16679)
        BinIndex = 50;
    else {
        BinIndex = (int)BinVal - 16091;
        BinIndex /= 12;
        BinIndex += 1;
    }

    pBin[BinIndex] += 1.0;

    // update Total count
	Total += 1.0;
}

// Calculate cumulative chi^2
double CACBinomialChi2::Calc()
{
    double Sum = 0;
	for (int i=0; i<51; i++)
		Sum += (pBin[i] * pBin[i])/(Total * pTable[i]);

	return Sum-Total;
}

// Resets the cumulative test completely
void CACBinomialChi2::ResetAll(void)
{
	ZeroMemory(pBin, 51*sizeof(double));

	Total = 0;
}

// Initializes pTable
void CACBinomialChi2::InitPTable(void)
{
    pTable[0] = 0.0199136;
    pTable[1] = 0.00443703;
    pTable[2] = 0.00523732;
    pTable[3] = 0.00613872;
    pTable[4] = 0.00714366;
    pTable[5] = 0.00825334;
    pTable[6] = 0.00946896;
    pTable[7] = 0.0107857;
    pTable[8] = 0.0121988;
    pTable[9] = 0.0136985;
    pTable[10] = 0.0152738;
    pTable[11] = 0.0169106;
    pTable[12] = 0.0185897;
    pTable[13] = 0.0202911;
    pTable[14] = 0.021992;
    pTable[15] = 0.0236668;
    pTable[16] = 0.0252903;
    pTable[17] = 0.0268349;
    pTable[18] = 0.0282722;
    pTable[19] = 0.0295779;
    pTable[20] = 0.0307263;
    pTable[21] = 0.0316944;
    pTable[22] = 0.0324642;
    pTable[23] = 0.0330194;
    pTable[24] = 0.0333472;
    pTable[25] = 0.0334425;
    pTable[26] = 0.0333044;
    pTable[27] = 0.0329349;
    pTable[28] = 0.0323407;
    pTable[29] = 0.031536;
    pTable[30] = 0.0305358;
    pTable[31] = 0.0293616;
    pTable[32] = 0.0280353;
    pTable[33] = 0.026582;
    pTable[34] = 0.0250301;
    pTable[35] = 0.0234031;
    pTable[36] = 0.0217306;
    pTable[37] = 0.020037;
    pTable[38] = 0.018347;
    pTable[39] = 0.0166828;
    pTable[40] = 0.0150652;
    pTable[41] = 0.0135096;
    pTable[42] = 0.0120311;
    pTable[43] = 0.0106397;
    pTable[44] = 0.0093446;
    pTable[45] = 0.00814988;
    pTable[46] = 0.00705929;
    pTable[47] = 0.00607215;
    pTable[48] = 0.00518732;
    pTable[49] = 0.00440057;
    pTable[50] = 0.0200105;
}
