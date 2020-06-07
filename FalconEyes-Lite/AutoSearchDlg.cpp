// AutoSearchDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "FalconEyes.h"
#include "AutoSearchDlg.h"
#include "afxdialogex.h"
#include "../IppImage/MatColor.h"


// AutoSearchDlg 대화 상자

IMPLEMENT_DYNAMIC(AutoSearchDlg, CDialogEx)

AutoSearchDlg::AutoSearchDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SIMUL_AUTOSEARCH, pParent)
{

}

AutoSearchDlg::~AutoSearchDlg()
{
}

void AutoSearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_LOG, m_listLog);
}


BEGIN_MESSAGE_MAP(AutoSearchDlg, CDialogEx)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BN_OPEN, &AutoSearchDlg::OnBnClickedBnOpen)
	ON_BN_CLICKED(IDC_BN_NEXT, &AutoSearchDlg::OnBnClickedBnNext)
	ON_BN_CLICKED(IDC_BN_PREV, &AutoSearchDlg::OnBnClickedBnPrev)
	ON_BN_CLICKED(IDC_BN_SEARCH, &AutoSearchDlg::OnBnClickedBnSearch)
	ON_BN_CLICKED(IDC_BN_AUTOSEARCH, &AutoSearchDlg::OnBnClickedBnAutosearch)
END_MESSAGE_MAP()


// AutoSearchDlg 메시지 처리기


