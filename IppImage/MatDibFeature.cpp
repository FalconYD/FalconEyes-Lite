#pragma once
#include "stdafx.h"
#include "MatDibFeature.h"
#include "afxwin.h"
//#include "MatDib.h"

#define CHECK_WEAK_EDGE(x,y)\
pEdge = matDst.ptr<BYTE>(y);\
if(pEdge[x] == WEAK_EDGE){\
pEdge[x] = STRONG_EDGE;\
vecStrong_edges.push_back(Point(x,y));\
}

void MatEdgeCanny(MatDib& Src, MatDib& Dst, float sigma, float th_low, float th_high)
{
	register int nStepX, nStepY;
	int nW = Src.GetWidth();
	int nH = Src.GetHeight();
	Mat matSrc, matDst;
	matSrc = Src.GetMatImg();
	matDst = Mat::zeros(Size(nW, nH), CV_8U);

	// 1. 가우시안 필터링
	float** pGauss = new float*[nH];
	for (nStepY = 0; nStepY < nH; nStepY++)
	{
		pGauss[nStepY] = new float[nW];
		memset(pGauss[nStepY], 0x00, sizeof(pGauss[nStepY][0]) * nW);
	}

	DibFilterGaussian(Src, pGauss, sigma);
	// 2. 그래디언트 구하기
	float** pGx = new float*[nH];
	for (nStepY = 0; nStepY < nH; nStepY++)
	{
		pGx[nStepY] = new float[nW];
		memset(pGx[nStepY], 0x00, sizeof(pGx[nStepY][0]) * nW);
	}
	float** pGy = new float*[nH];
	for (nStepY = 0; nStepY < nH; nStepY++)
	{
		pGy[nStepY] = new float[nW];
		memset(pGy[nStepY], 0x00, sizeof(pGy[nStepY][0]) * nW);
	}
	float** pMag = new float*[nH];
	for (nStepY = 0; nStepY < nH; nStepY++)
	{
		pMag[nStepY] = new float[nW];
		memset(pMag[nStepY], 0x00, sizeof(pMag[nStepY][0]) * nW);
	}
	
	BYTE *pEdge;
	for (nStepY = 1; nStepY < nH - 1; nStepY++)
	{
		for (nStepX = 1; nStepX < nW - 1; nStepX++)
		{
			pGx[nStepY][nStepX] = -pGauss[nStepY - 1][nStepX - 1] - 2 * pGauss[nStepY][nStepX - 1] - pGauss[nStepY + 1][nStepX - 1]
				+ pGauss[nStepY - 1][nStepX + 1] + 2 * pGauss[nStepY][nStepX + 1] + pGauss[nStepY + 1][nStepX + 1];
			pGy[nStepY][nStepX] = -pGauss[nStepY - 1][nStepX - 1] - 2 * pGauss[nStepY - 1][nStepX] - pGauss[nStepY - 1][nStepX + 1]
				+ pGauss[nStepY + 1][nStepX - 1] + 2 * pGauss[nStepY + 1][nStepX] + pGauss[nStepY + 1][nStepX + 1];

			pMag[nStepY][nStepX] = sqrt(pGx[nStepY][nStepX] * pGx[nStepY][nStepX] + pGy[nStepY][nStepX] * pGy[nStepY][nStepX]);
		}
	}

	// 3. 비최대 억제

	enum DISTRICT {AREA0 = 0, AREA45, AREA90, AREA135, NOAREA};

	const BYTE STRONG_EDGE = 255;
	const BYTE WEAK_EDGE = 128;

	std::vector<Point> vecStrong_edges;

	float fAng;
	int nDistrict;
	bool bLocal_max;
	
	for (nStepY = 1; nStepY < nH - 1; nStepY++)
	{
		for (nStepX = 1; nStepX < nW - 1; nStepX++)
		{
			// 그래디언트 크기가 th_low보다 큰 픽셀에 대해서만 국지적 최대 검사
			// 국지적 최대인 픽셀에 댛새서만 강한 엣지 또느 약한 엣지로 설정
			if (pMag[nStepY][nStepX] > th_low)
			{
				if (pGx[nStepY][nStepX] != 0.f)
				{
					fAng = static_cast<float>(atan2(pGy[nStepY][nStepX], pGx[nStepY][nStepX]) * 180 / CV_PI);
					if (((fAng >= -22.5f) && (fAng < 22.5f)) || (fAng >= 157.5f) || (fAng < -157.5f))
						nDistrict = AREA0;
					else if (((fAng >= 22.5f) && (fAng < 67.5f)) || ((fAng >= -157.5f) && (fAng < -112.5f)))
						nDistrict = AREA45;
					else if (((fAng >= 67.5f) && (fAng < 112.5f)) || ((fAng >= -112.5f) && (fAng < -67.5f)))
						nDistrict = AREA90;
					else
						nDistrict = AREA135;
				}
				else
					nDistrict = AREA90;

				// 국지적 최대 검사
				bLocal_max = false;

				switch (nDistrict)
				{
				case AREA0:
					if ((pMag[nStepY][nStepX] >= pMag[nStepY][nStepX - 1]) && (pMag[nStepY][nStepX] > pMag[nStepY][nStepX + 1]))
						bLocal_max = true;
					break;
				case AREA45:
					if ((pMag[nStepY][nStepX] >= pMag[nStepY - 1][nStepX - 1]) && (pMag[nStepY][nStepX] > pMag[nStepY + 1][nStepX + 1]))
						bLocal_max = true;
					break;
				case AREA90:
					if ((pMag[nStepY][nStepX] >= pMag[nStepY - 1][nStepX]) && (pMag[nStepY][nStepX] > pMag[nStepY + 1][nStepX]))
						bLocal_max = true;
					break;
				case AREA135:
				default:
					if ((pMag[nStepY][nStepX] >= pMag[nStepY - 1][nStepX + 1]) && (pMag[nStepY][nStepX] > pMag[nStepY + 1][nStepX - 1]))
						bLocal_max = true;
					break;
				}

				// 강한 엣지와 약한 엣지 구분
				if (bLocal_max)
				{
					if (pMag[nStepY][nStepX] > th_high)
					{
						matDst.data[nStepY * nW + nStepX] = STRONG_EDGE;
						vecStrong_edges.push_back(Point(nStepX, nStepY));
					}
					else
						matDst.data[nStepY * nW + nStepX] = WEAK_EDGE;
				}
			}
		}
	}

	// 4. 히스테리시스 엣지 트래킹
	while (!vecStrong_edges.empty())
	{
		Point p = vecStrong_edges.back();
		vecStrong_edges.pop_back();

		int x = p.x, y = p.y;

			CHECK_WEAK_EDGE(x+1, y)
			CHECK_WEAK_EDGE(x + 1, y + 1)
			CHECK_WEAK_EDGE(x, y + 1)
			CHECK_WEAK_EDGE(x - 1, y + 1)
			CHECK_WEAK_EDGE(x - 1, y)
			CHECK_WEAK_EDGE(x - 1, y - 1)
			CHECK_WEAK_EDGE(x, y - 1)
			CHECK_WEAK_EDGE(x + 1, y - 1)
	}
	for (nStepY = 0; nStepY < nH; nStepY++)
	{
		for (nStepX = 0; nStepX < nW; nStepX++)
		{
			if (matDst.data[nStepY * nW + nStepX] == WEAK_EDGE) matDst.data[nStepY * nW + nStepX] = 0;
		}
	}

	for (nStepY = 0; nStepY < nH; nStepY++)
		delete[] pGauss[nStepY];
	delete[] pGauss;
	for (nStepY = 0; nStepY < nH; nStepY++)
		delete[] pGx[nStepY];
	delete[] pGx;
	for (nStepY = 0; nStepY < nH; nStepY++)
		delete[] pGy[nStepY];
	delete[] pGy;
	for (nStepY = 0; nStepY < nH; nStepY++)
		delete[] pMag[nStepY];
	delete[] pMag;

	Dst.SetMatImg(matDst);
}

