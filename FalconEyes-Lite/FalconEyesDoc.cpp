
// FalconEyesDoc.cpp : CFalconEyesDoc 클래스의 구현
//

#include "stdafx.h"
#include "FileNewDlg.h"
#include "BrightnessConstrastDlg.h"
#include "GammaCorrectionDlg.h"
#include "HistogramDlg.h"
#include "ArithmeticLogicalDlg.h"
#include "GaussianDlg.h"
#include "AddNoiseDlg.h"
#include "DiffusionDlg.h"
#include "TranslateDlg.h"
#include "ResizeDlg.h"
#include "RotateDlg.h"
#include "CannyDlg.h"
#include "Binarization.h"
#include "HarrisCornerDlg.h"
#include "CColorCombineDlg.h"
#include "CFourierDescDlg.h"
#include "FeatureDlg.h"

// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "FalconEyes.h"
#endif

#include "FalconEyesDoc.h"
#include "FalconEyesView.h"
#include "MainFrm.h"
// #include <propkey.h>
// #include <mmsystem.h>
// #pragma comment(lib, "winmm.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CFalconEyesDoc

IMPLEMENT_DYNCREATE(CFalconEyesDoc, CDocument)

BEGIN_MESSAGE_MAP(CFalconEyesDoc, CDocument)
	ON_COMMAND(ID_WINDOW_DUPLICATE, &CFalconEyesDoc::OnWindowDuplicate)
	ON_COMMAND(ID_EDIT_COPY, &CFalconEyesDoc::OnEditCopy)
	ON_COMMAND(ID_IMAGE_INVERSE, &CFalconEyesDoc::OnImageInverse)
	ON_COMMAND(ID_IMAGE_BRIGHTNESSCONTRAST, &CFalconEyesDoc::OnImageBrightnesscontrast)
	ON_COMMAND(ID_IMAGE_GAMMACORRECTION, &CFalconEyesDoc::OnImageGammacorrection)
	ON_COMMAND(ID_HISTOGRAM_STRETCHING, &CFalconEyesDoc::OnHistogramStretching)
	ON_COMMAND(ID_HISTOGRAM_EQUALIZATION, &CFalconEyesDoc::OnHistogramEqualization)
	ON_COMMAND(ID_IMAGE_ARITHMETICLOGICAL, &CFalconEyesDoc::OnImageArithmeticlogical)
	ON_COMMAND(ID_BITPLANE_SLICING, &CFalconEyesDoc::OnBitplaneSlicing)
	ON_COMMAND(ID_IMAGE_TRANSLATE, &CFalconEyesDoc::OnImageTranslate)
	ON_COMMAND(ID_IMAGE_RESIZE, &CFalconEyesDoc::OnImageResize)
	ON_COMMAND(ID_IMAGE_ROTATE, &CFalconEyesDoc::OnImageRotate)
	ON_COMMAND(ID_IMAGE_MIRROR, &CFalconEyesDoc::OnImageMirror)
	ON_COMMAND(ID_IMAGE_FLIP, &CFalconEyesDoc::OnImageFlip)
	ON_COMMAND(ID_HISTOGRAM, &CFalconEyesDoc::OnHistogram)
	ON_COMMAND(ID_FILTER_MEAN, &CFalconEyesDoc::OnFilterMean)
	ON_COMMAND(ID_FILTER_MID_MEAN, &CFalconEyesDoc::OnFilterMidMean)
	ON_COMMAND(ID_FILTER_GAUSSIAN, &CFalconEyesDoc::OnFilterGaussian)
	ON_COMMAND(ID_FILTER_LAPLACIAN, &CFalconEyesDoc::OnFilterLaplacian)
	ON_COMMAND(ID_FILTER_UNSHARPMASK, &CFalconEyesDoc::OnFilterUnsharpmask)
	ON_COMMAND(ID_FILTER_HIGHBOOST, &CFalconEyesDoc::OnFilterHighboost)
	ON_COMMAND(ID_ADD_NOISE, &CFalconEyesDoc::OnAddNoise)
	ON_COMMAND(ID_FILTER_MEDIAN, &CFalconEyesDoc::OnFilterMedian)
	ON_COMMAND(ID_FILTER_DIFFUSION, &CFalconEyesDoc::OnFilterDiffusion)
	ON_COMMAND(ID_FILTER_DFT, &CFalconEyesDoc::OnFilterDft)
	ON_UPDATE_COMMAND_UI(ID_BITPLANE_SLICING, &CFalconEyesDoc::OnUpdateBitplaneSlicing)
	ON_UPDATE_COMMAND_UI(ID_HISTOGRAM, &CFalconEyesDoc::OnUpdateHistogram)
	ON_UPDATE_COMMAND_UI(ID_IMAGE_TRANSLATE, &CFalconEyesDoc::OnUpdateImageTranslate)
	ON_UPDATE_COMMAND_UI(ID_IMAGE_RESIZE, &CFalconEyesDoc::OnUpdateImageResize)
	ON_UPDATE_COMMAND_UI(ID_IMAGE_ROTATE, &CFalconEyesDoc::OnUpdateImageRotate)
	ON_UPDATE_COMMAND_UI(ID_IMAGE_MIRROR, &CFalconEyesDoc::OnUpdateImageMirror)
	ON_UPDATE_COMMAND_UI(ID_IMAGE_FLIP, &CFalconEyesDoc::OnUpdateImageFlip)
	ON_UPDATE_COMMAND_UI(ID_FILTER_MEAN, &CFalconEyesDoc::OnUpdateFilterMean)
	ON_UPDATE_COMMAND_UI(ID_FILTER_MID_MEAN, &CFalconEyesDoc::OnUpdateFilterMidMean)
	ON_UPDATE_COMMAND_UI(ID_FILTER_GAUSSIAN, &CFalconEyesDoc::OnUpdateFilterGaussian)
	ON_UPDATE_COMMAND_UI(ID_FILTER_LAPLACIAN, &CFalconEyesDoc::OnUpdateFilterLaplacian)
	ON_UPDATE_COMMAND_UI(ID_FILTER_UNSHARPMASK, &CFalconEyesDoc::OnUpdateFilterUnsharpmask)
	ON_UPDATE_COMMAND_UI(ID_FILTER_HIGHBOOST, &CFalconEyesDoc::OnUpdateFilterHighboost)
	ON_UPDATE_COMMAND_UI(ID_ADD_NOISE, &CFalconEyesDoc::OnUpdateAddNoise)
	ON_UPDATE_COMMAND_UI(ID_FILTER_MEDIAN, &CFalconEyesDoc::OnUpdateFilterMedian)
	ON_UPDATE_COMMAND_UI(ID_FILTER_DIFFUSION, &CFalconEyesDoc::OnUpdateFilterDiffusion)
	ON_UPDATE_COMMAND_UI(ID_SEGMENT_BINARIZATION, &CFalconEyesDoc::OnUpdateSegmentBinarization)
	ON_UPDATE_COMMAND_UI(ID_SEGMENT_LABELING, &CFalconEyesDoc::OnUpdateSegmentLabeling)
	ON_UPDATE_COMMAND_UI(ID_CONTOUR_TRACING, &CFalconEyesDoc::OnUpdateContourTracing)
	ON_UPDATE_COMMAND_UI(ID_MORPHOLOGY_EROSION, &CFalconEyesDoc::OnUpdateMorphologyErosion)
	ON_UPDATE_COMMAND_UI(ID_MORPHOLOGY_DILATION, &CFalconEyesDoc::OnUpdateMorphologyDilation)
	ON_UPDATE_COMMAND_UI(ID_MORPHOLOGY_OPENING, &CFalconEyesDoc::OnUpdateMorphologyOpening)
	ON_UPDATE_COMMAND_UI(ID_MORPHOLOGY_CLOSING, &CFalconEyesDoc::OnUpdateMorphologyClosing)
	ON_UPDATE_COMMAND_UI(ID_GRAYMORPH_EROSION, &CFalconEyesDoc::OnUpdateGraymorphErosion)
	ON_UPDATE_COMMAND_UI(ID_GRAYMORPH_DILATION, &CFalconEyesDoc::OnUpdateGraymorphDilation)
	ON_UPDATE_COMMAND_UI(ID_GRAYMORPH_OPENING, &CFalconEyesDoc::OnUpdateGraymorphOpening)
	ON_UPDATE_COMMAND_UI(ID_GRAYMORPH_CLOSING, &CFalconEyesDoc::OnUpdateGraymorphClosing)
	ON_UPDATE_COMMAND_UI(ID_FOURIER_DESCRIPTOR, &CFalconEyesDoc::OnUpdateFourierDescriptor)
	ON_UPDATE_COMMAND_UI(ID_INVARIANT_MOMENTS, &CFalconEyesDoc::OnUpdateInvariantMoments)
	ON_UPDATE_COMMAND_UI(ID_ZERNIKE_MOMENTS, &CFalconEyesDoc::OnUpdateZernikeMoments)
	ON_UPDATE_COMMAND_UI(ID_TEMPLATE_MATCHING, &CFalconEyesDoc::OnUpdateTemplateMatching)
	ON_UPDATE_COMMAND_UI(ID_FOURIER_DFT, &CFalconEyesDoc::OnUpdateFourierDft)
	ON_UPDATE_COMMAND_UI(ID_FOURIER_DFTRC, &CFalconEyesDoc::OnUpdateFourierDftrc)
	ON_UPDATE_COMMAND_UI(ID_FOURIER_FFT, &CFalconEyesDoc::OnUpdateFourierFft)
	ON_UPDATE_COMMAND_UI(ID_EDGE_ROBERTS, &CFalconEyesDoc::OnUpdateEdgeRoberts)
	ON_UPDATE_COMMAND_UI(ID_EDGE_PREWITT, &CFalconEyesDoc::OnUpdateEdgePrewitt)
	ON_UPDATE_COMMAND_UI(ID_EDGE_SOBEL, &CFalconEyesDoc::OnUpdateEdgeSobel)
	ON_UPDATE_COMMAND_UI(ID_EDGE_CANNY, &CFalconEyesDoc::OnUpdateEdgeCanny)
	ON_UPDATE_COMMAND_UI(ID_HOUGH_LINE, &CFalconEyesDoc::OnUpdateHoughLine)
	ON_UPDATE_COMMAND_UI(ID_HARRIS_CORNER, &CFalconEyesDoc::OnUpdateHarrisCorner)
	ON_COMMAND(ID_FOURIER_DFT, &CFalconEyesDoc::OnFourierDft)
	ON_COMMAND(ID_COLOR_GRAYSCALE, &CFalconEyesDoc::OnColorGrayscale)
	ON_UPDATE_COMMAND_UI(ID_COLOR_GRAYSCALE, &CFalconEyesDoc::OnUpdateColorGrayscale)
	ON_COMMAND(ID_EDGE_ROBERTS, &CFalconEyesDoc::OnEdgeRoberts)
	ON_COMMAND(ID_EDGE_PREWITT, &CFalconEyesDoc::OnEdgePrewitt)
	ON_COMMAND(ID_EDGE_SOBEL, &CFalconEyesDoc::OnEdgeSobel)
	ON_COMMAND(ID_EDGE_CANNY, &CFalconEyesDoc::OnEdgeCanny)
	ON_COMMAND(ID_SEGMENT_BINARIZATION, &CFalconEyesDoc::OnSegmentBinarization)
	ON_COMMAND(ID_SEGMENT_LABELING, &CFalconEyesDoc::OnSegmentLabeling)
	ON_COMMAND(ID_CONTOUR_TRACING, &CFalconEyesDoc::OnContourTracing)
	ON_COMMAND(ID_HOUGH_LINE, &CFalconEyesDoc::OnHoughLine)
	ON_COMMAND(ID_FILTER_GAUSSIAN_CV, &CFalconEyesDoc::OnFilterGaussianCv)
	ON_UPDATE_COMMAND_UI(ID_FILTER_GAUSSIAN_CV, &CFalconEyesDoc::OnUpdateFilterGaussianCv)
	ON_COMMAND(ID_HARRIS_CORNER, &CFalconEyesDoc::OnHarrisCorner)
	ON_COMMAND(ID_WINDOW_ROI, &CFalconEyesDoc::OnWindowRoi)
	ON_COMMAND(ID_COLOR_GRAY_YUV, &CFalconEyesDoc::OnColorGrayYuv)
	ON_UPDATE_COMMAND_UI(ID_COLOR_GRAY_CV, &CFalconEyesDoc::OnUpdateColorGrayCv)
	ON_COMMAND(ID_COLOR_GRAY_CV, &CFalconEyesDoc::OnColorGrayCv)
	ON_UPDATE_COMMAND_UI(ID_COLOR_GRAY_YUV, &CFalconEyesDoc::OnUpdateColorGrayYuv)
	ON_COMMAND(ID_COLOR_SPLIT_RGB, &CFalconEyesDoc::OnColorSplitRgb)
	ON_COMMAND(ID_COLOR_SPLIT_HSI, &CFalconEyesDoc::OnColorSplitHsi)
	ON_COMMAND(ID_COLOR_SPLIT_YUV, &CFalconEyesDoc::OnColorSplitYuv)
	ON_UPDATE_COMMAND_UI(ID_COLOR_SPLIT_RGB, &CFalconEyesDoc::OnUpdateColorSplitRgb)
	ON_UPDATE_COMMAND_UI(ID_COLOR_SPLIT_HSI, &CFalconEyesDoc::OnUpdateColorSplitHsi)
	ON_UPDATE_COMMAND_UI(ID_COLOR_SPLIT_YUV, &CFalconEyesDoc::OnUpdateColorSplitYuv)
	ON_COMMAND(ID_COLOR_COMBINE_RGB, &CFalconEyesDoc::OnColorCombineRgb)
	ON_COMMAND(ID_COLOR_COMBINE_HSI, &CFalconEyesDoc::OnColorCombineHsi)
	ON_COMMAND(ID_COLOR_COMBINE_YUV, &CFalconEyesDoc::OnColorCombineYuv)
	ON_COMMAND(ID_COLOR_EDGE, &CFalconEyesDoc::OnColorEdge)
	ON_UPDATE_COMMAND_UI(ID_COLOR_EDGE, &CFalconEyesDoc::OnUpdateColorEdge)
	ON_COMMAND(ID_MORPHOLOGY_EROSION, &CFalconEyesDoc::OnMorphologyErosion)
	ON_COMMAND(ID_MORPHOLOGY_DILATION, &CFalconEyesDoc::OnMorphologyDilation)
	ON_COMMAND(ID_MORPHOLOGY_OPENING, &CFalconEyesDoc::OnMorphologyOpening)
	ON_COMMAND(ID_MORPHOLOGY_CLOSING, &CFalconEyesDoc::OnMorphologyClosing)
	ON_COMMAND(ID_GRAYMORPH_EROSION, &CFalconEyesDoc::OnGraymorphErosion)
	ON_COMMAND(ID_GRAYMORPH_DILATION, &CFalconEyesDoc::OnGraymorphDilation)
	ON_COMMAND(ID_GRAYMORPH_OPENING, &CFalconEyesDoc::OnGraymorphOpening)
	ON_COMMAND(ID_GRAYMORPH_CLOSING, &CFalconEyesDoc::OnGraymorphClosing)

	ON_COMMAND(ID_SEGMENT_BINARIZATION_OTSU, &CFalconEyesDoc::OnSegmentBinarizationOtsu)
	ON_UPDATE_COMMAND_UI(ID_SEGMENT_BINARIZATION_OTSU, &CFalconEyesDoc::OnUpdateSegmentBinarizationOtsu)
	ON_COMMAND(ID_SEGMENT_BINARIZATION_ADAPTIVE, &CFalconEyesDoc::OnSegmentBinarizationAdaptive)
	ON_UPDATE_COMMAND_UI(ID_SEGMENT_BINARIZATION_ADAPTIVE, &CFalconEyesDoc::OnUpdateSegmentBinarizationAdaptive)
	ON_COMMAND(ID_FOURIER_FFT_CV, &CFalconEyesDoc::OnFourierFftCv)
	ON_UPDATE_COMMAND_UI(ID_FOURIER_FFT_CV, &CFalconEyesDoc::OnUpdateFourierFftCv)
	ON_COMMAND(ID_FOURIER_IFFT_CV, &CFalconEyesDoc::OnFourierIfftCv)
	ON_UPDATE_COMMAND_UI(ID_FOURIER_IFFT_CV, &CFalconEyesDoc::OnUpdateFourierIfftCv)
	ON_COMMAND(ID_FOURIER_DESCRIPTOR, &CFalconEyesDoc::OnFourierDescriptor)
	ON_COMMAND(ID_INVARIANT_MOMENTS, &CFalconEyesDoc::OnInvariantMoments)
	ON_COMMAND(ID_ZERNIKE_MOMENTS, &CFalconEyesDoc::OnZernikeMoments)
	ON_COMMAND(ID_TEMPLATE_MATCHING, &CFalconEyesDoc::OnTemplateMatching)
	ON_COMMAND(ID_EDIT_UNDO, &CFalconEyesDoc::OnEditUndo)
	ON_COMMAND(ID_EDIT_REDO, &CFalconEyesDoc::OnEditRedo)
	ON_COMMAND(ID_EDIT_ORIGIN, &CFalconEyesDoc::OnEditOrigin)
	ON_UPDATE_COMMAND_UI(ID_EDIT_ORIGIN, &CFalconEyesDoc::OnUpdateEditOrigin)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, &CFalconEyesDoc::OnUpdateEditRedo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, &CFalconEyesDoc::OnUpdateEditUndo)
	ON_COMMAND(ID_TEMPLATE_MATCHING_CV, &CFalconEyesDoc::OnTemplateMatchingCv)
