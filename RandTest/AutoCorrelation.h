#pragma once

#pragma warning( disable : 4005 )
#include <stdint.h>
#pragma warning( default : 4005 )
#include "BitCount.h"
#include "ACBinomialChi2.h"
#include "Gamma.h"
#include "Chi2.hpp"

class CAutoCorrelation
{
public:
	CAutoCorrelation(void);
	~CAutoCorrelation(void);
	// Inserts a 32 bit word into the unit test
	void InsertWord32(uint32_t InWord32);

	// Resets current unit testing
	void ResetTest(void);
	// Resets cumulative scores
	void ResetAll(double* pTotalBiasCount, double* pBlockBiasCount);

	// Cumulative z-score
	double cumulativeACZScore[32];
    double prevCumulativeACZScore[32];

	// Keeps track of all blocks in cumulative testing
	double totalBlockCount;

	// P-values of cumulative chi^2 tests
	double P_Chi2[32];

	// Highest order to be tested
	int maxOrder;

protected:
	// History bit stream of last 32 plus current 32 bits (=64bits)
	uint64_t bitStream;

    // First word in block to calc bias difference
//    uint32_t firstBlockWord32;

	// Counts up "ands" for a block = multiply in cross correlation
//	int blockAndCount[32];
    double blockXorCount[32];

	// Counts up cumulative "ands" for each AC order
    double totalXorCount[32];
    double totalAndCount[32];

	// Chi^2 test cumulative for each order
    void CreateChi2Tests();
    Chi2* MetaChi2[32];

	// Incomplete Gamma function
	CGamma Gamma;

	// Counts 32 bits words in current block test
	int blockWordCount;

	// To quickly count bits within 32 bit word
	CBitCount BitCount;

	// Pointer to the just calculated block bias one count
	double* pBlockBiasCount;
	// Pointer to the cumulative (including current block) one count
	double* pTotalBiasCount;
};
