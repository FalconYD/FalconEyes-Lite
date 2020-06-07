#pragma once
#include "stdafx.h"
#include "MatDibExt.h"

void DibInverse(MatDib& src)
{
	//m_matImg = ~m_matImg;
	
	uint unShiftStep = static_cast<int>(src.GetWidth() * src.GetHeight() * src.GetBitCount()/8.);
	uchar* p = src.GetMatAddr();
	for (uint nStep = 0; nStep < unShiftStep; nStep++)
	{
		p[nStep] = 255 - p[nStep];
	}
	src.UpdateBack();
	src.UpdateDib();
}

void DibBrightness(MatDib& src, int n)
{
	uint unShiftStep = static_cast<int>(src.GetWidth() * src.GetHeight()* src.GetBitCount() / 8.);
	uchar* p = src.GetMatAddr();
	int nTemp = 0;
	for (uint nStep = 0; nStep < unShiftStep; nStep++)
	{
		p[nStep] = limit(p[nStep] + n);
	}
	src.UpdateBack();
	src.UpdateDib();
}

void DibConstrast(MatDib& src, int n)
{
	uint unShiftStep = static_cast<int>(src.GetWidth() * src.GetHeight()* src.GetBitCount() / 8.);
	uchar* p = src.GetMatAddr();
	for (uint nStep = 0; nStep < unShiftStep; nStep++)
	{
		p[nStep] = static_cast<uchar>(limit(p[nStep] + (p[nStep] - 128) * n / 100.0));
	}
	src.UpdateBack();
	src.UpdateDib();
}

void DibGammaCorrection(MatDib& src, float gamma)
{
	uint nStep;
	float inv_gamma = 1.f / gamma;
	float gamma_table[256];
	for (nStep = 0; nStep < 256; nStep++)
		gamma_table[nStep] = pow((nStep / 255.f), inv_gamma);

	uint unShiftStep = static_cast<int>(src.GetWidth() * src.GetHeight()* src.GetBitCount() / 8.);
	uchar* p = src.GetMatAddr();
	int nTemp = 0;
	for (nStep = 0; nStep < unShiftStep; nStep++)
	{
		p[nStep] = static_cast<uchar>(limit(gamma_table[p[nStep]] * 255 + 0.5f));
	}
	src.UpdateBack();
	src.UpdateDib();
}

void DibHistogram(MatDib& src, float histo[256])
{
	uint nStep;
	short nChannel = static_cast<int>(src.GetBitCount() / 8.0);
	uint unShiftStep = static_cast<uint>(src.GetWidth() * src.GetHeight()* nChannel);
	uchar* p = src.GetMatAddr();
	uint cnt[256];
	memset(cnt, 0, sizeof(int) * 256);
	for (nStep = 0; nStep < unShiftStep; nStep++)
	{
		cnt[p[nStep]]++;
	}

	for (nStep = 0; nStep < 256; nStep++)
	{
		histo[nStep] = static_cast<float>(cnt[nStep] / (double)unShiftStep);
	}
}

void DibHistogramStretching(MatDib& src)
{
	uint nStep;
	uint unShiftStep = static_cast<int>(src.GetWidth() * src.GetHeight()* src.GetBitCount() / 8.);
	uchar* p = src.GetMatAddr();
	// 최대, 최소 그레이스케일 값 계산
	BYTE max, min;
	min = 255;
	max = 0;

	for (nStep = 0; nStep < unShiftStep; nStep++)
	{
		if (max <= p[nStep])
			max = p[nStep];
		if (min >= p[nStep])
			min = p[nStep];
	}

	if (max == min)
		return;

	// 히스토그램 스트래칭
	for (nStep = 0; nStep < unShiftStep; nStep++)
	{
		p[nStep] = static_cast<uchar>(limit((p[nStep] - min) * 255 / (double)(max - min)));
	}
	src.UpdateBack();
	src.UpdateDib();;
}

void DibHistogramEqualization(MatDib& src)
{
	uint nStep;
	uint unShiftStep = static_cast<int>(src.GetWidth() * src.GetHeight()* src.GetBitCount() / 8.);
	uchar* p = src.GetMatAddr();
	int nTemp = 0;
	// 히스토그램 계산

	float hist[256] = { 0, };

	DibHistogram(src, hist);

	// 히스토그램 누적 함수 계산
	float cdf[256] = { 0.0, };
	cdf[0] = hist[0];

	for (nStep = 1; nStep < 256; nStep++)
	{
		cdf[nStep] = cdf[nStep - 1] + hist[nStep];
	}
	// 히스토그램 균등화
	for (nStep = 0; nStep < unShiftStep; nStep++)
	{
		p[nStep] = static_cast<BYTE>(limit(cdf[p[nStep]] * 255));
	}
	src.UpdateBack();
	src.UpdateDib();
}

