//#include "StdAfx.h"
#include "Entropy.h"
#include "math.h"


CEntropy::CEntropy()
: E(1.0)
{
    CreateChi2Test();
	pow2_32 = pow( 2.0, 32 );

	ResetAll();
}

CEntropy::~CEntropy()
{
    delete MetaChi2;
}

double CEntropy::CalcStandardDeviation(uint32_t K, uint32_t m)
{
	double	sd;				// standard deviation
	double	clk;			// intermediate calc variable
	double	data[15][4];	// data needed for sd calculation

	// initialize data matrix
	data[1][1]  = 2.5769918;  data[1][2]  = 0.3313257;  data[1][3]  = 0.4381809;
	data[2][1]  = 2.9191004;  data[2][2]  = 0.3516506;  data[2][3]  = 0.4050170;
	data[3][1]  = 3.1291382;  data[3][2]  = 0.3660832;  data[3][3]  = 0.3856668;
	data[4][1]  = 3.2547450;  data[4][2]  = 0.3758725;  data[4][3]  = 0.3743782;
	data[5][1]  = 3.3282150;  data[5][2]  = 0.3822459;  data[5][3]  = 0.3678269;
	data[6][1]  = 3.3704039;  data[6][2]  = 0.3862500;  data[6][3]  = 0.3640569;
	data[7][1]  = 3.3942629;  data[7][2]  = 0.3886906;  data[7][3]  = 0.3619091;
	data[8][1]  = 3.4075860;  data[8][2]  = 0.3901408;  data[8][3]  = 0.3606982;
	data[9][1]  = 3.4149476;  data[9][2]  = 0.3909846;  data[9][3]  = 0.3600222;
	data[10][1] = 3.4189794;  data[10][2] = 0.3914671;  data[10][3] = 0.3596484;
	data[11][1] = 3.4211711;  data[11][2] = 0.3917390;  data[11][1] = 0.3594433;
	data[12][1] = 3.4223549;  data[12][2] = 0.3918905;  data[12][3] = 0.3593316;
	data[13][1] = 3.4229908;  data[13][2] = 0.3919740;  data[13][3] = 0.3592712;
	data[14][1] = 3.4233308;  data[14][2] = 0.3920198;  data[14][3] = 0.3592384;

	clk = sqrt( data[m-2][2] + (data[m-2][3]*pow( 2.0, (double)m )/K) );
	sd = clk * sqrt( data[m-2][1] / K );

	return sd;
}

void CEntropy::Initialize(uint32_t BitCount, uint32_t qFactor )
{
	this->BitCount = BitCount;

	// get initialization blocks used
	Q = qFactor * 256; // 20*256 = 5120

	// get total number of m-bit blocks
	nb = (unsigned long)floor( BitCount / 8.0 ); // 524288

	// get number of calculation blocks used
	K = nb - Q; // 519168

	// calc sd
	sd = CalcStandardDeviation( K, 8 );

	// zero out tab
	memset( tab, 0, 256*4 );

	blockWordCount = 0;
	Sum = 0;
}

void CEntropy::InsertWord32(uint32_t InWord32)
{
	int i;
	uint8_t RndByte;

	for ( i=0; i<=3; i++ )
	{
		RndByte = (uint8_t)(InWord32>>(8*i));

		blockWordCount++;

		if ( blockWordCount<=Q )
		{
			tab[RndByte] = blockWordCount;
		}
		else
		{
			Sum += fcoef( blockWordCount - tab[RndByte] );
            tab[RndByte] = blockWordCount;
		}
	}

	if ( (blockWordCount*8)>=BitCount )
	{
		H = Sum/((double)K);

		ETotal += H/8.;
		totalBlockCount++;
		E = ETotal / (totalBlockCount);
		double BlockZScore = ( (H-8.)/sd );
		ZScoreTotal += BlockZScore;
        MetaChi2->Insert(Sum);
        P_Chi2 = MetaChi2->GetPvalue();
		if (totalBlockCount!=0)
			cumulativeZScore = ZScoreTotal / sqrt(totalBlockCount);
		Initialize( 4194304, 20 );
	}
}

double CEntropy::fcoef(uint32_t i)
{
	// set constants
	const double	l2 = log(2.0);
	const double	c = -0.8327462;
	const int		limit = 23;

	double	retval;		// return value
	unsigned long 	kk;			// universal index
	int		j;			// intermediate calc value

	retval = 0;
	if ( i<limit )
	{
		for ( kk=1; kk<i; kk++ )
		{
			retval += 1/(double)kk;
		}

		retval /= l2;
	}
	else
	{
		j = i - 1;
		retval = ( log((double)j)/l2 ) - c + ( ((1./(2.*j)) - (1./(12.*j*j)))/l2 );
	}

	return retval;
}


// Reset cumulative test
void CEntropy::ResetAll(void)
{
	E = 1.0;
	P_Chi2 = .5;
	cumulativeZScore = 0.;
	ZScoreTotal = 0;
	ETotal = 0;
	totalBlockCount = 0.;

    MetaChi2->Reset();
//	Chi2.Init();
	Initialize( 4194304, 20 );
}

void CEntropy::CreateChi2Test(void) {

double pTable[] = {0.02012108166,0.04008956584,0.06018692852,0.08018501372,0.1001359343,0.120192745,0.1401963459,0.16005801,0.1802017087,0.200031418,0.2200301029,0.2399762859,0.2599755987,0.2800970982,0.3000327584,0.3202120847,0.340234622,0.360070732,0.3800778627,0.3998619416,0.4199041271,0.4398490175,0.4601300439,0.4798811487,0.5000167861,0.5201045044,0.5399738275,0.5600664317,0.5798227432,0.5998761946,0.619978835,0.6402984742,0.6599255874,0.6799283734,0.7001158234,0.7199709181,0.7401361424,0.7599607824,0.7802489806,0.8001210836,0.8200605293,0.8400672073,0.8601944042,0.880037642,0.900350094,0.9203249058,0.9401774272,0.9600776027,0.9807874005,1.0};

    double boundryTable[] = {
        4151655.83724745500, 4151904.48938296180, 4152066.26934954900, 4152189.24478157190, 4152290.52925698830,
        4152378.39198942950, 4152456.23714622110, 4152526.37086037970, 4152591.89903046660, 4152652.04845737200,
        4152709.17705694870, 4152763.25623371170, 4152815.06782631900, 4152865.15788602550, 4152913.08992392620,
        4152960.15932061100, 4153005.64889538610, 4153049.71294774950, 4153093.31400005010, 4153135.74272596740,
        4153178.16347156510, 4153219.94024266860, 4153262.09135455310, 4153302.92978435200, 4153344.45511117900,
        4153385.87801960110, 4153426.95058751110, 4153468.69217352150, 4153510.04595550760, 4153552.45207967800,
        4153595.52232766520, 4153639.76729502670, 4153683.32874056320, 4153728.72452108000, 4153775.75883651480,
        4153823.43728935770, 4153873.56984770070, 4153924.85234509460, 4153979.78749536580, 4154036.49777039420,
        4154096.92328405190, 4154161.94629989700, 4154232.98319796660, 4154310.19911737930, 4154399.23572011480,
        4154500.86437934400, 4154623.25103316270, 4154783.62485346620, 4155045.02386120380, 1e100
    };

    MetaChi2 = new Chi2(50, pTable, true, boundryTable, false);
}
