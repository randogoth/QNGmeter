//#include "StdAfx.h"
#include "MonkeyBitmap.h"
#include <memory.h>

CMonkeyBitmap::CMonkeyBitmap(void)
{
	Clearmap();
}

CMonkeyBitmap::~CMonkeyBitmap(void)
{
}

// Sets the entire bit memory map to 0's
void CMonkeyBitmap::Clearmap(void)
{
	memset( Word, 0, 131072 );
}

// Check in Bitmap if this word has already been tested, true if tested previously, false otherwise;
bool CMonkeyBitmap::CheckWord(uint32_t Word20Bit)
{
	bool bRet = false;

	uint32_t Index;
	uint32_t BitMask;

	// Find the index in an array of 32 bit words
	Index = Word20Bit / 32;
	// Now find the slot of the remainder
	BitMask = 1<<(Word20Bit%32);

	// Check if bit-slot already filled
	if ( BitMask & Word[Index] )
		bRet = true;
	// Fill bit-slot for future
	Word[Index] |= BitMask;
	
	return bRet;
}
