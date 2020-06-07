
// FalconEyesDoc.h : CFalconEyesDoc 클래스의 인터페이스
//


#pragma once


class CFalconEyesDoc : public CDocument
{
protected: // serialization에서만 만들어집니다.
	CFalconEyesDoc() noexcept;
	DECLARE_DYNCREATE(CFalconEyesDoc)

// 특성입니다.
public:
// 작업입니다.
	FalconTimer mc_Timer;
public:

// 재정의입니다.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 구현입니다.
public:
	virtual ~CFalconEyesDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 검색 처리기에 대한 검색 콘텐츠를 설정하는 도우미 함수
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	// Opencv Mat 객체
	MatDib m_Dib;
	Mat m_matBuffer[IMAGE_BUFFER_MAX_SIZE];
	short m_nBufferCnt;
	short m_nBufferIndex;
	void m_fn_UpdateUndoBuffer();
	afx_msg void OnWindowDuplicate();
	afx_msg void OnEditCopy();
	afx_msg void OnImageInverse();
	afx_msg void OnImageBrightnesscontrast();
	afx_msg void OnImageGammacorrection();
	afx_msg void OnHistogramStretching();
	afx_msg void OnHistogramEqualization();
	afx_msg void OnImageArithmeticlogical();
	afx_msg void OnBitplaneSlicing();
	afx_msg void OnImageTranslate();
	afx_msg void OnImageResize();
	afx_msg void OnImageRotate();
	afx_msg void OnImageMirror();
	afx_msg void OnImageFlip();
	afx_msg void OnHistogram();
	afx_msg void OnFilterMean();
	afx_msg void OnFilterMidMean();
	afx_msg void OnFilterGaussian();
	afx_msg void OnFilterLaplacian();
	afx_msg void OnFilterUnsharpmask();
	afx_msg void OnFilterHighboost();
	afx_msg void OnAddNoise();
	afx_msg void OnFilterMedian();
	afx_msg void OnFilterDiffusion();
	afx_msg void OnFilterDft();
	afx_msg void OnUpdateBitplaneSlicing(CCmdUI *pCmdUI);
	afx_msg void OnUpdateHistogram(CCmdUI *pCmdUI);
	afx_msg void OnUpdateImageTranslate(CCmdUI *pCmdUI);
	afx_msg void OnUpdateImageResize(CCmdUI *pCmdUI);
	afx_msg void OnUpdateImageRotate(CCmdUI *pCmdUI);
	afx_msg void OnUpdateImageMirror(CCmdUI *pCmdUI);
	afx_msg void OnUpdateImageFlip(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFilterMean(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFilterMidMean(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFilterGaussian(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFilterLaplacian(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFilterUnsharpmask(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFilterHighboost(CCmdUI *pCmdUI);
	afx_msg void OnUpdateAddNoise(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFilterMedian(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFilterDiffusion(CCmdUI *pCmdUI);
	afx_msg void OnUpdateSegmentBinarization(CCmdUI *pCmdUI);
	afx_msg void OnUpdateSegmentLabeling(CCmdUI *pCmdUI);
	afx_msg void OnUpdateContourTracing(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMorphologyErosion(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMorphologyDilation(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMorphologyOpening(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMorphologyClosing(CCmdUI *pCmdUI);
	afx_msg void OnUpdateGraymorphErosion(CCmdUI *pCmdUI);
	afx_msg void OnUpdateGraymorphDilation(CCmdUI *pCmdUI);
	afx_msg void OnUpdateGraymorphOpening(CCmdUI *pCmdUI);
	afx_msg void OnUpdateGraymorphClosing(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFourierDescriptor(CCmdUI *pCmdUI);
	afx_msg void OnUpdateInvariantMoments(CCmdUI *pCmdUI);
	afx_msg void OnUpdateZernikeMoments(CCmdUI *pCmdUI);
	afx_msg void OnUpdateTemplateMatching(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFourierDft(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFourierDftrc(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFourierFft(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEdgeRoberts(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEdgePrewitt(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEdgeSobel(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEdgeCanny(CCmdUI *pCmdUI);
	afx_msg void OnUpdateHoughLine(CCmdUI *pCmdUI);
	afx_msg void OnUpdateHarrisCorner(CCmdUI *pCmdUI);
	afx_msg void OnFourierDft();
	afx_msg void OnColorGrayscale();
	afx_msg void OnUpdateColorGrayscale(CCmdUI *pCmdUI);
	afx_msg void OnEdgeRoberts();
	afx_msg void OnEdgePrewitt();
	afx_msg void OnEdgeSobel();
	afx_msg void OnEdgeCanny();
	afx_msg void OnSegmentBinarization();
	afx_msg void OnSegmentLabeling();
	afx_msg void OnContourTracing();
	afx_msg void OnHoughLine();
	afx_msg void OnFilterGaussianCv();
	afx_msg void OnUpdateFilterGaussianCv(CCmdUI *pCmdUI);
	afx_msg void OnHarrisCorner();
	afx_msg void OnWindowRoi();
	afx_msg void OnColorGrayYuv();
	afx_msg void OnUpdateColorGrayCv(CCmdUI *pCmdUI);
	afx_msg void OnColorGrayCv();
	afx_msg void OnUpdateColorGrayYuv(CCmdUI *pCmdUI);
	afx_msg void OnColorSplitRgb();
	afx_msg void OnColorSplitHsi();
	afx_msg void OnColorSplitYuv();
	afx_msg void OnUpdateColorSplitRgb(CCmdUI *pCmdUI);
	afx_msg void OnUpdateColorSplitHsi(CCmdUI *pCmdUI);
	afx_msg void OnUpdateColorSplitYuv(CCmdUI *pCmdUI);
	afx_msg void OnColorCombineRgb();
	afx_msg void OnColorCombineHsi();
	afx_msg void OnColorCombineYuv();
	afx_msg void OnColorEdge();
	afx_msg void OnUpdateColorEdge(CCmdUI *pCmdUI);
	afx_msg void OnMorphologyErosion();
	afx_msg void OnMorphologyDilation();
	afx_msg void OnMorphologyOpening();
	afx_msg void OnMorphologyClosing();
	afx_msg void OnGraymorphErosion();
	afx_msg void OnGraymorphDilation();
	afx_msg void OnGraymorphOpening();
	afx_msg void OnGraymorphClosing();
	afx_msg void OnSegmentBinarizationOtsu();
	afx_msg void OnUpdateSegmentBinarizationOtsu(CCmdUI *pCmdUI);
	afx_msg void OnSegmentBinarizationAdaptive();
	afx_msg void OnUpdateSegmentBinarizationAdaptive(CCmdUI *pCmdUI);
	afx_msg void OnFourierFftCv();
	afx_msg void OnUpdateFourierFftCv(CCmdUI *pCmdUI);
	afx_msg void OnFourierIfftCv();
	afx_msg void OnUpdateFourierIfftCv(CCmdUI *pCmdUI);
	afx_msg void OnFourierDescriptor();
	afx_msg void OnInvariantMoments();
	afx_msg void OnZernikeMoments();
	afx_msg void OnTemplateMatching();
	afx_msg void OnEditUndo();
	afx_msg void OnEditRedo();
	afx_msg void OnEditOrigin();
	afx_msg void OnUpdateEditOrigin(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditRedo(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditUndo(CCmdUI *pCmdUI);
	afx_msg void OnTemplateMatchingCv();
};
