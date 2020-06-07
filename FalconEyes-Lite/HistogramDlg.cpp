// HistogramDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "FalconEyes.h"
#include "HistogramDlg.h"
#include "afxdialogex.h"


// CHistogramDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CHistogramDlg, CDialogEx)

CHistogramDlg::CHistogramDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_HISTOGRAM, pParent)
{
	memset(m_Histogram, 0, sizeof(256));
}

CHistogramDlg::~CHistogramDlg()
{
}

void CHistogramDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CHistogramDlg, CDialogEx)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CHistogramDlg 메시지 처리기입니다.
void CHistogramDlg::SetImage(MatDib* pDib)
{
	unsigned nShift, nStep;
	if (pDib != NULL)
	{
		float histo[256] = { 0.f, };
		//if (pDib->GetBitCount() == 8)
		//{
		DibHistogram(*pDib, histo);
		//}
		float max_histo = histo[0];

		for (nShift = 0; nShift < 256; nShift++)
		{
			if (histo[nShift] > max_histo)
			{
				max_histo = histo[nShift];
			}
		}

		for (nStep = 0; nStep < 256; nStep++)
		{
			m_Histogram[nStep] = static_cast<int>(histo[nStep] * 100.0 / max_histo);
		}
	}
	else
	{
		memset(m_Histogram, 0, sizeof(256));
	}
}

void CHistogramDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: Add your message handler code here
					   // Do not call CDialogEx::OnPaint() for painting messages
	CGdiObject* pOldPen = dc.SelectStockObject(DC_PEN);

	const int NGAP = 5;
	CRect rect;
	GetDlgItem(IDC_PIC)->GetWindowRect(rect);
	ScreenToClient(rect);


	rect.left += NGAP;
	rect.right -= NGAP;
	rect.top += NGAP;
	rect.bottom -= NGAP;


	// 히스토그램 박스
	dc.SetDCPenColor(RGB(128, 128, 128));
	dc.MoveTo(rect.left, rect.top);
	dc.LineTo(rect.right, rect.top);
	dc.LineTo(rect.right, rect.bottom);
	dc.LineTo(rect.left, rect.bottom);
	dc.LineTo(rect.left, rect.top);

	int nStepX = 0;
	double dWidthScale, dHeightScale;
	CRect tmp;
	dc.SetDCPenColor(RGB(0, 0, 0));
	tmp = rect;
	dWidthScale = 256. / static_cast<double>(tmp.Width());
	dHeightScale = static_cast<double>(tmp.Height()) / 100.;

	dc.TextOutW(static_cast<int>((tmp.left + tmp.right) / 2.), tmp.bottom + 25, TEXT("GRAY"));
	for (nStepX = 0; nStepX < tmp.Width(); nStepX++)
	{
		dc.MoveTo(tmp.left + nStepX, tmp.bottom);
		dc.LineTo(tmp.left + nStepX, tmp.bottom - static_cast<int>(m_Histogram[static_cast<int>(nStepX*dWidthScale)] * dHeightScale));
	}
	for (int nStepX = 0; nStepX < tmp.Width(); nStepX++)
	{
		dc.SetDCPenColor(RGB(static_cast<int>(nStepX*dWidthScale), static_cast<int>(nStepX*dWidthScale), static_cast<int>(nStepX*dWidthScale)));
		dc.MoveTo(tmp.left + nStepX, tmp.bottom + 7);
		dc.LineTo(tmp.left + nStepX, tmp.bottom + 22);
	}


	dc.SelectObject(pOldPen);
}
