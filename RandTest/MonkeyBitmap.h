#pragma once

#pragma warning( disable : 4005 )
#include <stdint.h>
#pragma warning( default : 4005 )

class CMonkeyBitmap
{
public:
	CMonkeyBitmap(void);
	~CMonkeyBitmap(void);

	// Sets the entire bit memory map to 0's
	void Clearmap(void);
	// Check in Bitmap if this word has already been used, true if tested previously, false otherwise;
	bool CheckWord(uint32_t Word20Bit);

protected:
	// Bitmap
	uint32_t Word[32768];
};
