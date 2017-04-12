#ifndef MAKEPPM_HEADER
#define MAKEPPM_HEADER

enum ppmMode {RGB,BW};

char* getPPMColor(int x);

char* getPPMBW(int x);

void makePPM(int matrixSize, int ** matrix,enum ppmMode mode,int d);

#endif
