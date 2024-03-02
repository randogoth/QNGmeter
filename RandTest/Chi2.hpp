#pragma once

#include <memory.h>
#include "Gamma.h"


class Chi2 {
public:
    Chi2(int binCount=10, double* pTable=0, bool pTableCumulative=false, double* boundryTable=0, bool reverseBoundrySense=false) {
        this->binCount = binCount;
        this->binned = new double[binCount];
        this->boundryTable = new double[binCount];
        this->reverseBoundrySense = reverseBoundrySense;
        this->pTable = new double[binCount];

        if (pTable == 0) {
            for (int i=0; i<binCount; i++) {
                this->pTable[i] = 1.0 / binCount;
                this->boundryTable[i] = (1.0+i) / binCount;
            }
        }
        else {
            memcpy(this->pTable, pTable, sizeof(double)*binCount);
            memcpy(this->boundryTable, boundryTable, sizeof(double)*binCount);
        }

        if (pTableCumulative == true) {
            for (int i=(binCount-1); i>=1; i--) {
                this->pTable[i] = pTable[i] - pTable[i-1];
            }
        }

        Reset();
    }

    ~Chi2() {
        delete[] pTable;
        delete[] boundryTable;
        delete[] binned;
    }

    void Reset() {
        doRecalc = true;
        for (int i=0; i<binCount; i++)
            binned[i] = 0;
    }

    void Insert(double inValue) {
        int i = 0;
        if (reverseBoundrySense == false) {
            while (inValue > boundryTable[i])
                i++;
        }
        else {
            while (inValue < boundryTable[i])
                i++;
        }
        binned[i]++;
        doRecalc = true;
    }

    double GetChi2() {
        if (doRecalc == true)
            Recalc();

        return chi2;
    }

    double GetPvalue() {
        if (doRecalc == true)
            Recalc();

		return (Gamma.Gamma(binCount-1, chi2));
    }

private:
    void Recalc() {
	    double sum = 0;
        double total = 0;

	    for (int i=0; i<binCount; i++) {
            sum += (binned[i]*binned[i]) / pTable[i] ;
            total += binned[i];
        }
        sum /= total;

        chi2 = sum - total;
    }

    double chi2;
    double pValue;

    bool doRecalc;
    int binCount;
    double* boundryTable;
    bool reverseBoundrySense;
    double* pTable;

    CGamma Gamma;

public:
    double* binned;

};