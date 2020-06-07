#include "stdafx.h"
#include "MatDibFourier.h"


MatDibFourier::MatDibFourier()
	: m_nWidth(0), m_nHeight(0)
{
}


MatDibFourier::~MatDibFourier()
{
}

void MatDibFourier::m_fnSetImage(Mat& img)
{
	m_nWidth = img.cols;
	m_nHeight = img.rows;

	m_matReal = img.clone();
	m_matImag = Mat::zeros(Size(m_nWidth, m_nHeight), CV_8U);	// 허수부는 모두 0으로 초기화
}

void MatDibFourier::m_fnGetImage(Mat& img)
{
	if (m_matReal.data != NULL)
		return;

	// 실수부의 값을 이용하여 출력 영상을 생성
	img = Mat::zeros(Size(m_nWidth, m_nHeight), CV_8U);

	double* pRe = (double*)m_matReal.data;
	BYTE* ptr = img.data;

	int size = m_matReal.cols * m_matReal.rows;
	for (int i = 0; i < size; i++)
	{
		ptr[i] = static_cast<BYTE>(limit(pRe[i] + 0.5));
	}
}

void MatDibFourier::m_fnGetSpectrumImage(Mat& img)
{
	register int nStepX, nStepY;

	img = Mat::zeros(Size(m_nWidth, m_nHeight), CV_8U);
	BYTE* ptr;// = img.data;

	double* pRe;// = (double*)m_matReal.data;
	double* pIm;// = (double*)m_matImag.data;

	// 스펙트럼 값을 저장할 임시 버퍼
	Mat matSpectrum = Mat::zeros(Size(m_nWidth, m_nHeight), CV_64FC1);
	double* pSpec;// = (double*)matSpectrum.data;

	//---
	// 복소수 값의 절댓값을 로그 변환하여 저장.
	// 정규화된 결과 이미지 생성을 위하여 스펙트럼의 최댓값 계산.
	//---
	double mag, max_value = 0;
	for (nStepY = 0; nStepY < m_nHeight; nStepY++)
	{
		pRe = m_matReal.ptr<double>(nStepY);
		pIm = m_matImag.ptr<double>(nStepY);
		pSpec = matSpectrum.ptr<double>(nStepY);
		for (nStepX = 0; nStepX < m_nWidth; nStepX++)
		{
			mag = sqrt(pRe[nStepX] * pRe[nStepX] + pIm[nStepX] * pIm[nStepX]);
			pSpec[nStepX] = log(mag + 1);

			if (pSpec[nStepX] > max_value)
				max_value = pSpec[nStepX];
		}
	}
	
	//---
	// 출력 영상 생성. 푸리에 변환 결과를 SHIFT 하고,
	// 출력 영상의 최댓값이 255가 되도록 그레이스케일 값을 정규화.
	//---
	int nPosX, nPosY;
	for (nStepY = 0; nStepY < m_nHeight; nStepY++)
	{
		ptr = img.ptr<BYTE>(nStepY);
		for (nStepX = 0; nStepX < m_nWidth; nStepX++)
		{
			nPosX = static_cast<int>(nStepX + m_nWidth / 2.);
			nPosY = static_cast<int>(nStepY + m_nHeight / 2.);

			if (nPosX >= m_nWidth) nPosX -= m_nWidth;
			if (nPosY >= m_nHeight) nPosY -= m_nHeight;
			pSpec = matSpectrum.ptr<double>(nPosY);
			ptr[nStepX] = static_cast<BYTE>(limit(pSpec[nPosX] * 255 / max_value));
		}
	}
}

