#pragma once
typedef enum EN_THREAD_STATUS
{
	THREAD_STAT_NONE = 0,
	THREAD_STAT_ACTIVE,
	THREAD_STAT_RUNNING,
	THREAD_STAT_COMPLETE
}THREAD_STATUS;

typedef struct ST_THREAD_PARAM
{
	void*	cObjectPointer1;
	void*	cObjectPointer2;
	int		nScanInterval;
	int		nThreadIndex;
	bool	bThreadFlag;
	THREAD_STATUS enState;
	//SOCKET  ClientSocket;

	ST_THREAD_PARAM()
	{
		cObjectPointer1 = nullptr;
		cObjectPointer2 = nullptr;
		nScanInterval = 300;
		bThreadFlag = false;
		enState = THREAD_STAT_NONE;
	}
}THREAD_PARAM;

typedef struct stSharedMemoryHeader
{
	unsigned int nMemsize;
	short nWidth;
	short nHeight;
	short nColor;
	short nCount;
	char* pData;
	stSharedMemoryHeader()
	{
		memset(this, 0x00, sizeof(stSharedMemoryHeader));
	}
}SharedMemHeader;

typedef struct ST_CIRCLE_RANSAC_RESULT
{
	Point pntCenter;
	UINT nRadius;
	ST_CIRCLE_RANSAC_RESULT()
	{
		memset(this, 0x00, sizeof(ST_CIRCLE_RANSAC_RESULT));
	}
	ST_CIRCLE_RANSAC_RESULT(Point pnt, UINT radius)
	{
		pntCenter = pnt;
		nRadius = radius;
	}
}CIRCLE_RANSAC_RESULT;

typedef struct ST_LINE_PARAM
{
	double a;	// 1차 방정식 기울기
	double b;	// 1차 방정식 y절편
	double x;	// 기울기가 무한대 일 때 x절편
	ST_LINE_PARAM()
	{
		memset(this, 0x00, sizeof(ST_LINE_PARAM));
	}
}LINE_PARAM;

typedef struct ST_ELLIPSE_PARAM
{
	double cx;
	double cy;
	double angle;
	double w;
	double h;
	ST_ELLIPSE_PARAM()
	{
		memset(this, 0x00, sizeof(ST_ELLIPSE_PARAM));
	}
}ELLIPSE_PARAM;

typedef struct ST_POINT
{
	size_t x;
	size_t y;
	ST_POINT()
	{
		x = 0;
		y = 0;
	}
	ST_POINT(size_t nx, size_t ny)
	{
		x = nx;
		y = ny;
	}
}DEFPOINT;

/**
@struct	삼성 프로토콜?
@brief	
@remark	
 - data 동적할당 할 것
@author	선경규(Kyeong Kyu - Seon)
@date	2019/5/10  9:43
*/
// 
// struct _typeCommSever
// {
// 	char STX = 0x05;
// 	char Rsvd0[2] = { 0x00, };
// 	char Rsvd1[2] = { 0x00, };
// 	int Length;
// 	_typeCommServerData data;
// 	char ETX = 0x0A;
// 	char chNull = 0x00;
// };
// 
// struct _typeCommServerData
// {
// 	char* Direction;
// 	char* SendRecvInfo;
// 	void* message;
// };