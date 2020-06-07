#include "stdafx.h"
#include "MatDib.h"
#define DIB_HEADER_MARKER ((WORD) ('M' << 8) | 'B')


MatDib::MatDib()
	:m_nWidth(0), m_nHeight(0), m_nBitCount(0), m_nDibSize(0), m_pDib(NULL)
{
}

MatDib::MatDib(const MatDib& dib)
	: m_nWidth(dib.m_nWidth), m_nHeight(dib.m_nHeight), m_nBitCount(dib.m_nBitCount), m_nDibSize(dib.m_nDibSize), m_pDib(NULL)
{
	if (dib.m_pDib != NULL)
	{
		m_pDib = new BYTE[m_nDibSize];
		memcpy(m_pDib, dib.m_pDib, m_nDibSize);
		m_matImg = dib.m_matImg.clone();
		m_matBack = dib.m_matBack.clone();
	}
}

MatDib::~MatDib()
{
	Release();
}

void MatDib::Release()
{
	if (m_pDib)
	{
		delete[] m_pDib;
		m_pDib = NULL;
	}
	if (m_matBack.data != nullptr)
		m_matBack.release();
	if (m_matImg.data != nullptr)
		m_matImg.release();
	if (m_matDraw.data != nullptr)
		m_matDraw.release();
	m_nHeight = 0;
	m_nWidth = 0;
	m_nDibSize = 0;
	m_nApend = 0;
	m_nBitCount = 0;
}

BOOL MatDib::CreateGrayImage(LONG nWidth, LONG nHeight)
{
	if (m_pDib)
		DestroyBitmap();

	m_matImg = Mat::zeros(Size(nWidth, nHeight), CV_8U);
	
	m_nWidth = nWidth;
	m_nHeight = nHeight;
	m_nBitCount = 8;

	DWORD dwWidthStep = (m_nWidth * m_nBitCount / 8 + 3) & ~3;
	DWORD dwSizeImage = (m_nHeight * dwWidthStep);
	m_nDibSize = sizeof(BITMAPINFOHEADER) + (sizeof(RGBQUAD) * GetPaletteNums()) + dwSizeImage;

	m_pDib = new BYTE[m_nDibSize];
	if (m_pDib == NULL)
		return FALSE;

	// BITMAPINFOHEADER 구조체 설정
	LPBITMAPINFOHEADER lpbmi = (LPBITMAPINFOHEADER)m_pDib;
	lpbmi->biSize = sizeof(BITMAPINFOHEADER);
	lpbmi->biWidth = m_nWidth;
	lpbmi->biHeight = -m_nHeight;
	lpbmi->biPlanes = 1;
	lpbmi->biBitCount = m_nBitCount;
	lpbmi->biCompression = BI_RGB;
	lpbmi->biSizeImage = dwSizeImage;
	lpbmi->biXPelsPerMeter = 0;
	lpbmi->biYPelsPerMeter = 0;
	lpbmi->biClrUsed = 0;
	lpbmi->biClrImportant = 0;

	// 그레이스케일 팔레트 설정
	RGBQUAD* pPal = (RGBQUAD*)((BYTE*)m_pDib + sizeof(BITMAPINFOHEADER));
	for (int i = 0; i < 256; i++)
	{
		pPal->rgbBlue = (BYTE)i;
		pPal->rgbGreen = (BYTE)i;
		pPal->rgbRed = (BYTE)i;
		pPal->rgbReserved = 0;
		pPal++;
	}

	// 픽셀데이터 초기화
	//m_matBack = m_matImg.clone();
	UpdateBack();
	UpdateDib();
	return TRUE;
}