void MatHoughLine(MatDib& Src, std::vector<MatLineParam>& lines, int threshold)
{
	register int nStepX, nStepY;

	int nW = Src.GetWidth();
	int nH = Src.GetHeight();

	int num_rho = static_cast<int>(sqrt((double)nW*nW + nH*nH) * 2);
	int num_ang = 360;

	Mat matSrc = Src.GetMatImg();

	//----------------------------------------------------------------
	// 0 - PI 각도에 해당하는 sin, cos 함수의 값을 룩업테이블에 저장
	//----------------------------------------------------------------

	float* sin_tbl = new float[num_ang];
	float* cos_tbl = new float[num_ang];

	for (nStepX = 0; nStepX < num_ang; nStepX++)
	{
		sin_tbl[nStepX] = static_cast<float>(sin(nStepX * CV_PI / num_ang));
		cos_tbl[nStepX] = static_cast<float>(cos(nStepX * CV_PI / num_ang));
	}

	//----------------------------------------------------------------
	// 축척 배열(Avccumulate array) 생성
	//----------------------------------------------------------------

	int** pAcc = new int*[num_rho];
	for (nStepY = 0; nStepY < num_rho; nStepY++)
	{
		pAcc[nStepY] = new int[num_ang];
		memset(pAcc[nStepY], 0, sizeof(pAcc[nStepY][0]) * num_ang);
	}
	int m, n;
	for (nStepY = 0; nStepY < nH; nStepY++)
	{
		for (nStepX = 0; nStepX < nW; nStepX++)
		{
			if (matSrc.data[nStepY * nW + nStepX] > 128)
			{
				TRACE("X: %d, Y: %d \r\n",nStepX, nStepY);
				for (n = 0; n < num_ang; n++)
				{
					//nM = static_cast<int>(floor(nStepX * sin_tbl[nN] + nStepY*cos_tbl[nN] + 0.5f));
					m = static_cast<int>(floor(nStepX * sin_tbl[n] + nStepY * cos_tbl[n]));
					m += static_cast<int>(num_rho / 2.);

					pAcc[m][n]++;
				}
			}
		}
	}

	//----------------------------------------------------------------
	// 임계값보다 큰 국지적 최댓값최댓값을 찾아 직선 성분으로 결정
	//----------------------------------------------------------------
	
	lines.clear();
	int value;
	for (m = 0; m < num_rho; m++)
	{
		for (n = 0; n < num_ang; n++)
		{
			value = pAcc[m][n];
			//if (value > threshold)
			if(value > 1)
			{
				if (value >= pAcc[m - 1][n] && value >= pAcc[m - 1][n + 1] &&
					value >= pAcc[m][n + 1] && value >= pAcc[m + 1][n + 1] &&
					value >= pAcc[m + 1][n] && value >= pAcc[m + 1][n - 1] &&
					value >= pAcc[m][n - 1] && value >= pAcc[m - 1][n - 1])
				{
					lines.push_back(MatLineParam(m - (num_rho / 2), n * CV_PI / num_ang, value));
				}
			}
		}
	}

	//----------------------------------------------------------------
	// 동적 할당한 메모리 해제
	//----------------------------------------------------------------
	for (nStepY = 0; nStepY < num_rho; nStepY++)
		delete[] pAcc[nStepY];
	delete[] pAcc;

	delete[] sin_tbl;
	delete[] cos_tbl;
}

