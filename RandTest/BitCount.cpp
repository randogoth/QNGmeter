//#include "StdAfx.h"
#include "BitCount.h"

uint8_t CBitCount::Table16[65536];
bool CBitCount::IsInitialized = false;

CBitCount::CBitCount(void)
{
	// Initialize, if not already done so
	if (!IsInitialized)
	{
		for (int i=0; i<=65535; i++)
		{
			// Fill up table
			uint8_t BitCount = 0;
			for (int b=0; b<16; b++)
				BitCount += ((i>>b)&0x1);

			Table16[i] = BitCount;
		}

		IsInitialized = true;
	}
}

CBitCount::~CBitCount(void)
{
}