BOOL AutoSearchDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_nRed = 0;
	m_nGreen = 0;
	m_nBlue = 0;
	m_nHue = 0;
	m_nSaturation = 0;
	m_nIntensity = 0;
	m_nTotalCount = 0;
	m_nNormalCount = 0;
	m_nDefectCount = 0;

	((CButton*)GetDlgItem(IDC_BN_SEARCH))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_BN_AUTOSEARCH))->EnableWindow(FALSE);

	((CComboBox*)GetDlgItem(IDC_CB_FILTERSIZE))->AddString(L"3");
	((CComboBox*)GetDlgItem(IDC_CB_FILTERSIZE))->AddString(L"5");
	((CComboBox*)GetDlgItem(IDC_CB_FILTERSIZE))->AddString(L"7");
	((CComboBox*)GetDlgItem(IDC_CB_FILTERSIZE))->AddString(L"9");
	((CComboBox*)GetDlgItem(IDC_CB_FILTERSIZE))->AddString(L"11");
	((CComboBox*)GetDlgItem(IDC_CB_FILTERSIZE))->SetCurSel(3);

	((CEdit*)GetDlgItem(IDC_EDIT_PATTERNDISX))->SetWindowTextW(L"335");
	((CEdit*)GetDlgItem(IDC_EDIT_PATTERNDISY))->SetWindowTextW(L"335");
	((CEdit*)GetDlgItem(IDC_EDIT_PERCENT))->SetWindowTextW(L"75");
	((CEdit*)GetDlgItem(IDC_EDIT_SIGMA))->SetWindowTextW(L"1.4");

	m_fn_DrawChartControl(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void AutoSearchDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


BOOL AutoSearchDlg::PreTranslateMessage(MSG* pMsg)
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

void AutoSearchDlg::m_fn_DrawChartControl(BOOL bUpdate)
{
	CRect rect;

	GetDlgItem(IDC_GRAPH_RGB)->GetWindowRect(&rect);
	ScreenToClient(&rect);
	if (!m_chrtRGB.Create(rect, this, IDC_GRAPH_RGB))
	{
		if (bUpdate)
		{
			if (!m_chrtRGB.GetSafeHwnd())
			{
				AfxMessageBox(_T("Unable to create the chart control"));
				return;
			}
		}
		else
		{
			if (m_chrtRGB.GetSafeHwnd())
			{
				AfxMessageBox(_T("Unable to create the chart control"));
				return;
			}
		}
		m_chrtRGB.Reset();
		m_chrtRGB.SetAutoScale(FALSE);
	}

	m_chrtRGB.SetTitle(_T("Avg Histo"));
	m_chrtRGB.SetBKColor(RGB(255, 255, 255));
	m_chrtRGB.ShowTooltip(FALSE);
	m_chrtRGB.SetMaxValue(100.0);
	m_chrtRGB.AddBar(m_nRed, _T("R"), RGB(255, 0, 0), TRUE);
	m_chrtRGB.AddBar(m_nGreen, _T("G"), RGB(0, 255, 0), TRUE);
	m_chrtRGB.AddBar(m_nBlue, _T("B"), RGB(0, 0, 255), TRUE);
	m_chrtRGB.RedrawAll();

	GetDlgItem(IDC_GRAPH_HSV)->GetWindowRect(&rect);
	ScreenToClient(&rect);
	if (!m_chrtHSV.Create(rect, this, IDC_GRAPH_HSV))
	{
		if (bUpdate)
		{
			if (!m_chrtHSV.GetSafeHwnd())
			{
				AfxMessageBox(_T("Unable to create the chart control"));
				return;
			}
		}
		else
		{
			if (m_chrtHSV.GetSafeHwnd())
			{
				AfxMessageBox(_T("Unable to create the chart control"));
				return;
			}
		}
		m_chrtHSV.Reset();
		m_chrtHSV.SetAutoScale(FALSE);
	}
	m_chrtHSV.SetTitle(_T("HSI Histo"));
	m_chrtHSV.SetBKColor(RGB(255, 255, 255));
	m_chrtHSV.ShowTooltip(FALSE);
	m_chrtHSV.SetMaxValue(100.0);
	m_chrtHSV.AddBar(m_nHue, _T("H"), RGB(255, 0, 0), TRUE);
	m_chrtHSV.AddBar(m_nSaturation, _T("S"), RGB(0, 255, 0), TRUE);
	m_chrtHSV.AddBar(m_nIntensity, _T("I"), RGB(0, 0, 255), TRUE);
	m_chrtHSV.RedrawAll();

	GetDlgItem(IDC_GRAPH_DETECTRATE)->GetWindowRect(&rect);
	ScreenToClient(&rect);
	if (!m_chrtDetectRate.Create(rect, this, IDC_GRAPH_RGB))
	{
		if (bUpdate)
		{
			if (!m_chrtDetectRate.GetSafeHwnd())
			{
				AfxMessageBox(_T("Unable to create the chart control"));
				return;
			}
		}
		else
		{
			if (m_chrtDetectRate.GetSafeHwnd())
			{
				AfxMessageBox(_T("Unable to create the chart control"));
				return;
			}
		}
		m_chrtDetectRate.Reset();
		m_chrtDetectRate.SetAutoScale(FALSE);
	}
	m_chrtDetectRate.SetTitle(_T("Detect Rate"));
	m_chrtDetectRate.SetBKColor(RGB(255, 255, 255));
	m_chrtDetectRate.ShowTooltip(FALSE);
	m_chrtDetectRate.SetMaxValue(100.0);
	m_chrtDetectRate.AddBar(m_nTotalCount, _T("Total"), RGB(60, 60, 60), TRUE);
	m_chrtDetectRate.AddBar(m_nNormalCount, _T("Normal"), RGB(60, 200, 60), TRUE);
	m_chrtDetectRate.AddBar(m_nDefectCount, _T("Defect"), RGB(200, 60, 60), TRUE);

	m_chrtDetectRate.RedrawAll();
}

void AutoSearchDlg::m_fn_OptimizedSetParam()
{
	if (!m_matDisplay.empty())
	{
		RECT r;
		GetDlgItem(IDC_PIC_DISPLAY)->GetClientRect(&r);
		m_szWinSize.width = r.right;
		m_szWinSize.height = r.bottom;
		int bpp = m_matDisplay.channels() * 8;
		m_cimgDisplay.Create(m_szWinSize.width, m_szWinSize.height, bpp);
		m_hdcDraw = m_cimgDisplay.GetDC();

		m_bitInfoDraw.bmiHeader.biBitCount = bpp;
		m_bitInfoDraw.bmiHeader.biWidth = m_matDisplay.cols;
		m_bitInfoDraw.bmiHeader.biHeight = -m_matDisplay.rows;
		m_bitInfoDraw.bmiHeader.biPlanes = 1;
		m_bitInfoDraw.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		m_bitInfoDraw.bmiHeader.biCompression = BI_RGB;
		m_bitInfoDraw.bmiHeader.biClrImportant = 0;
		m_bitInfoDraw.bmiHeader.biClrUsed = 0;
		m_bitInfoDraw.bmiHeader.biSizeImage = 0;
		m_bitInfoDraw.bmiHeader.biXPelsPerMeter = 0;
		m_bitInfoDraw.bmiHeader.biYPelsPerMeter = 0;
		SetStretchBltMode(m_hdcDraw, HALFTONE);
		m_dcOut = ::GetDC(GetDlgItem(IDC_PIC_DISPLAY)->m_hWnd);
	}
}
void AutoSearchDlg::m_fn_OptimizedDrawImage()
{
	// No Stratch Code
	//SetDIBitsToDevice(m_hdcDraw, 0, 0, m_szWinSize.width, m_szWinSize.height, 0, 0, 0, m_matDisplay.rows, m_matDisplay.data, &m_bitInfoDraw, DIB_RGB_COLORS);
	StretchDIBits(m_hdcDraw, 0, 0, m_szWinSize.width, m_szWinSize.height, 0, 0, m_matDisplay.cols, m_matDisplay.rows, m_matDisplay.data, &m_bitInfoDraw, DIB_RGB_COLORS, SRCCOPY);
	m_cimgDisplay.BitBlt(m_dcOut, 0, 0);
}
void AutoSearchDlg::m_fn_OptimizedRelease()
{
	if (m_dcOut != NULL)
		::ReleaseDC(GetDlgItem(IDC_PIC_DISPLAY)->m_hWnd, m_dcOut);

	if (!m_cimgDisplay.IsNull())
	{
		m_cimgDisplay.ReleaseDC();
		m_cimgDisplay.Destroy();
	}
}

void AutoSearchDlg::OnBnClickedBnOpen()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog fdlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, _T("Bitmap Files (*.bmp)|*.bmp|JPEG Files (*.jpg)|*.jpg|All Files(*.*)|*.*||"));
	CFileFind finder;
	BOOL bWorking = false;
	
	std::wstring wstrfilename, wstrTmp;
	char chFilename[MAX_PATH];
	
	if (fdlg.DoModal() == IDOK)
	{
		wstrfilename = fdlg.GetPathName();
		_in_fn_Uni2Multi(wstrfilename.c_str(), chFilename);
		m_strPath = fdlg.GetFolderPath();
		wstrfilename = m_strPath + L"\\*.bmp";
		bWorking = finder.FindFile(wstrfilename.c_str());
		if (!bWorking)
			return;
		while (finder.FindNextFileW())
		{
			if (finder.IsDirectory())
			{
				continue;
			}
			wstrTmp = finder.GetFileName();
			if (wcscmp(wstrTmp.c_str(), fdlg.GetFileName()) == 0)
			{
				m_nFileIndex = static_cast<int>(m_vecFile.size());
			}
			m_vecFile.push_back(wstrTmp);
		}
		if (m_nFileIndex < 0)
			m_nFileIndex = 0;
		wstrTmp = m_strPath + L"\\" + m_vecFile.at(m_nFileIndex);
		_in_fn_Uni2Multi(wstrTmp.c_str(), chFilename);
		m_matDisplay = imread(chFilename);
		char chTitle[MAX_PATH] = { 0, };
		_in_fn_Uni2Multi(m_vecFile.at(m_nFileIndex).c_str(), chTitle);
		m_fn_WriteLog("FileOpen : %s", chTitle);

		((CButton*)GetDlgItem(IDC_BN_SEARCH))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_BN_AUTOSEARCH))->EnableWindow(TRUE);

		m_fn_UpdateInfo();
		m_fn_OptimizedSetParam();
		m_fn_OptimizedDrawImage();
		m_fn_OptimizedRelease();
	}
}


