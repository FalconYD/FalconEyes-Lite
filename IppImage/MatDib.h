#pragma once
#include <opencv2/opencv.hpp>
#include <windows.h>
#include <math.h>
#include <algorithm>
#include <random>
#include <time.h>
#include "RGBBYTE.h"
using namespace cv;

/**
@class	MatDib
@brief	이미지 클래스
@remark	
 - OpenCV를 사용하여 이미지를 읽고 쓴다.
@author	선경규(Kyeong Kyu - Seon)
@date	2019/10/21  17:07
*/
class MatDib
{
public:
	// 생성자와 소멸자
	MatDib();
	MatDib(const MatDib& dib);
	~MatDib();

	// 비트맵 생성과 소멸
	/**	
	@fn		BOOL CreateGrayImage(LONG nWidth, LONG nHeight)
	@brief	8bit Image 생성
	@return	성공시 TRUE, 실패시 FALSE
	@param	nWidth 이미지 폭
	@param	nHeight	이미지 높이
	@remark	
	 - 
	@author	선경규(Kyeong Kyu - Seon)
	@date	2019/10/21  17:24
	*/
	BOOL CreateGrayImage(LONG nWidth, LONG nHeight);

	/**
	@fn		BOOL CreateRgbImage(LONG nWidth, LONG nHeight)
	@brief	24bit Image 생성
	@return	성공시 TRUE, 실패시 FALSE
	@param	nWidth 이미지 폭
	@param	nHeight	이미지 높이
	@remark
	 -
	@author	선경규(Kyeong Kyu - Seon)
	@date	2019/10/21  17:09
	*/
	BOOL CreateRgbImage(LONG nWidth, LONG nHeight);

	/**	
	@fn		void DestroyBitmap()
	@brief	Bitmap 파괴
	@return	void
	@param	void
	@remark	
	 - 
	@author	선경규(Kyeong Kyu - Seon)
	@date	2019/10/21  17:10
	*/
	void DestroyBitmap();

	// 파일 입출력
	/**	
	@fn		BOOL Load(const char* filename)
	@brief	Image File Load
	@return	성공시 TRUE, 실패시 FALSE
	@param	filename 파일 로드 경로
	@remark	
	 - 
	@author	선경규(Kyeong Kyu - Seon)
	@date	2019/10/21  17:11
	*/
	BOOL Load(const char* filename);

	BOOL LoadRaw(const char* filename, int nType, int nWidth, int nHeight, int Offset, int Channel, bool bLittle);
	/**	
	@fn		BOOL Save(const char* filename)
	@brief	Image File Save
	@return	성공시 TRUE, 실패시 FALSE
	@param	filename 파일 저장 경로
	@remark	
	 - 
	@author	선경규(Kyeong Kyu - Seon)
	@date	2019/10/21  17:11
	*/
	BOOL Save(const char* filename);

	// 비트맵 화면 출력
	/**	
	@fn		void Draw(HDC hdc, int dx = 0, int dy = 0)
	@brief	DIB 출력 함수
	@return	void
	@param	hdc 대상 HDC
	@param	dx 대상 x좌표
	@param	dy 대상 y좌표
	@remark	
	 - 
	@author	선경규(Kyeong Kyu - Seon)
	@date	2019/10/21  17:13
	*/
	void Draw(HDC hdc, int dx = 0, int dy = 0);

	/**	
	@fn		void Draw(HDC hdc, int dx, int dy, int dw, int dh, DWORD dwRop = SRCCOPY, bool bHalftone = false)
	@brief	DIB 출력 함수
	@return	void
	@param	hdc 대상 HDC
	@param	dx 대상 x좌표
	@param	dy 대상 y좌표
	@param	dw 대상 폭
	@param	dh 대상 길이
	@param	dwRop DIB 파라메터
	@param	bHalftone 하프톤 모드
	@remark	
	 - 
	@author	선경규(Kyeong Kyu - Seon)
	@date	2019/10/21  17:16
	*/
	void Draw(HDC hdc, int dx, int dy, int dw, int dh, DWORD dwRop = SRCCOPY, bool bHalftone = false);

