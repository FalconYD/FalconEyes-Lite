#include "stdafx.h"
#include "MatAvi.h"
#include "MatDibExt.h"
#include "RGBBYTE.h"


MatAvi::MatAvi()
{
	// AVIFile 라이브러리를 추가한다.
	AVIFileInit();

	// 맴버 초기화
// 	m_pAviFile = nullptr;
// 	m_pVideoStream = nullptr;
// 	m_pVideoFrame = nullptr;
// 
// 	m_nWidth = 0;
// 	m_nHeight = 0;
// 	m_nTotalFrame = 0;
// 	m_nFrameRate = 0;
}


MatAvi::~MatAvi()
{
	Close();

	// AVIFile 라이브러리 사용을 종료한다.
	AVIFileExit();
}

BOOL MatAvi::Open(LPCTSTR lpszPathName)
{
	int i;
	HRESULT hr;

	AVIFILEINFO aviFileInfo;
	AVISTREAMINFO aviStreamInfo;
	PAVISTREAM pAviTmpStream = nullptr;

	// 혹시 기존에 파일을 open 한 적이 있으면 닫는다.
	if (m_pAviFile)
	{
		Close();
		m_pAviFile = nullptr;
	}
	
	// AVI 파일을 열고, 파일 인터페이스에 대한 포인터를 M-pAviFile에 저장한다.
	hr = AVIFileOpen(&m_pAviFile, lpszPathName, OF_READ, NULL);
	if (hr)
		return FALSE;

	// AVI 파일에 대한 정보를 얻는다.
	hr = AVIFileInfo(m_pAviFile, &aviFileInfo, sizeof(AVIFILEINFO));
	if (hr)
	{
		Close();
		return FALSE;
	}

	for (i = 0; i < (LONG)aviFileInfo.dwStreams; i++)
	{
		// AVI 파일과 관련된 스트림 인터페이스의 주소를 얻는다.
		// fccType이 0L으로 지정되면 모든 스트림을 검색한다.
		hr = AVIFileGetStream(m_pAviFile, &pAviTmpStream, 0L, i);

		if (hr)
			continue;

		// 위에서 받은 스트림 인터페이스에 대한 정보를 얻는다.
		hr = AVIStreamInfo(pAviTmpStream, &aviStreamInfo, sizeof(AVISTREAMINFO));

		if(hr)
			continue;

		if (aviStreamInfo.fccType == streamtypeVIDEO)
		{
			m_pVideoStream = pAviTmpStream;
			
			// AVI 스트림의 사용 참조 개수를 증가시킨다.
			AVIStreamAddRef(m_pVideoStream);

			// 전체 샘플 수 (VIDEO의 경우 전체 프레임 수)
			m_nTotalFrame = aviStreamInfo.dwLength;

			// 초당 샘플 수 (VIDEO의 경우 초당 프레임 수)
			m_nFrameRate = aviStreamInfo.dwRate / aviStreamInfo.dwScale;

			// 비디오 화면의 가로, 세로 크기

			RECT rc = aviStreamInfo.rcFrame;
			m_nWidth = rc.right - rc.left;
			m_nHeight = rc.bottom - rc.top;

			// 비디오 스트림으로부터 압축되지 않은 비디오 프레임을 준비한다.
			m_pVideoFrame = AVIStreamGetFrameOpen(m_pVideoStream, NULL);

			if (!m_pVideoFrame)
			{
				Close();
				return FALSE;
			}
		}
		else
		{
			// Video 스트림이 아닌 경우 사용하지 않고 그대로 반환
			AVIStreamRelease(pAviTmpStream);
		}
	}

	return TRUE;
}

void MatAvi::Close()
{
	if (m_pVideoFrame != nullptr)
	{
		AVIStreamGetFrameClose(m_pVideoFrame);
		m_pVideoFrame = nullptr;
	}

	if ( m_pVideoStream != nullptr)
	{
		AVIStreamRelease(m_pVideoStream);
		m_pVideoStream = nullptr;
	}

	if (m_pAviFile != nullptr)
	{
		AVIFileRelease(m_pAviFile);
		m_pAviFile = nullptr;
	}
}

