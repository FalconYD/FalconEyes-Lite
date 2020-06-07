#include "stdafx.h"
#include "MatDibSegment.h"
#include "MatDib.h"
#include "MatDibExt.h"
#include "MatDibFourier.h"

void MatBinarization(MatDib& Src, MatDib& Dst, int threshold)
{
	//Src.CreateGrayImage(Dst.GetWidth(), Dst.GetHeight());
	Dst = Src;
	int size = static_cast<int>(Src.GetSize());
	Mat matSrc = Src.GetMatImg();
	Mat matDst = Dst.GetMatImg();

	for (int i = 0; i < size; i++)
	{
		matDst.data[i] = (matSrc.data[i] <= threshold) ? 0 : 255;
	}
	Dst.SetMatImg(matDst);
}

int MatBinarizationIterative(MatDib& Src)
{
	float hist[256] = { 0, };
	DibHistogram(Src, hist);

	int  i, T, Told;

	float sum = 0.f;
	for (i = 0; i < 256; i++)
		sum += (i*hist[i]);

	T = static_cast<int>(sum + .5f);

	float a1, b1, u1, a2, b2, u2;
	do 
	{
		Told = T;
		
		a1 = b1 = u1 = 0.f;
		for (i = 0; i <= Told; i++)
		{
			a1 += (i*hist[i]);
			b1 += hist[i];
		}

		if (b1 != 0.f)
			u1 = a1 / b1;

		a2 = b2 = u2 = 0.f;
		for (i = Told + 1; i < 256; i++)
		{
			a2 += (i*hist[i]);
			b2 += hist[i];
		}

		if (b2 != 0.f)
			u2 = a2 / b2;

		T = static_cast<int>((u1 + u2) / 2 + .5f);
	} while (T != Told);

	return T;
}