	/**	
	@fn		void DrawScale(HDC hdc, int dx, int dy, int dw, int dh, DWORD dwRop = SRCCOPY)
	@brief	DIB Scale 출력 함수
	@return	void
	@param	hdc 대상 HDC
	@param	dx 대상 x좌표
	@param	dy 대상 y좌표
	@param	dw 대상 폭
	@param	dh 대상 길이
	@param	dwRop DIB 파라메터
	@remark	
	 - 
	@author	선경규(Kyeong Kyu - Seon)
	@date	2019/10/21  17:29
	*/
	void DrawScale(HDC hdc, int dx, int dy, int dw, int dh, DWORD dwRop = SRCCOPY);

	/**	
	@fn		void Draw(HDC hdc, int dx, int dy, int dw, int dh, int sx, int sy, int sw, int sh, DWORD dwRop = SRCCOPY, bool bHalftone = false)
	@brief	DIB 출력 함수
	@return	void
	@param	hdc 대상 HDC
	@param	dx 대상 x좌표
	@param	dy 대상 y좌표
	@param	dw 대상 폭
	@param	dh 대상 길이
	@param	sx 원본 x좌표
	@param	sy 원본 y좌표
	@param	sw 원본 폭
	@param	sh 원본 길이
	@param	dwRop DIB 파라메터
	@param	bHalftone 하프톤 모드
	@remark	
	 - 
	@author	선경규(Kyeong Kyu - Seon)
	@date	2019/10/21  17:30
	*/
	void Draw(HDC hdc, int dx, int dy, int dw, int dh, int sx, int sy, int sw, int sh, DWORD dwRop = SRCCOPY, bool bHalftone = false);

	// Windows 환경에서 클립보드로 복사하기 및 붙여넣기
	/**	
	@fn		BOOL CopyToClipboard()
	@brief	클립 보드로 복사하기
	@return	성공시 TRUE, 실패시 FALSE
	@param	void
	@remark	
	 - 
	@author	선경규(Kyeong Kyu - Seon)
	@date	2019/10/21  17:32
	*/
	BOOL CopyToClipboard();

	/**	
	@fn		BOOL PasteFromClipboard()
	@brief	클립보드에서 붙여넣기
	@return	성공시 TRUE, 실패시 FALSE
	@param	void
	@remark	
	 - 
	@author	선경규(Kyeong Kyu - Seon)
	@date	2019/10/21  17:33
	*/
	BOOL PasteFromClipboard();

	// 대입 연산자 재정의와 비트맵 복사
	/**	
	@fn		MatDib& operator=(const MatDib& dib)
	@brief	객체 복사 연산자 재정의
	@return	MatDib 객체
	@param	&MatDib 객체
	@remark	
	 - 
	@author	선경규(Kyeong Kyu - Seon)
	@date	2019/10/21  17:34
	*/
	MatDib& operator=(const MatDib& dib);

	// 비트맵 정보 반환 함수
	/**	
	@fn		LONG GetHeight()
	@brief	이미지 길이를 얻는 함수
	@return	이미지의 길이
	@param	void
	@remark	
	 - 
	@author	선경규(Kyeong Kyu - Seon)
	@date	2019/10/21  17:35
	*/
	LONG GetHeight() const { return m_nHeight; }

	/**	
	@fn		LONG GetWidth()
	@brief	이미지 폭을 얻는 함수
	@return	이미지의 폭
	@param	void
	@remark	
	 - 
	@author	선경규(Kyeong Kyu - Seon)
	@date	2019/10/21  17:36
	*/
	LONG GetWidth() const { return m_nWidth; }

	/**	
	@fn		LONG GetApend()
	@brief	Apend 값 반환 함수
	@return	Apend 값
	@param	void
	@remark	
	 - 
	@author	선경규(Kyeong Kyu - Seon)
	@date	2019/10/21  17:36
	*/
	LONG GetApend() const { return m_nApend; }

	/**	
	@fn		WORD GetBitCount()
	@brief	이미지 Bit 수 반환 함수
	@return	이미지 Bit
	@param	void
	@remark	
	 - 
	@author	선경규(Kyeong Kyu - Seon)
	@date	2019/10/21  17:57
	*/
	WORD GetBitCount() const { return m_nBitCount; }

	/**	
	@fn		DWORD GetDibSize()
	@brief	Dib 사이즈 반환 함수
	@return	Dib 사이즈
	@param	void
	@remark	
	 - 
	@author	선경규(Kyeong Kyu - Seon)
	@date	2019/10/21  17:58
	*/
	DWORD GetDibSize() const { return m_nDibSize; }