BOOL MatDib::CreateRgbImage(LONG nWidth, LONG nHeight)
{
	if (m_pDib)
		DestroyBitmap();

	m_matImg = Mat::zeros(Size(nWidth, nHeight), CV_8UC3);
	m_nWidth = nWidth;
	m_nHeight = nHeight;
	m_nBitCount = 24;

	DWORD dwWidthStep = (m_nWidth * m_nBitCount / 8 + 3) & ~3;
	DWORD dwSizeImage = (m_nHeight * dwWidthStep);
	m_nDibSize = sizeof(BITMAPINFOHEADER) + dwSizeImage;

	m_pDib = new BYTE[m_nDibSize];
	if (m_pDib == NULL)
		return FALSE;

	// BITMAPINFOHEADER 구조체 설정
	LPBITMAPINFOHEADER lpbmi = (LPBITMAPINFOHEADER)m_pDib;
	lpbmi->biSize = sizeof(BITMAPINFOHEADER);
	lpbmi->biWidth = m_nWidth;
	lpbmi->biHeight = -m_nHeight;
	lpbmi->biPlanes = 1;
	lpbmi->biBitCount = m_nBitCount;
	lpbmi->biCompression = BI_RGB;
	lpbmi->biSizeImage = dwSizeImage;
	lpbmi->biXPelsPerMeter = 0;
	lpbmi->biYPelsPerMeter = 0;
	lpbmi->biClrUsed = 0;
	lpbmi->biClrImportant = 0;

	// 픽셀 데이터 초기화
	//BYTE* pData = GetDIBitsAddr();
	//memset(pData, 0, dwSizeImage);
	//m_matBack = m_matImg.clone();
	UpdateBack();
	UpdateDib();
	return TRUE;
}

void MatDib::DestroyBitmap()
{
	if (m_pDib)
	{
		delete[] m_pDib;
		m_pDib = NULL;
	}

	m_nWidth = 0;
	m_nHeight = 0;
	m_nBitCount = 0;
	m_nDibSize = 0;
}

BOOL MatDib::Load(const char* filename)
{
	return LoadIMG(filename);
}

BOOL MatDib::LoadRaw(const char* filename, int nType, int nWidth, int nHeight, int Offset, int Channel, bool bLittle)
{

	std::ifstream infile(filename, std::ios::binary | std::ios::ate);

	int nCVtype = 0;
	int nSize = 0;
	auto len = infile.tellg();
	std::string chData(len, '\0');
	infile.seekg(0);
	infile.read(&chData[0], len);
	
	switch (nType)
	{
	case 0:
		nCVtype = CV_8UC1;
		nSize = nWidth * nHeight * 1 * Channel;
		break;
	case 1:
		nCVtype = CV_16UC1;
		nSize = nWidth * nHeight * 2 * Channel;
		break;
	case 2:
		nCVtype = CV_32FC1;
		//nCVtype = CV_8UC4;
		nSize = nWidth * nHeight * 4 * Channel;
		break;
	default:
		break;
	}
	
	m_matRaw = Mat::zeros(Size(nWidth, nHeight), nCVtype);
	memcpy(m_matRaw.data, chData.data(), len);
	
	double dMin, dMax;
	cv::minMaxIdx(m_matRaw, &dMin, &dMax);
	m_matRaw.convertTo(m_matImg, CV_8U, 255/(dMax - dMin));
	
	m_matBack = m_matImg.clone();

	BITMAPINFOHEADER bmih;
	bmih.biBitCount = m_matImg.channels() * 8;
	bmih.biWidth = m_matImg.cols;
	bmih.biHeight = -m_matImg.rows;
	bmih.biPlanes = 1;
	bmih.biSize = sizeof(BITMAPINFOHEADER);
	bmih.biCompression = BI_RGB;
	bmih.biClrImportant = 0;
	bmih.biClrUsed = 0;
	bmih.biSizeImage = 0;
	bmih.biXPelsPerMeter = 0;
	bmih.biYPelsPerMeter = 0;

	m_nWidth = bmih.biWidth;
	m_nHeight = -bmih.biHeight;

	m_nBitCount = bmih.biBitCount;


	// 픽셀 데이터 공간 계산
	DWORD dwWidthStep = (DWORD)((m_nWidth * m_nBitCount / 8 + 3) & ~3);
	DWORD dwSizeImage = m_nHeight * dwWidthStep;

	m_nDibSize = sizeof(BITMAPINFOHEADER) + dwSizeImage;
	if (m_nBitCount == 24)
		m_nDibSize = sizeof(BITMAPINFOHEADER) + dwSizeImage;
	else
		m_nDibSize = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * (1LL << m_nBitCount) + dwSizeImage;

	// Packed-DIB 저장할 메모리 공간 동적 할당
// 	if (m_pDib)
// 		DestroyBitmap();

	m_pDib = new BYTE[m_nDibSize];
	if (m_pDib == NULL)
	{
		return FALSE;
	}

	// 파일로부터 PACKED-DIB 크기만큼을 읽기
	memcpy(m_pDib, &bmih, sizeof(bmih));

	// 색상 테이블 작성
	RGBQUAD* pPal = (RGBQUAD*)((BYTE*)m_pDib + sizeof(BITMAPINFOHEADER));
	if (m_nBitCount == 8)
	{
		for (int i = 0; i < 256; i++)
		{
			pPal->rgbBlue = (BYTE)i;
			pPal->rgbGreen = (BYTE)i;
			pPal->rgbRed = (BYTE)i;
			pPal->rgbReserved = 0;
			pPal++;
		}
		for (int nStep = 0; nStep < m_matImg.rows; nStep++)
		{
			memcpy(m_pDib + sizeof(bmih) + sizeof(RGBQUAD) * 256 + dwWidthStep * nStep,
				m_matImg.data + m_matImg.cols * static_cast<int>(m_nBitCount / 8.0)* nStep, dwWidthStep);
		}
	}
	else
	{
		for (int nStep = 0; nStep < m_matImg.rows; nStep++)
		{
			memcpy(m_pDib + sizeof(bmih) + dwWidthStep * nStep,
				m_matImg.data + m_matImg.cols * static_cast<int>(m_nBitCount / 8.0)* nStep, dwWidthStep);
		}
	}
	//delete[] chData;
	return TRUE;
}