void MatAvi::DrawFrame(HDC hDC, int nFrame)
{
	BYTE* pDib = (BYTE*)AVIStreamGetFrame(m_pVideoFrame, nFrame);
	if (!pDib)
		return;

	LPBITMAPINFO lpbi;
	LPSTR lpDIBBits;

	lpbi = (LPBITMAPINFO)pDib;

	if (lpbi->bmiHeader.biBitCount == 8)
		lpDIBBits = (LPSTR)pDib + sizeof(BITMAPINFOHEADER) + 1024;
	else
		lpDIBBits = (LPSTR)pDib + sizeof(BITMAPINFOHEADER);

	::SetDIBitsToDevice(hDC,	// hDC
		0,						// DestX
		0,						// DestY
		m_nWidth,				// nSrcWidth
		m_nHeight,				// nSrcHeight
		0,						// SrcX
		0,						// SrcY
		0,						// nStartScan
		(WORD)m_nHeight,		// nNumScans
		lpDIBBits,				// lpBits
		lpbi,					// lpBitsInfo
		DIB_RGB_COLORS);		// wUsage
}

void MatAvi::DrawFrame(HDC hDC, int nFrame, int dx, int dy)
{
	DrawFrame(hDC, nFrame, dx, dy, GetWidth(), GetHeight());
}

void MatAvi::DrawFrame(HDC hDC, int nFrame, int dx, int dy, int dw, int dh, DWORD dwRop)
{
	BYTE* pDib = (BYTE*)AVIStreamGetFrame(m_pVideoFrame, nFrame);
	if (!pDib)
		return;

	LPBITMAPINFO lpbi;
	LPSTR lpDIBBits;

	lpbi = (LPBITMAPINFO)pDib;

	if (lpbi->bmiHeader.biBitCount == 8)
		lpDIBBits = (LPSTR)pDib + sizeof(BITMAPINFOHEADER) + 1024;
	else
		lpDIBBits = (LPSTR)pDib + sizeof(BITMAPINFOHEADER);

	::SetStretchBltMode(hDC, COLORONCOLOR);
	::StretchDIBits(hDC,	// hDC
		dx,					// XDest
		dy,					// YDest
		dw,					// nDestWidth
		dh,					// nDestHeight
		0,					// XSrc
		0,					// YSrc
		m_nWidth,			// nSrcWidth
		m_nHeight,			// nSrcHeight
		lpDIBBits,			// lpBits
		lpbi,				// lpBitsInfo
		DIB_RGB_COLORS,		// wUsage
		SRCCOPY);			// dwROP
}

BOOL MatAvi::GetFrame(int nFrame, MatDib& dib)
{
	LPVOID lpDib;
	LPSTR lpDIBBits;
	LPBITMAPINFO lpbi;

	lpDib = AVIStreamGetFrame(m_pVideoFrame, nFrame);
	lpbi = (LPBITMAPINFO)lpDib;

	if (lpbi->bmiHeader.biBitCount == 8)
	{
		lpDIBBits = (LPSTR)lpDib + sizeof(BITMAPINFOHEADER) + 1024;
		Mat tmp(Size(lpbi->bmiHeader.biWidth, lpbi->bmiHeader.biHeight), CV_8UC3);
		memcpy(tmp.data, lpDIBBits, lpbi->bmiHeader.biSizeImage);
		flip(tmp, tmp, 0);
		dib.SetMatImg(tmp);
	}
	else if (lpbi->bmiHeader.biBitCount == 24)
	{
		lpDIBBits = (LPSTR)lpDib + sizeof(BITMAPINFOHEADER);
		Mat tmp(Size(lpbi->bmiHeader.biWidth, lpbi->bmiHeader.biHeight), CV_8UC3);
		memcpy(tmp.data, lpDIBBits, lpbi->bmiHeader.biSizeImage);
		flip(tmp, tmp, 0);
		dib.SetMatImg(tmp);
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}