BOOL DibAdd(MatDib& dst, MatDib src1, MatDib src2)
{
	int w = src1.GetWidth();
	int h = src1.GetHeight();

	if (w != src2.GetWidth() || h != src2.GetHeight())
		return FALSE;


	int size = static_cast<int>(dst.GetSize());
	BYTE* p1 = src1.GetMatAddr();
	BYTE* p2 = src2.GetMatAddr();
	BYTE* p3 = dst.GetMatAddr();

	for (int i = 0; i < size; i++)
		p3[i] = limit(p1[i] + p2[i]);
	
	dst.UpdateBack();
	dst.UpdateDib();
	return TRUE;
}

BOOL DibAvg(MatDib& dst, MatDib src1, MatDib src2)
{
	int w = src1.GetWidth();
	int h = src1.GetHeight();

	if (w != src2.GetWidth() || h != src2.GetHeight())
		return FALSE;

	int size = static_cast<int>(dst.GetSize());
	BYTE* p1 = src1.GetMatAddr();
	BYTE* p2 = src2.GetMatAddr();
	BYTE* p3 = dst.GetMatAddr();

	for (int i = 0; i < size; i++)
		p3[i] = static_cast<BYTE>((p1[i] - p2[i]) / 2.0);
	
	dst.UpdateBack();
	dst.UpdateDib();
	return TRUE;
}

BOOL DibDiff(MatDib& dst, MatDib src1, MatDib src2) 
{
	int w = src1.GetWidth();
	int h = src1.GetHeight();

	if (w != src2.GetWidth() || h != src2.GetHeight())
		return FALSE;

	if (!dst.IsValid())
	{
		dst.CreateGrayImage(w,h);
	}
	int size = static_cast<int>(dst.GetSize());
	BYTE* p1 = src1.GetMatAddr();
	BYTE* p2 = src2.GetMatAddr();
	BYTE* p3 = dst.GetMatAddr();

	int diff;
	for (int i = 0; i < size; i++)
	{
		diff = p1[i] - p2[i];
		p3[i] = static_cast<BYTE>((diff >= 0) ? diff : -diff);
	}

	dst.UpdateBack();
	dst.UpdateDib();
	return TRUE;
}

BOOL DibAnd(MatDib& dst, MatDib src1, MatDib src2)
{
	int w = src1.GetWidth();
	int h = src1.GetHeight();

	if (w != src2.GetWidth() || h != src2.GetHeight())
		return FALSE;

	int size = static_cast<int>(dst.GetSize());
	BYTE* p1 = src1.GetMatAddr();
	BYTE* p2 = src2.GetMatAddr();
	BYTE* p3 = dst.GetMatAddr();

	for (int i = 0; i < size; i++)
		p3[i] = static_cast<BYTE>(p1[i] & p2[i]);

	dst.UpdateBack();
	dst.UpdateDib();
	return TRUE;
}
BOOL DibOR(MatDib& dst, MatDib src1, MatDib src2)
{
	int w = src1.GetWidth();
	int h = src1.GetHeight();

	if (w != src2.GetWidth() || h != src2.GetHeight())
		return FALSE;

	int size = static_cast<int>(dst.GetSize());
	BYTE* p1 = src1.GetMatAddr();
	BYTE* p2 = src1.GetMatAddr();
	BYTE* p3 = dst.GetMatAddr();

	for (int i = 0; i < size; i++)
		p3[i] = static_cast<BYTE>(p1[i] | p2[i]);

	dst.UpdateBack();
	dst.UpdateDib();
	return TRUE;
}

BOOL DibSub(MatDib& dst, MatDib src1, MatDib src2)
{
	int w = src1.GetWidth();
	int h = src1.GetHeight();

	if (w != src2.GetWidth() || h != src2.GetHeight())
		return FALSE;

	int size = static_cast<int>(dst.GetSize());
	BYTE* p1 = src1.GetMatAddr();
	BYTE* p2 = src2.GetMatAddr();
	BYTE* p3 = dst.GetMatAddr();

	for (int i = 0; i < size; i++)
	{
		p3[i] = limit(p1[i] - p2[i]);
	}
	dst.UpdateBack();
	dst.UpdateDib();
	return TRUE;
}

void DibToGray(MatDib src, MatDib& dst)
{
	int size = static_cast<int>(src.GetSize());
	BYTE* p1 = src.GetMatAddr();
	int nW = src.GetWidth();
	int nH = src.GetHeight();
	DWORD dwBit = src.GetBitCount();

	if (dst.IsValid())
		dst.DestroyBitmap();

	dst.CreateGrayImage(nW, nH);

	BYTE* p2 = dst.GetMatAddr();
	int i = 0;
	int j = 0;
	int nCh = static_cast<int>(src.GetBitCount() / 8.0);
	while (j < size)
	{
		//p2[j] = static_cast<BYTE>(p1[i + 2] * 0.299 + p1[i + 1] * 0.587 + p1[i] * 0.114);
		p2[j] = static_cast<BYTE>((p1[i + 2] * 4899 + p1[i + 1] * 9617 + p1[i] * 1868) >> 14);
		j++;
		i += nCh;
	}
	dst.UpdateBack();
	dst.UpdateDib();
}

