#pragma once
#include "MatDib.h"
#include <vfw.h>
class MatAvi
{
public:
	MatAvi();
	~MatAvi();

public:
	// 맴버 함수
	BOOL Open(LPCTSTR lpszPathname);
	bool OpenUsbCam();
	void Close();

	// 그리기 함수
	void DrawFrame(HDC hDC, int nFrame);
	void DrawFrame(HDC hDC, int nFrame, int dx, int dy);
	void DrawFrame(HDC hDC, int nFrame, int dx, int dy, int dw, int dh, DWORD dwRop = SRCCOPY);

	// 현재 프레임 캡처 함수
	BOOL GetFrame(int nFrame, MatDib& dib);

	// AVI 파일 정보 반환 함수
	int GetFrameRate() { return m_nFrameRate; }
	int GetTotalFrame() { return m_nTotalFrame; }
	int GetHeight() { return m_nHeight; }
	int GetWidth() { return m_nWidth; }
	BOOL IsValid() { return (m_pAviFile != NULL); }
protected:
	//멤버 변수
	PAVIFILE m_pAviFile = nullptr;		// AVI 파일 인터페이스 포인터
	PAVISTREAM m_pVideoStream = nullptr;	// 비디오 스트림 포인터
	PGETFRAME m_pVideoFrame = nullptr;	// 비디오 프레임 포인터
	
	int m_nWidth = 0;				// 비디오 프레임(영상) 가로크기
	int m_nHeight = 0;				// 비디오 프레임(영상) 세로크기

	int m_nTotalFrame = 0;			// 전체 프레임 개수
	int m_nFrameRate = 0;			// 초당 프레임 비율	
};