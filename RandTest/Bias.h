// 1/0 Balance - expected value is p(0) = p(1) = 0.5

#pragma once

#pragma warning( disable : 4005 )
#include <stdint.h>
#pragma warning( default : 4005 )
#include "BitCount.h"
#include "Chi2.hpp"
#include "Gamma.h"

class CBias
{
	friend class CBiasAndAC;

public:
	CBias(void);
	~CBias(void);

	// Tests a 32 bit word
	void InsertWord32(uint32_t InWord32);

	// Resets all
	void ResetAll();

	// Cumulative z score
	double cumulativeBiasZScore;

	// Cumulative chi^2 results
	double P_Chi2;

	// Keeps track of all blocks in cumulative testing
	double totalBlockCount;

protected:
	// Counts 1-bits per block
	double blockBiasCount;
	// Cumulative bias 1-bit count
	double totalBiasCount;
	double formerBlockBiasCount;

	// Resets current test
	void ResetTest();

	// Quick 32 bit word bit count table lookup
	CBitCount BitCount;

	// Keeps track of words tested in block
	int blockWordCount;

	// Calculates the chi^2
    void CreateChi2Test();
    Chi2* MetaChi2;

	// Incomplete Gamma function
	CGamma Gamma;
};