void DibToGrayCV(MatDib src, MatDib& dst)
{
	Mat matSrc, matDst;
	matSrc = src.GetMatImg();
	cvtColor(matSrc, matDst, ColorConversionCodes::COLOR_BGR2GRAY);
	dst.SetMatImg(matDst);
}

void DibBitPlane(MatDib src, MatDib& dst, int nBit)
{
	int size = static_cast<int>(src.GetSize());
	BYTE* p1 = src.GetMatAddr();
	BYTE* p2 = dst.GetMatAddr();
	for (int i = 0; i < size; i++)
	{
		p2[i] = (p1[i] & 1 << nBit) ? 255 : 0;
	}
	dst.UpdateBack();
	dst.UpdateDib();
}


void DibFilterMean(MatDib src, MatDib& dst, int size)
{
	Mat matSrc, matDst;
	matSrc = src.GetMatImg();
	matDst = dst.GetMatImg();
	blur(matSrc, matDst, Size(size, size));
	dst.SetMatImg(matDst);
}

void DibFilterMedianMean(MatDib src, MatDib& dst, int ksize)
{
	Mat matSrc, matDst;
	matSrc = src.GetMatImg();
	matDst = dst.GetMatImg();
	medianBlur(matSrc, matDst, 3);
	dst.SetMatImg(matDst);
}

void DibFilterWeightMean(MatDib src, MatDib& dst)
{
	Mat matSrc, matDst;
	int nW = src.GetWidth();
	int nH = src.GetHeight();

	matSrc = src.GetMatImg();
	matDst = dst.GetMatImg();

	int nMask[3][3] = {
		{1,2,1},
		{2,4,2},
		{1,2,1},
	};

	int nStepX, nStepY, nPosX, nPosY, nSum;

	for (nStepY = 1; nStepY < nH - 1; nStepY++)
	for (nStepX = 1; nStepX < nW - 1; nStepX++)
	{
		nSum = 0;
		for (nPosY = 0; nPosY < 3; nPosY++)
			for (nPosX = 0; nPosX < 3; nPosX++)
			{
				nSum += matSrc.data[(nStepX - 1 + nPosX) + nW * (nStepY - 1 + nPosY)] * nMask[nPosX][nPosY];
			}
		matDst.data[nStepX + nW * nStepY] = static_cast<BYTE>(limit(nSum / 16.+ 0.5));
	}
	dst.SetMatImg(matDst);
}

void DibFilterGaussian(MatDib src, float** pDst, float sigma)
{
	register int i, j, k, x;

	int nW = src.GetWidth();
	int nH = src.GetHeight();

	Mat matSrc = src.GetMatImg();

	//---------------------------------------------------------------
	// 1차원 가우시안 마스크 & 실수 연산을 위한 버퍼 이미지 생성
	//---------------------------------------------------------------
	int dim = static_cast<int> (2 * 4 * sigma + 1.0);
	if (dim < 3) dim = 3;
	if (dim % 2 == 0) dim++;
	int dim2 = static_cast<int>(dim / 2.);

	float* pMask = new float[dim];
	memset(pMask, 0x00, sizeof(float)*dim);
	for (i = 0; i < dim; i++)
	{
		x = i - dim2;
		pMask[i] = static_cast<float>(exp(-(x*x) / (2 * sigma*sigma)) / (sqrt(2 * CV_PI)*sigma));
	}

	float** pBuf = new float*[nH];
	for (int y = 0; y < nH; y++)
	{
		pBuf[y] = new float[nW];
		memset(pBuf[y], 0x00, sizeof(pBuf[y][0]) * nW);
	}
	//---------------------------------------------------------------
	// 세로 방향 마스크 연산
	//---------------------------------------------------------------

	float sum1, sum2;
	for (i = 0; i < nW; i++)
	{
		for (j = 0; j < nH; j++)
		{
			sum1 = sum2 = 0.0f;
			for (k = 0; k < dim; k++)
			{
				x = k - dim2 + j;
				if (x >= 0 && x < nH)
				{
					sum1 += pMask[k];
					sum2 += (pMask[k] * matSrc.data[x * nW + i]);
				}
			}
			pBuf[j][i] = sum2 / sum1;
		}
	}
	

	//---------------------------------------------------------------
	// 가로 방향 마스크 연산
	//---------------------------------------------------------------

	for (j = 0; j < nH; j++)
	{
		for (i = 0; i < nW; i++)
		{
			sum1 = sum2 = 0.f;

			for (k = 0; k < dim; k++)
			{
				x = k - dim2 + i;

				if (x >= 0 && x < nW)
				{
					sum1 += pMask[k];
					sum2 += (pMask[k] * pBuf[j][x]);
				}
			}
			pDst[j][i] = sum2 / sum1;
		}
	}
	src.UpdateBack();
	src.UpdateDib();
	delete pMask;
	for (int y = 0; y < nH; y++)
		delete[] pBuf[y];
	delete[] pBuf;
}