int MatLabeling(Mat& matSrc, std::vector<MatLabelInfo>& labels)
{
	int nW = matSrc.cols;
	int nH = matSrc.rows;

	//std::vector<std::vector<int>> vecMap(nH, std::vector<int>(nW,0));
	int** pMap = new int*[nH];
	for (int y = 0; y < nH; y++)
	{
		pMap[y] = new int[nW];
		memset(pMap[y], 0, sizeof(pMap[y][0]) * nW);
	}

	const int MAX_LABEL = 100000;
	int eq_tbl[MAX_LABEL][2] = { {0,}, };

	// -----
	// 첫 번째 스캔 - 초기 레이블 지정 및 등가 테이블 생성
	// -----
	register int nStepX, nStepY;
	int label = 0, maxl, minl, min_eq, max_eq;

	for (nStepY = 1; nStepY < nH; nStepY++)
	{
		for (nStepX = 1; nStepX < nW; nStepX++)
		{
			if (matSrc.data[nStepY * nW + nStepX] == 255)
			{
				// 바로 위 픽셀과 왼쪽 픽셀 모두에 레이블이 존재하는 경우
				if ((pMap[nStepY - 1][nStepX] != 0) && (pMap[nStepY][nStepX - 1] != 0))
				{
					if (pMap[nStepY - 1][nStepX] == pMap[nStepY][nStepX - 1])
					{
						// 두 레이블이 서로 같은 경우
						pMap[nStepY][nStepX] = pMap[nStepY - 1][nStepX];
					}
					else
					{
						// 두 레이블이 서로 다른 경우, 작은 레이블을 부여
						maxl = __max(pMap[nStepY - 1][nStepX], pMap[nStepY][nStepX - 1]);
						minl = __min(pMap[nStepY - 1][nStepX], pMap[nStepY][nStepX - 1]);

						pMap[nStepY][nStepX] = minl;

						min_eq = __min(eq_tbl[maxl][1], eq_tbl[minl][1]);
						max_eq = __max(eq_tbl[maxl][1], eq_tbl[minl][1]);

						eq_tbl[eq_tbl[max_eq][1]][1] = min_eq;
					}
				}
				else if (pMap[nStepY - 1][nStepX] != 0)
				{
					pMap[nStepY][nStepX] = pMap[nStepY - 1][nStepX];
				}
				else if (pMap[nStepY][nStepX - 1] != 0)
				{
					pMap[nStepY][nStepX] = pMap[nStepY][nStepX - 1];
				}
				else
				{
					// 이웃에 레이블이 존재하지 않으면 새로운 레이블을 부여
					label++;
					pMap[nStepY][nStepX] = label;
					eq_tbl[label][0] = label;
					eq_tbl[label][1] = label;
				}
			}
		}
	}
	// -----
	// 등가 테이블 정리
	// -----
	int temp;
	for (nStepX = 1; nStepX < label; nStepX++)
	{
		temp = eq_tbl[nStepX][1];
		if (temp != eq_tbl[nStepX][0])
		{
			eq_tbl[nStepX][1] = eq_tbl[temp][1];
		}
	}

	// 등가 테이블의 레이블을 1부터 차례대로 증가시키기
	int* hash = new int[label + 1];
	memset(hash, 0, sizeof(int)*(label + 1));

	for (nStepX = 1; nStepX <= label; nStepX++)
	{
		hash[eq_tbl[nStepX][1]] = eq_tbl[nStepX][1];
	}

	int label_cnt = 1;
	for (nStepX = 1; nStepX <= label; nStepX++)
	{
		if (hash[nStepX] != 0)
		{
			hash[nStepX] = label_cnt++;
		}
	}

	for (nStepX = 1; nStepX <= label; nStepX++)
	{
		eq_tbl[nStepX][1] = hash[eq_tbl[nStepX][1]];
	}

	delete[] hash;

	//-----
	// 두 번째 스캔 - 등가 테이블을 이용하여 모든 픽셀에 고유의 레이블 부여
	//-----

	int** pDst = new int*[nH];
	for (nStepY = 0; nStepY < nH; nStepY++)
	{
		pDst[nStepY] = new int[nW];
		memset(pDst[nStepY], 0, sizeof(pDst[nStepY][0])*nW);
	}
	//std::vector<std::vector<int>> vecDst(nH, std::vector<int>(nW, 0));

	int nIdx;
	for (nStepY = 1; nStepY < nH; nStepY++)
	{
		for (nStepX = 1; nStepX < nW; nStepX++)
		{
			if (pMap[nStepY][nStepX] != 0)
			{
				nIdx = pMap[nStepY][nStepX];
				pDst[nStepY][nStepX] = eq_tbl[nIdx][1];
			}
		}
	}

	//-----
	// MatLableInfo 정보 작성
	//-----
	labels.resize(label_cnt - 1);

	MatLabelInfo* pLabel;
	for (nStepY = 1; nStepY < nH; nStepY++)
	{
		for (nStepX = 1; nStepX < nW; nStepX++)
		{
			if (pDst[nStepY][nStepX] != 0)
			{
				pLabel = &labels.at(pDst[nStepY][nStepX] - 1);

				pLabel->pixels.push_back(Point(nStepX, nStepY));
				pLabel->cx += nStepX;
				pLabel->cy += nStepY;

				if (nStepX < pLabel->minx) pLabel->minx = nStepX;
				if (nStepX > pLabel->maxx) pLabel->maxx = nStepX;
				if (nStepY < pLabel->miny) pLabel->miny = nStepY;
				if (nStepY > pLabel->maxy) pLabel->maxy = nStepY;
				ASSERT(pLabel->pixels.size() != 0);
			}
		}
	}

	for (MatLabelInfo& label : labels)
	{
		ASSERT(label.pixels.size() != 0);
		label.cx /= static_cast<int>(label.pixels.size());
		label.cy /= static_cast<int>(label.pixels.size());
	}

	for (nStepY = 0; nStepY < nH; nStepY++)
		delete[] pMap[nStepY];
	delete[] pMap;

	for (nStepY = 0; nStepY < nH; nStepY++)
		delete[] pDst[nStepY];
	delete[] pDst;

	return (label_cnt - 1);
}

