#pragma once
#include "MatDib.h"
#include <vfw.h>
class MatAvi
{
public:
	MatAvi();
	~MatAvi();

public:
	// �ɹ� �Լ�
	BOOL Open(LPCTSTR lpszPathname);
	bool OpenUsbCam();
	void Close();

	// �׸��� �Լ�
	void DrawFrame(HDC hDC, int nFrame);
	void DrawFrame(HDC hDC, int nFrame, int dx, int dy);
	void DrawFrame(HDC hDC, int nFrame, int dx, int dy, int dw, int dh, DWORD dwRop = SRCCOPY);

	// ���� ������ ĸó �Լ�
	BOOL GetFrame(int nFrame, MatDib& dib);

	// AVI ���� ���� ��ȯ �Լ�
	int GetFrameRate() { return m_nFrameRate; }
	int GetTotalFrame() { return m_nTotalFrame; }
	int GetHeight() { return m_nHeight; }
	int GetWidth() { return m_nWidth; }
	BOOL IsValid() { return (m_pAviFile != NULL); }
protected:
	//��� ����
	PAVIFILE m_pAviFile = nullptr;		// AVI ���� �������̽� ������
	PAVISTREAM m_pVideoStream = nullptr;	// ���� ��Ʈ�� ������
	PGETFRAME m_pVideoFrame = nullptr;	// ���� ������ ������
	
	int m_nWidth = 0;				// ���� ������(����) ����ũ��
	int m_nHeight = 0;				// ���� ������(����) ����ũ��

	int m_nTotalFrame = 0;			// ��ü ������ ����
	int m_nFrameRate = 0;			// �ʴ� ������ ����	
};