void MatDibFourier::m_fnGetPhaseImage(Mat& img)
{
	register int nStepX, nStepY;

	img = Mat::zeros(Size(m_nWidth, m_nHeight), CV_8U);
	BYTE* ptr;// = img.data;

	double* pRe;// = (double*)m_matReal.data;
	double* pIm;// = (double*)m_matImag.data;

	// 위상각을 저장할 임시버퍼
	Mat matPhase = Mat::zeros(Size(m_nWidth, m_nHeight), CV_8U);
	double* pPhase;// = (double*)matPhase.data;

	//---
	// 복소수 값의 위상각을 저장. atan2 함수의 반환 값은 -PI ~ PI이다.
	//---

	for (nStepY = 0; nStepY < m_nHeight; nStepY++)
	{
		pRe = m_matReal.ptr<double>(nStepY);
		pIm = m_matImag.ptr<double>(nStepY);
		pPhase = matPhase.ptr<double>(nStepY);
		for (nStepX = 0; nStepX < m_nWidth; nStepX++)
		{
			pPhase[nStepX] = atan2(pIm[nStepX], pRe[nStepX]);
		}
	}

	//---
	// 출력 영상 생성. 푸리에 변환 결과를 SHIFT하고,
	// 출력 영상의 최댓값이 255가 되도록 그레이스케일 값을 정규화.
	//---

	int nPosX, nPosY;
	for (nStepY = 0; nStepY < m_nHeight; nStepY++)
	{
		ptr = img.ptr<BYTE>(nStepY);
		for (nStepX = 0; nStepX < m_nWidth; nStepX++)
		{
			nPosX = static_cast<int>(nStepX + m_nWidth / 2.);
			nPosY = static_cast<int>(nStepY + m_nHeight / 2.);

			if (nStepX > m_nWidth) nPosX -= m_nWidth;
			if (nStepY > m_nHeight) nPosY -= m_nHeight;
			pPhase = matPhase.ptr<double>(nPosY);
			ptr[nStepX] = static_cast<BYTE>(limit(pPhase[nPosX] * 128 / CV_PI * 128));
		}
	}
}

void MatDibFourier::m_fnDFT(int dir)
{
	if (m_matReal.data == NULL)
		return;

	//---
	// real, imag 복사본 생성
	//---

	Mat matReal = m_matReal.clone();
	Mat matImag = m_matImag.clone();

	double* pRe;// = (double*)m_matReal.data;
	double* pIm;// = (double*)m_matImag.data;

	double* pReCpy;// = (double*)matReal.data;
	double* pImCpy;// = (double*)matImag.data;

	//---
	// 이산 푸리에 변환 (또는 역변환)
	//---

	register int nStepX, nStepY, nPosX, nPosY;
	double dSum_re, dSum_im, temp;

	for (nStepY = 0; nStepY < m_nHeight; nStepY++)
	{
		pRe = m_matReal.ptr<double>(nStepY);
		pIm = m_matImag.ptr<double>(nStepY);
		for (nStepX = 0; nStepX < m_nWidth; nStepX++)
		{
			dSum_re = dSum_im = 0;
			for (nPosY = 0; nPosY < m_nHeight; nPosY++)
			{
				pReCpy = matReal.ptr<double>(nPosY);
				pImCpy = matImag.ptr<double>(nPosY);
				for (nPosX = 0; nPosX < m_nWidth; nPosX++)
				{
					temp = 2 * dir * CV_PI * (static_cast<double>(nStepX) * nPosX / m_nWidth + static_cast<double>(nStepY) * nPosY / m_nHeight);
					dSum_re += (pReCpy[nPosX] * cos(temp) - pImCpy[nPosX] * sin(temp));
					dSum_im += (pReCpy[nPosX] * sin(temp) + pImCpy[nPosX] * sin(temp));
				}
				pReCpy = NULL;
				pImCpy = NULL;
			}
			pRe[nStepX] = dSum_re;
			pIm[nStepX] = dSum_im;

			if (dir == -1)
			{
				pRe[nStepX] /= (m_nWidth * m_nHeight);
				pIm[nStepX] /= (m_nWidth * m_nHeight);
			}
		}
		pRe = NULL;
		pIm = NULL;
	}
}