int MatLabeling(MatDib& dibSrc, std::vector<MatLabelInfo>& labels)
{
	int nW = dibSrc.GetWidth();
	int nH = dibSrc.GetHeight();

	Mat matSrc = dibSrc.GetMatImg();
	//std::vector<std::vector<int>> vecMap(nH, std::vector<int>(nW,0));
	int** pMap = new int*[nH];
	for (int y = 0; y < nH; y++)
	{
		pMap[y] = new int[nW];
		memset(pMap[y], 0, sizeof(pMap[y][0]) * nW);
	}

	const int MAX_LABEL = 100000;
	int eq_tbl[MAX_LABEL][2] = { {0,}, };

	// -----
	// 첫 번째 스캔 - 초기 레이블 지정 및 등가 테이블 생성
	// -----
	register int nStepX, nStepY;
	int label = 0, maxl, minl, min_eq, max_eq;

	for (nStepY = 1; nStepY < nH; nStepY++)
	{
		for (nStepX = 1; nStepX < nW; nStepX++)
		{
			if (matSrc.data[nStepY * nW + nStepX] == 255)
			{
				// 바로 위 픽셀과 왼쪽 픽셀 모두에 레이블이 존재하는 경우
				if ((pMap[nStepY - 1][nStepX] != 0) && (pMap[nStepY][nStepX - 1] != 0))
				{
					if (pMap[nStepY - 1][nStepX] == pMap[nStepY][nStepX - 1])
					{
						// 두 레이블이 서로 같은 경우
						pMap[nStepY][nStepX] = pMap[nStepY - 1][nStepX];
					}
					else
					{
						// 두 레이블이 서로 다른 경우, 작은 레이블을 부여
						maxl = __max(pMap[nStepY - 1][nStepX], pMap[nStepY][nStepX - 1]);
						minl = __min(pMap[nStepY - 1][nStepX], pMap[nStepY][nStepX - 1]);

						pMap[nStepY][nStepX] = minl;

						min_eq = __min(eq_tbl[maxl][1], eq_tbl[minl][1]);
						max_eq = __max(eq_tbl[maxl][1], eq_tbl[minl][1]);

						eq_tbl[eq_tbl[max_eq][1]][1] = min_eq;
					}
				}
				else if (pMap[nStepY - 1][nStepX] != 0)
				{
					pMap[nStepY][nStepX] = pMap[nStepY - 1][nStepX];
				}
				else if (pMap[nStepY][nStepX - 1] != 0)
				{
					pMap[nStepY][nStepX] = pMap[nStepY][nStepX - 1];
				}
				else
				{
					// 이웃에 레이블이 존재하지 않으면 새로운 레이블을 부여
					label++;
					pMap[nStepY][nStepX] = label;
					eq_tbl[label][0] = label;
					eq_tbl[label][1] = label;
				}
			}
		}
	}
	// -----
	// 등가 테이블 정리
	// -----
	int temp;
	for (nStepX = 1; nStepX < label; nStepX++)
	{
		temp = eq_tbl[nStepX][1];
		if (temp != eq_tbl[nStepX][0])
		{
			eq_tbl[nStepX][1] = eq_tbl[temp][1];
		}
	}

	// 등가 테이블의 레이블을 1부터 차례대로 증가시키기
	int* hash = new int[label + 1];
	memset(hash, 0, sizeof(int)*(label + 1));

	for (nStepX = 1; nStepX <= label; nStepX++)
	{
		hash[eq_tbl[nStepX][1]] = eq_tbl[nStepX][1];
	}

	int label_cnt = 1;
	for (nStepX = 1; nStepX <= label; nStepX++)
	{
		if (hash[nStepX] != 0)
		{
			hash[nStepX] = label_cnt++;
		}
	}

	for (nStepX = 1; nStepX <= label; nStepX++)
	{
		eq_tbl[nStepX][1] = hash[eq_tbl[nStepX][1]];
	}

	delete[] hash;

	//-----
	// 두 번째 스캔 - 등가 테이블을 이용하여 모든 픽셀에 고유의 레이블 부여
	//-----

	int** pDst = new int*[nH];
	for (nStepY = 0; nStepY < nH; nStepY++)
	{
		pDst[nStepY] = new int[nW];
		memset(pDst[nStepY], 0, sizeof(pDst[nStepY][0])*nW);
	}
	//std::vector<std::vector<int>> vecDst(nH, std::vector<int>(nW, 0));

	int nIdx;
	for (nStepY = 1; nStepY < nH; nStepY++)
	{
		for (nStepX = 1; nStepX < nW; nStepX++)
		{
			if (pMap[nStepY][nStepX] != 0)
			{
				nIdx = pMap[nStepY][nStepX];
				pDst[nStepY][nStepX] = eq_tbl[nIdx][1];
			}
		}
	}

	//-----
	// MatLableInfo 정보 작성
	//-----
	labels.resize(label_cnt - 1);

	MatLabelInfo* pLabel;
	for (nStepY = 1; nStepY < nH; nStepY++)
	{
		for (nStepX = 1; nStepX < nW; nStepX++)
		{
			if (pDst[nStepY][nStepX] != 0)
			{
				pLabel = &labels.at(pDst[nStepY][nStepX] - 1);
				
				pLabel->pixels.push_back(Point(nStepX, nStepY));
				pLabel->cx += nStepX;
				pLabel->cy += nStepY;

				if (nStepX < pLabel->minx) pLabel->minx = nStepX;
				if (nStepX > pLabel->maxx) pLabel->maxx = nStepX;
				if (nStepY < pLabel->miny) pLabel->miny = nStepY;
				if (nStepY > pLabel->maxy) pLabel->maxy = nStepY;
				ASSERT(pLabel->pixels.size() != 0);
			}
		}
	}

	for (MatLabelInfo& label : labels)
	{
		ASSERT(label.pixels.size() != 0);
		label.cx /= static_cast<int>(label.pixels.size());
		label.cy /= static_cast<int>(label.pixels.size());
	}

	for (nStepY = 0; nStepY < nH; nStepY++)
		delete[] pMap[nStepY];
	delete[] pMap;

	for (nStepY = 0; nStepY < nH; nStepY++)
		delete[] pDst[nStepY];
	delete[] pDst;

	return (label_cnt - 1);
}

