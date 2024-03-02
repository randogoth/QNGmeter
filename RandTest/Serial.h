#pragma once

#pragma warning( disable : 4005 )
#include <stdint.h>
#pragma warning( default : 4005 )
#include "Gamma.h"
#include "Chi2.hpp"
#include "stdio.h"

class CSerial
{
public:
	CSerial(void);
	~CSerial(void);
	// Inserts a 32 bit word into the serial test
	void InsertWord32(uint32_t InWord32);

	// Resets cumulative testing
	void ResetAll(void);
	// Resets current block test
	void ResetTest(void);

	// Serial test cumulative result;
	double cumulativeSerialChi2;

	// Keeps track of all block in cumulative results
	double totalBlockCount;

	// Cumulative chi^2 results
	double P_Chi2;

protected:
	// Wordstream
	uint64_t wordStream;
	// Pointer to current bit in wordStream
	int bitPointer;

	// Counts the number of 8/7 bit words binned in this block
	uint32_t blockWordCount;

	// Binning for 8-bit words
	double Bin8[256];
	// Binning for 7-bit words
	double Bin7[128];

	// Cumulative binning for 8-bit words
	double CumulativeBin8[256];
	// Cumulative binning for 7-bit words
	double CumulativeBin7[128];

	// Returns sum of the bis squared for the block 8bit words
	double BlockSumBinsSquared8(void);
	// Returns sum of the bis squared for the block 7bit words
	double BlockSumBinsSquared7(void);

	// Meta chi^2 test
	Chi2* MetaChi2;
    void CreateChi2Test();

	// Incomplete gamma function
	CGamma Gamma;

	// Adds new block to cumulative bins 8 bit words
	double CumulativeBins8(void);

	// Adds new block to cumulative bins 7 bit words
	double CumulativeBins7(void);

	double QuickCompare8(void);
	double QuickCompare7(void);
	double QuickCompare88(void);
	double QuickCompare77(void);
};