END_MESSAGE_MAP()


// CFalconEyesDoc 생성/소멸

CFalconEyesDoc::CFalconEyesDoc() noexcept
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.
}

CFalconEyesDoc::~CFalconEyesDoc()
{
}

BOOL CFalconEyesDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	BOOL ret = TRUE;
	if (theApp.m_pNewDib == NULL)
	{
		CFileNewDlg dlg;
		if (dlg.DoModal() == IDOK)
		{
			if (dlg.m_nType == 0)
				ret = m_Dib.CreateGrayImage(dlg.m_nWidth, dlg.m_nHeight);
			else
				ret = m_Dib.CreateRgbImage(dlg.m_nWidth, dlg.m_nHeight);
		}
		else
		{
			ret = FALSE;
		}
	}
	else
	{
		m_Dib = *(theApp.m_pNewDib);
		theApp.m_pNewDib = NULL;
	}
	m_nBufferIndex = 0;
	m_nBufferCnt = 1;
	m_matBuffer[m_nBufferIndex] = m_Dib.GetMatImg();
	
	// SDI 문서는 이 문서를 다시 사용합니다.

	return ret;
}

// CFalconEyesDoc serialization
void CFalconEyesDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
	}
}

#ifdef SHARED_HANDLERS

// 축소판 그림을 지원합니다.
void CFalconEyesDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 문서의 데이터를 그리려면 이 코드를 수정하십시오.
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 검색 처리기를 지원합니다.
void CFalconEyesDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 예: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CFalconEyesDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CFalconEyesDoc 진단

#ifdef _DEBUG
void CFalconEyesDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CFalconEyesDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CFalconEyesDoc 명령

BOOL CFalconEyesDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	BOOL res = m_Dib.Load(CT2A(lpszPathName));
	if (res)
	{
		AfxPrintInfo(TEXT("[파일 열기] 파일 경로: %s, 가로: %d px, 세로: %d, Color: %d"), lpszPathName, m_Dib.GetWidth(), m_Dib.GetHeight(), 0x01 << m_Dib.GetBitCount());
		m_nBufferIndex = 0;
		m_nBufferCnt = 1;
		m_matBuffer[m_nBufferIndex] = m_Dib.GetMatImg();
	}
	return res;
}