void MatDrawLine(MatDib& Dst, MatLineParam line, BYTE c)
{
	int w = Dst.GetWidth();
	int h = Dst.GetHeight();

	//

	int x1 = 0, y1 = 0, x2 = 0, y2 = 0;
	if ((line.ang >= 0 && line.ang < CV_PI / 4.) || (line.ang >= 3 * CV_PI / 4. && line.ang < CV_PI))
	{
		x1 = 0;
		y1 = static_cast<int>(floor(line.rho / cos(line.ang) + .5));
		x2 = w - 1;
		y2 = static_cast<int>(floor((line.rho - x2 * sin(line.ang)) / cos(line.ang) + .5));
	}
	else
	{
		y1 = 0;
		x1 = static_cast<int>(floor(line.rho / sin(line.ang) + .5));
		y2 = h - 1;
		x2 = static_cast<int>(floor((line.rho - y2 * cos(line.ang)) / sin(line.ang) + .5));
	}
	MatDrawLine(Dst, x1, y1, x2, y2, c);
}

void MatDrawLine(MatDib& Dst, int x1, int y1, int x2, int y2, BYTE c)
{
	int w = Dst.GetWidth();
	int h = Dst.GetHeight();
	Mat matSrc = Dst.GetMatImg();

	// 브레제남 알고리즘(Bresenham's Algorithm)에 의한 직선 그리기

	int dx, dy, inc_x, inc_y, fraction;

	dx = x2 - x1;
	inc_x = (dx > 0) ? 1 : -1;
	dx = abs(dx) << 1;

	dy = y2 - y1;
	inc_y = (dy > 0) ? 1 : -1;
	dy = abs(dy) << 1;

	if (x1 >= 0 && x1 < w && y1 >= 0 && y1 < h)
		matSrc.data[y1 * w + x1] = c;

	if (dx >= dy)
	{
		fraction = dy - (dx >> 1);

		while (x1 != x2)
		{
			if ((fraction >= 0) && (fraction || (inc_x > 0)))
			{
				fraction -= dx;
				y1 += inc_y;
			}

			fraction += dy;
			x1 += inc_x;

			if (x1 >= 0 && x1 < w && y1 >= 0 && y1 < h)
				matSrc.data[y1 * w + x1] = c;
		}
	}
	else
	{
		fraction = dx - (dy >> 1);

		while (y1 != y2)
		{
			if ((fraction >= 0) && (fraction || (inc_y > 0)))
			{
				fraction -= dy;
				x1 += inc_x;
			}

			fraction += dx;
			y1 += inc_y;

			if (x1 >= 0 && x1 < w && y1 >= 0 && y1 < h)
				matSrc.data[y1 * w + x1] = c;
		}
	}
	
	Dst.SetMatImg(matSrc);
}

