//#include "StdAfx.h"
#include "AutoCorrelation.h"
#include "math.h"

CAutoCorrelation::CAutoCorrelation(void)
: maxOrder(32)
, bitStream(0)
{
    CreateChi2Tests();
//    ZeroMemory(cumulativeACZScore, 32*sizeof(double));
	memset(cumulativeACZScore, 0, 32*sizeof(double));
	for (int i=0; i<32; i++)
		P_Chi2[i] = .5;
    ResetTest();
	// Initializing app must call ResetAll()!!!
}

CAutoCorrelation::~CAutoCorrelation(void)
{
    for (int i=0; i<32; i++)
        delete MetaChi2[i];
}

// Inserts a 32 bit word into the unit test
// Must insert word into Bias test first, AC is completely dependent upon Bias
void CAutoCorrelation::InsertWord32(uint32_t InWord32)
{
    bitStream >>= 32;
    bitStream |= (uint64_t)InWord32 << 32;

	int Order;
	for (Order=1; Order<=maxOrder; Order++)
    {
        totalAndCount[Order-1] += BitCount.GetCount32((uint32_t)bitStream & (uint32_t)(bitStream>>Order));
        blockXorCount[Order-1] += BitCount.GetCount32((uint32_t)bitStream ^ (uint32_t)(bitStream>>Order));
    }

	// Every 2048 32 bit words do a unit and cumulative calculation
	if ((++blockWordCount)>=2048)
	{
		for (Order=1; Order<=maxOrder; Order++)
		{
            totalXorCount[Order-1] += blockXorCount[Order-1];
			double NN = (65536.*(totalBlockCount+1));
//            double XX = totalXorCount[Order-1];

//            cumulativeACZScore[Order-1] = -(2*XX-NN) / sqrt(NN);

            // fractional ANDs
            double fAnd = totalAndCount[Order-1] / NN;
            // fractional bias
            double fBias = *pTotalBiasCount / NN;
            // full AC calculation (a-b^2) / ((1-b) * b)
            if ((fBias*fBias)<=0 || fBias>=1)
                cumulativeACZScore[Order-1] = 0;
            else
                cumulativeACZScore[Order-1] = sqrt(NN) * (fAnd - fBias*fBias)/(fBias - fBias*fBias);

            MetaChi2[Order-1]->Insert(blockXorCount[Order-1]);
            P_Chi2[Order-1] = MetaChi2[Order-1]->GetPvalue();//Gamma.Gamma(50, ACChi2[Order-1].Calc());
		}
		ResetTest();
		totalBlockCount++;
	}
}

// Resets current unit testing
void CAutoCorrelation::ResetTest(void)
{
	blockWordCount = 0;
    for (int i=0; i<32; i++)
        blockXorCount[i] = 0;
}

// Resets cumulative scores
void CAutoCorrelation::ResetAll(double* pTotalBiasCount, double* pBlockBiasCount)
{
	this->pBlockBiasCount = pBlockBiasCount;
	this->pTotalBiasCount = pTotalBiasCount;

	totalBlockCount = 0;
//	ZeroMemory(totalAndCount, 32*sizeof(double));
	memset(totalAndCount, 0, 32*sizeof(double));
//    ZeroMemory(totalXorCount, 32*sizeof(double));
	memset(totalXorCount, 0, 32*sizeof(double));
	for (int i=0; i<32; i++)
		P_Chi2[i] = .5;

	for (int i=0; i<32; i++)
	{
        MetaChi2[i]->Reset();
//		ACChi2[i].ResetAll();
		cumulativeACZScore[i] = 0.;
        prevCumulativeACZScore[i] = 0.;
		P_Chi2[i] = .5;
	}

	bitStream = 0;
	ResetTest();
}

void CAutoCorrelation::CreateChi2Tests(void) {
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

    for (int i=0; i<32; i++)
        MetaChi2[i] = new Chi2(49, pTable, false, boundryTable, false);
}