void MatDibFourier::m_fnDFTRC(int dir)
{
	if (m_matReal.data == NULL)
		return;

	//---
	// 행(Row) 단위 이산 푸리에 변환 (또는 역변환)
	//---

	Mat matReal_tmp, matImag_tmp;

	matReal_tmp = Mat::zeros(Size(m_nWidth, 1), CV_8U);
	matImag_tmp = Mat::zeros(Size(m_nWidth, 1), CV_8U);

	double* dRe = (double*)matReal_tmp.data;
	double* dIm = (double*)matImag_tmp.data;

	double* pRe = (double*)m_matReal.data;
	double* pIm = (double*)m_matImag.data;

	register int nStepX, nStepY;

	for (nStepY = 0; nStepY < m_nHeight; nStepY++)
	{
		for (nStepX = 0; nStepX < m_nWidth; nStepX++)
		{
			dRe[nStepX] = pRe[nStepX + nStepY * m_nWidth];
			dIm[nStepX] = pIm[nStepX + nStepY * m_nWidth];
		}

		g_fnDFT1d(dRe, dIm, m_nWidth, dir);

		for (nStepX = 0; nStepX < m_nWidth; nStepX++)
		{
			pRe[nStepX + nStepY * m_nWidth] = dRe[nStepX];
			pIm[nStepX + nStepY * m_nWidth] = dIm[nStepX];
		}
	}

	//---
	//
	//---

	matReal_tmp = Mat::zeros(Size(1, m_nHeight), CV_8U);
	matImag_tmp = Mat::zeros(Size(1, m_nHeight), CV_8U);

	dRe = (double*)matReal_tmp.data;
	dIm = (double*)matImag_tmp.data;
	
	memset(dRe, 0, sizeof(double) * m_nHeight);
	memset(dIm, 0, sizeof(double) * m_nHeight);

	for (nStepX = 0; nStepX < m_nWidth; nStepX++)
	{
		for (nStepY = 0; nStepY < m_nHeight; nStepY++)
		{
			dRe[nStepY] = pRe[nStepX + nStepY * m_nWidth];
			dIm[nStepY] = pIm[nStepX + nStepY * m_nWidth];
		}

		g_fnDFT1d(dRe, dIm, m_nHeight, dir);

		for (nStepY = 0; nStepY < m_nHeight; nStepY++)
		{
			pRe[nStepX + nStepY * m_nWidth] = dRe[nStepY];
			pIm[nStepX + nStepY * m_nWidth] = dIm[nStepY];
		}
	}
}

void MatDibFourier::m_fnFFT(int dir)
{

}

void MatDibFourier::m_fnLowPassIdeal(int cutoff)
{

}

void MatDibFourier::m_fnHighPassIdeal(int cutoff)
{

}

void MatDibFourier::m_fnLowPassGaussian(int cutoff)
{

}

void MatDibFourier::m_fnHighPassGaussian(int cutoff)
{

}

void g_fnDFT1d(double* re, double* im, int N, int dir)
{
	double* tr = new double[N];
	double* ti = new double[N];
	memcpy(tr, re, sizeof(double)*N);
	memcpy(ti, im, sizeof(double)*N);

	register int i, x;
	double sum_re, sum_im, temp;
	for (i = 0; i < N; i++)
	{
		sum_re = sum_im = 0;

		for (x = 0; x < N; x++)
		{
			temp = 2 * dir * CV_PI * ((double)i * x / N);

			sum_re += (tr[x] * cos(temp) + ti[x] * sin(temp));
			sum_im += (ti[x] * cos(temp) - tr[x] * sin(temp));
		}

		re[i] = sum_re;
		im[i] = sum_im;
	}

	if (dir == -1)
	{
		for (i = 0; i < N; i++)
		{
			re[i] /= (double)N;
			im[i] /= (double)N;
		}
	}

	delete[] tr;
	delete[] ti;
}

void g_fnFFT1d(double* re, double* im, int N, int dir)
{

}

bool g_fnIsPowerOf2(int n)
{
	return 0;
}