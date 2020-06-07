#pragma once
#include "MatDib.h"

typedef struct _MotionVector
{
	int x;
	int y;
}MotionVector;

class MatFullSearch
{
public:
	MatFullSearch();
	~MatFullSearch();

public:
	MatDib* m_pImage1 = nullptr;
	MatDib* m_pImage2 = nullptr;

	MotionVector** m_pMotion = nullptr;

	int m_nBlockRow;
	int m_nBlockCol;

public:
	void SetImages(MatDib* pImage1, MatDib* pImage2);
	void FullSearch();
	MotionVector** GetMotionVector();
	void GetMotionImage(MatDib& dibMotion);

protected:
	void InitMotion();
	void DeleteMotion();

	MotionVector BlockMotion(int bx, int by);
	double GetMad(int x1, int y1, int x2, int y2);
};

