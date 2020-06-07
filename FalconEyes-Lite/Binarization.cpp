// Binarization.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "FalconEyes.h"
#include "Binarization.h"
#include "afxdialogex.h"


// CBinarization 대화 상자입니다.

IMPLEMENT_DYNAMIC(CBinarization, CDialogEx)

CBinarization::CBinarization(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_BINARIZTION, pParent)
	, m_nThreshold(0)
{

}

CBinarization::~CBinarization()
{
}

void CBinarization::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_THRESHOLD_SLIDER, m_sliderThreshold);
	DDX_Text(pDX, IDC_EDIT_THRESHOLD, m_nThreshold);
	DDV_MinMaxInt(pDX, m_nThreshold, 0, 255);
}


BEGIN_MESSAGE_MAP(CBinarization, CDialogEx)
	ON_WM_PAINT()
	ON_WM_HSCROLL()
	ON_EN_CHANGE(IDC_EDIT_THRESHOLD, &CBinarization::OnEnChangeEditThreshold)
END_MESSAGE_MAP()


// CBinarization 메시지 처리기입니다.
void CBinarization::SetImage(MatDib& dib)
{
	m_dibSrc = dib;
	m_nThreshold = MatBinarizationIterative(dib);
}

void CBinarization::MakePreviewImage()
{
	//m_dibRes = m_dibSrc;
	MatBinarization(m_dibSrc, m_dibRes, m_nThreshold);	
}



BOOL CBinarization::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_sliderThreshold.SetRange(0, 255);
	m_sliderThreshold.SetTicFreq(32);
	m_sliderThreshold.SetPageSize(32);
	m_sliderThreshold.SetPos(m_nThreshold);

	CRect rect;
	GetDlgItem(IDC_IMAGE_PREVIEW)->GetClientRect(rect);
	DibResizeNearest(m_dibSrc, m_dibSrc, rect.Width(), rect.Height());
	MakePreviewImage();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CBinarization::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.
	CPaintDC dcPreview(GetDlgItem(IDC_IMAGE_PREVIEW));
	m_dibRes.Draw(dcPreview.m_hDC, 0, 0);
}




void CBinarization::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (m_sliderThreshold.GetSafeHwnd() == pScrollBar->GetSafeHwnd())
	{
		int nPos = m_sliderThreshold.GetPos();
		m_nThreshold = nPos;
		UpdateData(FALSE);

		MakePreviewImage();
		Invalidate(FALSE);
	}
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CBinarization::OnEnChangeEditThreshold()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_sliderThreshold.SetPos(m_nThreshold);

	MakePreviewImage();
	Invalidate(FALSE);
}
