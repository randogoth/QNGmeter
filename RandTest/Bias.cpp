//#include "StdAfx.h"
#include "Bias.h"
#include "math.h"

CBias::CBias(void)
: cumulativeBiasZScore(0)
, P_Chi2(.5)
{
    CreateChi2Test();

	ResetAll();
}

CBias::~CBias(void)
{
    delete MetaChi2;
}

// Tests a 32 bit word
void CBias::InsertWord32(uint32_t InWord32)
{
	blockBiasCount += BitCount.GetCount32(InWord32);

	// Every 2^16 bits calc cumulative bias and chi^2
	if ( (++blockWordCount)>=2048 )
	{
		// calculate unit bias
		double BlockZScore = (2.*blockBiasCount-(32*blockWordCount))/(sqrt(32.0*blockWordCount));

		// Chi^2 for bias using binomial chi^2 test Chi2Binomial
        MetaChi2->Insert(blockBiasCount);
        P_Chi2 = MetaChi2->GetPvalue();

		totalBiasCount += blockBiasCount;
		formerBlockBiasCount = blockBiasCount;
		totalBlockCount++;

		cumulativeBiasZScore = ((2.*totalBiasCount) - (65536.*totalBlockCount)) / sqrt(65536.*totalBlockCount);

		ResetTest();
	}
}

// Resets current test
void CBias::ResetTest()
{
	blockWordCount = 0;
	blockBiasCount = 0;
}

// Resets all
void CBias::ResetAll()
{
	P_Chi2 = .5;
	cumulativeBiasZScore = 0.;
	totalBiasCount = 0;
	totalBlockCount = 0;
//	BiasChi2.ResetAll();
    MetaChi2->Reset();
	ResetTest();
}

void CBias::CreateChi2Test(void) {
    double pTable[] = {
        0.0016751591157826984, 0.0008125704223141780, 0.0011538175281394167, 0.0016130041110183372, 0.0022200160266961827,
        0.0030081473713346360, 0.0040129572987985991, 0.0052705078207208266, 0.0068149541705240074, 0.0086755302334023398,
        0.0108730598645528967, 0.0134162222471676280, 0.0162978932289932796, 0.0194919592594017396, 0.0229510396552503468,
        0.0266055419211793493, 0.0303644043131267252, 0.0341177483099464753, 0.0377414795440626524, 0.0411036575708872835,
        0.0440722296391575050, 0.0465235232774633227, 0.0483507488411394182, 0.0494717022548410683, 0.0467242519481981447,
        0.0494717022548410683, 0.0483507488411394182, 0.0465235232774633227, 0.0440722296391575050, 0.0411036575708872835,
        0.0377414795440626524, 0.0341177483099464753, 0.0303644043131267252, 0.0266055419211793493, 0.0229510396552503468,
        0.0194919592594017396, 0.0162978932289932796, 0.0134162222471676280, 0.0108730598645528967, 0.0086755302334023398,
        0.0068149541705240074, 0.0052705078207208266, 0.0040129572987985991, 0.0030081473713346360, 0.0022200160266961827,
        0.0016130041110183372, 0.0011538175281394167, 0.0008125704223141780, 0.0016751591157826984
    };
    double boundryTable[] = {
        32392, 32408, 32424, 32440, 32456, 32472, 32488, 32504, 32520,
        32536, 32552, 32568, 32584, 32600, 32616, 32632, 32648, 32664, 32680,
        32696, 32712, 32728, 32744, 32760, 32775, 32791, 32807, 32823, 32839,
        32855, 32871, 32887, 32903, 32919, 32935, 32951, 32967, 32983, 32999,
        33015, 33031, 33047, 33063, 33079, 33095, 33111, 33127, 33143, 1e100
    };

    MetaChi2 = new Chi2(49, pTable, false, boundryTable, false);
}