BOOL CFalconEyesDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return m_Dib.Save(CT2A(lpszPathName));
}


void CFalconEyesDoc::OnWindowDuplicate()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	AfxNewImage(m_Dib);
}


void CFalconEyesDoc::OnEditCopy()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (m_Dib.IsValid())
		m_Dib.CopyToClipboard();
}


void CFalconEyesDoc::OnImageInverse()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	
	MatDib dib;
	dib = m_Dib;
	mc_Timer.Reset();
	dib.SetMatImg(~dib.GetMatImg());
	//DibInverse(dib);
	AfxPrintInfo(TEXT("[반전] 입력 영상 : %s (%d ms)"), GetTitle(), mc_Timer.Elapse());
	AfxNewImage(dib);
	UpdateAllViews(NULL);
}


void CFalconEyesDoc::OnImageBrightnesscontrast()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CBrightnessConstrastDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		MatDib dib;
		dib = m_Dib;
		mc_Timer.Reset();
		DibBrightness(dib, dlg.m_nBrightness);
		DibConstrast(dib, dlg.m_nContrast);
		AfxPrintInfo(TEXT("[밝기/명암 조정] 입력 영상 : %s, 밝기: %d, 명암: %d (%d ms)"), GetTitle(), dlg.m_nBrightness, dlg.m_nContrast, mc_Timer.Elapse());
		AfxNewImage(dib);
		UpdateAllViews(NULL);
	}
}


void CFalconEyesDoc::OnImageGammacorrection()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CGammaCorrectionDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		MatDib dib;
		dib = m_Dib;
		mc_Timer.Reset();
		DibGammaCorrection(dib, dlg.m_fGamma);
		AfxPrintInfo(TEXT("[감마 보정] 입력 영상 : %s, 감마 : %4.2f (%d ms)"), GetTitle(), dlg.m_fGamma, mc_Timer.Elapse());
		AfxNewImage(dib);
		UpdateAllViews(NULL);
	}
}


void CFalconEyesDoc::OnHistogramStretching()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	MatDib dib;
	dib = m_Dib;
	mc_Timer.Reset();
	DibHistogramStretching(dib);
	AfxPrintInfo(TEXT("[히스토그램 스트래칭] 입력 영상 : %s (%d ms)"), GetTitle(), mc_Timer.Elapse());
	AfxNewImage(dib);
	UpdateAllViews(NULL);
}


void CFalconEyesDoc::OnHistogramEqualization()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	MatDib dib;
	
	dib = m_Dib;
	if (dib.GetBitCount() == 8)
	{
		mc_Timer.Reset();
		DibHistogramEqualization(dib);
		AfxPrintInfo(TEXT("[히스토그램 균등화] 입력 영상 : %s (%d ms)"), GetTitle(), mc_Timer.Elapse());
		AfxNewImage(dib);
		UpdateAllViews(NULL);
	}
	else if(dib.GetBitCount() == 24)
	{
		Mat matSrc, matY, matU, matV, matDst;
		mc_Timer.Reset();
		matSrc = dib.GetMatImg();
		MatColorSplitYUV(matSrc, matY, matU, matV);
		dib.SetMatImg(matY);
		DibHistogramEqualization(dib);
		matY = dib.GetMatImg();
		MatColorCombineYUV(matY, matU, matV, matDst);
		dib.SetMatImg(matDst);
		AfxPrintInfo(TEXT("[히스토그램 균등화] 입력 영상 : %s (%d ms)"), GetTitle(), mc_Timer.Elapse());
		AfxNewImage(dib);
		UpdateAllViews(NULL);
	}
}


void CFalconEyesDoc::OnImageArithmeticlogical()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CArithmeticLogicalDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		CFalconEyesDoc* pDoc1 = (CFalconEyesDoc*)dlg.m_pDoc1;
		CFalconEyesDoc* pDoc2 = (CFalconEyesDoc*)dlg.m_pDoc2;

		MatDib dib;
		dib = m_Dib;
		BOOL ret = FALSE;

		mc_Timer.Reset();
		switch (dlg.m_nFunc)
		{
		case 0: ret = DibAdd(dib, pDoc1->m_Dib, pDoc2->m_Dib); break;
		case 1: ret = DibSub(dib, pDoc1->m_Dib, pDoc2->m_Dib); break;
		case 2: ret = DibAvg(dib, pDoc1->m_Dib, pDoc2->m_Dib); break;
		case 3: ret = DibDiff(dib, pDoc1->m_Dib, pDoc2->m_Dib); break;
		case 4: ret = DibAnd(dib, pDoc1->m_Dib, pDoc2->m_Dib); break;
		case 5: ret = DibOR(dib, pDoc1->m_Dib, pDoc2->m_Dib); break;
		}

		if (ret)
		{
			TCHAR* op[] = { TEXT("덧셈"), TEXT("뺄셈"), TEXT("평균"), TEXT("차이"), TEXT("논리 곱"), TEXT("논리 합") };
			AfxPrintInfo(TEXT("[산술/논리] [%s] 입력 영상 #1: %s, 입력 영상 #2: %s (%d ms)"), op[dlg.m_nFunc], pDoc1->GetTitle(), pDoc2->GetTitle(), mc_Timer.Elapse());
			AfxNewImage(dib);
		}
		else
			AfxMessageBox(TEXT("Not Matched Image Size"));
	}
}


void CFalconEyesDoc::OnBitplaneSlicing()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	MatDib dib;
	dib = m_Dib;
	mc_Timer.Reset();
	for (int i = 0; i < 8; i++)
	{
		DibBitPlane(m_Dib, dib, i);
		AfxNewImage(dib);
	}

	AfxPrintInfo(TEXT("[비트 평면 분할] 입력 영상 : %s (%d ms)"), GetTitle(), mc_Timer.Elapse());
}


void CFalconEyesDoc::OnImageTranslate()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CTranslateDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		MatDib dib;
		dib = m_Dib;
		mc_Timer.Reset();
		DibTranslate(m_Dib, dib, dlg.m_nNewSX, dlg.m_nNewSY);
		AfxPrintInfo(TEXT("[이동 변환] 입력 영상 : %s, 가로 이동: %d, 세로 이동: %d (%d ms)"), GetTitle(), dlg.m_nNewSX, dlg.m_nNewSY, mc_Timer.Elapse());
		AfxNewImage(dib);
		UpdateAllViews(NULL);
	}
}


void CFalconEyesDoc::OnImageResize()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CResizeDlg dlg;
	dlg.m_nOldWidth = m_Dib.GetWidth();
	dlg.m_nOldHeight = m_Dib.GetHeight();
	if (dlg.DoModal() == IDOK)
	{
		MatDib dib;
		dib = m_Dib;
		mc_Timer.Reset();
		switch (dlg.m_nInterpolation)
		{
		case 0:
			DibResizeNearest(m_Dib, dib, dlg.m_nNewWidth, dlg.m_nNewHeight);
			break;
		case 1:
			DibResizeBilinear(m_Dib, dib, dlg.m_nNewWidth, dlg.m_nNewHeight);
			break;
		case 2:
			DibResizeCubic(m_Dib, dib, dlg.m_nNewWidth, dlg.m_nNewHeight);
			break;
		default:
			break;
		}
		TCHAR* Noise[] = { TEXT("최근방 이웃 보간법"), TEXT("양선형 보간법"), TEXT("3차 회선 보간법") };
		AfxPrintInfo(TEXT("[크기 변환] 입력 영상 : %s, 새 가로 크기: %d, 새 세로 크기: %d, 보간법: %s (%d ms)"), GetTitle(), dlg.m_nNewWidth, dlg.m_nNewHeight, Noise[dlg.m_nInterpolation], mc_Timer.Elapse());
		AfxNewImage(dib);
		UpdateAllViews(NULL);
	}
}


void CFalconEyesDoc::OnImageRotate()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CRotateDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		MatDib dib;
		dib = m_Dib;
		mc_Timer.Reset();
		switch (dlg.m_nRotate)
		{
		case 0:
			DibRotate90(m_Dib, dib);
			break;
		case 1:
			DibRotate180(m_Dib, dib);
			break;
		case 2:
			DibRotate270(m_Dib, dib);
			break;
		case 3:
			DibRotate(m_Dib, dib, (double)dlg.m_fAngle);
			break;
		default:
			break;
		}
		TCHAR* Rotate[] = { TEXT("90도"), TEXT("180도"), TEXT("270도") };
		if (dlg.m_nRotate != 3)
			AfxPrintInfo(TEXT("[회전 변환] 입력 영상: %s, 회전 각도: %s (%d ms)"), GetTitle(), Rotate[dlg.m_nRotate], mc_Timer.Elapse());
		else
			AfxPrintInfo(TEXT("[회전 변환] 입력 영상: %s, 회전 각도: %4.2f도 (%d ms)"), GetTitle(), dlg.m_fAngle, mc_Timer.Elapse());

		AfxNewImage(dib);
		UpdateAllViews(NULL);
	}
}


void CFalconEyesDoc::OnImageMirror()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	MatDib dib;
	dib = m_Dib;
	mc_Timer.Reset();
	DibMatMirror(m_Dib, dib);
	AfxPrintInfo(TEXT("[좌우 대칭] 입력 영상 : %s (%d ms)"), GetTitle(), mc_Timer.Elapse());
	AfxNewImage(dib);
	UpdateAllViews(NULL);
}


void CFalconEyesDoc::OnImageFlip()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	MatDib dib;
	dib = m_Dib;
	mc_Timer.Reset();
	DibMatFlip(m_Dib, dib);
	AfxPrintInfo(TEXT("[상하 대칭] 입력 영상 : %s (%d ms)"), GetTitle(), mc_Timer.Elapse());
	AfxNewImage(dib);
	UpdateAllViews(NULL);
}