void AutoSearchDlg::OnBnClickedBnNext()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	std::wstring wstrTmp;
	char chFilename[MAX_PATH];
	int nSize;
	nSize = static_cast<int>(m_vecFile.size());
	if (++m_nFileIndex >= nSize)
	{
		m_nFileIndex = 0;
	}
	wstrTmp = m_strPath + L"\\" + m_vecFile.at(m_nFileIndex);
	_in_fn_Uni2Multi(wstrTmp.c_str(), chFilename);
	m_matDisplay = imread(chFilename);
	char chTitle[MAX_PATH] = { 0, };
	_in_fn_Uni2Multi(m_vecFile.at(m_nFileIndex).c_str(), chTitle);
	m_fn_WriteLog("FileOpen : %s", chTitle);

	m_fn_UpdateInfo();
	m_fn_OptimizedSetParam();
	m_fn_OptimizedDrawImage();
	m_fn_OptimizedRelease();
}


void AutoSearchDlg::OnBnClickedBnPrev()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	std::wstring wstrTmp;
	char chFilename[MAX_PATH];
	int nSize;
	nSize = static_cast<int>(m_vecFile.size());
	if (--m_nFileIndex < 0)
	{
		m_nFileIndex = nSize -1;
	}
	wstrTmp = m_strPath + L"\\" + m_vecFile.at(m_nFileIndex);
	_in_fn_Uni2Multi(wstrTmp.c_str(), chFilename);
	m_matDisplay = imread(chFilename);
	char chTitle[MAX_PATH] = { 0, };
	_in_fn_Uni2Multi(m_vecFile.at(m_nFileIndex).c_str(), chTitle);
	m_fn_WriteLog("FileOpen : %s", chTitle);

	m_fn_UpdateInfo();
	m_fn_OptimizedSetParam();
	m_fn_OptimizedDrawImage();
	m_fn_OptimizedRelease();
}

