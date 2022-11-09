#ifndef DATAPROCESSING_H
#define DATAPROCESSING_H

#include "audioInformation.h"
#include <fftw3.h>

int getFileSize(FILE *inFile);
void analyze_FFTW_Results(Visualizer_Pkg_ptr, struct FFTWop, int, int, size_t);
void setupDFTForSound(Visualizer_Pkg_ptr, uint8_t *, int);
void processWAVFile(uint32_t, int, Visualizer_Pkg_ptr );

#endif
