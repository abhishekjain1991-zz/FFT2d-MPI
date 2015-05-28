# Makefile for ECE8893, 2D FFT assignment

CXX      = mpic++
CXXFLAGS = -Wall -g

fft2d:	fft2d.o Complex.o InputImage.o
	$(CXX) -g -o fft2d fft2d.o Complex.o InputImage.o

clean:
	@rm *.o fft2d