BOOL MatDib::Save(const char* filename)
{
	return SaveIMG(filename);
}

BOOL MatDib::LoadIMG(const char* filename)
{
	std::string tmp = filename;
	m_matImg = imread(tmp, ImreadModes::IMREAD_UNCHANGED);
	m_matBack = m_matImg.clone();

	BITMAPINFOHEADER bmih;
	bmih.biBitCount = m_matImg.channels() * 8;
	bmih.biWidth = m_matImg.cols;
	bmih.biHeight = -m_matImg.rows;
	bmih.biPlanes = 1;
	bmih.biSize = sizeof(BITMAPINFOHEADER);
	bmih.biCompression = BI_RGB;
	bmih.biClrImportant = 0;
	bmih.biClrUsed = 0;
	bmih.biSizeImage = 0;
	bmih.biXPelsPerMeter = 0;
	bmih.biYPelsPerMeter = 0;

	m_nWidth = bmih.biWidth;
	m_nHeight = -bmih.biHeight;

	m_nBitCount = bmih.biBitCount;

	
	// 픽셀 데이터 공간 계산
	DWORD dwWidthStep = (DWORD)((m_nWidth * m_nBitCount / 8 + 3) &~3);
	DWORD dwSizeImage = m_nHeight * dwWidthStep;

	m_nDibSize = sizeof(BITMAPINFOHEADER) + dwSizeImage;
	if (m_nBitCount == 24)
		m_nDibSize = sizeof(BITMAPINFOHEADER) + dwSizeImage;
	else
		m_nDibSize = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * (1LL << m_nBitCount) + dwSizeImage;

	// Packed-DIB 저장할 메모리 공간 동적 할당
	if (m_pDib)
		DestroyBitmap();

	m_pDib = new BYTE[m_nDibSize];
	if (m_pDib == NULL)
	{
		return FALSE;
	}

	// 파일로부터 PACKED-DIB 크기만큼을 읽기
	memcpy(m_pDib, &bmih, sizeof(bmih));
	
	// 색상 테이블 작성
	RGBQUAD* pPal = (RGBQUAD*)((BYTE*)m_pDib + sizeof(BITMAPINFOHEADER));
	if (m_nBitCount == 8)
	{
		for (int i = 0; i < 256; i++)
		{
			pPal->rgbBlue = (BYTE)i;
			pPal->rgbGreen = (BYTE)i;
			pPal->rgbRed = (BYTE)i;
			pPal->rgbReserved = 0;
			pPal++;
		}
		for (int nStep = 0; nStep < m_matImg.rows; nStep++)
		{
			memcpy(m_pDib + sizeof(bmih) + sizeof(RGBQUAD) * 256 + dwWidthStep * nStep,
				m_matImg.data + m_matImg.cols * static_cast<int>(m_nBitCount / 8.0) * nStep, dwWidthStep);
		}
	}
	else
	{
		for (int nStep = 0; nStep < m_matImg.rows; nStep++)
		{
			memcpy(m_pDib + sizeof(bmih) + dwWidthStep * nStep, 
				m_matImg.data + m_matImg.cols * static_cast<int>(m_nBitCount/8.0) * nStep, dwWidthStep);
		}
	}
	return TRUE;
}