void AutoSearchDlg::m_fn_UpdateInfo()
{
	std::wstring wstrTmp;
	wstrTmp.resize(_MAX_PATH);
	wnsprintf(&wstrTmp[0], (int)wstrTmp.length(), L"%d / %d", m_nFileIndex, m_vecFile.size() - 1);
	wstrTmp.resize(wcslen(wstrTmp.data()) + 1);

	SetDlgItemText(IDC_STATIC_IMAGECOUNT, wstrTmp.c_str());

	wstrTmp = m_strPath + L"\\" + m_vecFile.at(m_nFileIndex);
	SetDlgItemText(IDC_STATIC_FILEPATH, wstrTmp.c_str());

	int nStep;
	double dRed = 0.0, dGreen = 0.0, dBlue = 0.0;
	double dHue = 0.0, dSatuation = 0.0, dIntensity = 0.0;
	double dh, ds, di;
	int nChannel = m_matDisplay.channels();
	int nImgSize = m_matDisplay.cols * m_matDisplay.rows;
	int nDataSize = nImgSize * nChannel;
	uchar* pData = m_matDisplay.data;
	if (nChannel == 3)
	{
		for (nStep = 0; nStep < nDataSize; nStep += 3)
		{
			dBlue += (double)pData[nStep + 0] / (double)nImgSize;
			dGreen += (double)pData[nStep + 1] / (double)nImgSize;
			dRed += (double)pData[nStep + 2] / (double)nImgSize;
			RGB_TO_HSI((double)pData[nStep + 2], (double)pData[nStep + 1], (double)pData[nStep + 0], dh, ds, di);
			dHue += dh / (double)nImgSize;
			dSatuation += ds / (double)nImgSize;
			dIntensity += di / (double)nImgSize;
		}
		m_nBlue = static_cast<int>((dBlue / 255.) * 100);
		m_nGreen = static_cast<int>((dGreen / 255.) * 100);
		m_nRed = static_cast<int>((dRed / 255.) * 100);

		m_nHue = static_cast<int>(dHue * 100);
		m_nSaturation = static_cast<int>(dSatuation * 100);
		m_nIntensity = static_cast<int>(dIntensity);
	}

	m_fn_DrawChartControl();
}

void AutoSearchDlg::OnBnClickedBnSearch()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	const int CROPSIZE = 5;
	CString strtmp;
	m_stSearchParam.nPatternDisX = GetDlgItemInt(IDC_EDIT_PATTERNDISX);;
	m_stSearchParam.nPatternDisY = GetDlgItemInt(IDC_EDIT_PATTERNDISY);;
	
	GetDlgItemText(IDC_EDIT_SIGMA, strtmp);
	const double dSigma = _ttof(strtmp);

	cv::Mat matSrc, matDst, matROI;

	m_stSearchParam.nFilterSize = static_cast<int>(GetDlgItemInt(IDC_CB_FILTERSIZE) / 2.0);

	GetDlgItemText(IDC_EDIT_PERCENT, strtmp);
	m_stSearchParam.dMatchingRate = _ttof(strtmp);

	matSrc = m_matDisplay.clone();

	if (matSrc.channels() == 3)
		cv::cvtColor(matSrc, matSrc, ColorConversionCodes::COLOR_BGR2GRAY);

	Rect rectROI(CROPSIZE, CROPSIZE, matSrc.cols - CROPSIZE*2, matSrc.rows - CROPSIZE*2);
	matROI = matSrc(rectROI).clone();
	int dim = static_cast<int> (2 * 4 * dSigma + 1.0);
	if (dim > 9)
		dim = 9;
	GaussianBlur(matROI, matROI, Size(static_cast<int>(sqrt(dim)), static_cast<int>(sqrt(dim))), static_cast<int>(dSigma));

	m_stSearchParam.nWidth = matROI.cols;
	m_stSearchParam.nHeight = matROI.rows;

	matDst = Mat(Size(m_stSearchParam.nWidth, m_stSearchParam.nHeight), CV_8UC1, Scalar(255));
	uchar* pSrc = matROI.data;
	uchar* pDst = matDst.data;
	m_fn_SearchDefect(pSrc, pDst, m_stSearchParam);