	/**	
	@fn		double GetSize()
	@brief	이미지 픽셀 수 반환 함수
	@return	이미지 픽셀 수
	@param	void
	@remark	
	 - 
	@author	선경규(Kyeong Kyu - Seon)
	@date	2019/10/21  18:00
	*/
	unsigned int GetSize() const { return m_nHeight * m_nWidth; }

	/**	
	@fn		LPBITMAPINFO GetBitmapInfoAddr()
	@brief	Bitmap Info 구조체 반환 함수
	@return	Bitmap Info 구조체 반환
	@param	void
	@remark	
	 - 
	@author	선경규(Kyeong Kyu - Seon)
	@date	2019/10/21  18:01
	*/
	LPBITMAPINFO GetBitmapInfoAddr() const { return (LPBITMAPINFO)m_pDib; }

	/**	
	@fn		BYTE* GetDIBitsAddrs()
	@brief	DIB 데이터 주소 반환 함수
	@return	DIB 데이터 주소 반환
	@param	void
	@remark	
	 - 
	@author	선경규(Kyeong Kyu - Seon)
	@date	2019/10/21  18:02
	*/
	BYTE* GetDIBitsAddr() const;

	/**	
	@fn		BYTE* GetMatAddr()
	@brief	내부 Mat객체의 데이터 주소 반환
	@return	Mat Data 주소
	@param	void
	@remark	
	 - 
	@author	선경규(Kyeong Kyu - Seon)
	@date	2019/10/21  18:03
	*/
	BYTE* GetMatAddr() const;
	//BYTE* GetDIBitsColorAddr() const;

	/**	
	@fn		int GetPaletteNums()
	@brief	팔렛트 수 반환 함수
	@return	팔렛트 수 반환
	@param	void
	@remark	
	 - 
	@author	선경규(Kyeong Kyu - Seon)
	@date	2019/10/21  18:05
	*/
	int GetPaletteNums() const;

	/**	
	@fn		BOOL IsValid()
	@brief	dib 객체의 사용 가능 판단 함수
	@return	dib 포인터가 NULL이면 FALSE, NULL이 아니면 TRUE
	@param	void
	@remark	
	 - 
	@author	선경규(Kyeong Kyu - Seon)
	@date	2019/10/21  18:45
	*/
	BOOL IsValid() const { return (m_pDib != NULL); }

	/**	
	@fn		Mat GetMatImg()
	@brief	Mat 객체를 반환
	@return	Mat 객체
	@param	void
	@remark	
	 - 
	@author	선경규(Kyeong Kyu - Seon)
	@date	2019/10/21  18:46
	*/
	Mat GetMatImg() const { return m_matImg.clone(); }

	/**	
	@fn		Mat GetBackMatImg()
	@brief	Mat 원본 이미지 반환 함수
	@return	Mat 원본 이미지
	@param	void
	@remark	
	 - 
	@author	선경규(Kyeong Kyu - Seon)
	@date	2019/10/21  18:47
	*/
	Mat GetBackMatImg() const { return m_matBack.clone(); }

	/**	
	@fn		BOOL SetMatImg(Mat matImg)
	@brief	Mat 이미지를 클래스에 등록
	@return	성공시 TRUE, 실패시 FALSE
	@param	셋팅할 Mat 이미지
	@remark	
	 - 
	@author	선경규(Kyeong Kyu - Seon)
	@date	2019/10/21  18:55
	*/
	BOOL SetMatImg(Mat matImg);

	/**	
	@fn		void UpdateBack()
	@brief	내부 Mat 이미지 객체 동기화 함수
	@return	void
	@param	void
	@remark	
	 - 
	@author	선경규(Kyeong Kyu - Seon)
	@date	2019/10/21  19:14
	*/
	void UpdateBack();

	/**	
	@fn		void UpdateBack(BYTE* data)
	@brief	BYTE 데이터를 Mat 데이터로 변환
	@return	void
	@param	data 이미지 포인터
	@remark	
	 - 내부적으로 이미지 180도 회전 수행
	 - 포인터 연산이므로 파라메터 객체 해제시 이미지 손상 우려있음.
	@author	선경규(Kyeong Kyu - Seon)
	@date	2019/10/21  19:15
	*/
	void UpdateBack(BYTE* data);