void CFalconEyesDoc::OnHistogram()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CHistogramDlg dlg;
	mc_Timer.Reset();
	dlg.SetImage(&m_Dib);
	dlg.DoModal();
	AfxPrintInfo(TEXT("[히스토그램] 입력 영상 : %s (%d ms)"), GetTitle(), mc_Timer.Elapse());
}


void CFalconEyesDoc::OnFilterMean()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	MatDib dib;
	dib = m_Dib;
	mc_Timer.Reset();
	DibFilterMean(m_Dib, dib, 3);
	AfxPrintInfo(TEXT("[평균 값 필터] 입력 영상 : %s (%d ms)"), GetTitle(), mc_Timer.Elapse());
	AfxNewImage(dib);
	UpdateAllViews(NULL);
}


void CFalconEyesDoc::OnFilterMidMean()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	MatDib dib;
	dib = m_Dib;
	mc_Timer.Reset();
	DibFilterWeightMean(m_Dib, dib);
	AfxPrintInfo(TEXT("[가중 평균 필터] 입력 영상 : %s (%d ms)"), GetTitle(), mc_Timer.Elapse());
	AfxNewImage(dib);
	UpdateAllViews(NULL);
}


void CFalconEyesDoc::OnFilterGaussian()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CGaussianDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		MatDib dib;
		dib = m_Dib;
		int nW = dib.GetWidth();
		int nH = dib.GetHeight();

		Mat matSrc = Mat::zeros(Size(nW, nH), CV_32FC1);
		float* fTmpData = (float*)matSrc.data;
		float** fData = new float*[nH];
		for (int i = 0; i < nH; i++)
		{
			fData[i] = &fTmpData[i*nW];
		}
		mc_Timer.Reset();
		DibFilterGaussian(dib, fData, dlg.m_fSigma);
		delete[] fData;
		matSrc.convertTo(matSrc, CV_8UC1);
		dib.SetMatImg(matSrc);

		AfxPrintInfo(TEXT("[가우시안 필터] 입력 영상 : %s, Sigma : %d (%d ms)"), GetTitle(), dlg.m_fSigma, mc_Timer);
		AfxNewImage(dib);
		UpdateAllViews(NULL);
	}
}

void CFalconEyesDoc::OnUpdateFilterGaussian(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}

void CFalconEyesDoc::OnFilterGaussianCv()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CGaussianDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		MatDib dib;
		dib = m_Dib;
		mc_Timer.Reset();
		DibFilterGaussian_CV(m_Dib, dib, dlg.m_fSigma);
		AfxPrintInfo(TEXT("[가우시안CV 필터] 입력 영상 : %s, Sigma : %d (%d ms)"), GetTitle(), dlg.m_fSigma, mc_Timer.Elapse());
		AfxNewImage(dib);
		UpdateAllViews(NULL);
	}
}

void CFalconEyesDoc::OnUpdateFilterGaussianCv(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CFalconEyesDoc::OnFilterLaplacian()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	MatDib dib;
	dib = m_Dib;
	mc_Timer.Reset();
	DibFilterLaplacian(m_Dib, dib);
	AfxPrintInfo(TEXT("[라플라시안 필터] 입력 영상 : %s (%d ms)"), GetTitle(), mc_Timer.Elapse());
	AfxNewImage(dib);
	UpdateAllViews(NULL);
}


void CFalconEyesDoc::OnFilterUnsharpmask()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	MatDib dib;
	dib = m_Dib;
	mc_Timer.Reset();
	DibFilterUnsharpMask(m_Dib, dib);
	AfxPrintInfo(TEXT("[언샤프 마스크 필터] 입력 영상 : %s (%d ms)"), GetTitle(), mc_Timer.Elapse());
	AfxNewImage(dib);
	UpdateAllViews(NULL);
}


void CFalconEyesDoc::OnFilterHighboost()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	MatDib dib;
	dib = m_Dib;
	float alpha = 1.2f;
	mc_Timer.Reset();
	DibFilterHighboost(m_Dib, dib, alpha);
	AfxPrintInfo(TEXT("[하이부스트 필터] 입력 영상 : %s (%d ms)"), GetTitle(), mc_Timer.Elapse());
	AfxNewImage(dib);
	UpdateAllViews(NULL);
}


void CFalconEyesDoc::OnAddNoise()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CAddNoiseDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		MatDib dib;
		dib = m_Dib;
		if (dlg.m_nNoiseType == 0)
		{
			mc_Timer.Reset();
			DibNoiseGaussian(m_Dib, dib, dlg.m_nAmount);
		}
		else
		{
			mc_Timer.Reset();
			DibNoiseSaltPepper(m_Dib, dib, dlg.m_nAmount);
		}
		TCHAR* Noise[] = { TEXT("가우시안"), TEXT("소금&후추") };
		AfxPrintInfo(TEXT("[잡음 추가] 입력 영상 : %s, 잡음 종류: %s, 잡음 양: %d (%d ms)"), GetTitle(), Noise[dlg.m_nNoiseType], dlg.m_nAmount, mc_Timer.Elapse());
		AfxNewImage(dib);
		UpdateAllViews(NULL);
	}
}


void CFalconEyesDoc::OnFilterMedian()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	MatDib dib;
	dib = m_Dib;
	mc_Timer.Reset();
	DibFilterMedianMean(m_Dib, dib, 3);
	AfxPrintInfo(TEXT("[미디언 필터] 입력 영상 : %s (%d ms)"), GetTitle(), mc_Timer.Elapse());
	AfxNewImage(dib);
	UpdateAllViews(NULL);
}


void CFalconEyesDoc::OnFilterDiffusion()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CDiffusionDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		MatDib dib;
		dib = m_Dib;
		mc_Timer.Reset();
		DibFilterDiffusion(m_Dib, dib, dlg.m_fLambda, dlg.m_fK, static_cast<int>(dlg.m_nIteration));
		AfxPrintInfo(TEXT("[비등방성 확산 필터] 입력 영상 : %s, Lambda: %4.2f, K: %4.2f, 반복 횟수: %d (%d ms)"), GetTitle(), dlg.m_fLambda, dlg.m_fK, dlg.m_nIteration, mc_Timer.Elapse());
		AfxNewImage(dib);
		UpdateAllViews(NULL);
	}
}

void CFalconEyesDoc::OnFilterDft()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	MatDib dib;
	dib = m_Dib;
	mc_Timer.Reset();
	DibMatDFT(m_Dib, dib);
	AfxPrintInfo(TEXT("[이산 푸리에 변환] 입력 영상 : %s (%d ms)"), GetTitle(), mc_Timer.Elapse());
	AfxNewImage(dib);
	UpdateAllViews(NULL);
}

void CFalconEyesDoc::OnUpdateBitplaneSlicing(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}

void CFalconEyesDoc::OnUpdateHistogram(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}

void CFalconEyesDoc::OnUpdateImageTranslate(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}

void CFalconEyesDoc::OnUpdateImageResize(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}

void CFalconEyesDoc::OnUpdateImageRotate(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}

void CFalconEyesDoc::OnUpdateImageMirror(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}

void CFalconEyesDoc::OnUpdateImageFlip(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}

void CFalconEyesDoc::OnUpdateFilterMean(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}

void CFalconEyesDoc::OnUpdateFilterMidMean(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}

void CFalconEyesDoc::OnUpdateFilterLaplacian(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}

void CFalconEyesDoc::OnUpdateFilterUnsharpmask(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}

void CFalconEyesDoc::OnUpdateFilterHighboost(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}

void CFalconEyesDoc::OnUpdateAddNoise(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}

void CFalconEyesDoc::OnUpdateFilterMedian(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}

void CFalconEyesDoc::OnUpdateFilterDiffusion(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}

void CFalconEyesDoc::OnUpdateSegmentBinarization(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}

void CFalconEyesDoc::OnUpdateSegmentLabeling(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}

void CFalconEyesDoc::OnUpdateContourTracing(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}

void CFalconEyesDoc::OnUpdateMorphologyErosion(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}

void CFalconEyesDoc::OnUpdateMorphologyDilation(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}

void CFalconEyesDoc::OnUpdateMorphologyOpening(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}

void CFalconEyesDoc::OnUpdateMorphologyClosing(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}

void CFalconEyesDoc::OnUpdateGraymorphErosion(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}

void CFalconEyesDoc::OnUpdateGraymorphDilation(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}

void CFalconEyesDoc::OnUpdateGraymorphOpening(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}

void CFalconEyesDoc::OnUpdateGraymorphClosing(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}

void CFalconEyesDoc::OnUpdateFourierDescriptor(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}

void CFalconEyesDoc::OnUpdateInvariantMoments(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}

void CFalconEyesDoc::OnUpdateZernikeMoments(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}

void CFalconEyesDoc::OnUpdateTemplateMatching(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}

void CFalconEyesDoc::OnUpdateFourierDft(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}

void CFalconEyesDoc::OnUpdateFourierDftrc(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}

void CFalconEyesDoc::OnUpdateFourierFft(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}

void CFalconEyesDoc::OnUpdateEdgeRoberts(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}

void CFalconEyesDoc::OnUpdateEdgePrewitt(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}

void CFalconEyesDoc::OnUpdateEdgeSobel(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}

void CFalconEyesDoc::OnUpdateEdgeCanny(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}

void CFalconEyesDoc::OnUpdateHoughLine(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}

void CFalconEyesDoc::OnUpdateHarrisCorner(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}