BOOL MatDib::SetMatImg(Mat matImg)
{
	m_matImg = matImg.clone();
	m_matBack = m_matImg.clone();
	
	BITMAPINFOHEADER bmih;
	bmih.biBitCount = matImg.channels() * 8;
	bmih.biWidth = matImg.cols;
	bmih.biHeight = -matImg.rows;
	bmih.biPlanes = 1;
	bmih.biSize = sizeof(BITMAPINFOHEADER);
	bmih.biCompression = BI_RGB;
	bmih.biClrImportant = 0;
	bmih.biClrUsed = 0;
	bmih.biSizeImage = 0;
	bmih.biXPelsPerMeter = 0;
	bmih.biYPelsPerMeter = 0;
	m_nWidth = bmih.biWidth;
	m_nHeight = -bmih.biHeight;
	m_nBitCount = bmih.biBitCount;
	DWORD dwWidthStep = (DWORD)((m_nWidth * m_nBitCount / 8 + 3) &~3);
	DWORD dwSizeImage = m_nHeight * dwWidthStep;

	if (m_nBitCount == 24)
		m_nDibSize = sizeof(bmih) + dwSizeImage;
	else
		m_nDibSize = sizeof(bmih) + sizeof(RGBQUAD) * (1LL << m_nBitCount) + dwSizeImage;
	//
	//	// Packed-DIB 저장할 메모리 공간 동적 할당
	if (m_pDib)
	{
		delete[] m_pDib;
		m_pDib = NULL;
	}
	m_pDib = new BYTE[m_nDibSize];
	if (m_pDib == NULL)
	{
		return FALSE;
	}

	// 파일로부터 PACKED-DIB 크기만큼을 읽기
	memcpy(m_pDib, &bmih, sizeof(bmih));
	
	RGBQUAD* pPal = (RGBQUAD*)((BYTE*)m_pDib + sizeof(bmih));
	if (m_nBitCount == 8)
	{
		for (int i = 0; i < 256; i++)
		{
			pPal->rgbBlue = (BYTE)i;
			pPal->rgbGreen = (BYTE)i;
			pPal->rgbRed = (BYTE)i;
			pPal->rgbReserved = 0;
			pPal++;
		}
		for (int nStep = 0; nStep < m_matImg.rows; nStep++)
		{
			memcpy(m_pDib + sizeof(bmih) + sizeof(RGBQUAD) * 256 + dwWidthStep * nStep,
				m_matImg.data + m_matImg.cols * static_cast<int>(m_nBitCount / 8.0) * nStep, dwWidthStep);
		}
	}
	else
	{
		for (int nStep = 0; nStep < m_matImg.rows; nStep++)
		{
			memcpy(m_pDib + sizeof(bmih) + dwWidthStep * nStep,
				m_matImg.data + m_matImg.cols * static_cast<int>(m_nBitCount / 8.0) * nStep, dwWidthStep);
		}
	}
	if (m_matImg.data != NULL)
		return TRUE;
	else
		return FALSE;
}


BOOL MatDib::SaveIMG(const char* filename)
{
	if (!IsValid())
		return FALSE;
	std::string tmp = filename;
	imwrite(tmp, m_matImg);

	return TRUE;
}

void MatDib::Draw(HDC hdc, int dx, int dy)
{
	if (m_pDib == NULL)
		return;


	LPBITMAPINFO lpbi = (LPBITMAPINFO)m_pDib;
	LPVOID lpBits = (LPVOID)GetDIBitsAddr();

	::SetDIBitsToDevice(hdc,	// hdc
		dx,						// DestX
		dy,						// DestY
		m_nWidth,				// nSrcWidth
		m_nHeight,				// nSrcHeight
		0,						// SrcX
		0,						// SrcY
		0,						// nStartScan
		m_nHeight,				// nNumScans
		lpBits,					// lpBits
		lpbi,					// lpBitsInfo
		DIB_RGB_COLORS);		// wUsage
}

void MatDib::Draw(HDC hdc, int dx, int dy, int dw, int dh, DWORD dwRop, bool bHalftone)
{
	Draw(hdc, dx, dy, dw, dh, 0, 0, m_nWidth, m_nHeight, dwRop, bHalftone);
}