// 	for (nStepX = nFilterSize; nStepX < nWidth - nFilterSize; ++nStepX)
// 	{
// 		if (nStepX > nWidth - nDistance)
// 		{
// 			nStepX2 = nStepX - nDistance;
// 		}
// 		else
// 		{
// 			nStepX2 = nStepX + nDistance;
// 		}
// 		for (nStepY = 2; nStepY < nHeight - 2; ++nStepY)
// 		{
// 			// N*N Filter
// 
// 			if (nStepY > (nHeight - nDistance2))
// 			{
// 				nStepY2 = nStepY - nDistance;
// 			}
// 			else
// 			{
// 				nStepY2 = nStepY + nDistance2;
// 			}
// 			for (int y = -nFilterSize; y < nFilterSize; ++y)
// 			{
// 				for (int x = -nFilterSize; x < nFilterSize; ++x)
// 				{
// 					if (m_fn_CompareValue(pSrc[nStepY * nWidth + nStepX], pSrc[(nStepY + y) * nWidth + (nStepX2 + x)], dMatchingRate))
// 					{
// 						pDst[nStepY * nWidth + nStepX] = 0;
// 					}
// 				}
// 			}
// 			if (pDst[nStepY * nWidth + nStepX] != 0)
// 			{
// 				for (int y = -nFilterSize; y < nFilterSize; ++y)
// 				{
// 					for (int x = -nFilterSize; x < nFilterSize; ++x)
// 					{
// 						if (m_fn_CompareValue(pSrc[nStepY * nWidth + nStepX], pSrc[(nStepY2 + y) * nWidth + (nStepX + x)], dMatchingRate))
// 						{
// 							pDst[nStepY * nWidth + nStepX] = 0;
// 						}
// 					}
// 				}
// 			}
// 			// ~N*N Filter
// 		}
// 	}

	cv::line(matDst, Point(0, (int)(m_stSearchParam.nFilterSize / 2.0)), Point(matDst.cols, (int)(m_stSearchParam.nFilterSize / 2.0)), Scalar(0), m_stSearchParam.nFilterSize);
	cv::line(matDst, Point(0, (int)(matDst.rows - m_stSearchParam.nFilterSize / 2.0)), Point(matDst.cols, (int)(matDst.rows - m_stSearchParam.nFilterSize / 2.0)), Scalar(0), m_stSearchParam.nFilterSize);
	cv::line(matDst, Point((int)(m_stSearchParam.nFilterSize / 2.0), 0), Point((int)(m_stSearchParam.nFilterSize / 2.0), matDst.rows), Scalar(0), m_stSearchParam.nFilterSize);
	cv::line(matDst, Point((int)(matDst.cols - m_stSearchParam.nFilterSize / 2.0), 0), Point((int)(matDst.cols - m_stSearchParam.nFilterSize / 2.0), matDst.rows), Scalar(0), m_stSearchParam.nFilterSize);

	// = matDst.clone()
	std::vector<MatLabelInfo> labels;
	MatLabeling(matDst, labels);

	int cnt = 0;
	for (auto label : labels)
	{

		label.cx += CROPSIZE;
		label.cy += CROPSIZE;
		label.minx += CROPSIZE;
		label.miny += CROPSIZE;
		label.maxx += CROPSIZE;
		label.maxy += CROPSIZE;
		cnt++;
		cv::rectangle(m_matDisplay, Rect(label.minx - CROPSIZE, label.miny - CROPSIZE, (int)((label.maxx - label.minx) + CROPSIZE * 2), (int)((label.maxy - label.miny) + CROPSIZE * 2)), Scalar(0, 0, 255), 2);
	}
	m_fn_WriteLog("Defect Count : %d, %d", labels.size(), cnt);

	m_fn_UpdateInfo();
	m_fn_OptimizedSetParam();
	m_fn_OptimizedDrawImage();
	m_fn_OptimizedRelease();
}

bool AutoSearchDlg::m_fn_CompareValue(uchar uchData1, uchar uchData2, double dPercent)
{
	return ((uchData1 >= (uchData2 * dPercent / 100.)) && ((uchData1 * dPercent / 100.) <= uchData2));
}

