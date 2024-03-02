#pragma once

#pragma warning( disable : 4005 )
#include <stdint.h>
#pragma warning( default : 4005 )

class CBitCount
{
public:
	CBitCount(void);
	~CBitCount(void);

	// Get bitcount for this 32 bit word
//	__forceinline uint8_t GetCount32(uint32_t InWord32) {
	uint8_t GetCount32(uint32_t InWord32) {
//	    PUSHORT pWordDiv16 = (PUSHORT)&InWord32;
        return (Table16[(uint16_t)InWord32] + Table16[(uint16_t)(InWord32>>16)]);
    }

private:
	// Table to keep bit counts of 16 bit values
	static uint8_t Table16[65536];
	static bool IsInitialized;
};
