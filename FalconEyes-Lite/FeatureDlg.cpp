// FeatureDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "FalconEyes.h"
#include "FalconEyesDoc.h"
#include "FeatureDlg.h"
#include "afxdialogex.h"
#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"


// FeatureDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(FeatureDlg, CDialogEx)

FeatureDlg::FeatureDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_FEATURE, pParent)
{

}

FeatureDlg::~FeatureDlg()
{
}

void FeatureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CB_SOURCE1, m_cbSource1);
	DDX_Control(pDX, IDC_CB_SOURCE2, m_cbSource2);
	DDX_Control(pDX, IDC_CB_MODE, m_cbMode);
}


BEGIN_MESSAGE_MAP(FeatureDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BN_EXCUTE, &FeatureDlg::OnBnClickedBnExcute)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// FeatureDlg 메시지 처리기입니다.

BOOL FeatureDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
		return TRUE;
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
		return TRUE;
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_SPACE)
		return TRUE;
	return CDialogEx::PreTranslateMessage(pMsg);
}


BOOL FeatureDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	//m_cbSource1
	m_cbMode.AddString(_T("SURF"));
	m_cbMode.AddString(_T("SIFT"));
	m_cbMode.SetCurSel(0);
	m_fn_UpdateDoc();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void FeatureDlg::m_fn_UpdateDoc()
{
	CFalconEyesApp* pApp = (CFalconEyesApp*)AfxGetApp();
	POSITION pos = pApp->m_pImageDocTemplate->GetFirstDocPosition();
	int nIdx = 0;
	CString strTitle;
	m_cbSource1.ResetContent();
	m_cbSource2.ResetContent();
	while (pos != NULL)
	{
		CFalconEyesDoc* pDoc = (CFalconEyesDoc*)pApp->m_pImageDocTemplate->GetNextDoc(pos);

		strTitle = pDoc->GetTitle();

		m_cbSource1.InsertString(nIdx, strTitle);
		m_cbSource2.InsertString(nIdx, strTitle);

		m_cbSource1.SetItemDataPtr(nIdx, pDoc);
		m_cbSource2.SetItemDataPtr(nIdx, pDoc);
		nIdx++;
	}

	m_cbSource1.SetCurSel(0);
	m_cbSource2.SetCurSel(0);

	if (nIdx > 1)
		m_cbSource2.SetCurSel(1);
}

void FeatureDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void FeatureDlg::OnBnClickedBnExcute()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strMode;
	m_cbMode.GetWindowText(strMode);
	CFalconEyesDoc* pDoc1 = (CFalconEyesDoc*)m_cbSource1.GetItemDataPtr(m_cbSource1.GetCurSel());
	CFalconEyesDoc* pDoc2 = (CFalconEyesDoc*)m_cbSource2.GetItemDataPtr(m_cbSource2.GetCurSel());
	Mat matSrc1 = pDoc1->m_Dib.GetMatImg();
	Mat matSrc2 = pDoc2->m_Dib.GetMatImg();
	Mat matResult;
	if (strMode == _T("SURF"))
	{
		m_fn_SURF(matSrc1, matSrc2, matResult);
		
	}
	else if(strMode == _T("SIFT"))
	{
		m_fn_SIFT(matSrc1, matSrc2, matResult);
	}
	MatDib dib;
	dib.SetMatImg(matResult);
	AfxNewImage(dib, strMode);
	AfxPrintInfo(_T("[영상특징매칭/%s] %s, %s"), strMode, pDoc1->GetTitle(), pDoc2->GetTitle());
}