void AutoSearchDlg::m_fn_SearchDefect(uchar* pSrc, uchar* pDst, SearchParam& stParam)
{
	int x, y;
	int nStepX, nStepY, nStepX2, nStepY2;
	for (nStepX = stParam.nFilterSize; nStepX < stParam.nWidth - stParam.nFilterSize; ++nStepX)
	{
		if (nStepX >= stParam.nWidth - stParam.nPatternDisX - stParam.nFilterSize)
		{
			nStepX2 = nStepX - stParam.nPatternDisX;
		}
		else
		{
			nStepX2 = nStepX + stParam.nPatternDisX;
		}
		for (nStepY = stParam.nFilterSize; nStepY < stParam.nHeight - stParam.nFilterSize; ++nStepY)
		{
			// N*N Filter

			if (nStepY >= (stParam.nHeight - stParam.nPatternDisY - stParam.nFilterSize))
			{
				nStepY2 = nStepY - stParam.nPatternDisY;
			}
			else
			{
				nStepY2 = nStepY + stParam.nPatternDisY;
			}
			for (y = -stParam.nFilterSize; y < stParam.nFilterSize; ++y)
			{
				for (x = -stParam.nFilterSize; x < stParam.nFilterSize; ++x)
				{
					if (m_fn_CompareValue(pSrc[nStepY * stParam.nWidth + nStepX], pSrc[(nStepY + y) * stParam.nWidth + (nStepX2 + x)], stParam.dMatchingRate))
					{
						pDst[nStepY * stParam.nWidth + nStepX] = 0;
					}
				}
			}
			if (pDst[nStepY * stParam.nWidth + nStepX] != 0)
			{
				for (y = -stParam.nFilterSize; y < stParam.nFilterSize; ++y)
				{
					for (x = -stParam.nFilterSize; x < stParam.nFilterSize; ++x)
					{
						if (m_fn_CompareValue(pSrc[nStepY * stParam.nWidth + nStepX], pSrc[(nStepY2 + y) * stParam.nWidth + (nStepX + x)], stParam.dMatchingRate))
						{
							pDst[nStepY * stParam.nWidth + nStepX] = 0;
						}
					}
				}
			}
			// ~N*N Filter
		}
	}
}

void AutoSearchDlg::OnBnClickedBnAutosearch()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_stAutoSearchParam.bThreadFlag)
	{
		m_stAutoSearchParam.bThreadFlag = false;
// 		while (m_stAutoSearchParam.enState == THREAD_STATUS::THREAD_STAT_RUNNING)
// 		{
// 			Sleep(10);
// 		}
		SetDlgItemText(IDC_BN_AUTOSEARCH, L"Auto Search");
		((CButton*)GetDlgItem(IDC_BN_SEARCH))->EnableWindow(TRUE);
	}
	else
	{
		m_stAutoSearchParam.bThreadFlag = true;
		m_stAutoSearchParam.cObjectPointer1 = this;
		m_stAutoSearchParam.enState = THREAD_STATUS::THREAD_STAT_NONE;
		m_stAutoSearchParam.nScanInterval = 1;

		m_stSearchParam.nPatternDisX = GetDlgItemInt(IDC_EDIT_PATTERNDISX);
		m_stSearchParam.nPatternDisY = GetDlgItemInt(IDC_EDIT_PATTERNDISY);
		m_fn_WriteLog("Run Auto Search");
		HANDLE hThread = (HANDLE*)_beginthreadex(NULL, 0, THREAD_AUTOSEARCH, &m_stAutoSearchParam, 0, NULL);
		CloseHandle(hThread);
		SetDlgItemText(IDC_BN_AUTOSEARCH, L"Stop");
		((CButton*)GetDlgItem(IDC_BN_SEARCH))->EnableWindow(FALSE);
	}
	
}