void MatContourTracing(MatDib& dibSrc, int sx, int sy, std::vector<Point>& cp)
{
	int nW = dibSrc.GetWidth();
	int nH = dibSrc.GetHeight();

	Mat matSrc = dibSrc.GetMatImg();

	cp.clear();

	if (matSrc.data[sy * nW + sx] != 255)
		return;

	int x, y, nx, ny;

	int d, cnt;
	int dir[8][2] = { // 진행 방향을 나타내는 배열
		{1, 0},
		{1, 1},
		{0, 1},
		{-1, 1},
		{-1, 0},
		{-1, -1},
		{0, -1},
		{1, -1}
	};

	x = sx;
	y = sy;
	d = cnt = 0;

	while (TRUE)
	{
		nx = x + dir[d][0];
		ny = y + dir[d][1];

		if (nx < 0 || nx >= nW || ny < 0 || ny >= nH || matSrc.data[ny * nW + nx] == 0)
		{
			// 진행 방향에 있는 픽셀이 객체가 아닌 경우,
			// 시계 방향으로 진행방향을 바꾸고 다시 시도한다.
			if (++d > 7) d = 0;
			cnt++;

			// 8방향 모두 배경인 경우
			if (cnt >= 8)
			{
				cp.push_back(Point(x, y));
				break; // 외곽선 추적을 끝냄.
			}
		}
		else
		{
			// 진행 방향의 픽셀이 객체일 경우, 현재 점을 외곽선 정보에 저장
			cp.push_back(Point(x, y));
			
			// 진행 방향으로 이동
			x = nx;
			y = ny;

			// 방향 정보 초기화
			cnt = 0;
			d = (d + 6) % 8; // d = d - 2 와 같은 형태
		}

		// 시작점으로 돌아왔고, 진행 방향이 초기화 된 경우,
		// 외곽선 추적을 끝낸다.
		if (x == sx && y == sy && d == 0)
			break;
	}
}

