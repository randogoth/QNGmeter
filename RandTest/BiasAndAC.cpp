//#include "StdAfx.h"
#include "BiasAndAC.h"

CBiasAndAC::CBiasAndAC(void)
{
	ResetAll();
}

CBiasAndAC::~CBiasAndAC(void)
{
}

// Inserts a 32 bit word into both Bias an AC tests
void CBiasAndAC::InsertWord32(uint32_t InWord32)
{
	Bias.InsertWord32(InWord32);
	AC.InsertWord32(InWord32);
}

// Resets all in Bias and AutoCorrelation
void CBiasAndAC::ResetAll(void)
{
	Bias.ResetAll();
	AC.ResetAll(&Bias.totalBiasCount, &Bias.formerBlockBiasCount);
}
