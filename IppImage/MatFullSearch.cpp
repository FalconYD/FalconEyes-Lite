#include "stdafx.h"
#include "MatFullSearch.h"
#include "MatDibFeature.h"

#define BLOCK_SIZE 16
#define WINDOW_SIZE 3
MatFullSearch::MatFullSearch()
{
}


MatFullSearch::~MatFullSearch()
{
	DeleteMotion();
}

void MatFullSearch::SetImages(MatDib* pImage1, MatDib* pImage2)
{
	int w = pImage1->GetWidth();
	int h = pImage1->GetHeight();

	ASSERT(w == pImage2->GetWidth() && h == pImage2->GetHeight());

	m_pImage1 = pImage1;
	m_pImage2 = pImage2;

	m_nBlockCol = w / BLOCK_SIZE;
	m_nBlockRow = h / BLOCK_SIZE;

	// m_nBlockCol, m_nBlockRow 값을 설정하였으니
	// Motion vector를 저장할 공간(m_pMotion)을 할당한다.
	InitMotion();
}

void MatFullSearch::InitMotion()
{
	if (m_pMotion)
		DeleteMotion();

	m_pMotion = new MotionVector*[m_nBlockRow];
	for (int i = 0; i < m_nBlockRow; i++)
	{
		m_pMotion[i] = new MotionVector[m_nBlockCol];
		memset(m_pMotion[i], 0, sizeof(MotionVector)*m_nBlockCol);
	}
}

void MatFullSearch::DeleteMotion()
{
	if (m_pMotion != nullptr)
	{
		for (register int i = 0; i < m_nBlockRow; i++)
		{
			delete[] m_pMotion[i];
		}
		delete[] m_pMotion;
		m_pMotion = nullptr;
	}
}

MotionVector** MatFullSearch::GetMotionVector()
{
	return m_pMotion;
}

void MatFullSearch::FullSearch()
{
	for(int j = 0; j< m_nBlockRow; j++)
		for (int i = 0; i < m_nBlockCol; i++)
		{
			// 각 블럭에서 motion vector를 구하여 2차원 배열 m_pMotion에 저장
			m_pMotion[j][i] = BlockMotion(i, j);
		}
}

MotionVector MatFullSearch::BlockMotion(int bx, int by)
{
	int x1, y1, x2 = 0, y2 = 0;
	double err, min_err;
	MotionVector motion;

	x1 = bx * BLOCK_SIZE;
	y1 = by * BLOCK_SIZE;

	min_err = GetMad(x1, y1, x1, y1);
	motion.x = 0;
	motion.y = 0;

	for ( int j = -WINDOW_SIZE; j <= WINDOW_SIZE; j++)
		for (int i = -WINDOW_SIZE; i <= WINDOW_SIZE; i++)
		{
			if (j == 0 && i == 0) continue;

			x2 = x1 + i;
			y2 = y1 + j;

			err = GetMad(x1, y1, x2, y2);

			if (err < min_err)
			{
				min_err = err;
				motion.x = i;
				motion.y = j;
			}
		}

	if (min_err < 2.0)
		motion.x = motion.y = 0;

	return motion;
}

double MatFullSearch::GetMad(int x1, int y1, int x2, int y2)
{
	int w = m_pImage1->GetWidth();
	int h = m_pImage1->GetHeight();

	BYTE* ptr1 = m_pImage1->GetDIBitsAddr();
	BYTE* ptr2 = m_pImage2->GetDIBitsAddr();

	int cnt = 0;
	int temp, sum = 0;

	for (int j = 0; j < BLOCK_SIZE; j++)
		for (int i = 0; i < BLOCK_SIZE; i++)
		{
			if (x1 + i < 0 || x1 + i >= w || y1 + j < 0 || y1 + j >= h ||
				x2 + i < 0 || x2 + i >= w || y2 + j < 0 || y2 + j >= h)
				continue;

			temp = ptr1[(y1 + j) * w + x1 + i] - ptr2[(y2 + j)*w + x2 + i];

			if (temp < 0) temp = -temp;
			sum += temp;
			cnt++;
		}
	return ((double)sum / cnt);
}

void MatFullSearch::GetMotionImage(MatDib& imgMotion)
{
	int w = m_pImage1->GetWidth();
	int h = m_pImage1->GetHeight();

	imgMotion.CreateGrayImage(w, h);

	int cx, cy;
	for(int j = 0; j < m_nBlockRow; j++)
		for (int i = 0; i < m_nBlockCol; i++)
		{
			if(m_pMotion[j][i].x == 0 && m_pMotion[j][i].y == 0)
				continue;

			cx = i * BLOCK_SIZE + BLOCK_SIZE / 2;
			cy = j * BLOCK_SIZE + BLOCK_SIZE / 2;

			MatDrawLine(imgMotion, cx, cy, cx + m_pMotion[j][i].x, cy + m_pMotion[j][i].y, 255);
	}
}