void MatMorphologyErosion(Mat& matSrc, Mat& matDst)
{
	int i, j;
	int w = matSrc.cols;
	int h = matSrc.rows;

	matDst = matSrc.clone();

	BYTE* pDst = matDst.data;
	BYTE* pSrc = matSrc.data;

	for (j = 1; j < h - 1; j++)
	{
		for (i = 1; i < w - 1; i++)
		{
			if (pSrc[j * w + i] != 0)
			{
				if (pSrc[j * w + i] == 0 || pSrc[(j - 1) * w + (i + 1)] == 0 ||
					pSrc[j * w + (i - 1)] == 0 || pSrc[j * w + (i + 1)] == 0 ||
					pSrc[(j + 1) * w + (i - 1)] == 0 || pSrc[(j + 1) * w + i] == 0 ||
					pSrc[(j + 1) * w + (i + 1)] == 0 || pSrc[(j - 1) * w + (i - 1)] == 0)
				{
					pDst[j * w + i] = 0;
				}
			}
		}
	}
}

void MatMorphologyDilation(Mat& matSrc, Mat& matDst)
{
	int i, j;
	int w = matSrc.cols;
	int h = matSrc.rows;

	matDst = matSrc.clone();

	BYTE* pDst = matDst.data;
	BYTE* pSrc = matSrc.data;

	for (j = 1; j < h-1; j++)
	{
		for (i = 1; i < w-1; i++)
		{
			if (pSrc[j * w + i] == 0)
			{
				if (pSrc[(j - 1) * w + i] != 0 || pSrc[(j - 1) * w + (i + 1)] != 0 ||
					pSrc[j * w + (i - 1)] != 0 || pSrc[j * w + (i + 1)] != 0 ||
					pSrc[(j + 1) * w + (i - 1)] != 0 || pSrc[(j + 1) * w + i] != 0 ||
					pSrc[(j + 1) * w + (i + 1)] != 0 || pSrc[(j - 1) * w + (i - 1)] != 0)
				{
					pDst[j * w + i] = 255;
				}
			}
		}
	}
}

void MatMorphologyOpening(Mat& matSrc, Mat& matDst)
{
	Mat matTmp;
	MatMorphologyErosion(matSrc, matTmp);
	MatMorphologyDilation(matTmp, matDst);
}

void MatMorphologyClosing(Mat& matSrc, Mat& matDst)
{
	Mat matTmp;
	MatMorphologyDilation(matSrc, matTmp);
	MatMorphologyErosion(matTmp, matDst);
}

void MatMorphologyGrayErosion(Mat& matSrc, Mat& matDst)
{
	int i, j, m, n, x, y, pmin;
	int w = matSrc.cols;
	int h = matSrc.rows;
	
	matDst = matSrc.clone();

	BYTE* pDst = matDst.data;
	BYTE* pSrc = matSrc.data;

	for (j = 0; j < h; j++)
	{
		for (i = 0; i < w; i++)
		{
			pmin = 255;

			for (n = -1; n <= 1; n++)
			{
				for (m = -1; m <= 1; m++)
				{
					x = i + m;
					y = j + n;
					if (x >= 0 && x < w && y >= 0 && y < h)
					{
						if (pSrc[y * w + x] < pmin)
							pmin = pSrc[y * w + x];
					}
				}
			}

			pDst[j * w + i] = pmin;
		}
	}
}

void MatMorphologyGrayDilation(Mat& matSrc, Mat& matDst)
{
	int i, j, m, n, x, y, pmax;
	int w = matSrc.cols;
	int h = matSrc.rows;

	matDst = matSrc.clone();

	BYTE* pDst = matDst.data;
	BYTE* pSrc = matSrc.data;

	for (j = 0; j < h; j++)
	{
		for (i = 0; i < w; i++)
		{
			pmax = 0;
			for (n = -1; n <= 1; n++)
			{
				for (m = -1; m <= 1; m++)
				{
					x = i + m;
					y = j + n;
					if (x >= 0 && x < w && y >= 0 && y < h)
					{
						if (pSrc[y * w + x] > pmax)
							pmax = pSrc[y * w + x];
					}
				}
			}
			pDst[j * w + i] = pmax;
		}
	}
}

