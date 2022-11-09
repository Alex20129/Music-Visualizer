CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

PROJECT(mvisual)

SET(CMAKE_CXX_FLAGS "-O3 -Wall -fbuiltin -g -std=c++11 -no-pie")
SET(CMAKE_C_FLAGS "-O3 -Wall -fbuiltin -g -std=c99 -no-pie")

FIND_PACKAGE(SDL2 REQUIRED)

LINK_LIBRARIES(SDL2 fftw3 pthread m)

ADD_EXECUTABLE(${CMAKE_PROJECT_NAME}
	audioInformation.c
	dataprocessing.c
	visualize.c)

