#pragma once
#include "MatDib.h"

class MatDibFourier
{
public:
	MatDibFourier();
	~MatDibFourier();
	int m_nWidth;
	int m_nHeight;
	Mat m_matReal;
	Mat m_matImag;

	void m_fnSetImage(Mat& img);
	void m_fnGetImage(Mat& img);
	void m_fnGetSpectrumImage(Mat& img);
	void m_fnGetPhaseImage(Mat& img);

	void m_fnDFT(int dir);
	void m_fnDFTRC(int dir);
	void m_fnFFT(int dir);

	void m_fnLowPassIdeal(int cutoff);
	void m_fnHighPassIdeal(int cutoff);
	void m_fnLowPassGaussian(int cutoff);
	void m_fnHighPassGaussian(int cutoff);
};

void g_fnDFT1d(double* re, double* im, int N, int dir);
void g_fnFFT1d(double* re, double* im, int N, int dir);
bool g_fnIsPowerOf2(int n);