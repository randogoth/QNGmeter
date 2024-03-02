#pragma once

#pragma warning( disable : 4005 )
#include <stdint.h>
#pragma warning( default : 4005 )
#include "Gamma.h"
#include "Chi2.hpp"
#include "Stat.h"

class CEntropy
{
public:
	double P_Chi2;
	inline double fcoef(uint32_t i);
	double sd;
	double E;
	double H;
	double cumulativeZScore;
	double totalBlockCount;
	uint32_t blockWordCount;
	void InsertWord32(uint32_t InWord32);
	void Initialize(uint32_t BitCount, uint32_t qFactor);
	CEntropy();
	virtual ~CEntropy();

protected:
	CStat Stat;
	double pow2_32;
	CGamma Gamma;
	double ETotal;
	double ZScoreTotal;
	double Sum;
	uint32_t K;
	uint32_t nb;
	uint32_t Q;
	uint32_t BitCount;
	uint32_t tab[256];
	double CalcStandardDeviation(uint32_t K, uint32_t m);
    Chi2* MetaChi2;
    void CreateChi2Test(void);

public:
	// Reset cumulative test
	void ResetAll(void);
};
