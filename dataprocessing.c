#include "dataprocessing.h"
#include <SDL2/SDL.h>
#include <assert.h> 
#include <math.h>

enum {real, imag}; //real and imaginary
extern volatile int keeprunning;
extern const int BUCKETS;

void setupDFTForSound(Visualizer_Pkg_ptr vis_pkg_ptr, uint8_t *buffer, int bytesRead)
{
	int bytewidth=vis_pkg_ptr->bitsize / 8;
	SDL_AudioSpec *wavSpec=GetSDL_AudioSpec(vis_pkg_ptr);
	SDL_AudioFormat fmt=wavSpec->format;
	int channels=wavSpec->channels, count=0, c;
	int frames=bytesRead / (bytewidth * channels);
	struct FFTWop* fftwop=vis_pkg_ptr->fftw_ptr;

	for(c=0; c<channels; ++c)
	{
		fftwop[c].p=fftw_plan_dft_1d(frames, fftwop[c].in, fftwop[c].out, FFTW_FORWARD, FFTW_MEASURE);
    }

	while(count<frames)
	{
		for(c=0; c< channels; ++c)
		{
			fftwop[c].in[count][real]=vis_pkg_ptr->GetAudioSample(buffer, fmt);
			fftwop[c].in[count][imag]=0.0;
            buffer+=bytewidth;
        }
        count++;
    }
}

int getFileSize(FILE *inFile)
{
	int fileSize=0;
    fseek(inFile,0,SEEK_END);

    fileSize=ftell(inFile);

    fseek(inFile,0,SEEK_SET);

    return fileSize;
}

void processWAVFile(uint32_t wavLength, int buffer_size, Visualizer_Pkg_ptr vis_pkg_ptr)
{
	struct FFTWop* dft=vis_pkg_ptr->fftw_ptr;
	int channels=GetSDL_AudioSpec(vis_pkg_ptr)->channels;
	size_t bytesRead;

	FILE* wavFile=fopen(vis_pkg_ptr->filename, "r");
	int filesize=getFileSize(wavFile);

	Uint8* buffer=(Uint8*)malloc(buffer_size*sizeof(Uint8));

	int packet_index=0, i;
	//Skip header information in .WAV file
	bytesRead=fread(buffer, sizeof buffer[0], filesize-wavLength, wavFile);

	//Reading actual audio data
	while((bytesRead=fread(buffer, sizeof buffer[0], buffer_size/sizeof(buffer[0]), wavFile))>0)
	{
        vis_pkg_ptr->setupDFT(vis_pkg_ptr, buffer, bytesRead);
		for(i=0; i<channels; ++i)
		{
            fftw_execute(dft[i].p);
			analyze_FFTW_Results(vis_pkg_ptr, dft[i], packet_index, i, bytesRead);
            fftw_destroy_plan(dft[i].p);
        }
        packet_index++;
    }
	fclose(wavFile);

    /*MEMORY MANAGEMENT*/
    free(buffer);
	for(i=0; i<channels; ++i)
	{
        free(dft[i].in);
        free(dft[i].out);
    }
    free(dft);
}

void analyze_FFTW_Results(Visualizer_Pkg_ptr packet, struct FFTWop fftwop, int packet_index, int ch, size_t bytesRead)
{
	int i, j;
	double freq, magnitude;
	double *peakmaxArray=(double*)malloc(BUCKETS*sizeof(double));

	SDL_AudioSpec *wavSpec=GetSDL_AudioSpec(packet);

	double nyquistF=wavSpec->freq / 2;
	double freq_bin[BUCKETS+1];
	for(i=0; i<BUCKETS+1; i++)
	{
		freq_bin[i]=nyquistF/BUCKETS*i;
		//printf("freq_bin[%i]=%.0lf\n", i, freq_bin[i]);
	}

	int frames=bytesRead / (wavSpec->channels * packet->bitsize / 8);
	struct FFTW_Results *results=GetFFTW_Results(packet);

	for(i=0; i<BUCKETS; ++i)
	{
		peakmaxArray[i]=0.0;
	}

	for(j=0; j<frames/2; ++j)
	{
		freq=j * (double)wavSpec->freq / (double)frames;
		magnitude=sqrt( (fftwop.out[j][real]*fftwop.out[j][real]) + (fftwop.out[j][imag]*fftwop.out[j][imag]) );
		for(i=0; i<BUCKETS; ++i)
		{
			if((freq>freq_bin[i]) && (freq<=freq_bin[i+1]))
			{
				if(magnitude>peakmaxArray[i])
				{
					peakmaxArray[i]=magnitude;
                }
            }
        }
    }

	for(i=0; i<BUCKETS; i++)
	{
		results[packet_index].peakmagMatrix[ch][i]=10*(log10(peakmaxArray[i]));
    }

    free(peakmaxArray);
}