void DibFilterGaussian_CV(MatDib src, MatDib& dst, float sigma)
{
	// opencv Mat 가우시안 함수
	Mat matSrc = src.GetMatImg();
	Mat matDst = dst.GetMatImg();
	int dim = static_cast<int> (2 * 4 * sigma + 1.0);
	if (dim > 9)
		dim = 9;
	GaussianBlur(matSrc, matDst, Size(static_cast<int>(sqrt(dim)), static_cast<int>(sqrt(dim))), static_cast<int>(sigma));
	dst.SetMatImg(matDst);
}

void DibFilterBilateral(MatDib src, MatDib& dst, int d, float sigmacolor, float sigmaspace)
{
	Mat matSrc, matDst;
	matSrc = src.GetMatImg();
	matDst = dst.GetMatImg();
	bilateralFilter(matSrc, matDst, d, sigmacolor, sigmaspace);
	dst.SetMatImg(matDst);
}

void DibFilterLaplacian(MatDib src, MatDib& dst)
{
	Mat matSrc, matDst;
	int bit = src.GetBitCount();
	int w = src.GetWidth();
	int h = src.GetHeight();

	matSrc = src.GetMatImg();
	matDst = dst.GetMatImg();

	int stepX, stepY;
	double sum;
	int nPosX = 0;

	for (stepY = 1; stepY < h - 1; stepY++)
	{
		for (stepX = 1; stepX < w - 1; stepX ++)
		{
			nPosX = stepX;
			sum = matSrc.data[nPosX + w * (stepY - 1)]
				+ matSrc.data[(nPosX - 1) + w * stepY]
				+ matSrc.data[nPosX + w * (stepY + 1)]
				+ matSrc.data[(nPosX + 1) + w * stepY]
				- 4* matSrc.data[nPosX + w * stepY];
			matDst.data[nPosX + w * stepY] = static_cast<uchar>(limit(sum));
		}
	}
	dst.SetMatImg(matDst);
}

void DibFilterUnsharpMask(MatDib src, MatDib& dst)
{
	Mat matSrc, matDst;
	int bit = src.GetBitCount();
	int w = src.GetWidth();
	int h = src.GetHeight();
	
	matSrc = src.GetMatImg();
	matDst = dst.GetMatImg();
	
	int stepX, stepY;
	double sum;
	int nPosX = 0;

	for (stepY = 1; stepY < h - 1; stepY++)
	{
		for (stepX = 1; stepX < w - 1; stepX++)
		{
			nPosX = stepX;
			sum = 5 * matSrc.data[nPosX + w * stepY]
				- matSrc.data[nPosX + w * (stepY - 1)]
				- matSrc.data[(nPosX - 1) + w * stepY]
				- matSrc.data[nPosX + w * (stepY + 1)]
				- matSrc.data[(nPosX + 1) + w * stepY];
			matDst.data[nPosX + w * stepY] = static_cast<uchar>(limit(sum));
		}
	}
	dst.SetMatImg(matDst);
}

void DibFilterHighboost(MatDib src, MatDib& dst, float alpha)
{
	Mat matSrc, matDst;
	int w = src.GetWidth();
	int h = src.GetHeight();

	matSrc = src.GetMatImg();
	matDst = dst.GetMatImg();

	int stepX, stepY;
	float sum;
	int nPosX = 0;
	for (stepY = 1; stepY < h - 1; stepY++)
	{
		for (stepX = 1; stepX < w - 1; stepX++)
		{
			nPosX = stepX++;
			sum = (4 + alpha)* matSrc.data[nPosX + w * stepY]
				- matSrc.data[nPosX + w * (stepY - 1)]
				- matSrc.data[(nPosX - 1) + w * stepY]
				- matSrc.data[nPosX + w * (stepY + 1)]
				- matSrc.data[(nPosX + 1) + w * stepY];
			matDst.data[nPosX + w * stepY] = static_cast<uchar>(limit(sum + 0.5f));
		}
	}
	dst.SetMatImg(matDst);
}
void DibFilterDiffusion(MatDib src, MatDib& dst, float lambda, float k, int iter)
{
	Mat matSrc, matDst;
	int w = src.GetWidth();
	int h = src.GetHeight();

	matSrc = src.GetMatImg();
	matDst = dst.GetMatImg();

	register int stepX, stepY, step;
	double gradn, grads, grade, gradw;
	double gcn, gcs, gce, gcw;
	double k2 = k * k;
	for (step = 0; step < iter; step++)
	{
		for (stepY = 1; stepY < h - 1; stepY++)
		{
			for (stepX = 1; stepX < w - 1; stepX++)
			{
				gradn = static_cast<double>(matSrc.data[stepX + w * (stepY - 1)] - matSrc.data[stepX + w * stepY]);
				grads = static_cast<double>(matSrc.data[stepX + w * (stepY + 1)] - matSrc.data[stepX + w * stepY]);
				grade = static_cast<double>(matSrc.data[(stepX - 1) + w * stepY] - matSrc.data[stepX + w * stepY]);
				gradw = static_cast<double>(matSrc.data[(stepX + 1) + w * stepY] - matSrc.data[stepX + w * stepY]);
				gcn = static_cast<double>(gradn / (1.0f + gradn * gradn / k2));
				gcs = static_cast<double>(grads / (1.0f + grads * grads / k2));
				gce = static_cast<double>(grade / (1.0f + grade * grade / k2));
				gcw = static_cast<double>(gradw / (1.0f + gradw * gradw / k2));
				matDst.data[stepX + w * stepY] = static_cast<uchar>(matSrc.data[stepX + w * stepY] + lambda * (gcn + gcs + gce + gcw));
			}
		}
	}
	dst.SetMatImg(matDst);
}

