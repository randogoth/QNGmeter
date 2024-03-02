// OQSO - Overlapping-Quadruples-Sparse-Occupancy test
//   Expected values are mean = 141909.47 and standard deviation = 295
//   (G. Marsaglia and A. Zaman, Computers Math. Applic.,
//   Vol. 26, No. 9, pp 1-10, 1993)

#pragma once

#pragma warning( disable : 4005 )
#include <stdint.h>
#pragma warning( default : 4005 )
#include "MonkeyBitmap.h"
#include "Chi2.hpp"
#include "Gamma.h"
#include "Stat.h"

class CMonkey
{
public:
	CMonkey(void);
	~CMonkey(void);

	// Cumulative z-score
	double cumulativeZScore;

	// Total number of unit z-scores
	double totalBlockCount;

	// Cumulative chi^2 results
	double P_Chi2;

	// Submits a 32 bit word for OQSO testing
	void InsertWord32(uint32_t InWord32);
	// Resets cumulative and current test
	void ResetAll(void);

protected:
	// Resets current test
	void ResetTest(void);

    void CreateChi2Test();

	// Keeps track of missing words
	CMonkeyBitmap MonkeyBitmap;

	// Incomplete Gamma function
	CGamma Gamma;
	// Chi2 Test
	Chi2* MetaChi2;
	// Adds ZtoP transformation
	CStat Stat;

	// Missing word count
	uint32_t MissingWords;
	// Words (20 bit overlapped) tested
	uint32_t WordCount;

	// Actual wordstream
	uint64_t WordStream;
	// Wordstream 5-bit letter pointer 
	int LetterPointer;
	// Current 20-bit word under investigation
	uint32_t CurrentWord;

	// Sum of unit z-scores
	double ZScoreTotal;
	uint64_t MissingWordsTotal;

	// Stored value of 2^32
	double pow2_32;
};
