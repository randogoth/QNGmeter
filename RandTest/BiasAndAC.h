#pragma once

#pragma warning( disable : 4005 )
#include <stdint.h>
#pragma warning( default : 4005 )
#include "Bias.h"
#include "AutoCorrelation.h"

class CBiasAndAC
{
public:
	CBiasAndAC(void);
	~CBiasAndAC(void);
	// Inserts a 32 bit word into both Bias an AC tests
	void InsertWord32(uint32_t InWord32);

	// Bias test
	CBias Bias;
	// Auto-correlation test
	CAutoCorrelation AC;

	// Resets all in Bias and AutoCorrelation
	void ResetAll(void);
};