void DibTranslate(MatDib src, MatDib& dst, int sx, int sy)
{
	Mat matSrc, matDst;
	int w = src.GetWidth();
	int h = src.GetHeight();

	matSrc = src.GetMatImg();
	matDst = dst.GetMatImg();

	int i, j, x, y;

	for (j = 0; j < h; j++)
	{
		for (i = 0; i < w; i++)
		{
			x = i - sx;
			y = j - sy;
			matDst.data[i + w * j] = 0;
			if (x >= 0 && x < w && y >= 0 && y < h)
			{
				matDst.data[i + w * j] = matSrc.data[x + w * y];
			}
		}
	}
	dst.SetMatImg(matDst);
}

void DibResizeNearest(MatDib src, MatDib& dst, int nw, int nh)
{
	Mat matSrc, matDst;
	matSrc = src.GetMatImg();
	int w = src.GetWidth();
	int h = src.GetHeight();

	dst.CreateGrayImage(nw, nh);
	matDst = dst.GetMatImg();


	int i, j, x, y, nPos;
	double rx, ry;
	for (j = 0; j < nh; j++)
	{
		for (i = 0; i < nw; i++)
		{
			nPos = i;
			rx = static_cast<double> (w - 1) * nPos / (nw - 1);
			ry = static_cast<double> (h - 1) * j / (nh - 1);
			x = static_cast<int> (rx + 0.5);
			y = static_cast<int>(ry + 0.5);

			if (x >= w) x = w - 1;
			if (y >= h) y = h - 1;

			matDst.data[nPos + nw * j] = matSrc.data[x + w * y];
		}
	}
	dst.SetMatImg(matDst);
}

void DibResizeBilinear(MatDib src, MatDib& dst, int nw, int nh)
{
	Mat matSrc, matDst;
	matSrc = src.GetMatImg();
	int w = src.GetWidth();
	int h = src.GetHeight();

	dst.CreateGrayImage(nw, nh);
	matDst = dst.GetMatImg();

	int i, j, x1, y1, x2, y2, nPos;
	double rx, ry, p, q, value;
	for (j = 0; j < nh; j++)
	{
		for (i = 0; i < nw; i++)
		{
			nPos = i;
			rx = static_cast<double>(w - 1) * nPos / (nw - 1);
			ry = static_cast<double>(h - 1) * j / (nh - 1);

			x1 = static_cast<int>(rx);
			y1 = static_cast<int>(ry);

			x2 = x1 + 3; if (x2 >= w) x2 = w - 1;
			y2 = y1; if (y2 == h) y2 = h - 1;

			p = rx - x1;
			q = ry - y1;

			value = (1. - p) * (1. - q) * matSrc.data[x1 + w * y1]
				+ p * (1. - q) * matSrc.data[x2 + w * y1]
				+ (1. - p) * q * matSrc.data[x1 + w * y2]
				+ p * q * matSrc.data[x2 + w * y2];

			matDst.data[nPos + nw * j] = static_cast<BYTE>(limit(value + .5));
		}
	}
	dst.SetMatImg(matDst);
}

void DibResizeCubic(MatDib src, MatDib& dst, int nw, int nh)
{
	Mat matSrc, matDst;
	matSrc = src.GetMatImg();
	int w = src.GetWidth();
	int h = src.GetHeight();

	dst.CreateGrayImage(nw, nh);
	matDst = dst.GetMatImg();

	int i, j, x1, x2, x3, x4, y1, y2, y3, y4, nPos;
	double v1, v2, v3, v4, v;
	double rx, ry, p, q;
	for (j = 0; j < nh; j++)
	{
		for (i = 0; i < nw; i++)
		{
			nPos = i;
			rx = static_cast<double>(w - 1) * nPos / (nw - 1);
			ry = static_cast<double>(h - 1) * j / (nh - 1);

			x2 = static_cast<int>(rx);

			x1 = x2 - 1; if (x1 < 0) x1 = 0;
			x3 = x2 + 1; if (x3 >= w) x3 = w - 1;
			x4 = x2 + 2; if (x4 >= w) x4 = w - 1;
			p = rx - x2;

			y2 = static_cast<int>(ry);
			y1 = y2 - 1; if (y1 < 0) y1 = 0;
			y3 = y2 + 1; if (y3 >= h) y3 = h - 1;
			y4 = y2 + 2; if (y4 >= h) y4 = h - 1;
			q = ry - y2;

			v1 = cubic_interpolation(matSrc.data[x1 + w * y1], matSrc.data[x2 + w * y1], matSrc.data[x3 + w * y1], matSrc.data[x4 + w * y1], p);
			v2 = cubic_interpolation(matSrc.data[x1 + w * y2], matSrc.data[x2 + w * y2], matSrc.data[x3 + w * y2], matSrc.data[x4 + w * y2], p);
			v3 = cubic_interpolation(matSrc.data[x1 + w * y3], matSrc.data[x2 + w * y3], matSrc.data[x3 + w * y3], matSrc.data[x4 + w * y3], p);
			v4 = cubic_interpolation(matSrc.data[x1 + w * y4], matSrc.data[x2 + w * y4], matSrc.data[x3 + w * y4], matSrc.data[x4 + w * y4], p);

			v = cubic_interpolation(v1, v2, v3, v4, q);
			matDst.data[nPos + nw * j] = static_cast<BYTE>(limit(v + 0.5));
		}
	}
	dst.SetMatImg(matDst);
}

