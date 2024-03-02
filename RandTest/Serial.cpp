//#include "StdAfx.h"
#include "Serial.h"
#include "math.h"

//CRITICAL_SECTION cs;
//FILE* pFile;
//char filename[200];

CSerial::CSerial(void)
{
/*InitializeCriticalSection(&cs);
EnterCriticalSection(&cs);
unsigned short num = GetTickCount();
sprintf(filename, "STable_%i.txt", num);
LeaveCriticalSection(&cs);*/

for (int i=0; i<50; i++)
    CreateChi2Test();
	ResetAll();
	P_Chi2 = 0.5;
	cumulativeSerialChi2 = 127.3339;
}

CSerial::~CSerial(void)
{
    delete MetaChi2;
}

// Inserts a 32 bit word into the serial test
void CSerial::InsertWord32(uint32_t InWord32)
{
    wordStream >>= 32;
    wordStream |= (uint64_t)InWord32 << 32;

	// Shift through each bit to create a sub-word
	while ( bitPointer >= 1 )
	{
		// Take an overlapping 8 and 7 bit word from stream
		uint32_t Bit8Word = ((uint32_t)(wordStream>>bitPointer)) & 0x000000ff;
		uint32_t Bit7Word = Bit8Word>>1;

		// Stuff these words into independent bins for this test block
		Bin8[Bit8Word]++;
		Bin7[Bit7Word]++;

		// Every 8192 words binned do a block and cumulative calc
		if ( (++blockWordCount)>=(16*262144) )
		{
			// Calc chi^2 value for this block
			double BlockSerialChi2 = BlockSumBinsSquared8() - BlockSumBinsSquared7();

            MetaChi2->Insert(BlockSerialChi2);

			// Insert into meta chi^2 test
            P_Chi2 = MetaChi2->GetPvalue();

			// Calc cumulative chi^2 test
			totalBlockCount++;

			cumulativeSerialChi2 = QuickCompare8() - QuickCompare7();
			ResetTest();
		}

		bitPointer --;
	}

	bitPointer = 32;
}

// Resets cumulative testing
void CSerial::ResetAll(void)
{
	wordStream = 0;
	bitPointer = 24;
	cumulativeSerialChi2 = 127.3339;
	P_Chi2 = 0.5;
	totalBlockCount = 0;
//	ZeroMemory(CumulativeBin8, 256*sizeof(double));
	memset(CumulativeBin8, 0, 256*sizeof(double));
//	ZeroMemory(CumulativeBin7, 128*sizeof(double));
	memset(CumulativeBin7, 0, 128*sizeof(double));
    MetaChi2->Reset();
	ResetTest();
}

// Resets current block test
void CSerial::ResetTest(void)
{
//	ZeroMemory(Bin8, 256*sizeof(double));
	memset(Bin8, 0, 256*sizeof(double));
//	ZeroMemory(Bin7, 128*sizeof(double));
	memset(Bin7, 0, 128*sizeof(double));
	blockWordCount = 0;
}

// Returns sum of the bis squared for the block of 8bit words
double CSerial::BlockSumBinsSquared8(void)
{
	double Sum = 0;

	double val;
	for (int i=0; i<256; i++)
	{
		val = Bin8[i] - ((blockWordCount/256.));
		Sum += val*val;
	}

	Sum /= ((blockWordCount/256.));

	return Sum;
}

// Returns sum of the bis squared for the block of 7bit words
double CSerial::BlockSumBinsSquared7(void)
{
	double Sum = 0;

	double val;
	for (int i=0; i<128; i++)
	{
		val = Bin7[i] - ((blockWordCount/128.));
		Sum += val*val;
	}

	Sum /= ((blockWordCount/128.));

	return Sum;
}

// Adds new block to cumulative Squared 8bit words
double CSerial::CumulativeBins8(void)
{
	double MSBS8 = 0.;

	for (int i=0; i<256; i++)
	{
		CumulativeBin8[i] += Bin8[i];
		MSBS8 += CumulativeBin8[i]*CumulativeBin8[i];
	}

	return MSBS8;
}

// Adds new block to cumulative Squared 7bit words
double CSerial::CumulativeBins7(void)
{
	double MSBS7 = 0.;

	for (int i=0; i<128; i++)
	{
		CumulativeBin7[i] += Bin7[i];
		MSBS7 += CumulativeBin7[i]*CumulativeBin7[i];
	}

	return MSBS7;
}

