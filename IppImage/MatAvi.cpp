#include "stdafx.h"
#include "MatAvi.h"
#include "MatDibExt.h"
#include "RGBBYTE.h"


MatAvi::MatAvi()
{
	// AVIFile ���̺귯���� �߰��Ѵ�.
	AVIFileInit();

	// �ɹ� �ʱ�ȭ
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

	// AVIFile ���̺귯�� ����� �����Ѵ�.
	AVIFileExit();
}

BOOL MatAvi::Open(LPCTSTR lpszPathName)
{
	int i;
	HRESULT hr;

	AVIFILEINFO aviFileInfo;
	AVISTREAMINFO aviStreamInfo;
	PAVISTREAM pAviTmpStream = nullptr;

	// Ȥ�� ������ ������ open �� ���� ������ �ݴ´�.
	if (m_pAviFile)
	{
		Close();
		m_pAviFile = nullptr;
	}
	
	// AVI ������ ����, ���� �������̽��� ���� �����͸� M-pAviFile�� �����Ѵ�.
	hr = AVIFileOpen(&m_pAviFile, lpszPathName, OF_READ, NULL);
	if (hr)
		return FALSE;

	// AVI ���Ͽ� ���� ������ ��´�.
	hr = AVIFileInfo(m_pAviFile, &aviFileInfo, sizeof(AVIFILEINFO));
	if (hr)
	{
		Close();
		return FALSE;
	}

	for (i = 0; i < (LONG)aviFileInfo.dwStreams; i++)
	{
		// AVI ���ϰ� ���õ� ��Ʈ�� �������̽��� �ּҸ� ��´�.
		// fccType�� 0L���� �����Ǹ� ��� ��Ʈ���� �˻��Ѵ�.
		hr = AVIFileGetStream(m_pAviFile, &pAviTmpStream, 0L, i);

		if (hr)
			continue;

		// ������ ���� ��Ʈ�� �������̽��� ���� ������ ��´�.
		hr = AVIStreamInfo(pAviTmpStream, &aviStreamInfo, sizeof(AVISTREAMINFO));

		if(hr)
			continue;

		if (aviStreamInfo.fccType == streamtypeVIDEO)
		{
			m_pVideoStream = pAviTmpStream;
			
			// AVI ��Ʈ���� ��� ���� ������ ������Ų��.
			AVIStreamAddRef(m_pVideoStream);

			// ��ü ���� �� (VIDEO�� ��� ��ü ������ ��)
			m_nTotalFrame = aviStreamInfo.dwLength;

			// �ʴ� ���� �� (VIDEO�� ��� �ʴ� ������ ��)
			m_nFrameRate = aviStreamInfo.dwRate / aviStreamInfo.dwScale;

			// ���� ȭ���� ����, ���� ũ��

			RECT rc = aviStreamInfo.rcFrame;
			m_nWidth = rc.right - rc.left;
			m_nHeight = rc.bottom - rc.top;

			// ���� ��Ʈ�����κ��� ������� ���� ���� �������� �غ��Ѵ�.
			m_pVideoFrame = AVIStreamGetFrameOpen(m_pVideoStream, NULL);

			if (!m_pVideoFrame)
			{
				Close();
				return FALSE;
			}
		}
		else
		{
			// Video ��Ʈ���� �ƴ� ��� ������� �ʰ� �״�� ��ȯ
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