void CFalconEyesDoc::OnFourierDft()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
 	int nW = m_Dib.GetWidth();
 	int nH = m_Dib.GetHeight();
 
 	if (nW * nH > 128 * 128)
 	{
 		CString msg = _T("영상의 크기가 커서 시간이 오래 걸릴 수 있습니다. \n 계속 하시겠습니까?");
 		if (AfxMessageBox(msg, MB_OKCANCEL) != IDOK)
 			return;
 	}
 
 	CWaitCursor wait;
 	MatDib matOut;
 	MatDibFourier fourier;
 	Mat matTemp;
 	matTemp = m_Dib.GetMatImg();
 	fourier.m_fnSetImage(matTemp);
 
 	DWORD t1 = timeGetTime();
 	fourier.m_fnDFT(1);
 
 	Mat matImgSpec;
	mc_Timer.Reset();
 	fourier.m_fnGetSpectrumImage(matImgSpec);
	AfxPrintInfo(_T("[푸리에변환/DFT/Spectrum] 입력 영상: %s (%d ms)"), GetTitle(), mc_Timer.Elapse());
 	matImgSpec.convertTo(matImgSpec, CV_8UC1);
 	matOut = m_Dib;
 	matOut.SetMatImg(matImgSpec);
 	AfxNewImage(matOut);
 
 	Mat matImgPhase;
	mc_Timer.Reset();
 	fourier.m_fnGetPhaseImage(matImgPhase);
	AfxPrintInfo(_T("[푸리에변환/DFT/Phase] 입력 영상: %s (%d ms)"), GetTitle(), mc_Timer.Elapse());
 	matImgPhase.convertTo(matImgPhase, CV_8UC1);
 	matOut = m_Dib;
 	matOut.SetMatImg(matImgPhase);
 	AfxNewImage(matOut);
 
 	fourier.m_fnDFT(-1);
 
 	DWORD t2 = timeGetTime();
 
 	Mat matImage;
 	fourier.m_fnGetImage(matImage);
 	matImage.convertTo(matImage, CV_8UC1);
 	matOut = m_Dib;
	matOut.SetMatImg(matImage);
	/*MatDib dib;
	dib = m_Dib;
	dib.MatDFT(dib);
	AfxPrintInfo(_T("[푸리에변환/DFT] 입력 영상: %s"), GetTitle());
	AfxNewImage(dib);*/
}

void CFalconEyesDoc::OnColorGrayscale()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	MatDib dib;
	dib = m_Dib;
	mc_Timer.Reset();
	DibToGray(m_Dib, dib);
	//dib.ToGrayCV(dib);
	AfxPrintInfo(TEXT("[그레이 변환] 입력 영상: %s, 가로: %d px, 세로: %d, Color: %d (%d ms)"), GetTitle(), dib.GetWidth(), dib.GetHeight(), dib.GetBitCount() << 0x01, mc_Timer.Elapse());
	AfxNewImage(dib);
}

void CFalconEyesDoc::OnUpdateColorGrayscale(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_Dib.GetBitCount() != 8);
}

void CFalconEyesDoc::OnEdgeRoberts()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	MatDib dib;
	dib = m_Dib;
	mc_Timer.Reset();
	DibEdgeRoberts(m_Dib, dib);
	AfxPrintInfo(TEXT("[마스크 기반 엣지 검출/로버츠] 입력 영상: %s (%d ms)"), GetTitle(), mc_Timer.Elapse());
	AfxNewImage(dib);
}

void CFalconEyesDoc::OnEdgePrewitt()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	MatDib dib;
	dib = m_Dib;
	mc_Timer.Reset();
	DibEdgeRoberts(m_Dib, dib);
	AfxPrintInfo(TEXT("[마스크 기반 엣지 검출/프리윗] 입력 영상: %s (%d ms)"), GetTitle(), mc_Timer.Elapse());
	AfxNewImage(dib);
}

void CFalconEyesDoc::OnEdgeSobel()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	MatDib dib;
	dib = m_Dib;
	mc_Timer.Reset();
	DibEdgeRoberts(m_Dib, dib);
	AfxPrintInfo(TEXT("[마스크 기반 엣지 검출/소벨] 입력 영상: %s (%d ms)"), GetTitle(), mc_Timer.Elapse());
	AfxNewImage(dib);
}

void CFalconEyesDoc::OnEdgeCanny()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CCannyDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		MatDib dib;
		dib = m_Dib;
		mc_Timer.Reset();
		MatEdgeCanny(m_Dib, dib, dlg.m_fSigma, dlg.m_fLowTh, dlg.m_fHighTh);
		AfxPrintInfo(TEXT("[ 캐니 엣지 검출 입력 영상: %s, sigma: %4.2f, Low Th: %4.2f, High Th : %4.2f (%d ms)"), GetTitle(), mc_Timer.Elapse());
		AfxNewImage(dib);
	}
}

void CFalconEyesDoc::OnSegmentBinarization()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CBinarization dlg;
	dlg.SetImage(m_Dib);
	if (dlg.DoModal() == IDOK)
	{
		MatDib dibDst = m_Dib;
		mc_Timer.Reset();
		MatBinarization(m_Dib, dibDst, dlg.m_nThreshold);
		AfxPrintInfo(_T("[이진화] 입력 영상: %s, 임계값: %d (%d ms)"), GetTitle(), dlg.m_nThreshold, mc_Timer.Elapse());
		AfxNewImage(dibDst);
	}
}

void CFalconEyesDoc::OnSegmentLabeling()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	MatDib dib;
	std::vector<MatLabelInfo> labels;
	mc_Timer.Reset();
	int label_cnt = MatLabeling(m_Dib, labels);
	dib = m_Dib;
	Mat matDst = dib.GetMatImg();
	int nW = dib.GetWidth();

	for (MatLabelInfo& info : labels)
	{
		for (int nStepY = info.miny; nStepY <= info.maxy; nStepY++)
			matDst.data[nStepY * nW + info.minx] = matDst.data[nStepY * nW + info.maxx] = 128;
		for (int nStepX = info.minx; nStepX <= info.maxx; nStepX++)
			matDst.data[info.miny * nW + nStepX] = matDst.data[info.maxy * nW + nStepX] = 128;
	}

	/*for(MatLabelInfo& info : labels)
	{
		MatDib dst;
		dst.CreateGrayImage(info.maxx - info.minx + 1, info.maxy - info.miny + 1);
		Mat Dst = dst.GetMatImg();
		for (Point& pt : info.pixels)
		{
			Dst.data[(pt.y - info.miny) * (info.maxx - info.minx + 1) + (pt.x - info.minx)] = 255;
		}
		dst.SetMatImg(Dst);
		AfxNewImage(dst);
	}*/
	AfxPrintInfo(TEXT("[레이블링] 입력 영상: %s, 객체 갯수: %d (%d ms)"), GetTitle(), label_cnt, mc_Timer.Elapse());
	dib.SetMatImg(matDst);
	AfxNewImage(dib);
}

void CFalconEyesDoc::OnContourTracing()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	MatDib dib;
	std::vector<MatLabelInfo> labels;
	mc_Timer.Reset();
	int label_cnt = MatLabeling(m_Dib, labels);
	dib = m_Dib;
	int nW = dib.GetWidth();
	int nH = dib.GetHeight();
	Mat matContour = Mat::zeros(Size(nW, nH), CV_8U);
	

	for (MatLabelInfo& info : labels)
	{
		std::vector<Point> cp;
		MatContourTracing(dib, info.pixels[0].x, info.pixels[0].y, cp);

		for (Point& pt : cp)
			matContour.data[pt.y * nW + pt.x] = 255;
	}

	AfxPrintInfo(TEXT("[외곽선 추적] 입력 영상: %s (%d ms)"), GetTitle(), mc_Timer.Elapse());
	dib.SetMatImg(matContour);
	AfxNewImage(dib);
}

void CFalconEyesDoc::OnHoughLine()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	MatDib dibDst;
	MatDib dibCanny;
	dibDst = m_Dib;
	dibCanny = m_Dib;
	mc_Timer.Reset();
	MatEdgeCanny(m_Dib, dibCanny, 1.4f, 30.f, 60.f);

	//dibDst = dibCanny;
	std::vector<MatLineParam> lines;
	MatHoughLine(dibCanny, lines);

	if (lines.size() == 0)
	{
		AfxMessageBox(_T(""));
		return;
	}
	std::sort(lines.begin(), lines.end());
	// 최대 10개의 직선만 화면에 그려줌
	int cnt = __min(10, static_cast<int>(lines.size()));
	for (int i = 0; i < cnt; i++)
		MatDrawLine(dibDst, lines[i], 255);

	AfxPrintInfo(TEXT("[허프 선 검출] 입력 영상: %s, 중요 직선 : rho = %4.2f, angle = %4.2f, vote = %d (%d ms)"), GetTitle(), lines[0].rho, (lines[0].ang * 180 / 3.14f), lines[0].vote, mc_Timer.Elapse());
	AfxNewImage(dibDst);
}

void CFalconEyesDoc::OnHarrisCorner()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CHarrisCornerDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		MatDib dibDst;
		dibDst = m_Dib;
		int nW = dibDst.GetWidth();
		int nH = dibDst.GetHeight();
		
		std::vector<Point> corners;
		mc_Timer.Reset();
		MatHarrisCorner(dibDst, corners, dlg.m_nHarrisTh);
		Mat matSrc = dibDst.GetMatImg();
		BYTE** ptr = new BYTE*[nH];
		for (int j = 0; j < nH; j++)
		{
			ptr[j] = &matSrc.data[j * nW];
		}

		int x, y;
		for (Point cp : corners)
		{
			x = cp.x;
			y = cp.y;

			ptr[y - 1][x - 1] = ptr[y - 1][x] = ptr[y - 1][x + 1] = 0;
			ptr[y][x - 1] = ptr[y][x] = ptr[y][x + 1] = 0;
			ptr[y + 1][x - 1] = ptr[y + 1][x] = ptr[y + 1][x + 1] = 0;
		}
		delete[] ptr;
		dibDst.SetMatImg(matSrc);
		AfxPrintInfo(TEXT("[해리스 코너 검출] 입력 영상: %s, Threshold: %d, 검출된 코너 갯수: %d (%d ms)"), GetTitle(), dlg.m_nHarrisTh, corners.size(), mc_Timer.Elapse());
		AfxNewImage(dibDst);
	}
}