void MatMorphologyGrayOpening(Mat& matSrc, Mat& matDst)
{
	Mat matTmp;
	MatMorphologyGrayErosion(matSrc, matTmp);
	MatMorphologyGrayDilation(matTmp, matDst);
}

void MatMorphologyGrayClosing(Mat& matSrc, Mat& matDst)
{
	Mat matTmp;
	MatMorphologyGrayDilation(matSrc, matTmp);
	MatMorphologyGrayErosion(matTmp, matDst);
}

void MatBinarizationCVOtsu(MatDib& Src, MatDib& Dst)
{
	//Src.CreateGrayImage(Dst.GetWidth(), Dst.GetHeight());
	Dst = Src;
	int size = static_cast<int>(Src.GetSize());
	Mat matSrc = Src.GetMatImg();
	Mat matDst = Dst.GetMatImg();

	cv::threshold(matSrc, matDst, 0, 255, THRESH_BINARY | THRESH_OTSU);
	
	Dst.SetMatImg(matDst);
}

void MatBinarizationCVAdaptive(MatDib& Src, MatDib& Dst)
{
	Dst = Src;
	int size = static_cast<int>(Src.GetSize());
	Mat matSrc = Src.GetMatImg();
	Mat matDst = Dst.GetMatImg();

	cv::adaptiveThreshold(matSrc, matDst, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 99, 10);

	Dst.SetMatImg(matDst);
}

void MatFourierDescriptor(MatDib& dibSrc, int sx, int sy, int percent, std::vector<Point>& cp)
{
	MatContourTracing(dibSrc, sx, sy, cp);

	int num = (int)cp.size();
	double* x = new double[num];
	double* y = new double[num];

	for (int step = 0; step < num; step++)
	{
		x[step] = static_cast<double>(cp[step].x);
		y[step] = static_cast<double>(cp[step].y);
	}

	g_fnDFT1d(x, y, num, 1);

	int p = num * percent / 100;
	for (int step = p; step < num; step++)
	{
		x[step] = 0.;
		y[step] = 0.;
	}

	g_fnDFT1d(x, y, num, -1);

	int w = dibSrc.GetWidth();
	int h = dibSrc.GetHeight();

	cp.clear();
	Point pt;
	for (int step = 0; step < num; step++)
	{
		pt.x = limit(static_cast<int>(x[step] + 0.5), 0, w - 1);
		pt.y = limit(static_cast<int>(y[step] + 0.5), 0, h - 1);
		cp.push_back(pt);
	}

	delete[] x;
	delete[] y;
}
void MatInvariantMoments(MatDib& dibSrc, double m[7])
{
	double g00, g10, g01, g20, g02, g11, g30, g03, g21, g12;
	double c00, c20, c02, c11, c30, c03, c21, c12;
	double n20, n02, n11, n30, n03, n21, n12;
	
	// 기하학적 모멘트 구하기

	g00 = MatGeometricMoment(dibSrc, 0, 0);
	g10 = MatGeometricMoment(dibSrc, 1, 0);
	g01 = MatGeometricMoment(dibSrc, 0, 1);
	g20 = MatGeometricMoment(dibSrc, 2, 0);
	g02 = MatGeometricMoment(dibSrc, 0, 2);
	g11 = MatGeometricMoment(dibSrc, 1, 1);
	g30 = MatGeometricMoment(dibSrc, 3, 0);
	g03 = MatGeometricMoment(dibSrc, 0, 3);
	g21 = MatGeometricMoment(dibSrc, 2, 1);
	g12 = MatGeometricMoment(dibSrc, 1, 2);

	// 중심 모멘트 구하기

	double cx = g10 / g00;
	double cy = g02 / g00;

	c00 = g00;
	c20 = g20 - cx * g10;
	c02 = g02 - cy * g01;
	c11 = g11 - cx * g01;
	c30 = g30 - 3 * cx * g20 + 2 * cx * cx * g10;
	c03 = g03 - 3 * cy * g02 + 2 * cy * cy * g01;
	c21 = g21 - 2 * cx * g11 - cy * g20 + 2 * cx * cx * g01;
	c12 = g12 - 2 * cy * g11 - cx * g02 + 2 * cy * cy * g10;

	// 정규화된 중심 모멘트

	n20 = c20 / pow(c00, 2.);
	n02 = c02 / pow(c00, 2.);
	n11 = c11 / pow(c00, 2.);
	n30 = c30 / pow(c00, 2.5);
	n03 = c03 / pow(c00, 2.5);
	n21 = c21 / pow(c00, 2.5);
	n12 = c12 / pow(c00, 2.5);

	// 불변 모멘트 구하기

	m[0] = n20 + n02;
	m[1] = (n20 - n02)*(n20 - n02) + 4 * n11 * n11;
	m[2] = (n30 - 3 * n12)*(n30 - 3 * n12) + (3 * n21 - n03) * (3 * n21 - n03);
	m[3] = (n30 + n12) * (n30 + n12) + (n21 + n03)*(n21 + n03);
	m[4] = (n30 - 3 * n12)*(n30 + n12) *((n30 + n12)*(n30 + n12) - 3 * (n21 + n03)*(n21 + n03))
		+ (3 * n21 - n03)*(n21 + n03) * (3 * (n30 + n12)*(n30 + n12) - (n21 + n03)*(n21 + n03));
	m[5] = (n20 - n02) * ((n30 + n12)*(n30 + n12) - (n21 + n03)*(n21 + n03)) + 4 * n11*(n30 + n12)*(n21 + n03);
	m[6] = (3 * n21 - n03)*(n30 + n12) * ((n30 + n12) * (n30 + n12) - 3 * (n21 + n03)*(n21 + n03))
		+ (3 * n12 - n30)*(n21 + n03)*(3 * (n30 + n12)*(n30 + n12) - (n21 + n03)*(n21 + n03));
}

