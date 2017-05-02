#ifndef MAKEPPM_HEADER
#define MAKEPPM_HEADER
#include "doIteration.h"
enum ppmMode {RGB,BW};

char* getPPMColor(int x);

void * runMakePPM(void * args);

#endif