void FeatureDlg::m_fn_SURF(Mat matSrc1, Mat matSrc2, Mat& matResult)
{
	Ptr<Feature2D> detector = xfeatures2d::SURF::create(800);
	std::vector<KeyPoint> keyPoint1, keyPoint2;
	Mat matDescriptor1, matDescriptor2;
	detector->detectAndCompute(matSrc1, Mat(), keyPoint1, matDescriptor1);
	detector->detectAndCompute(matSrc2, Mat(), keyPoint2, matDescriptor2);
	BFMatcher matcher;
	std::vector<DMatch> vecMatches;
	matcher.match(matDescriptor1, matDescriptor2, vecMatches);
	if (vecMatches.size() == 0)
		return;
	std::sort(vecMatches.begin(), vecMatches.end());
	std::vector<DMatch> vecGood;
	double minDist = vecMatches.front().distance;
	double maxDist = vecMatches.back().distance;
	const int GOOD = std::min(50, (int)(vecMatches.size() * 0.1));
	for (int nStep = 0; nStep < GOOD; nStep++)
	{
		vecGood.push_back(vecMatches[nStep]);
	}
	drawMatches(matSrc1, keyPoint1, matSrc2, keyPoint2, vecGood, matResult, Scalar::all(-1), Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

	std::vector<Point2f> obj;
	std::vector<Point2f> scene;

	for (size_t i = 0; i < vecGood.size(); i++)
	{
		obj.push_back(keyPoint1[vecGood[i].queryIdx].pt);
		scene.push_back(keyPoint2[vecGood[i].trainIdx].pt);
	}

	std::vector<Point2f> obj_corners(4);
	obj_corners[0] = Point(0, 0);
	obj_corners[1] = Point(matSrc1.cols, 0);
	obj_corners[2] = Point(matSrc1.cols, matSrc1.rows);
	obj_corners[3] = Point(0, matSrc1.rows);
	std::vector<Point2f> scene_corners(4);

	Mat H = findHomography(obj, scene, RANSAC);
	if (H.empty())
		return;
	perspectiveTransform(obj_corners, scene_corners, H);

	line(matResult, scene_corners[0] + Point2f((float)matSrc1.cols, 0), scene_corners[1] + Point2f((float)matSrc1.cols, 0), Scalar(0, 255, 0), 2, LINE_AA);
	line(matResult, scene_corners[1] + Point2f((float)matSrc1.cols, 0), scene_corners[2] + Point2f((float)matSrc1.cols, 0), Scalar(0, 255, 0), 2, LINE_AA);
	line(matResult, scene_corners[2] + Point2f((float)matSrc1.cols, 0), scene_corners[3] + Point2f((float)matSrc1.cols, 0), Scalar(0, 255, 0), 2, LINE_AA);
	line(matResult, scene_corners[3] + Point2f((float)matSrc1.cols, 0), scene_corners[0] + Point2f((float)matSrc1.cols, 0), Scalar(0, 255, 0), 2, LINE_AA);
}

void FeatureDlg::m_fn_SIFT(Mat matSrc1, Mat matSrc2, Mat& matResult)
{
	Ptr<Feature2D> detector = xfeatures2d::SIFT::create(800);
	std::vector<KeyPoint> keyPoint1, keyPoint2;
	Mat matDescriptor1, matDescriptor2;
	detector->detectAndCompute(matSrc1, Mat(), keyPoint1, matDescriptor1);
	detector->detectAndCompute(matSrc2, Mat(), keyPoint2, matDescriptor2);
	BFMatcher matcher;
	std::vector<DMatch> vecMatches;
	matcher.match(matDescriptor1, matDescriptor2, vecMatches);
	if (vecMatches.size() == 0)
		return;
	std::sort(vecMatches.begin(), vecMatches.end());
	std::vector<DMatch> vecGood;
	double minDist = vecMatches.front().distance;
	double maxDist = vecMatches.back().distance;
	const int GOOD = std::min(50, (int)(vecMatches.size() * 0.1));
	for (int nStep = 0; nStep < GOOD; nStep++)
	{
		vecGood.push_back(vecMatches[nStep]);
	}
	drawMatches(matSrc1, keyPoint1, matSrc2, keyPoint2, vecGood, matResult, Scalar::all(-1), Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

	std::vector<Point2f> obj;
	std::vector<Point2f> scene;

	for (size_t i = 0; i < vecGood.size(); i++)
	{
		obj.push_back(keyPoint1[vecGood[i].queryIdx].pt);
		scene.push_back(keyPoint2[vecGood[i].trainIdx].pt);
	}

	std::vector<Point2f> obj_corners(4);
	obj_corners[0] = Point(0, 0);
	obj_corners[1] = Point(matSrc1.cols, 0);
	obj_corners[2] = Point(matSrc1.cols, matSrc1.rows);
	obj_corners[3] = Point(0, matSrc1.rows);
	std::vector<Point2f> scene_corners(4);

	Mat H = findHomography(obj, scene, RANSAC);
	if (H.empty())
		return;
	perspectiveTransform(obj_corners, scene_corners, H);

	line(matResult, scene_corners[0] + Point2f((float)matSrc1.cols, 0), scene_corners[1] + Point2f((float)matSrc1.cols, 0), Scalar(0, 255, 0), 2, LINE_AA);
	line(matResult, scene_corners[1] + Point2f((float)matSrc1.cols, 0), scene_corners[2] + Point2f((float)matSrc1.cols, 0), Scalar(0, 255, 0), 2, LINE_AA);
	line(matResult, scene_corners[2] + Point2f((float)matSrc1.cols, 0), scene_corners[3] + Point2f((float)matSrc1.cols, 0), Scalar(0, 255, 0), 2, LINE_AA);
	line(matResult, scene_corners[3] + Point2f((float)matSrc1.cols, 0), scene_corners[0] + Point2f((float)matSrc1.cols, 0), Scalar(0, 255, 0), 2, LINE_AA);
}