double cubic_interpolation(double v1, double v2, double v3, double v4, double d)
{
	double v, p1, p2, p3, p4;

	p1 = 2 * v2;
	p2 = -v1 + v3;
	p3 = 2 * v1 - 5 * v2 + 4 * v3 - v4;
	p4 = -v1 + 3 * v2 - 3 * v3 + v4;

	v = (p1 + d * (p2 + d * (p3 + d * p4))) / 2.0;
	return v;
}

void DibNoiseGaussian(MatDib src, MatDib& dst, int amount)
{
	int size = static_cast<int>(src.GetSize());

	BYTE* pSrc = src.GetMatAddr();
	BYTE* pDst = dst.GetMatAddr();

	unsigned int seed = static_cast<unsigned int>(time(NULL));
	std::default_random_engine generator(seed);
	std::normal_distribution<double> distribution(0.0, 1.0);

	double rn;
	for (int i = 0; i < size; i++)
	{
		rn = distribution(generator) * 255 * amount / 100.0;
		pDst[i] = static_cast<BYTE>(limit(pSrc[i] + rn));
	}
	dst.UpdateBack();
	dst.UpdateDib();
}

void DibNoiseSaltPepper(MatDib src, MatDib& dst, int amount)
{
	int size = static_cast<int>(src.GetSize());

	BYTE* pSrc = src.GetMatAddr();
	BYTE* pDst = dst.GetMatAddr();
	memcpy(pDst, pSrc, sizeof(BYTE) * size);
	unsigned int seed = static_cast<unsigned int>(time(0));
	std::default_random_engine generator(seed);
	std::uniform_int_distribution<int> distribution(0, size - 1);

	int num = static_cast<int>(size * amount / 100.0);
	for (int i = 0; i < num; i++)
	{
		pDst[distribution(generator)] = (i & 0x01) * 255;
	}
	dst.UpdateBack();
	dst.UpdateDib();
}

void DibRotate(MatDib src, MatDib& dst, double angle)
{
	int w = src.GetWidth();
	int h = src.GetHeight();

	double rad = (angle * CV_PI) / 180.0;
	double cos_value = cos(rad);
	double sin_value = sin(rad);

	// 회전 후 생성되는 영상의 크기를 계산.
	// 4개의 코너 포인트의 이동 좌표를 계산하여 최대, 최소점의 차이를 구한다.

	int nx, ny, minx, miny, maxx, maxy, nw, nh;

	minx = maxx = 0;
	miny = maxy = 0;

	nx = static_cast<int>(floor((w - 1) * cos_value + 0.5));
	ny = static_cast<int>(floor((w - 1) * sin_value + 0.5));
	minx = (minx < nx) ? minx : nx; maxx = (maxx > nx) ? maxx : nx;
	miny = (miny < ny) ? miny : ny; maxy = (maxy > ny) ? maxy : ny;

	nx = static_cast<int>(floor(-(h - 1) * sin_value + 0.5));
	ny = static_cast<int>(floor((h - 1) * cos_value + 0.5));
	minx = (minx < nx) ? minx : nx; maxx = (maxx > nx) ? maxx : nx;
	miny = (miny < ny) ? miny : ny; maxy = (maxy > ny) ? maxy : ny;

	nx = static_cast<int>(floor((w - 1) * cos_value - (h - 1) * sin_value + 0.5));
	ny = static_cast<int>(floor((w - 1) * sin_value + (h - 1) * cos_value + 0.5));
	minx = (minx < nx) ? minx : nx; maxx = (maxx > nx) ? maxx : nx;
	miny = (miny < ny) ? miny : ny; maxy = (maxy > ny) ? maxy : ny;

	nw = maxx - minx + 1;
	nh = maxy - miny + 1;

	// 실제 회전 변환
	Mat matSrc, matDst;
	matSrc = src.GetMatImg();

	dst.CreateGrayImage(nw, nh);
	matDst = dst.GetMatImg();

	int i, j, x1, x2, y1, y2;
	double rx, ry, p, q, temp;

	for (j = miny; j <= maxy; j++)
	{
		for (i = minx; i <= maxx; i++)
		{
			rx = i * cos_value + j * sin_value;
			ry = -i * sin_value + j * cos_value;

			x1 = static_cast<int>(rx);
			y1 = static_cast<int>(ry);

			// 원본 영상 내에 포함된 좌표가 아니라면 무시.
			if (x1 < 0 || x1 > w - 1 || y1 < 0 || y1 > h - 1)
				continue;

			x2 = x1 + 1; if (x2 == w) x2 = w - 1;
			y2 = y1 + 1; if (y2 == h) y2 = h - 1;

			p = rx - x1;
			q = ry - y1;

			temp = (1.0 - p) * (1.0 - q) * matSrc.data[x1 + w * y1]
				+ p * (1.0 - q) * matSrc.data[x2 + w * y1]
				+(1.0 - p) * q * matSrc.data[x1 + w * y2]
				+ p * q * matSrc.data[x2 + w * y2];
			matDst.data[(i-minx) + (maxx - minx + 1) * (j-miny)] = static_cast<BYTE>(limit(temp));
		}
	}
	dst.SetMatImg(matDst);
}