void MatDib::DrawScale(HDC hdc, int dx, int dy, int dw, int dh, DWORD dwRop)
{
	
	if (m_nWidth >= m_nHeight)
	{
		Draw(hdc, dx, dy, dw, static_cast<int>(dw * m_nHeight / (double)m_nWidth), 0, 0, m_nWidth, m_nHeight, dwRop);
	}
	else
	{
		Draw(hdc, dx, dy, static_cast<int>(dh* m_nWidth / (double)m_nHeight), dh, 0, 0, m_nWidth, m_nHeight, dwRop);
	}
}
void MatDib::Draw(HDC hdc, int dx, int dy, int dw, int dh, int sx, int sy, int sw, int sh, DWORD dwRop, bool bHalftone)
{
	if (m_pDib == NULL)
		return;

	LPBITMAPINFO lpbi = (LPBITMAPINFO)m_pDib;
	LPSTR lpDIBBits = (LPSTR)GetDIBitsAddr();

	if(bHalftone)
		SetStretchBltMode(hdc, HALFTONE);
	else
		SetStretchBltMode(hdc, COLORONCOLOR);
	::StretchDIBits(hdc,	// hdc
		dx,					// XDest
		dy,					// YDest
		dw,					// nDestWidth
		dh,					// nDestHeight
		sx,					// XSrc
		sy,					// YSrc
		sw,					// nSrcWidth
		sh,					// nSrcHeight
		lpDIBBits,			// lpBits
		lpbi,				// lpBitsInfo
		DIB_RGB_COLORS,		// wUsage
		SRCCOPY);			// dwRop
}

BOOL MatDib::CopyToClipboard()
{
	if (!::OpenClipboard(NULL))
		return FALSE;

	// DIB 전체를 전역 메모리 블럭에 복사
	DWORD dwDibSize = GetDibSize();
	HANDLE hDib = ::GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, dwDibSize);
	if (hDib == NULL)
	{
		::CloseClipboard();
		return FALSE;
	}

	LPVOID lpDib = ::GlobalLock((HGLOBAL)hDib);
	memcpy(lpDib, GetBitmapInfoAddr(), dwDibSize);
	::GlobalUnlock(hDib);

	// 클립보드에 데이터 입력
	::EmptyClipboard();
	::SetClipboardData(CF_DIB, hDib);
	::CloseClipboard();

	return TRUE;
}

BOOL MatDib::PasteFromClipboard()
{
	// CF_DIB 타입이 아니면 종료한다.
	if (!::IsClipboardFormatAvailable(CF_DIB))
		return FALSE;

	// 클립보드를 연다.
	if (!::OpenClipboard(NULL))
		return FALSE;

	// 클립보드 내용을 받아온다.
	HANDLE hDib = ::GetClipboardData(CF_DIB);
	if (hDib == NULL)
	{
		::CloseClipboard();
		return FALSE;
	}

	// 메모리 블럭의 크기는 DIB 전체 크기와 동일
	DWORD dwSize = (DWORD)::GlobalSize((HGLOBAL)hDib);
	LPVOID lpDib = ::GlobalLock((HGLOBAL)hDib);

	LPBITMAPINFOHEADER lpbi = (LPBITMAPINFOHEADER)lpDib;
	m_nWidth = lpbi->biWidth;
	m_nHeight = lpbi->biHeight;
	m_nBitCount = lpbi->biBitCount;

	DWORD dwWidthStep = (DWORD)((m_nWidth * m_nBitCount / 8 + 3) & ~3);
	DWORD dwSizeImage = m_nHeight * dwWidthStep;
	if (m_nBitCount == 24)
	{
		m_nDibSize = sizeof(BITMAPINFOHEADER) + dwSizeImage;
		m_matImg = Mat::zeros(Size(m_nWidth, m_nHeight), CV_8UC3);
	}
	else if (m_nBitCount == 32)
	{
		m_nDibSize = sizeof(BITMAPINFOHEADER) + dwSizeImage;
		m_matImg = Mat::zeros(Size(m_nWidth, m_nHeight), CV_8UC4);
	}
	else
	{
		m_nDibSize = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * (1LL << m_nBitCount) + dwSizeImage;
		m_matImg = Mat::zeros(Size(m_nWidth, m_nHeight), CV_8U);
	}

	// 현재 설정된 MatDib 객체가 있다면 삭제한다.
	if (m_pDib)
		DestroyBitmap();

	m_pDib = new BYTE[m_nDibSize];
	if (m_pDib == NULL)
	{
		::GlobalUnlock(hDib);
		::CloseClipboard();
		return FALSE;
	}

	memcpy(m_pDib, lpDib, m_nDibSize);
	memcpy(m_matImg.data, m_pDib + m_nDibSize - dwSizeImage, dwSizeImage);
	cv::flip(m_matImg, m_matImg, 0);
	m_matBack = m_matImg.clone();
	::GlobalUnlock(hDib);
	::CloseClipboard();

	return TRUE;
}

