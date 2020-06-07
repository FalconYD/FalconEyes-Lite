// Binarization.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "FalconEyes.h"
#include "Binarization.h"
#include "afxdialogex.h"


// CBinarization ��ȭ �����Դϴ�.

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


// CBinarization �޽��� ó�����Դϴ�.
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

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	m_sliderThreshold.SetRange(0, 255);
	m_sliderThreshold.SetTicFreq(32);
	m_sliderThreshold.SetPageSize(32);
	m_sliderThreshold.SetPos(m_nThreshold);

	CRect rect;
	GetDlgItem(IDC_IMAGE_PREVIEW)->GetClientRect(rect);
	DibResizeNearest(m_dibSrc, m_dibSrc, rect.Width(), rect.Height());
	MakePreviewImage();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CBinarization::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
					   // �׸��� �޽����� ���ؼ��� CDialogEx::OnPaint()��(��) ȣ������ ���ʽÿ�.
	CPaintDC dcPreview(GetDlgItem(IDC_IMAGE_PREVIEW));
	m_dibRes.Draw(dcPreview.m_hDC, 0, 0);
}




void CBinarization::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
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
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialogEx::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	m_sliderThreshold.SetPos(m_nThreshold);

	MakePreviewImage();
	Invalidate(FALSE);
}