void DibRotate90(MatDib src, MatDib& dst)
{
	Mat matSrc, matDst;
	matSrc = src.GetMatImg();
	int w = src.GetWidth();
	int h = src.GetHeight();

	dst.CreateGrayImage(h, w);
	matDst = dst.GetMatImg();

	int i, j;
	for (j = 0; j < h; j ++)
	{
		for (i = 0; i < w; i++)
		{
			matDst.data[j + i * h] = matSrc.data[i + ((h - 1) - j) * w];
		}
	}
	dst.SetMatImg(matDst);
}
void DibRotate180(MatDib src, MatDib& dst)
{
	Mat matSrc, matDst;
	matSrc = src.GetMatImg();
	int w = src.GetWidth();
	int h = src.GetHeight();

	dst.CreateGrayImage(w, h);
	matDst = dst.GetMatImg();

	int i, j;
	for (j = 0; j < h; j++)
	{
		for (i = 0; i < w; i ++)
		{
			matDst.data[i + j * w] = matSrc.data[(w - 1 - i) + ((h - 1) - j) * w];
		}
	}
	dst.SetMatImg(matDst);
}
void DibRotate270(MatDib src, MatDib& dst)
{
	Mat matSrc, matDst;
	matSrc = src.GetMatImg();
	int w = src.GetWidth();
	int h = src.GetHeight();

	dst.CreateGrayImage(h, w);
	matDst = dst.GetMatImg();

	int i, j;
	for (j = 0; j < h; j++)
	{
		for (i = 0; i < w; i++)
		{
			matDst.data[j + i * h] = matSrc.data[i + j * w];
		}
	}
	dst.SetMatImg(matDst);
}

void DibMatMirror(MatDib src, MatDib& dst)
{
	Mat matSrc, matDst;
	matSrc = src.GetMatImg();
	int w = src.GetWidth();
	int h = src.GetHeight();

	dst.CreateGrayImage(w, h);
	matDst = dst.GetMatImg();

	int i, j;
	for (j = 0; j < h; j++)
	{
		for (i = 0; i < w; i ++)
		{
			matDst.data[i + j * w] = matSrc.data[(w-1) - i + j * w];
		}
	}
	dst.SetMatImg(matDst);
}

void DibMatFlip(MatDib src, MatDib& dst)
{
	Mat matSrc, matDst;
	matSrc = src.GetMatImg();
	int w = src.GetWidth();
	int h = src.GetHeight();

	dst.CreateGrayImage(w, h);
	matDst = dst.GetMatImg();

	int i, j;
	for (j = 0; j < h; j++)
	{
		for (i = 0; i < w; i ++)
		{
			matDst.data[i + j * w] = matSrc.data[i + ((h - 1) - j) * w];
		}
	}

	dst.SetMatImg(matDst);
}

void DibMatDFT(MatDib src, MatDib& dst)
{
	Mat padded, gray;
	gray = src.GetMatImg();
	//cvtColor(m_matImg, gray, CV_BGR2GRAY);
	int m = getOptimalDFTSize(gray.rows);
	int n = getOptimalDFTSize(gray.cols);
	copyMakeBorder(gray, padded, 0, m - gray.rows, 0, n - gray.cols, BORDER_CONSTANT, Scalar::all(0));

	Mat planes[] = { Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F) };
	Mat complexl;
	merge(planes, 2, complexl);

	dft(complexl, complexl);
	

	split(complexl, planes);
	magnitude(planes[0], planes[1], planes[0]);
	Mat magl = planes[0];

	magl += Scalar::all(1);
	log(magl, magl);

	magl = magl(Rect(0, 0, magl.cols&-2, magl.rows & -2));

	int cx = magl.cols / 2;
	int cy = magl.rows / 2;

	Mat q0(magl, Rect(0, 0, cx, cy));
	Mat q1(magl, Rect(cx, 0, cx, cy));
	Mat q2(magl, Rect(0, cy, cx, cy));
	Mat q3(magl, Rect(cx, cy, cx, cy));

	Mat tmp;
	q0.copyTo(tmp);
	q3.copyTo(q0);
	tmp.copyTo(q3);

	q1.copyTo(tmp);
	q2.copyTo(q1);
	tmp.copyTo(q2);

	normalize(magl, magl, 0, 1, NormTypes::NORM_MINMAX);
	magl.convertTo(magl, CV_8U, 255.0);
	dst.SetMatImg(magl);
	//imshow("spectrum", magl);
}