MatDib& MatDib::operator=(const MatDib& dib)
{
	if (this == &dib)
		return *this;

	if (m_pDib)
		delete[] m_pDib;

	m_nWidth = dib.m_nWidth;
	m_nHeight = dib.m_nHeight;
	m_nBitCount = dib.m_nBitCount;
	m_nDibSize = dib.m_nDibSize;
	m_pDib = NULL;

	if (dib.m_pDib != NULL)
	{
		m_pDib = new BYTE[m_nDibSize];
		memcpy(m_pDib, dib.m_pDib, m_nDibSize);
	}
	m_matImg = dib.m_matImg.clone();
	m_matBack = dib.m_matBack.clone();
	m_matRaw = dib.m_matRaw.clone();
	return *this;
}

BYTE* MatDib::GetDIBitsAddr() const
{
	if (m_pDib == NULL)
		return NULL;

	LPBITMAPINFOHEADER lpbmi = (LPBITMAPINFOHEADER)m_pDib;
	return ((BYTE*)m_pDib + lpbmi->biSize + (sizeof(RGBQUAD) * GetPaletteNums()));
	//return m_matImg.data;
}

BYTE* MatDib::GetMatAddr() const 
{
	return m_matImg.data;
}

int MatDib::GetPaletteNums()const
{
	switch (m_nBitCount)
	{
	case 1:		return 2;
	case 4:		return 16;
	case 8:		return 256;
	default:	return 0;
	}
}

void MatDib::DrawLine(int l, int t, int r, int b, Scalar color, int thick)
{
	//if(m_matDraw.empty())
	m_matDraw = Mat::zeros(m_matImg.size(), m_matImg.type());
	cv::line(m_matDraw, Point(l, t), Point(r, b), color, thick);
	cv::add(m_matDraw, m_matBack, m_matImg);
	UpdateDib();
}

void MatDib::DrawRect(int l, int t, int r, int b, Scalar color, int thick)
{
	m_matDraw = Mat::zeros(m_matImg.size(), m_matImg.type());

	int nStartX = l, nStartY = t;
	if (l > r)	nStartX = r;
	if (t > b)	nStartY = b;

	cv::rectangle(m_matDraw, Rect(nStartX, nStartY, abs(r - l), abs(b - t)), color, thick);
	cv::add(m_matDraw, m_matBack, m_matImg);
	UpdateDib();
}

void MatDib::DrawCircle(int cx, int cy, int r, Scalar color, int thick)
{
	m_matDraw = Mat::zeros(m_matImg.size(), m_matImg.type());
	cv::circle(m_matDraw, Point(cx, cy), r, color, thick);
	
	cv::add(m_matDraw, m_matBack, m_matImg);
	UpdateDib();
}

void MatDib::DrawSelRect(int l, int t, int r, int b, Scalar color, int thick)
{
	m_matDraw = Mat::zeros(m_matImg.size(), m_matImg.type());
	const int shift = 6;
	int i;
	int nStartX = l, nEndX = r;
	int nStartY = t, nEndY = b;
	bool bCols, bRows;

	bCols = (l > r);
	bRows = (t > b);
	if (bCols)
	{
		nStartX = r;
		nEndX = l;
	}
	if (bRows)
	{
		nStartY = b;
		nEndY = t;
	}

	for (i = nStartX; i < nEndX; ++i)
	{
		if (i%shift == 3 || i%shift == 4 || i%shift == 5)
			continue;
		cv::line(m_matDraw, Point(i, t), Point(i, t), color, thick);
		cv::line(m_matDraw, Point(i, b), Point(i, b), color, thick);
	}
	
	for (i = nStartY; i < nEndY; ++i)
	{
		if (i%shift == 3 || i % shift == 4 || i % shift == 5)
			continue;
		cv::line(m_matDraw, Point(l, i), Point(l, i), color, thick);
		cv::line(m_matDraw, Point(r, i), Point(r, i), color, thick);
	}

	cv::bitwise_xor(m_matDraw, m_matBack, m_matImg);
	UpdateDib();
}