void CFalconEyesDoc::OnWindowRoi()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	MatDib dibDst;
	CFalconEyesView *pView = (CFalconEyesView *)(((CMainFrame *)AfxGetMainWnd())->GetActiveFrame())->GetActiveView();
	if (pView->m_enDrawMode == pView->DrawMode::_DRAW_SELECT)
	{
		dibDst = m_Dib;
		pView->m_rectSave.x += 1;
		pView->m_rectSave.y += 1;
		pView->m_rectSave.width -= 2;
		pView->m_rectSave.height -= 2;
		if (pView->m_rectSave.x < 0 || pView->m_rectSave.y < 0)
		{
			pView->m_rectSave.x = 0;
			pView->m_rectSave.y = 0;
		}
		dibDst.SetMatImg(dibDst.GetMatImg()(pView->m_rectSave).clone());
		AfxPrintInfo(TEXT("[ROI Rect] 입력 영상: %s, ROI x: %d, y: %d, w: %d, h: %d "), GetTitle(), pView->m_rectSave.x, pView->m_rectSave.y, pView->m_rectSave.width, pView->m_rectSave.height);
		AfxNewImage(dibDst);
	}
}

void CFalconEyesDoc::OnColorGrayYuv()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	MatDib dib;
	dib = m_Dib;
	mc_Timer.Reset();
	DibToGray(m_Dib, dib);
	AfxPrintInfo(TEXT("[그레이 변환] 입력 영상: %s, 가로: %d px, 세로: %d, Color: %d (%d ms)"), GetTitle(), dib.GetWidth(), dib.GetHeight(), dib.GetBitCount() << 0x01, mc_Timer.Elapse());
	AfxNewImage(dib);
}

void CFalconEyesDoc::OnUpdateColorGrayCv(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_Dib.GetBitCount() != 8);
}

void CFalconEyesDoc::OnColorGrayCv()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	MatDib dib;
	dib = m_Dib;
	mc_Timer.Reset();
	DibToGrayCV(m_Dib, dib);
	AfxPrintInfo(TEXT("[그레이 변환] 입력 영상: %s, 가로: %d px, 세로: %d, Color: %d (%d ms)"), GetTitle(), dib.GetWidth(), dib.GetHeight(), dib.GetBitCount() << 0x01, mc_Timer.Elapse());
	AfxNewImage(dib);
}

void CFalconEyesDoc::OnUpdateColorGrayYuv(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_Dib.GetBitCount() != 8);
}

void CFalconEyesDoc::OnColorSplitRgb()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	MatDib dibR, dibG, dibB;
	Mat matColor, matR, matG, matB;
	CString strTitle = GetTitle();
	matColor = m_Dib.GetMatImg();
	mc_Timer.Reset();
	MatColorSplitRGB(matColor, matR, matG, matB);

	dibR.SetMatImg(matR);
	dibG.SetMatImg(matG);
	dibB.SetMatImg(matB);

	AfxPrintInfo(_T("[색상 평면 나누기 / RGB] 입력 영상: %s (%d ms)"), strTitle, mc_Timer.Elapse());
	strTitle.Format(_T("%s_ch.Red"), GetTitle());
	AfxNewImage(dibR, strTitle);
	strTitle.Format(_T("%s_ch.Green"), GetTitle());
	AfxNewImage(dibG, strTitle);
	strTitle.Format(_T("%s_ch.Blue"), GetTitle());
	AfxNewImage(dibB, strTitle);
}

void CFalconEyesDoc::OnColorSplitHsi()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	MatDib dibH, dibS, dibI;
	Mat matColor, matH, matS, matI;
	CString strTitle = GetTitle();
	matColor = m_Dib.GetMatImg();
	mc_Timer.Reset();
	MatColorSplitHSI(matColor, matH, matS, matI);

	dibH.SetMatImg(matH);
	dibS.SetMatImg(matS);
	dibI.SetMatImg(matI);

	AfxPrintInfo(_T("[색상 평면 나누기 / HSI] 입력 영상: %s (%d ms)"), strTitle, mc_Timer.Elapse());
	strTitle.Format(_T("%s_ch.Hue"), GetTitle());
	AfxNewImage(dibH, strTitle);
	strTitle.Format(_T("%s_ch.Saturation"), GetTitle());
	AfxNewImage(dibS, strTitle);
	strTitle.Format(_T("%s_ch.Intensity"), GetTitle());
	AfxNewImage(dibI, strTitle);
}

void CFalconEyesDoc::OnColorSplitYuv()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	MatDib dibY, dibU, dibV;
	Mat matColor, matY, matU, matV;
	CString strTitle = GetTitle();
	matColor = m_Dib.GetMatImg();
	mc_Timer.Reset();
	MatColorSplitYUV(matColor, matY, matU, matV);

	dibY.SetMatImg(matY);
	dibU.SetMatImg(matU);
	dibV.SetMatImg(matV);

	AfxPrintInfo(_T("[색상 평면 나누기 / YUV] 입력 영상: %s (%d ms)"), strTitle, mc_Timer.Elapse());
	strTitle.Format(_T("%s_Y"),GetTitle());
	AfxNewImage(dibY, strTitle);
	strTitle.Format(_T("%s_U"), GetTitle());
	AfxNewImage(dibU, strTitle);
	strTitle.Format(_T("%s_V"), GetTitle());
	AfxNewImage(dibV, strTitle);
}

void CFalconEyesDoc::OnUpdateColorSplitRgb(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_Dib.GetBitCount() > 8);
}

void CFalconEyesDoc::OnUpdateColorSplitHsi(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_Dib.GetBitCount() > 8);
}

void CFalconEyesDoc::OnUpdateColorSplitYuv(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_Dib.GetBitCount() > 8);
}

void CFalconEyesDoc::OnColorCombineRgb()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	MatDib dib;
	dib = m_Dib;
	CColorCombineDlg dlg;
	dlg.m_strColorSpace = _T("RGB 색상 평면 합치기");
	if (dlg.DoModal() == IDOK)
	{
		CFalconEyesDoc* pDoc1 = (CFalconEyesDoc*)dlg.m_pDoc1;
		CFalconEyesDoc* pDoc2 = (CFalconEyesDoc*)dlg.m_pDoc2;
		CFalconEyesDoc* pDoc3 = (CFalconEyesDoc*)dlg.m_pDoc3;

		Mat matR = pDoc1->m_Dib.GetMatImg();
		Mat matG = pDoc2->m_Dib.GetMatImg();
		Mat matB = pDoc3->m_Dib.GetMatImg();
		Mat matColor; // 함수 안에서 할당한다.
		mc_Timer.Reset();
		if (MatColorCombineRGB(matR, matG, matB, matColor) == FALSE)
		{
			AfxMessageBox(_T("영상의 크기가 다릅니다.!"));
			return;
		}

		dib.SetMatImg(matColor);
		AfxPrintInfo(_T("[색상 평면 합치기 / RGB] 입력 영상: R: %s, G: %s, B: %s (%d ms)"), pDoc1->GetTitle(), pDoc2->GetTitle(), pDoc3->GetTitle(), mc_Timer.Elapse());
		AfxNewImage(dib);
	}
}

void CFalconEyesDoc::OnColorCombineHsi()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	MatDib dib;
	dib = m_Dib;
	CColorCombineDlg dlg;
	dlg.m_strColorSpace = _T("HSI 색상 평면 합치기");
	if (dlg.DoModal() == IDOK)
	{
		CFalconEyesDoc* pDoc1 = (CFalconEyesDoc*)dlg.m_pDoc1;
		CFalconEyesDoc* pDoc2 = (CFalconEyesDoc*)dlg.m_pDoc2;
		CFalconEyesDoc* pDoc3 = (CFalconEyesDoc*)dlg.m_pDoc3;

		Mat matH = pDoc1->m_Dib.GetMatImg();
		Mat matS = pDoc2->m_Dib.GetMatImg();
		Mat matI = pDoc3->m_Dib.GetMatImg();
		Mat matColor; // 함수 안에서 할당한다.
		mc_Timer.Reset();
		if (MatColorCombineHSI(matH, matS, matI, matColor) == FALSE)
		{
			AfxMessageBox(_T("영상의 크기가 다릅니다.!"));
			return;
		}

		dib.SetMatImg(matColor);
		AfxPrintInfo(_T("[색상 평면 합치기 / HSI] 입력 영상: H: %s, S: %s, I: %s (%d ms)"), pDoc1->GetTitle(), pDoc2->GetTitle(), pDoc3->GetTitle(), mc_Timer.Elapse());
		AfxNewImage(dib);
	}
}

void CFalconEyesDoc::OnColorCombineYuv()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	MatDib dib;
	dib = m_Dib;
	CColorCombineDlg dlg;
	dlg.m_strColorSpace = _T("YUV 색상 평면 합치기");
	if (dlg.DoModal() == IDOK)
	{
		CFalconEyesDoc* pDoc1 = (CFalconEyesDoc*)dlg.m_pDoc1;
		CFalconEyesDoc* pDoc2 = (CFalconEyesDoc*)dlg.m_pDoc2;
		CFalconEyesDoc* pDoc3 = (CFalconEyesDoc*)dlg.m_pDoc3;

		Mat matY = pDoc1->m_Dib.GetMatImg();
		Mat matU = pDoc2->m_Dib.GetMatImg();
		Mat matV = pDoc3->m_Dib.GetMatImg();
		Mat matColor; // 함수 안에서 할당한다.
		mc_Timer.Reset();
		if (MatColorCombineYUV(matY, matU, matV, matColor) == FALSE)
		{
			AfxMessageBox(_T("영상의 크기가 다릅니다.!"));
			return;
		}

		dib.SetMatImg(matColor);
		AfxPrintInfo(_T("[색상 평면 합치기 / YUV] 입력 영상: Y: %s, U: %s, V: %s (%d ms)"), pDoc1->GetTitle(), pDoc2->GetTitle(), pDoc3->GetTitle(), mc_Timer.Elapse());
		AfxNewImage(dib);
	}
}

