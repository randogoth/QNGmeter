//#include "StdAfx.h"
#include "Monkey.h"
#include "math.h"

CMonkey::CMonkey(void)
{
    CreateChi2Test();

	pow2_32 = pow( 2.0, 32 );
	ResetAll();
}

CMonkey::~CMonkey(void)
{
    delete MetaChi2;
}


// Submits a 32 bit word for OQSO testing
void CMonkey::InsertWord32(uint32_t InWord32)
{
    WordStream >>= 32;
    WordStream |= (uint64_t)InWord32 << 32;

	// Submit as many overlapped words as possible from wordstream
	while ( LetterPointer >= 0 )
	{
		// Word is 20 bits long, overlapped every 5 bits
		CurrentWord = (uint32_t)(WordStream>>LetterPointer);
		CurrentWord &= 0x000fffff;

		if ( !(MonkeyBitmap.CheckWord( CurrentWord )) )
			MissingWords--;

		// Test 2097152 words (or 10485775 bits)
		WordCount++;
		if ( WordCount>=2097152 )
		{
			// Calc current and cumulative z-scores
			totalBlockCount++;

            //double UnitZScore = -((double)MissingWords-141909.1945)/294.656;
			
            MetaChi2->Insert(MissingWords);

			//ZScoreTotal += UnitZScore;
			MissingWordsTotal += (uint64_t)MissingWords;
			
            P_Chi2 = MetaChi2->GetPvalue();

			if (totalBlockCount!=0) {
				//cumulativeZScore = ZScoreTotal / sqrt(totalBlockCount);
				cumulativeZScore = -((double)MissingWordsTotal - (totalBlockCount * 141909.104)) / (sqrt(totalBlockCount) * 294.656);
			}

			ResetTest();
		}

		LetterPointer -= 5;
	}
	
	LetterPointer += 32;
}

// Resets cumulative and current test
void CMonkey::ResetAll(void)
{
	cumulativeZScore = 0;
	WordStream = 0;
	ZScoreTotal = 0;
	MissingWordsTotal = 0;
	totalBlockCount = 0.;
	LetterPointer = 12;
	P_Chi2 = .5;
    MetaChi2->Reset();

	ResetTest();
}

// Resets current test
void CMonkey::ResetTest(void)
{
	MissingWords = 1048576;
	MonkeyBitmap.Clearmap();
	WordCount = 0;
}

void CMonkey::CreateChi2Test(void) {
    double pTable[] = {
        0.020035263116770285, 0.04007592896841139, 0.059900259673160094, 0.07990555454845777, 0.09973585094717441, 0.12019013336507156, 0.14037340713890295, 0.16024040737242062, 0.18006661762415743, 0.20040959652731188, 0.22005106263679536, 0.2407529898402695, 0.26025548231499596, 0.2805415527829255, 0.300377215413428, 0.3208231113598671, 0.3405774462690355, 0.36077911411228614, 0.38137696414272143, 0.4009988146079183, 0.4208736799354211, 0.4409523283066323, 0.4611839548890032, 0.4815165669138008, 0.5018973841149283, 0.522273249393845, 0.5425910443898824, 0.5614554567534166, 0.5815124046447061, 0.6026740748901427, 0.6222435463400903, 0.6427760999481655, 0.6616581917860309, 0.6825745983586168, 0.7017433191528237, 0.722654551841068, 0.7428220165027213, 0.7621991815446298, 0.7817526213870567, 0.8022460422705294, 0.8224190682008065, 0.8420653268505389, 0.8617367254552067, 0.8817041720232961, 0.9019201354926609, 0.9214978872400847, 0.9412230272875216, 0.9610265302820049, 0.981060464990248, 1.0
    };

    double boundaryTable[] = {
        141306, 141395, 141452, 141496, 141532, 141564, 141592, 141617, 141640, 141662,
        141682, 141702, 141720, 141738, 141755, 141772, 141788, 141804, 141820, 141835,
        141850, 141865, 141880, 141895, 141910, 141925, 141940, 141954, 141969, 141985,
        142000, 142016, 142031, 142048, 142064, 142082, 142100, 142118, 142137, 142158,
        142180, 142203, 142228, 142256, 142288, 142324, 142368, 142426, 142518, 1e100
    };

    MetaChi2 = new Chi2(50, pTable, true, boundaryTable, false);
}