double MatDib::MeasureLine(int l, int t, int r, int b, cv::Scalar color, int thick)
{
	if (m_nBitCount == 8)
	{
		color = cv::Scalar(255, 255, 255);
	}
	double dTextX, dTextY;
	int nDis = 0, nEdge = 5;
	char chText[32] = { 0x00, };
	
	m_matDraw = Mat::zeros(m_matImg.size(), m_matImg.type());
	if (l == r)
	{
		nDis = b - t;
		dTextX = l;
		dTextY = t + (nDis / 2.0);
		sprintf_s(chText, "%d", abs(nDis));
		cv::line(m_matDraw, cv::Point(((l - nEdge) < 0) ? 0 : (l - nEdge), t), cv::Point(((l + nEdge) > m_matDraw.cols) ? m_matDraw.cols : (l + nEdge), t), color, thick);
		cv::line(m_matDraw, cv::Point(((l - nEdge) < 0) ? 0 : (l - nEdge), b), cv::Point(((l + nEdge) > m_matDraw.cols) ? m_matDraw.cols : (l + nEdge), b), color, thick);
	}
	else if(t == b)
	{
		nDis = r - l;
		dTextX = l + nDis / 2.0;
		dTextY = t;
		sprintf_s(chText, "%d", abs(nDis));
		cv::line(m_matDraw, cv::Point(l, ((t - nEdge) < 0) ? 0 : (t - nEdge)), cv::Point(l, ((t + nEdge) > m_matDraw.rows) ? m_matDraw.rows : (t + nEdge)), color, thick);
		cv::line(m_matDraw, cv::Point(r, ((t - nEdge) < 0) ? 0 : (t - nEdge)), cv::Point(r, ((t + nEdge) > m_matDraw.rows) ? m_matDraw.rows : (t + nEdge)), color, thick);
	}
	cv::line(m_matDraw, Point(l, t), Point(r, b), color, thick);
	cv::putText(m_matDraw, chText, Point((int)dTextX, (int)dTextY), 1, 1.0, color, thick);
	if (m_nBitCount == 8)
		cv::bitwise_xor(m_matDraw, m_matBack, m_matImg);
	else
		cv::add(m_matDraw, m_matBack, m_matImg);
	UpdateDib();
	return nDis;
}

double MatDib::MeasureDiagonal(int l, int t, int r, int b, cv::Scalar color, int thick)
{
	if (m_nBitCount == 8)
	{
		color = cv::Scalar(255, 255, 255);
	}
	double dTextX, dTextY, dAngle;
	int nDis = 0, nEdge = 5;
	char chText[32] = { 0x00, };

	m_matDraw = Mat::zeros(m_matImg.size(), m_matImg.type());

	dAngle = atan((b - t) / (double)(r - l));
	nDis = static_cast<int>(sqrt(pow(abs(r - l),2) + pow(abs(t - b),2)));
	dTextX = l + (r - l) / 2.0;
	dTextY = t + (b - t) / 2.0;
	sprintf_s(chText, "%d", abs(nDis));
	
	line(m_matDraw, cv::Point(static_cast<int>(nEdge*cos(dAngle + CV_PI / 2.0) + l), static_cast<int>(nEdge*sin(dAngle + CV_PI / 2.0) + t)),
		cv::Point(static_cast<int>(nEdge*cos(dAngle - CV_PI / 2.0) + l), static_cast<int>(nEdge*sin(dAngle - CV_PI / 2.0) + t)), color, thick);
	line(m_matDraw, cv::Point(static_cast<int>(nEdge*cos(dAngle + CV_PI / 2.0) + r), static_cast<int>(nEdge*sin(dAngle + CV_PI / 2.0) + b)),
		cv::Point(static_cast<int>(nEdge*cos(dAngle - CV_PI / 2.0) + r), static_cast<int>(nEdge*sin(dAngle - CV_PI / 2.0) + b)), color, thick);
	line(m_matDraw, cv::Point(l, t), cv::Point(r, b), color, thick);

	cv::putText(m_matDraw, chText, Point((int)dTextX, (int)dTextY), 1, 1.0, color, thick);
	if(m_nBitCount == 8)
		cv::bitwise_xor(m_matDraw, m_matBack, m_matImg);
	else
		cv::add(m_matDraw, m_matBack, m_matImg);
	UpdateDib();
	return nDis;
}