void CFalconEyesDoc::OnColorEdge()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	Mat matEdge, matSrc;
	MatDib dib;
	dib = m_Dib;
	matSrc = dib.GetMatImg();
	mc_Timer.Reset();
	MatColorEdge(matSrc, matEdge);
	dib.SetMatImg(matEdge);

	AfxPrintInfo(_T("[컬러 엣지 검출] 입력 영상: %s (%d ms)"), GetTitle(), mc_Timer.Elapse());
	AfxNewImage(dib);
}

void CFalconEyesDoc::OnUpdateColorEdge(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_Dib.GetBitCount() == 24);
}

void CFalconEyesDoc::OnMorphologyErosion()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	Mat matDst, matSrc;
	MatDib dib;
	dib = m_Dib;
	matSrc = dib.GetMatImg();
	mc_Timer.Reset();
	MatMorphologyErosion(matSrc, matDst);
	dib.SetMatImg(matDst);

	AfxPrintInfo(_T("[이진 모폴로지/침식] 입력 영상: %s (%d ms)"), GetTitle(), mc_Timer.Elapse());
	AfxNewImage(dib);
}

void CFalconEyesDoc::OnMorphologyDilation()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	Mat matDst, matSrc;
	MatDib dib;
	dib = m_Dib;
	matSrc = dib.GetMatImg();
	mc_Timer.Reset();
	MatMorphologyDilation(matSrc, matDst);
	dib.SetMatImg(matDst);

	AfxPrintInfo(_T("[이진 모폴로지/팽창] 입력 영상: %s (%d ms)"), GetTitle(), mc_Timer.Elapse());
	AfxNewImage(dib);
}

void CFalconEyesDoc::OnMorphologyOpening()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	Mat matDst, matSrc;
	MatDib dib;
	dib = m_Dib;
	matSrc = dib.GetMatImg();
	mc_Timer.Reset();
	MatMorphologyOpening(matSrc, matDst);
	dib.SetMatImg(matDst);

	AfxPrintInfo(_T("[이진 모폴로지/열기] 입력 영상: %s (%d ms)"), GetTitle(), mc_Timer.Elapse());
	AfxNewImage(dib);
}

void CFalconEyesDoc::OnMorphologyClosing()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	Mat matDst, matSrc;
	MatDib dib;
	dib = m_Dib;
	matSrc = dib.GetMatImg();
	mc_Timer.Reset();
	MatMorphologyClosing(matSrc, matDst);
	dib.SetMatImg(matDst);

	AfxPrintInfo(_T("[이진 모폴로지/닫기] 입력 영상: %s (%d ms)"), GetTitle(), mc_Timer.Elapse());
	AfxNewImage(dib);
}

void CFalconEyesDoc::OnGraymorphErosion()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	Mat matDst, matSrc;
	MatDib dib;
	dib = m_Dib;
	matSrc = dib.GetMatImg();
	mc_Timer.Reset();
	MatMorphologyGrayErosion(matSrc, matDst);
	dib.SetMatImg(matDst);

	AfxPrintInfo(_T("[그레이스케일 모폴로지/침식] 입력 영상: %s (%d ms)"), GetTitle(), mc_Timer.Elapse());
	AfxNewImage(dib);
}

void CFalconEyesDoc::OnGraymorphDilation()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	Mat matDst, matSrc;
	MatDib dib;
	dib = m_Dib;
	matSrc = dib.GetMatImg();
	mc_Timer.Reset();
	MatMorphologyGrayDilation(matSrc, matDst);
	dib.SetMatImg(matDst);

	AfxPrintInfo(_T("[그레이스케일 모폴로지/팽창] 입력 영상: %s (%d ms)"), GetTitle(), mc_Timer.Elapse());
	AfxNewImage(dib);
}

void CFalconEyesDoc::OnGraymorphOpening()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	Mat matDst, matSrc;
	MatDib dib;
	dib = m_Dib;
	matSrc = dib.GetMatImg();
	mc_Timer.Reset();
	MatMorphologyGrayOpening(matSrc, matDst);
	dib.SetMatImg(matDst);

	AfxPrintInfo(_T("[그레이스케일 모폴로지/열기] 입력 영상: %s (%d ms)"), GetTitle(), mc_Timer.Elapse());
	AfxNewImage(dib);
}


void CFalconEyesDoc::OnGraymorphClosing()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	Mat matDst, matSrc;
	MatDib dib;
	dib = m_Dib;
	matSrc = dib.GetMatImg();
	mc_Timer.Reset();
	MatMorphologyGrayClosing(matSrc, matDst);
	dib.SetMatImg(matDst);

	AfxPrintInfo(_T("[그레이스케일 모폴로지/닫기] 입력 영상: %s (%d ms)"), GetTitle(), mc_Timer.Elapse());
	AfxNewImage(dib);
}



void CFalconEyesDoc::OnSegmentBinarizationOtsu()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	MatDib dibDst = m_Dib;
	mc_Timer.Reset();
	MatBinarizationCVOtsu(m_Dib, dibDst);

	AfxPrintInfo(_T("[이진화_CV_Otsu] 입력 영상: %s (%d ms)"), GetTitle(), mc_Timer.Elapse());
	AfxNewImage(dibDst);
}


void CFalconEyesDoc::OnUpdateSegmentBinarizationOtsu(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CFalconEyesDoc::OnSegmentBinarizationAdaptive()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	MatDib dibDst = m_Dib;
	mc_Timer.Reset();
	MatBinarizationCVAdaptive(m_Dib, dibDst);

	AfxPrintInfo(_T("[이진화_CV_Adaptive] 입력 영상: %s (%d ms)"), GetTitle(), mc_Timer.Elapse());
	AfxNewImage(dibDst);
}


void CFalconEyesDoc::OnUpdateSegmentBinarizationAdaptive(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}

void CFalconEyesDoc::OnFourierFftCv()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	MatDib dibDst, dibTmp;
	dibDst = m_Dib;
	dibTmp = dibDst;
	// Load an image
	Mat matSrc = dibDst.GetMatImg();
	Mat matDst = Mat::zeros(cv::Size(matSrc.cols, matSrc.rows), CV_8U);
	Mat matFImg, matFourier;
	Mat plane[2];
	mc_Timer.Reset();

	int cx = matSrc.cols / 2;
	int cy = matSrc.rows / 2;


	matSrc.convertTo(matFImg, CV_32F);
	dft(matFImg, matFourier, DFT_SCALE | DFT_COMPLEX_OUTPUT);
	split(matFourier, plane);
	cartToPolar(plane[0], plane[1], plane[0], plane[1]);

 	matFourier = plane[0].clone();
 	log(matFourier, matFourier);

	
	

	Mat q0(matFourier, Rect(0, 0, cx, cy));
	Mat q1(matFourier, Rect(cx, 0, cx, cy));
	Mat q2(matFourier, Rect(0, cy, cx, cy));
	Mat q3(matFourier, Rect(cx, cy, cx, cy));

	Mat tmp;
	q0.copyTo(tmp);
	q3.copyTo(q0);
	tmp.copyTo(q3);

	q1.copyTo(tmp);
	q2.copyTo(q1);
	tmp.copyTo(q2);

	normalize(matFourier, matFourier, 0, 1, NormTypes::NORM_MINMAX);

	//matFourier = plane[0];
	matFourier.convertTo(matDst, CV_8U, 255);

	AfxPrintInfo(_T("[FFT_CV] 입력 영상: %s (%d ms)"), GetTitle(), mc_Timer.Elapse());
	dibDst.SetMatImg(matDst);
	AfxNewImage(dibDst);

	plane[1].convertTo(matFImg, CV_8U, 255);
	dibTmp.SetMatImg(matFImg);
	AfxNewImage(dibTmp);
}


void CFalconEyesDoc::OnUpdateFourierFftCv(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CFalconEyesDoc::OnFourierIfftCv()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	MatDib dibDst;
	dibDst = m_Dib;

	// Load an image
	Mat matSrc = dibDst.GetMatImg();
	Mat matDst = Mat::zeros(cv::Size(matSrc.cols, matSrc.rows), CV_8U);
	
	Mat matInvFFT;
	Mat planes[2];
	mc_Timer.Reset();
	matSrc.convertTo(matInvFFT, CV_32F);
	
	//exp(matInvFFT, matInvFFT);
	//polarToCart(planes[0], planes[1], planes[0], planes[1]);
	
	//Mat planes[2];

	polarToCart(matInvFFT, Mat::zeros(matInvFFT.rows, matInvFFT.cols, matInvFFT.type()), planes[0], planes[1]);

	merge(planes, 2, matInvFFT);

	dft(matInvFFT, matInvFFT, DFT_INVERSE | DFT_SCALE | DFT_REAL_OUTPUT);

	split(matInvFFT, planes);

	matDst = planes[0];


	//dft(matInvFFT, matDst, DFT_SCALE | DFT_INVERSE | DFT_REAL_OUTPUT);
	matDst.convertTo(matDst, CV_8U, 255);
	AfxPrintInfo(_T("[FFT_CV] 입력 영상: %s (%d ms)"), GetTitle(), mc_Timer.Elapse());
	dibDst.SetMatImg(matDst);
	AfxNewImage(dibDst);
}


void CFalconEyesDoc::OnUpdateFourierIfftCv(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}

void CFalconEyesDoc::OnFourierDescriptor()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CFourierDescDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		MatDib dib;
		std::vector< MatLabelInfo> labels;
		mc_Timer.Reset();
		int nLabelCnt = MatLabeling(m_Dib, labels);
		if (nLabelCnt > 128)
		{
			AfxMessageBox(_T("Label이 128보다 큽니다."));
			return;
		}
		int nW = m_Dib.GetWidth();
		int nH = m_Dib.GetHeight();
		dib.CreateGrayImage(nW, nH);
		BYTE* ptr = dib.GetDIBitsAddr();
		
		for (MatLabelInfo& info : labels)
		{
			std::vector<Point> cp;
			MatFourierDescriptor(m_Dib, info.pixels[0].x, info.pixels[0].y, dlg.m_nPercent, cp);

			for (Point pt : cp)
			{
				ptr[pt.y * nW + pt.x] = 255;
			}
		}
		
		dib.UpdateBack();
		AfxPrintInfo(_T("[푸리에 기술자] 입력 영상: %s, 객체 개수: %d, 복원 비율: %d%% (%d ms)"), GetTitle(), nLabelCnt, dlg.m_nPercent, mc_Timer.Elapse());
		AfxNewImage(dib);
	}
}