double CSerial::QuickCompare8(void)
{
	double Sum = 0;

	double val;
	for (int i=0; i<256; i++)
	{
		CumulativeBin8[i] += Bin8[i];
		val = (CumulativeBin8[i] / (totalBlockCount * (blockWordCount/256.))) - 2.;
		Sum += val * CumulativeBin8[i];
	}
	Sum += (blockWordCount*totalBlockCount);

	Sum = 0;

	for (int i=0; i<256; i++)
	{
		val = CumulativeBin8[i] - ((blockWordCount/256.)*totalBlockCount);
		Sum += val*val;
	}

	Sum /= ((blockWordCount/256.)*totalBlockCount);

	return Sum;
}

double CSerial::QuickCompare7(void)
{
	double Sum = 0;

	double val;
	for (int i=0; i<128; i++)
	{
		CumulativeBin7[i] += Bin7[i];
		val = (CumulativeBin7[i] / (totalBlockCount * (blockWordCount/128.))) - 2.;
		Sum += val * CumulativeBin7[i];
	}
	Sum += (blockWordCount*totalBlockCount);

	Sum = 0;

	for (int i=0; i<128; i++)
	{
		val = CumulativeBin7[i] - ((blockWordCount/128.)*totalBlockCount);
		Sum += val*val;
	}

	Sum /= ((blockWordCount/128.)*totalBlockCount);

	return Sum;
}

double CSerial::QuickCompare88(void)
{
	double Sum = 0;

	double val;
	for (int i=0; i<256; i++)
	{
		val = (CumulativeBin8[i] / (totalBlockCount * (blockWordCount/256.))) - 2.;
		Sum += val * CumulativeBin8[i];
	}
	Sum += (blockWordCount*totalBlockCount);

	return Sum;
}

double CSerial::QuickCompare77(void)
{
	double Sum = 0;

	double val;
	for (int i=0; i<128; i++)
	{
		val = (CumulativeBin7[i] / (totalBlockCount * (blockWordCount/128.))) - 2.;
		Sum += val * CumulativeBin7[i];
	}
	Sum += (blockWordCount*totalBlockCount);


	return Sum;
}

void CSerial::CreateChi2Test(void) {
    double pTable[] = {0.019993076,0.019996996,0.020004305,0.020003831,0.019996602,0.020006223,0.020008612,0.019993278,0.020002222,0.020011793,0.019995081,0.020002287,0.019998775,0.020004405,0.019999817,0.020010438,0.019994155,0.020003369,0.020008374,0.019995879,0.020008228,0.019993876,0.020004749,0.020003208,0.019893452,0.020001708,0.020002342,0.020000863,0.020008678,0.020005665,0.020001275,0.02000936,0.020000663,0.019986486,0.020003196,0.020004123,0.020002657,0.020016139,0.019989378,0.020004423,0.019999113,0.019998812,0.020002742,0.020004996,0.020003909,0.019997932,0.020000403,0.020004695,0.019999863,0.020017545};

    double boundryTable[] = {
        162.960449218750, 157.338134765625, 153.770507812500, 151.080566406250, 148.885253906250,
        147.007324218750, 145.352050781250, 143.862548828125, 142.498779296875, 141.234375000000,
        140.051757812500, 138.935058593750, 137.873779296875, 136.858642578125, 135.883056640625,
        134.940429687500, 134.027099609375, 133.137695312500, 132.268798828125, 131.417968750000,
        130.581298828125, 129.757324218750, 128.942626953125, 128.135498046875, 127.337646484375,
        126.538574218750, 125.740966796875, 124.942871093750, 124.141845703125, 123.336181640625,
        122.523681640625, 121.701416015625, 120.867431640625, 120.019042968750, 119.151611328125,
        118.261962890625, 117.345703125000, 116.396728515625, 115.410400390625, 114.376464843750,
        113.285400390625, 112.123046875000, 110.870117187500, 109.499511718750, 107.970458984375,
        106.216796875000, 104.117187500000, 101.411621093750, 97.322021484375, 0.0
    };

    MetaChi2 = new Chi2(50, pTable, false, boundryTable, true);
}