void MatDib::DrawClearScreen()
{
	m_matDraw = Mat::zeros(m_matImg.size(), m_matImg.type());
	add(m_matDraw, m_matBack, m_matImg);
	UpdateDib();
}

void MatDib::UpdateBack()
{
	m_matBack = m_matImg.clone();
}


void MatDib::UpdateBack(BYTE* data)
{
	m_matImg.data = data;
	rotate(m_matImg, m_matImg, ROTATE_180);
	m_matBack = m_matImg.clone();
}

BOOL MatDib::UpdateDib()
{
	BITMAPINFOHEADER bmih;
	bmih.biBitCount = m_matImg.channels() * 8;
	bmih.biWidth = m_matImg.cols;
	bmih.biHeight = -m_matImg.rows;
	bmih.biPlanes = 1;
	bmih.biSize = sizeof(BITMAPINFOHEADER);
	bmih.biCompression = BI_RGB;
	bmih.biClrImportant = 0;
	bmih.biClrUsed = 0;
	bmih.biSizeImage = 0;
	bmih.biXPelsPerMeter = 0;
	bmih.biYPelsPerMeter = 0;
	m_nWidth = bmih.biWidth;
	m_nHeight = -bmih.biHeight;
	m_nBitCount = bmih.biBitCount;
	DWORD dwWidthStep = (DWORD)((m_nWidth * m_nBitCount / 8 + 3) &~3);
	DWORD dwSizeImage = m_nHeight * dwWidthStep;

	if (m_nBitCount == 24)
		m_nDibSize = sizeof(bmih) + dwSizeImage;
	else
		m_nDibSize = sizeof(bmih) + sizeof(RGBQUAD) * (1LL << m_nBitCount) + dwSizeImage;
	//
	//	// Packed-DIB 저장할 메모리 공간 동적 할당
	if (m_pDib)
	{
		delete[] m_pDib;
		m_pDib = NULL;
	}
	m_pDib = new BYTE[m_nDibSize];
	if (m_pDib == NULL)
	{
		return FALSE;
	}
	memcpy(m_pDib, &bmih, sizeof(bmih));

	RGBQUAD* pPal = (RGBQUAD*)((BYTE*)m_pDib + sizeof(bmih));
	if (m_nBitCount == 8)
	{
		for (int i = 0; i < 256; i++)
		{
			pPal->rgbBlue = (BYTE)i;
			pPal->rgbGreen = (BYTE)i;
			pPal->rgbRed = (BYTE)i;
			pPal->rgbReserved = 0;
			pPal++;
		}
		for (int nStep = 0; nStep < m_matImg.rows; nStep++)
		{
			memcpy(m_pDib + sizeof(bmih) + sizeof(RGBQUAD) * 256 + dwWidthStep * nStep,
				m_matImg.data + m_matImg.cols * static_cast<int>(m_nBitCount / 8.0) * nStep, dwWidthStep);
		}
	}
	else
	{
		for (int nStep = 0; nStep < m_matImg.rows; nStep++)
		{
			memcpy(m_pDib + sizeof(bmih) + dwWidthStep * nStep,
				m_matImg.data + m_matImg.cols * static_cast<int>(m_nBitCount / 8.0) * nStep, dwWidthStep);
		}
	}
	return TRUE;
}
/**	
@brief	Get Mat Pixel Value
@return	Pixel Data
@param	X Postion, Y Position, Return Type
@remark	
-		0 - Gray
-		1 - Red
-		2 - Green
-		3 - Blue
@author	선경규(Kyeong Kyu - Seon)
@date	2018/10/12  16:53
*/
BYTE MatDib::GetMatPixel(int x, int y, int type)
{
	BYTE pixel = 0;
	if (m_matBack.empty())
	{
		return pixel;
	}

	if (m_matBack.channels() == 1)
	{
		type = 0;
	}
	switch (type)
	{
	case 0:
		pixel = m_matBack.at<BYTE>(y, x);
		break;
	case 1:
		if(m_matBack.channels() == 3)
			pixel = m_matBack.at<Vec3b>(y, x)[2];
		break;
	case 2:
		if (m_matBack.channels() == 3)
			pixel = m_matBack.at<Vec3b>(y, x)[1];
		break;
	case 3:
		if (m_matBack.channels() == 3)
			pixel = m_matBack.at<Vec3b>(y, x)[0];
		break;
	default:
		pixel = 0;
		break;
	}
	return pixel;
}