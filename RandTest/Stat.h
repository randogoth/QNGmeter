#pragma once

class CStat
{
public:
	CStat(void);
	~CStat(void);

	// Calculate a p-value from a z-score
	static double ZtoP(double zScore);
};