	/**	
	@fn		BYTE GetMatPixel(int x, int y, int type = 0)
	@brief	Mat에서 Pixel Data 얻는 함수
	@return	x, y 좌표의 BYTE 데이터
	@param	x 이미지 x좌표
	@param	y 이미지 y좌표
	@param	type 얻을 픽셀 데이터 타입
	@remark	
	 - type 0:GrayScale, 1:Red, 2:Blue, 3:Green
	@author	선경규(Kyeong Kyu - Seon)
	@date	2019/10/21  19:19
	*/
	BYTE GetMatPixel(int x, int y, int type = 0);

	/**	
	@fn		BOOL UpdateDib()
	@brief	Mat 객체 이미지와 DIB 객체 동기화 함수
	@return	동기화 성공시 TRUE, 실패시 FALSE
	@param	void
	@remark	
	 - 
	@author	선경규(Kyeong Kyu - Seon)
	@date	2019/10/21  19:23
	*/
	BOOL UpdateDib();

	/**	
	@fn		void Release()
	@brief	객체 해제 함수
	@return	void
	@param	void
	@remark	
	 - 
	@author	선경규(Kyeong Kyu - Seon)
	@date	2019/10/22  8:39
	*/
	void Release();

	// Draw
	/**	
	@fn		void DrawSelRect(int l, int t, int w, int h, Scalar color, int thick)
	@brief	선택 사각형 그리는 함수
	@return	void
	@param	l Left
	@param	t Top
	@param	r Right
	@param	b Bottom
	@param	color Color Scalar
	@param	thick Thickness
	@remark	
	 - 
	@author	선경규(Kyeong Kyu - Seon)
	@date	2019/10/22  8:43
	*/
	void DrawSelRect(int l, int t, int r, int b, Scalar color, int thick);

	/**	
	@fn		void DrawLine(int l, int t, int r, int b, Scalar color, int thick)
	@brief	직선 그리는 함수
	@return	void
	@param	l Left
	@param	t Top
	@param	r Right
	@param	b Bottom
	@param	color Color Scalar
	@param	thick Thickness
	@remark	
	 - 
	@author	선경규(Kyeong Kyu - Seon)
	@date	2019/10/22  8:45
	*/
	void DrawLine(int l, int t, int r, int b, Scalar color, int thick);

	/**	
	@fn		void DrawRect(int l, int t, int r, int b, Scalar color, int thick)
	@brief	사각형 그리는 함수
	@return	void
	@param	l Left
	@param	t Top
	@param	r Right
	@param	b Bottom
	@param	color Color Scalar
	@param	thick Thickness
	@remark	
	 - 
	@author	선경규(Kyeong Kyu - Seon)
	@date	2019/10/22  8:47
	*/
	void DrawRect(int l, int t, int r, int b, Scalar color, int thick);
	void DrawCircle(int cx, int cy, int r, Scalar color, int thick);
	void DrawClearScreen();

	double MeasureLine(int l, int t, int r, int b, Scalar color, int thick);
	double MeasureDiagonal(int l, int t, int r, int b, Scalar color, int thick);

private:
	// BMP 파일 입출력
	BOOL LoadIMG(const char* filename);
	BOOL SaveIMG(const char* filename);
	
private:
	LONG m_nWidth; /// 비트맵 가로 크기(픽셀 단위)
	LONG m_nHeight; /// 비트맵 세로 크기(픽셀 단위)
	WORD m_nChannel; /// 비트맵 채널
	WORD m_nBitCount; /// 픽셀 당 비트 수
	DWORD m_nDibSize; /// DIB 전체 크기(BITMAPINFOHEADER + 색상 테이블 + 픽셀 데이터)
	WORD m_nApend; /// DIB Apend 수
	BYTE* m_pDib; /// DIB 시작 주소(BITMAPINFOHEADER 시작 주소)
	Mat m_matImg; /// 최종 출력 이미지
	Mat m_matDraw; /// Overay Mat 이미지
	Mat m_matBack; /// Mat 원본 이미지
	Mat m_matRaw;
};

// 유용한 전역 함수 정의

template<typename T>
inline T limit(const T& value)
{
	return ((value > 255) ? 255 : ((value < 0) ? 0 : value));
}

template<typename T>
inline T limit(const T& value, const T& lower, const T& upper)
{
	return ((value > upper) ? upper : ((value < lower) ? lower : value));
}