double MatGeometricMoment(MatDib& dibSrc, int p, int q)
{
	int w = dibSrc.GetWidth();
	int h = dibSrc.GetHeight();
	BYTE* ptr = dibSrc.GetDIBitsAddr();
	
	register int i, j, k;
	double moment = 0, xp, yq;
	for (j = 0; j< h; j++)
		for (i = 0; i < w; i++)
		{
			xp = yq = 1;
			for (k = 0; k < p; k++) xp *= i;
			for (k = 0; k < q; k++) yq *= j;

			moment += (xp  * yq * ptr[j * w + i]);
		}

	return moment;
}

bool MatZernikeMoments(MatDib& dibSrc, int n, int m, double& zr, double& zi)
{
	if (n < 0 || ((n - abs(m)) % 2 != 0) || abs(m) > n)
		return false;

	if (n > 8)
		return false;

	const int ZM_RADIUS = 100;
	const int ZM_SIZE = ZM_RADIUS * 2 + 1;
	const int FACT[] = { 1, 1, 2, 6, 24, 120, 720, 5040, 40320 };

	// 저니키 방사 다항식 계산

	double zm_poly[ZM_RADIUS + 1] = { 0. , };

	int sign;
	double rho;
	for (int r = 0; r <= ZM_RADIUS; r++)
	{
		for (int s = 0; s <= ((n - m) / 2); s++)
		{
			sign = (s % 2 == 0) ? 1 : -1;
			rho = static_cast<double>(r) / ZM_RADIUS;

			zm_poly[r] += (sign * FACT[n-s] * pow(rho, (n-2 *s))) / (FACT[s] * FACT[(n + m) / 2 - s] * FACT[(n - m) / 2 - s]);
		}
	}

	// 저니키 기저 함수 계산

	double zm_basis_real[ZM_SIZE][ZM_SIZE];
	double zm_basis_imag[ZM_SIZE][ZM_SIZE];
	int x, y, dist;
	double theta;

	for (y = 0; y < ZM_SIZE; y++)
		for (x = 0; x < ZM_SIZE; x++)
		{
			dist = static_cast<int>(hypot(y - ZM_RADIUS, x - ZM_RADIUS));

			if (dist <= ZM_RADIUS)
			{
				theta = atan2(y - ZM_RADIUS, x - ZM_RADIUS);
				zm_basis_real[y][x] = zm_poly[dist] * cos(m*theta);
				zm_basis_imag[y][x] = zm_poly[dist] * sin(m*theta);
			}
		}

	zm_basis_real[ZM_RADIUS][ZM_RADIUS] = zm_poly[0];
	zm_basis_imag[ZM_RADIUS][ZM_RADIUS] = 0.0;

	// 저니키 모멘트 계산
	MatDib dibDst;
	DibResizeBilinear(dibSrc, dibDst, ZM_SIZE, ZM_SIZE);
	BYTE* pDst = dibDst.GetDIBitsAddr();

	zr = zi = 0;
	for (y = 0; y < ZM_SIZE; y++)
		for (x = 0; x < ZM_SIZE; x++)
		{
			dist = static_cast<int>(hypot(y - ZM_RADIUS, x - ZM_RADIUS));

			if (dist <= ZM_RADIUS)
			{
				zr += ((pDst[y*ZM_SIZE + x] / 255.) * zm_basis_real[y][x]);
				zi -= ((pDst[y*ZM_SIZE + x] / 255.) * zm_basis_imag[y][x]); // Complex conjugate
			}
		}

	return true;
}

