# Music-Visualizer

  - Terminal-based Music Visualizer that displays the power spectrum of a *.wav file
  - Music visualizer using SDL2 for audio play and FFTW for Fourier Transformation

# Software

  - SDL2: https://wiki.libsdl.org/Installation
  - FFTW: http://www.fftw.org/download.html
  - GDB: for debugging on Linux
  - GCC: C compiler for Linux
  
# How To Build and Run

```bash
git clone git@github.com:Alex20129/Music-Visualizer.git
cd Music-Visualizer
mkdir build
cd build
cmake ../
make
```

## Synopsis:

```bash
./mvisual -f ~/Music/file.wav
```

Note: Supports only wav audio files.

# Brief overview about how the program works
  - Analyzes the digital information in the wav file.
  - Computes the fourier transformation every 'N' frames. Eg. computes the fourier transformation (FT) on 'N' L channel data and also computes the FT on 'N' R channel data.
  - Gathers frequency and magnitude information from the results of the fourier transformation and records it by storing into an array.
  - After analysis is finished the program will play music and display the corresponding information at the terminal.