void MatHarrisCorner(MatDib& Src, std::vector<Point>& corners, double th)
{
	register int i, j, x, y;

	int nW = Src.GetWidth();
	int nH = Src.GetHeight();

	Mat matSrc = Src.GetMatImg();
	BYTE* pTmp = (BYTE*)matSrc.data;
	BYTE** ptr = new BYTE*[nH];
	for (j = 0; j < nH; j++)
	{
		ptr[j] = &pTmp[j * nW];
	}

	//---
	// 1. (fx)*(fx), (fx)*(fy), (fy)*(fy) 계산
	//---
	float* tmpx2 = new float[nW*nH];
	float* tmpy2 = new float[nW*nH];
	float* tmpxy = new float[nW*nH];
	float** dx2 = new float*[nH];
	float** dy2 = new float*[nH];
	float** dxy = new float*[nH];
	for (j = 0; j < nH; j++)
	{
		dx2[j] = &tmpx2[j * nW];
		dy2[j] = &tmpy2[j * nW];
		dxy[j] = &tmpxy[j * nW];
	}

	float tx, ty;
	for (j = 1; j < nH - 1; j++)
	{
		for (i = 1; i < nW - 1; i++)
		{
			tx = (ptr[j - 1][i + 1] + ptr[j][i + 1] + ptr[j + 1][i + 1]
				- ptr[j - 1][i - 1] - ptr[j][i - 1] - ptr[j + 1][i - 1]) / 6.f;
			ty = (ptr[j + 1][i - 1] + ptr[j + 1][i] + ptr[j + 1][i + 1]
				- ptr[j - 1][i - 1] - ptr[j - 1][i] - ptr[j - 1][i + 1]) / 6.f;

			dx2[j][i] = tx * tx;
			dy2[j][i] = ty * ty;
			dxy[j][i] = tx * ty;
		}
	}
	//---
	// 2. 가우시안 필터링
	//---

	float* tmpGx2 = new float[nW * nH];
	float* tmpGy2 = new float[nW * nH];
	float* tmpGxy = new float[nW * nH];

	float** gdx2 = new float*[nH];
	float** gdy2 = new float*[nH];
	float** gdxy = new float*[nH];
	for (j = 0; j < nH; j++)
	{
		gdx2[j] = &tmpGx2[j * nW];
		gdy2[j] = &tmpGy2[j * nW];
		gdxy[j] = &tmpGxy[j * nW];
	}

	float g[5][5] = { 
		{ 1,  4,  6,  4, 1},
		{ 4, 16, 24, 16, 4},
		{ 6, 24, 36, 24, 6},
		{ 4, 16, 24, 16, 4}, 
		{ 1,  4,  6,  4, 1}
	};

	for (y = 0; y < 5; y++)
	{
		for (x = 0; x < 5; x++)
		{
			g[y][x] /= 256.f;
		}
	}

	float tx2, ty2, txy;
	for (j = 2; j < nH - 2; j++)
	{
		for (i = 2; i < nW - 2; i++)
		{
			tx2 = ty2 = txy = 0;
			for (y = 0; y < 5; y++)
			{
				for (x = 0; x < 5; x++)
				{
					tx2 += (dx2[j + y - 2][i + x - 2] * g[y][x]);
					ty2 += (dy2[j + y - 2][i + x - 2] * g[y][x]);
					txy += (dxy[j + y - 2][i + x - 2] * g[y][x]);
				}
			}

			gdx2[j][i] = tx2;
			gdy2[j][i] = ty2;
			gdxy[j][i] = txy;
		}
	}

	//---
	// 3. 코너 응답 함수 생성
	//---

	float* tmpCrf = new float[nW * nH];
	float** crf = new float*[nH];
	for (j = 0; j < nH; j++)
	{
		crf[j] = &tmpCrf[j * nW];
	}

	float k = 0.04f;
	for (j = 2; j < nH - 2; j++)
	{
		for (i = 2; i < nW - 2; i++)
		{
			crf[j][i] = (gdx2[j][i] * gdy2[j][i] - gdxy[j][i] * gdxy[j][i])
				- k* (gdx2[j][i] + gdy2[j][i]) * (gdx2[j][i] + gdy2[j][i]);
		}
	}

	//---
	// 4. 임계값보다 큰 국지적 최댓값을 찾아 코너 포인트로 결정
	//---

	corners.clear();
	float cvf_value;
	for (j = 2; j < nH - 2; j++)
	{
		for (i = 2; i < nW - 2; i++)
		{
			cvf_value = crf[j][i];
			if (cvf_value > th)
			{
				if (cvf_value > crf[j - 1][i] && cvf_value > crf[j - 1][i + 1] &&
					cvf_value > crf[j][i + 1] && cvf_value > crf[j + 1][i + 1] &&
					cvf_value > crf[j + 1][i] && cvf_value > crf[j + 1][i - 1] &&
					cvf_value > crf[j][i + 1] && cvf_value > crf[j - 1][i - 1])
				{
					corners.push_back(Point(i, j));
				}
			}
		}
	}
	delete tmpCrf;
	delete tmpGx2;
	delete tmpGy2;
	delete tmpGxy;
	delete tmpx2;
	delete tmpy2;
	delete tmpxy;
	delete[] crf;
	delete[] gdx2;
	delete[] gdy2;
	delete[] gdxy;
	delete[] dx2;
	delete[] dy2;
	delete[] dxy;
	delete[] ptr;
}