void DibEdgeRoberts(MatDib src, MatDib& dst)
{
	int nW = src.GetWidth();
	int nH = src.GetHeight();
	
	Mat matSrc = src.GetMatImg();
	Mat matOut = dst.GetMatImg();
	

	BYTE *pSrc1, *pSrc2;
	BYTE *pDst;
	int nStepX, nStepY, nH1, nH2;
	double dHval;
	for (nStepY = 1; nStepY < nH; nStepY++)
	{
		pSrc1 = matSrc.ptr<BYTE>(nStepY - 1);
		pSrc2 = matSrc.ptr<BYTE>(nStepY);
		pDst = matOut.ptr<BYTE>(nStepY);
		for (nStepX = 1; nStepX < nW - 1; nStepX++)
		{
			nH1 = pSrc2[nStepX] - pSrc1[nStepX - 1];
			nH2 = pSrc2[nStepX] - pSrc1[nStepX + 1];

			dHval = sqrt(static_cast<double>(nH1 * nH1 + nH2*nH2));

			pDst[nStepX] = static_cast<BYTE>(limit(dHval + 0.5));
		}
	}

	dst.SetMatImg(matOut);
}

void DibEdgePrewitt(MatDib src, MatDib& dst)
{
	int nW = src.GetWidth();
	int nH = src.GetHeight();
	//dib.CreateGrayImage(nW, nH);
	Mat matSrc = src.GetMatImg();
	Mat matOut = Mat::zeros(Size(nW, nH), CV_8UC1);
	
	BYTE *pSrc1, *pSrc2, *pSrc3;
	BYTE *pDst;
	int nStepX, nStepY, nH1, nH2;
	double dHval;
	for (nStepY = 1; nStepY < nH - 1; nStepY++)
	{
		pSrc1 = matSrc.ptr<BYTE>(nStepY - 1);
		pSrc2 = matSrc.ptr<BYTE>(nStepY);
		pSrc3 = matSrc.ptr<BYTE>(nStepY + 1);
		pDst = matOut.ptr<BYTE>(nStepY);
		for (nStepX = 1; nStepX < nW - 1; nStepX++)
		{
			nH1 = -pSrc1[nStepX - 1] - pSrc1[nStepX] - pSrc1[nStepX + 1]
				+ pSrc3[nStepX - 1] + pSrc3[nStepX] + pSrc3[nStepX + 1];
			nH2 = -pSrc1[nStepX - 1] - pSrc2[nStepX - 1] - pSrc3[nStepX - 1]
				+ pSrc1[nStepX + 1] + pSrc2[nStepX + 1] + pSrc3[nStepX + 1];

			dHval = sqrt(static_cast<double>(nH1 * nH1 + nH2*nH2));

			pDst[nStepX] = static_cast<BYTE>(limit(dHval + 0.5));
		}
	}

	dst.SetMatImg(matOut);
}

void DibEdgeSobel(MatDib src, MatDib& dst)
{
	int nW = src.GetWidth();
	int nH = src.GetHeight();
	dst.CreateGrayImage(nW, nH);
	Mat matSrc = src.GetMatImg();
	Mat matOut = dst.GetMatImg();


	BYTE *pSrc1, *pSrc2, *pSrc3;
	BYTE *pDst;
	int nStepX, nStepY, nH1, nH2;
	double dHval;
	for (nStepY = 1; nStepY < nH; nStepY++)
	{
		pSrc1 = matSrc.ptr<BYTE>(nStepY - 1);
		pSrc2 = matSrc.ptr<BYTE>(nStepY);
		pSrc3 = matSrc.ptr<BYTE>(nStepY + 1);
		pDst = matOut.ptr<BYTE>(nStepY);
		for (nStepX = 1; nStepX < nW - 1; nStepX++)
		{
			nH1 = -pSrc1[nStepX - 1] - 2 * pSrc1[nStepX] - pSrc1[nStepX + 1]
				+ pSrc3[nStepX - 1] + 2 * pSrc3[nStepX] + pSrc3[nStepX + 1];
			nH2 = -pSrc1[nStepX - 1] - 2 * pSrc2[nStepX - 1] - pSrc3[nStepX - 1]
				+ pSrc1[nStepX + 1] + 2 * pSrc2[nStepX + 1] + pSrc3[nStepX + 1];

			dHval = sqrt(static_cast<double>(nH1 * nH1 + nH2 * nH2));

			pDst[nStepX] = static_cast<BYTE>(limit(dHval + 0.5));
		}
	}

	dst.SetMatImg(matOut);
}