void AutoSearchDlg::m_fn_WriteLog(char* strMsg, ...)
{
	char chLog[LOG_TEXT_MAX_SIZE] = { 0, };
	char chBuff[LOG_TEXT_MAX_SIZE] = { 0, };
	wchar_t wLog[LOG_TEXT_MAX_SIZE] = { 0, };
	va_list ap;
	va_start(ap, strMsg);
	vsnprintf_s(chLog, LOG_TEXT_MAX_SIZE, strMsg, ap);
	va_end(strMsg);

	SYSTEMTIME systime;
	GetLocalTime(&systime);
	sprintf_s(chBuff,"[%02d : %02d : %02d : %03d] %s", systime.wHour, systime.wMinute, systime.wSecond, systime.wMilliseconds, chLog);

	_in_fn_Multi2Uni(chBuff, wLog);
	if (m_listLog.GetCount() > LIST_VIEW_COUNT)
		m_listLog.DeleteString(0);
	m_listLog.AddString(wLog);
	m_listLog.SetScrollPos(SB_VERT, m_listLog.GetCount(), TRUE);
	m_listLog.SetTopIndex(m_listLog.GetCount() - 1);

// 	if (strlen(chLog) > 0)
// 		mc_Log.m_fn_PushLog(chLog);
}

unsigned int AutoSearchDlg::THREAD_AUTOSEARCH(void* pParam)
{
	ST_THREAD_PARAM* stThreadParam = (ST_THREAD_PARAM*)pParam;
	AutoSearchDlg* pDlg = (AutoSearchDlg*)stThreadParam->cObjectPointer1;

	int nTotalFileCount = (int)pDlg->m_vecFile.size(), nFileStep = 0;
	int nDefectCount = 0, nNormalCount = 0;;

	const int CROPSIZE = 5;
	char chFilename[_MAX_PATH] = { 0, };
	char chFilePath[_MAX_PATH] = { 0, };
	char chFilePathTmp[_MAX_PATH] = { 0, };

	CString strtmp;
	cv::Mat matSrc, matDst, matROI;
	std::wstring wstrTmp;
	std::vector<MatLabelInfo> labels;
	
	pDlg->GetDlgItemText(IDC_EDIT_SIGMA, strtmp);
	const double dSigma = _ttof(strtmp);
	pDlg->m_nTotalCount = 100;
	
	stThreadParam->enState = THREAD_STATUS::THREAD_STAT_ACTIVE;
	while ((nTotalFileCount > nFileStep) && (stThreadParam->bThreadFlag))
	{
		Sleep(stThreadParam->nScanInterval);

		wstrTmp = pDlg->m_strPath + L"\\" + pDlg->m_vecFile.at(nFileStep);
		_in_fn_Uni2Multi(wstrTmp.c_str(), chFilePath);
		_in_fn_Uni2Multi(pDlg->m_strPath.c_str(), chFilePathTmp);
		_in_fn_Uni2Multi(pDlg->m_vecFile.at(nFileStep).c_str(), chFilename);
		
		pDlg->m_matDisplay = imread(chFilePath);
		
		if (!pDlg->m_matDisplay.empty())
		{
			pDlg->m_fn_WriteLog("File Open: %s", chFilename);
			stThreadParam->enState = THREAD_STATUS::THREAD_STAT_RUNNING;
			pDlg->m_stSearchParam.nPatternDisX = pDlg->GetDlgItemInt(IDC_EDIT_PATTERNDISX);;
			pDlg->m_stSearchParam.nPatternDisY = pDlg->GetDlgItemInt(IDC_EDIT_PATTERNDISY);;

			pDlg->m_stSearchParam.nFilterSize = static_cast<int>(pDlg->GetDlgItemInt(IDC_CB_FILTERSIZE) / 2.0);

			pDlg->GetDlgItemText(IDC_EDIT_PERCENT, strtmp);
			pDlg->m_stSearchParam.dMatchingRate = _ttof(strtmp);

			matSrc = pDlg->m_matDisplay.clone();

			if (matSrc.channels() == 3)
				cv::cvtColor(matSrc, matSrc, ColorConversionCodes::COLOR_BGR2GRAY);

			Rect rectROI(CROPSIZE, CROPSIZE, matSrc.cols - CROPSIZE * 2, matSrc.rows - CROPSIZE * 2);
			matROI = matSrc(rectROI).clone();
			int dim = static_cast<int> (2 * 4 * dSigma + 1.0);
			if (dim > 9)
				dim = 9;
			GaussianBlur(matROI, matROI, Size(static_cast<int>(sqrt(dim)), static_cast<int>(sqrt(dim))), static_cast<int>(dSigma));

			pDlg->m_stSearchParam.nWidth = matROI.cols;
			pDlg->m_stSearchParam.nHeight = matROI.rows;

			matDst = Mat(Size(pDlg->m_stSearchParam.nWidth, pDlg->m_stSearchParam.nHeight), CV_8UC1, Scalar(255));
			uchar* pSrc = matROI.data;
			uchar* pDst = matDst.data;
			pDlg->m_fn_SearchDefect(pSrc, pDst, pDlg->m_stSearchParam);

			cv::line(matDst, Point(0, (int)(pDlg->m_stSearchParam.nFilterSize / 2.0)), Point(matDst.cols, (int)(pDlg->m_stSearchParam.nFilterSize / 2.0)), Scalar(0), pDlg->m_stSearchParam.nFilterSize);
			cv::line(matDst, Point(0, (int)(matDst.rows - pDlg->m_stSearchParam.nFilterSize / 2.0)), Point(matDst.cols, (int)(matDst.rows - pDlg->m_stSearchParam.nFilterSize / 2.0)), Scalar(0), pDlg->m_stSearchParam.nFilterSize);
			cv::line(matDst, Point((int)(pDlg->m_stSearchParam.nFilterSize / 2.0), 0), Point((int)(pDlg->m_stSearchParam.nFilterSize / 2.0), matDst.rows), Scalar(0), pDlg->m_stSearchParam.nFilterSize);
			cv::line(matDst, Point((int)(matDst.cols - pDlg->m_stSearchParam.nFilterSize / 2.0), 0), Point((int)(matDst.cols - pDlg->m_stSearchParam.nFilterSize / 2.0), matDst.rows), Scalar(0), pDlg->m_stSearchParam.nFilterSize);


			labels.clear();
			MatLabeling(matDst, labels);

			for (auto label : labels)
			{
				label.cx += CROPSIZE;
				label.cy += CROPSIZE;
				label.minx += CROPSIZE;
				label.miny += CROPSIZE;
				label.maxx += CROPSIZE;
				label.maxy += CROPSIZE;

				cv::rectangle(pDlg->m_matDisplay, Rect(label.minx - CROPSIZE, label.miny - CROPSIZE, (int)((label.maxx - label.minx) + CROPSIZE*2), (int)((label.maxy - label.miny) + CROPSIZE*2)), Scalar(0, 0, 255), 2);
			}
			pDlg->m_fn_WriteLog("Defect Count : %d", labels.size());
			if(labels.size() > 0)
			{
				sprintf_s(chFilePathTmp, "%s\\Defect", chFilePathTmp);
				pDlg->m_nDefectCount = static_cast<int>((++nDefectCount / (double)nTotalFileCount) * 100);
			}
			else
			{
				sprintf_s(chFilePathTmp, "%s\\Normal", chFilePathTmp);
				pDlg->m_nNormalCount = static_cast<int>((++nNormalCount / (double)nTotalFileCount) * 100);
			}
			pDlg->m_fn_SaveMat(chFilePathTmp, chFilename);
			pDlg->m_fn_UpdateInfo();
			pDlg->m_fn_OptimizedSetParam();
			pDlg->m_fn_OptimizedDrawImage();
			pDlg->m_fn_OptimizedRelease();
		}
		else
		{
			pDlg->m_fn_WriteLog("FileOpen Error : %s", chFilename);
		}
		nFileStep++;
	}
	stThreadParam->enState = THREAD_STATUS::THREAD_STAT_COMPLETE;
	pDlg->m_fn_WriteLog("End Search");
	return 0;
}

void AutoSearchDlg::m_fn_SaveMat(char* chFilePath, char* chFileName)
{
	char chFileFullPath[MAX_PATH] = { 0, };
	wchar_t wFilePath[MAX_PATH] = { 0, };
	_in_fn_Multi2Uni(chFilePath, wFilePath);
	//g_fn_create
	m_fn_CreateDirectoryFullPath(wFilePath);
	sprintf_s(chFileFullPath, "%s\\%s", chFilePath, chFileName);
	if (!imwrite(chFileFullPath, m_matDisplay))
	{
		m_fn_WriteLog("FileSave Error : %s", chFileName);
	}

}
BOOL AutoSearchDlg::m_fn_CreateDirectoryFullPath(CString strPath)
{
	if (::PathIsDirectory(strPath) != FALSE)
		return TRUE;
	else {
		int i = 0;

		while (1) {
			i = strPath.Find(_T("\\"), i + 1);
			if (i <= 0) {
				CreateDirectory(strPath, 0);
				break;
			}
			CreateDirectory(strPath.Left(i + 1), 0);
		}
	}
	if (::PathIsDirectory(strPath) != FALSE)
		return TRUE;
	else
		return FALSE;

	return TRUE;
}