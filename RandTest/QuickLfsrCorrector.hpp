#pragma once

template <class T>
class QuickLfsrCorrector {
public:
    QuickLfsrCorrector() {
        T initVal = -1;
        T deltaVal = initVal / 11;
        for (int i=0; i<15; i++) {
            lfsr[i] = initVal;
            initVal -= deltaVal;
        }

        inPointer = 0;
        tab3 = initVal;
        initVal -= deltaVal;
        tab5 = initVal;
        initVal -= deltaVal;
        tab7 = initVal;
    }

    T Correct(T inVal) {
        inVal ^= tab3 ^ tab5 ^ tab7;
        
        inPointer--;
        if (inPointer<0)
            inPointer = 14;
        lfsr[inPointer] = inVal;

        int tabPointer = inPointer + 3;
        tabPointer %= 15;
        tab3 ^= lfsr[tabPointer];
        tabPointer += 5;
        tabPointer %= 15;
        tab5 ^= lfsr[tabPointer];
        tabPointer += 7;
        tabPointer %= 15;
        tab7 ^= lfsr[tabPointer];

        return inVal;
    }

private:
    T lfsr[15];
    T tab3;
    T tab5;
    T tab7;
    int inPointer;
};