void CFalconEyesDoc::OnInvariantMoments()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	double m[7] = { 0., };
	mc_Timer.Reset();
	MatInvariantMoments(m_Dib, m);

	double mm[7] = { 0., };
	CString strMoments = _T("");
	for (int i = 0; i < 7; i++)
	{
		if(m[i] == 0) continue;
		mm[i] = abs(log(abs(m[i])));
		strMoments.AppendFormat(_T("m[%d] = %6.3lf"), i + 1, mm[i]);
		if (i < 6) strMoments.Append(_T(", "));
	}

	AfxPrintInfo(_T("[불변 모멘트] 입력 영상: %s, 불변 모멘트(log): %s (%d ms)"), GetTitle(), strMoments, mc_Timer.Elapse());
}


void CFalconEyesDoc::OnZernikeMoments()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	AfxPrintInfo(_T("[저니키 모멘트] 입력 영상: %s, 저니키 모멘트(Mag) (n<=6):"), GetTitle());
	
	double zr, zi;
	CString strMoments = _T("");
	for (int n = 0; n <= 6; n++)
	{
		strMoments = _T("  ");
		for (int m = (n % 2); m <= n; m += 2)
		{
			mc_Timer.Reset();
			MatZernikeMoments(m_Dib, n, m, zr, zi);
			strMoments.AppendFormat(_T("|zm(%d, %d)| = %7.3lf (%d ms)"), n, m, hypot(zr, zi), mc_Timer.Elapse());
			if (m < n - 1) strMoments.Append(_T(", "));
		}
		AfxPrintInfo(strMoments);
	}
}


void CFalconEyesDoc::OnTemplateMatching()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	TCHAR szFilter[] = _T("Bitmap Files (*.BMP)|*.BMP|All Files (*.*)|*.*||");
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	MatDib dibDst = m_Dib;
	if (dlg.DoModal() != IDOK)
		return;

	CString pathName = dlg.GetPathName();
	MatDib dibTmpl;
	if (!dibTmpl.Load(CT2A(pathName)))
	{
		AfxMessageBox(_T("파일을 불러오지 못했습니다."));
		return;
	}
	AfxNewImage(dibTmpl);

	if (dibDst.GetWidth() < dibTmpl.GetWidth() || dibDst.GetHeight() < dibTmpl.GetHeight())
	{
		AfxMessageBox(_T("템플릿 영상의 크기가 입력 영상보다 큽니다."));
		return;
	}
	int* arrayMap = nullptr;
	int nMapSize = dibDst.GetWidth() * dibDst.GetHeight();
	arrayMap = new int[nMapSize];
	memset(arrayMap, 0x00, sizeof(int)*nMapSize);
	mc_Timer.Reset();
	Point dp = MatTemplatematching(dibDst, dibTmpl, arrayMap);
	
#if _DEBUG
	{
		MatDib dibCvt;
		dibCvt.CreateGrayImage(dibDst.GetWidth(), dibDst.GetHeight());
		BYTE* pCvt = dibCvt.GetDIBitsAddr();
		int* pMap = arrayMap;

		int max_value = 0;
		unsigned int i = 0;
		for (i = 0; i < dibCvt.GetSize(); i++)
		{
			if (pMap[i] > max_value) max_value = pMap[i];
		}

		if (max_value != 0)
		{
			for (i = 0; i < dibCvt.GetSize(); i++)
			{
				pCvt[i] = pMap[i] * 255 / max_value;
			}
		}

		AfxNewImage(dibCvt);
	}
#endif
	{// 입력 영상에 BOX 그리기
		int tw2 = dibTmpl.GetWidth() / 2;
		int th2 = dibTmpl.GetHeight() / 2;

		int minx = dp.x - tw2;
		int maxx = dp.x + tw2;
		int miny = dp.y - th2;
		int maxy = dp.y + th2;

		int w = dibDst.GetWidth();
		BYTE* ptr = dibDst.GetDIBitsAddr();

		for (int j = miny; j < maxy; j++)
			ptr[j * w + minx] = ptr[j * w + maxx] = 255;
		for (int i = minx; i < maxx; i++)
			ptr[miny * w + i] = ptr[maxy * w + i] = 255;
	}

	AfxPrintInfo(_T("[템플릿 매칭] 입력 영상: %s, 템플릿 영상: %s, 검출 좌표: (%d, %d) (%d ms)"), GetTitle(), dlg.GetFileName(), dp.x, dp.y, mc_Timer.Elapse());
	AfxNewImage(dibDst);
}


void CFalconEyesDoc::OnEditUndo()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (m_nBufferCnt <= IMAGE_BUFFER_MAX_SIZE)
	{
		if (m_nBufferIndex > 0)
			--m_nBufferIndex;
		else
			m_nBufferIndex = 0;

		POSITION pos = GetFirstViewPosition();
		CFalconEyesView* view = (CFalconEyesView*)GetNextView(pos);
		m_Dib.SetMatImg(m_matBuffer[m_nBufferIndex]);
		view->Invalidate();
	}
}


void CFalconEyesDoc::OnEditRedo()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	
	if (m_nBufferCnt <= IMAGE_BUFFER_MAX_SIZE)
	{
		if (m_nBufferIndex < IMAGE_BUFFER_MAX_SIZE - 1)
			++m_nBufferIndex;
		else
			m_nBufferIndex = IMAGE_BUFFER_MAX_SIZE-1;

		POSITION pos = GetFirstViewPosition();
		CFalconEyesView* view = (CFalconEyesView*)GetNextView(pos);
		m_Dib.SetMatImg(m_matBuffer[m_nBufferIndex]);
		view->Invalidate();
	}
}

void CFalconEyesDoc::m_fn_UpdateUndoBuffer()
{
	if (m_nBufferCnt < IMAGE_BUFFER_MAX_SIZE)
	{
		m_matBuffer[++m_nBufferIndex] = m_Dib.GetMatImg();
		++m_nBufferCnt;
	}
}


void CFalconEyesDoc::OnEditOrigin()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_nBufferIndex = 0;
	m_nBufferCnt = 1;
	
	POSITION pos = GetFirstViewPosition();
	CFalconEyesView* view = (CFalconEyesView*)GetNextView(pos);
	m_Dib.SetMatImg(m_matBuffer[m_nBufferIndex]);
	view->Invalidate();
}


void CFalconEyesDoc::OnUpdateEditOrigin(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	//pCmdUI->Enable(m_nBufferIndex > 0);
}


void CFalconEyesDoc::OnUpdateEditRedo(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_nBufferCnt != (m_nBufferIndex + 1));
}


void CFalconEyesDoc::OnUpdateEditUndo(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_nBufferIndex > 0);
}

void CFalconEyesDoc::OnTemplateMatchingCv()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	TCHAR szFilter[] = _T("Bitmap Files (*.BMP)|*.BMP|All Files (*.*)|*.*||");
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	MatDib dibDst = m_Dib;
	if (dlg.DoModal() != IDOK)
		return;

	CString pathName = dlg.GetPathName();
	MatDib dibTmpl;
	if (!dibTmpl.Load(CT2A(pathName)))
	{
		AfxMessageBox(_T("파일을 불러오지 못했습니다."));
		return;
	}
	AfxNewImage(dibTmpl);

	if (dibDst.GetWidth() < dibTmpl.GetWidth() || dibDst.GetHeight() < dibTmpl.GetHeight())
	{
		AfxMessageBox(_T("템플릿 영상의 크기가 입력 영상보다 큽니다."));
		return;
	}

	wchar_t chLog[LOG_TEXT_MAX_SIZE] = { 0, };
	int nCenterX = 0, nCenterY = 0;
	int matchMethod = TemplateMatchModes::TM_SQDIFF_NORMED;
	//int matchMethod = TemplateMatchModes::TM_SQDIFF;
	Mat matTemplate, matOut, matMark, matBack;
	nCenterX = static_cast<int>(dibTmpl.GetWidth() / 2.0);
	nCenterY = static_cast<int>(dibTmpl.GetHeight() / 2.0);
	matBack = m_Dib.GetMatImg();
	matMark = dibTmpl.GetMatImg();
	if (matMark.channels() != 1)
	{
		cvtColor(matMark, matMark, ColorConversionCodes::COLOR_BGR2GRAY);
	}
	mc_Timer.Reset();
	matchTemplate(matBack, matMark, matTemplate, matchMethod);
#if _DEBUG
	Mat matDebug;
	matTemplate.convertTo(matDebug, CV_8U, 255);
	dibDst.SetMatImg(matDebug);
	AfxNewImage(dibDst);
#endif
	int x, y;
	Point matchLoc;
	float* pData = (float*)matTemplate.data;
	float dValue = 1.0, dTmp;;
	for (y = 0; y < matTemplate.rows; y++)
	{
		for (x = 0; x < matTemplate.cols; x++)
		{
			dTmp = pData[y * matTemplate.cols + x];
			if (dValue >= dTmp)
			{
				dValue = dTmp;
				matchLoc.x = x;
				matchLoc.y = y;
			}
		}
	}
	cvtColor(matBack, matOut, ColorConversionCodes::COLOR_GRAY2BGR);
	rectangle(matOut, cv::Rect(matchLoc.x, matchLoc.y, matMark.cols, matMark.rows), Scalar(0, 0, 255));


	dibDst.SetMatImg(matOut);

	AfxPrintInfo(_T("[템플릿 매칭_CV] 입력 영상: %s, 템플릿 영상: %s, 검출 좌표: (%d, %d) (%d ms)"), GetTitle(), dlg.GetFileName(), matchLoc.x, matchLoc.y, mc_Timer.Elapse());
	AfxNewImage(dibDst);
}