Point MatTemplatematching(MatDib& dibSrc, MatDib& dibTmpl, int* arrayMap)
{
	int sw = dibSrc.GetWidth();
	int sh = dibSrc.GetHeight();
	int tw = dibTmpl.GetWidth();
	int th = dibTmpl.GetHeight();
	int tw2 = tw / 2;
	int th2 = th / 2;

// 	if (arrayMap != nullptr)
// 	{
// 		delete arrayMap;
// 		arrayMap = nullptr;
// 	}
// 
// 	arrayMap = new int[sw*sh];

	BYTE* pSrc = dibSrc.GetDIBitsAddr();
	BYTE* pTmpl = dibTmpl.GetDIBitsAddr();
	//BYTE* pMap = dibMap.GetDIBitsAddr();

	Point dp;

	int i, j, x, y;
	int min_value = 99999;
	int diff, sum_of_diff;
	for(j = th2; j < sh - th2; j++)
		for (i = tw2; i <= sw - tw2; i++)
		{
			sum_of_diff = 0;
			for (y = 0; y < th; y++)
			{
				for (x = 0; x < tw; x++)
				{
					diff = pSrc[(j - th2 + y) * sw + (i - tw2 + x)] - pTmpl[y*tw + x];
					sum_of_diff += (diff * diff);
				}
			}
			arrayMap[j * sw + i] = sum_of_diff / (tw * th);

			if (arrayMap[j * sw + i] < min_value)
			{
				min_value = arrayMap[j*sw + i];
				dp.x = i;
				dp.y = j;
			}
	}

	return dp;
}

Point MatTemplatematching_Optimize(MatDib& dibSrc, MatDib& dibTmpl, int* arrayMap)
{
	int sw = dibSrc.GetWidth();
	int sh = dibSrc.GetHeight();
	int tw = dibTmpl.GetWidth();
	int th = dibTmpl.GetHeight();
	int tw2 = tw / 2;
	int th2 = th / 2;

	BYTE* pSrc = dibSrc.GetDIBitsAddr();
	BYTE* pTmpl = dibTmpl.GetDIBitsAddr();

	Point dp;

	int i, j, x, y;
	int min_value = 99999;
	int diff, sum_of_diff;

	for (j = th2; j < sh - th2; j++)
		for (i = tw2; i <= sw - tw2; i++)
		{
			sum_of_diff = 0;
			for (y = 0; y < th; y++)
			{
				for (x = 0; x < tw; x++)
				{
					diff = pSrc[(j - th2 + y) * sw + (i - tw2 + x)] - pTmpl[y*tw + x];
					sum_of_diff += (diff * diff);
				}
			}

			arrayMap[j * sw + i] = sum_of_diff / (tw * th);

			if (arrayMap[j * sw + i] < min_value)
			{
				min_value = arrayMap[j*sw + i];
				dp.x = i;
				dp.y = j;
			}
		}

	return dp;
}