#pragma once
#include "define_Structure.h"
#include <queue>

const int N_CORRRANGE = 16;
const int N_SAMPLINGCOUNT = 3000;

void g_fn_CrossCorr(unsigned char* input1, unsigned char* input2, short len1, short len2, unsigned char* output);
void g_fn_CrossCorr(unsigned char* input1, unsigned char* input2, short len1, short len2, unsigned int* output);
double g_fn_Get2PointLength(Point pnt1, Point pnt2);
double g_fn_Get2PointLength(int x1, int y1, int x2, int y2);
Point2f g_fn_GetCircleCenter(Point2f pnt1, Point2f pnt2, Point2f pnt3);
Mat g_fn_fittingCircle_RANSAC(Mat ROImatImage, Point& pnt, int& radius);
Mat g_fn_fittingCircle_LMS(Mat ROImatImage, Point& pnt, int& radius);
Mat g_fn_fittingLine_RANSAC(Mat ROImatImage, LINE_PARAM& stLine);
Mat g_fn_fittingLine_LMS(Mat ROImatImage, LINE_PARAM& stLine);
Mat g_fn_fittingElipse(Mat ROImatImage, ELLIPSE_PARAM& stEllipse);
Mat g_fn_fittingElipse_RANSAC(Mat ROImatImage, ELLIPSE_PARAM& stEllipse);

LINE_PARAM g_fn_GetLineParam(Point2f pnt1, Point2f pnt2);
Point2f g_fn_GetLineIntersectionPoint(LINE_PARAM param1, LINE_PARAM param2);
Point2f g_fn_CheckCrossLine(Point2f pnt1, Point2f pnt2, Point2f pnt3, Point2f pnt4);

int g_fn_GetRandomNumber(int nMax);

// Computer Vision 알고리즘 구현 Part
//
// 2-1 명암 영상에서 히스토그램 계산
// 입력 : 명암 영상 f(j,i), 0<=j<=M-1, 0<=i<=N-1
// 출력 : 히스토그램 h(I)과 정규 히스토그램 h'(I), 0<=I<=L-1
//
// for(I=0 to L-1)	h(I)=0; //초기화
// for(j=0 to M-1)
//  for(i=0 to N-1) // f의 화소 (j,i) 각각에 대해
//   h(f(j,i))++;
// for(I=0 to L-1)
//  h'(I) = h(I)/(M*N); // 정규화한다.
void g_fn_Histogram(unsigned char* pSrc, size_t nWidth, size_t nHeight, unsigned int* pHistogram);

// 2-2 2차원 히스토그램 계산(HS공간)
// 입력 : H와 S채널 영상 fH(j,i), fS(j,i), 0<=j<=M-1, 0<=i<=N-1
// 출력 : 히스토그램 h(j,i)와 정규 히스토그램 ^h(j,i), 0<=j,i<=q-1 // L단계를 q단계로 양자화
//
// h(j,i), 0<=j,i<=q-1을 0으로 초기화 한다.
// for(j=0 to M-1)
//  for(i=0 to N-0) // 화소 (j,i) 각각에 대해
//   h(quantize(fH(j,i)), quantize(fS(j,i)))++;
// for(j=0 to q-1)
//  for(i=0 to q-1)
//   ^h(j,i)=h(j,i)/(M*N); // 정규화
void g_fn_Histogram2DCal_HS(unsigned char* pSrcH, unsigned char* pSrcS, size_t nWidth, size_t nHeight, unsigned int* pHistogram);

//  hr(j,i) = min(^hm(j,i)/^hi(j,i), 1.0), 0<=j,i <= q-1  ----- (2.4)
//※ quantize : 양자화, Scaling
// 2-3 히스토그램 역투영
// 입력 : H와 S채널 영상 gH(j,i), gS(j,i), 0<=j<=M-1, 0<=i<=N-1 // 얼굴을 검출하려는 영상
//       모델 히스토그램 ^hm(j,i), 0<=j,i<=q-1
// 출력 : 가능성 맵 o(j,i), 0<=j<=M-1, 0<=i<=N-1
//
// 영상 gH, gS에 [알고리즘 2-2]를 적용하여 정규 히스토그램 ^hi를 만든다.
// 식 (2.4)를 이용하여 ^hr을 구한다.
// for(j=0 to M-1)
//  for(i=0 to N-1)
//   o(j,i)=^hr(quantize(gH(j,i)), quantize(gS(j,i))); // 역투영
void g_fn_InverseProjectionHistogram(unsigned char* pSrcH, unsigned char* pSrcS, unsigned char* pSrcHisto, size_t nWidth, size_t nHeight, unsigned char* pDst);

// b(j,i) = { 1, f(j,i) >= T                    ----- (2.5)
//			{ 0, f(j,i) <  T
//
// T = argmax vbetween(t) _t∈{0,1,...,L-1}        ----(2.8)
// vbetween(t) = w0(t)(1-w0(t))(u0(t)-u1(t))^2
//
// 초깃값(t=0) : w0(0) = ^h(0), u0(0) = 0
// 순환식(t>0) :
//  w0(t) = w0(t-1) + ^h(t)                     ----- (2.9)
//  u0(t) = (w0(t-1)u0(t-1) + t^h(t)) / w0(t)
//  u1(t) = (u - w0(t)u0(t)) / (1 - w0(t))
//
// 2-4 오츄 알고리즘 효율적인 버전
// 입력 : 영상 f(j,i), 0<=j<=M-1, 0<=i<=N-1
// 출력 : 이진 영상 b(j,i), 0<=j<=M-1, 0<=i<=N-1
//
// [알고리즘 2-1]을 이용하여 f의 정규 히스토그램 ^h을 만든다.
// 식 (2.9)의 초기조건을 이용하여 w0(0)과 u0(0)을 계산한다.
// for(t=1 to L-1){
//  식 (2.9)의 순환식을 이용하여 w0(t), u0(t), u1(t)를 계산한다.
//  식 (2.8)을 이용하여 vbetween(t)를 계산한다.
// }
// 앞의 for 루프에서 가장큰 vbetween(t)를 보인 t를 임계값T로 취한다.
// 식 (2.5)로 f를 이진화하여 b를 만든다.
void g_fn_OtsuAlgorithm(unsigned char* pSrc, size_t nWidth, size_t nHeight, unsigned char* pDst);


// 2-5 범람 채움(4-연결성 버전)
// 입력 : 이진영상b(j,i), 0<=j<=M-1, 0<=i<=N-1
// 출력 : 번호를 매긴 영상 I(j,i), 0<=j<=M-1, 0<=i<=N-1
// b를 I로 복사한다. 이때 0은 0,1은 -1로 복사한다. // -1은 아직 번호를 안붙였음을 표시
// I의 경계, 즉j=0, j=M-1,i=0,i=N-1인 화소를 0으로 설정한다. // 영상 바깥으로 나가는 것을 방지
// label=1;
// for(j=1 to M-2)
//  for(i=1 to N-2){
//   if(I(j,i)=-1){
//    flood_fill4(I,j,i,label);
//    label++;
//   }
//  }
//
// //4-연결성 범람 채움 함수
// function flood_fill4(I,j,i,label){
//  if(I(j,i)=-1){
//   I(j,i)=lable;
//   flood_fill4(I,j,i+1,label); //east
//   flood_fill4(I,j-1,i,label); //north
//   flood_fill4(I,j,i-1,label); //west
//   flood_fill4(I,J+1,i,label); //south
//  }
// }
void g_floodfill4(char* pDst, size_t szStepX, size_t szStepY, size_t nWid, int label);

void g_fn_LineTracking(unsigned char* pSrc, size_t nWidth, size_t nHeight, char* pDst);

// 2-6 범람 채움(메모리 적게 쓰는 버전)
// 입력 : 이진 영상 b(j,i), 0<=j<=M-1, 0<=i<=N-1
// 출력 : 번호를 매긴 영상 I(j,i), 0<=j<=M-1, 0<=i<=N-1
// b를 I로 복사한다. 이때 0은 0,1은 -1로 복사한다. // -1은 아직 번호를 안붙였음을 표시
// I의 경계, 즉 j=0, j=M-1, i=0, i=N-1인 화소를 0으로 설정한다. // 영상 바깥으로 나가는 것 방지
// label=1;
// for(j=1 to M-2)
//  for(i=1 to N-2){
//   if(I(j,i)=-1){
//    efficient_floodfill4(I,j,i,label);
//    label++;
//   }
//  }
//
// //메모리를 적게 사용하는 효율적인 4-연결성 범람 채움 함수
// function efficient_floodfill(4(I,j,i,label){
//  Q=Φ; // 빈 큐 Q를 생성한다.
//  push(Q,(j,i));
//  while(Q != Φ){
//   (y,x) = pop(Q); // Q에서 원소를 하나 꺼낸다.
//   if(I(y,x) = -1){
//    left=right=x;
//    while(I(y,left-1)=-1) left--; // 아직 미처리 상태인 열을 찾는다.
//    while(I(y, right+1)=-1) right++;
//    for(c=left to right){
//     I(y,c)=label;
//     if(I(y-1,c)=-1 and (c=left or I(y-1,c-1) != -1)) push(Q,(y-1,c));
//     if(I(y+1,c)=-1 and (c=left or I(y+1,c-1) != -1)) push(Q,(y+1,c));
//    }
//   }
//  }
// }
void g_efficientfloodfill4(char* pDst, size_t szStepX, size_t szStepY, size_t nWid, int label);

void g_fn_LineTrackingEnhanced(unsigned char* pSrc, size_t nWidth, size_t nHeight, char* pDst);

// 표 2-1 기하 변환을 위한 동차 행렬
// 변환			동차행렬H				설명
//
//					  1  0  0
// 이동  T(ty,tx) =	( 0  1  0 )		y방향으로 ty, x방향으로 tx만큼 이동
//					  ty tx 1
//
//				 cosθ -sinθ 0
// 회전  R(θ) = ( sinθ cosθ  0 )		원점을 중심으로 시계방향으로 θ만큼 회전
//				   0    0   1
//
//					 sy  0  0
// 크기  S(sy,sx) = ( 0  sx  0 )		y방향으로 sy, x방향으로 sx만큼 확대
//					 0   0  1
//
//					 1  0  0			   1  hx 0
// 기울임 Shy(hy) = ( hy 1  0 ) Shx(hx) = ( 0  1  0 )   Shy : y방향으로 hy만큼 기울임
//					 0  0  1			   0  0  1	   Shx : x방향으로 hx만큼 기울임
//
//						  a11 a12 0
// (y', x', 1) = (y x 1)( a21 a22 0 )
//						  a31 a32 1
// y' = y*a11 + x*a21 + a31
// x' = y*a21 + x*a22 + a32
typedef enum EN_HOMOGENEOUSMODE
{
	EFM_MOVE = 0,
	EFM_ROTATE,
	EFM_SIZE,
	EFM_TILT
}HOMOGENEOUSMODE;

void g_fn_GetHomogeneousMatrix(double* filter, HOMOGENEOUSMODE mode, int x, int y, double theta);

// 2-7 전방 기하 변환
// 입력 : 영상 fsource(j,i), 0<=j<=M-1, 0<=i<=N-1, 변환 행렬 H
// 출력 : 기하 변환된 영상 ftarget(j,i), 0<=j<=M-1, 0<=i<=N-1
// for(j=0 to M-1)
//  for(i=0 to N-1){
//   (j,i)에 H를 적용하여 변환된 점 (j', i')를 구한다. 실수는 반올림하여 정수로 만든다.
//   ftarget(j', i')=fsource(j,i); // 영상 공간을 벗어난 점은 무시한다.
// }

// 2-8 후방 기하 변환
// 입력 : 영상 fsource(j,i), 0<=j<=M-1, 0<=i<=N-1, 변환 행렬 H
// 출력 : 기하 변환된 영상 ftarget(j,i), 0<=j<=M-1, 0<=i<=N-1
// for(j=0 to M-1)
//  for(i=0 to N-1){
//   (j,i)에 H^-1을 적용하여 변환된 점 (j', i')를 구한다. 실수는 반올림하여 정수를 만든다.
//   ftarget(j,i)=fsource(j',i'); // 영상 공간을 벗어난 점은 무시한다.
// }
void g_fn_HomogeneousTranform(unsigned char* pSrc, int Width, int Height, unsigned char* pDst, HOMOGENEOUSMODE mode, int x, int y, double theta, bool bPrev = false);

// G(y,x) = (1 / 2*pi*σ^2)*e^(-((y^2+x^2)/2*σ^2))

// 3-1 Marr-Hildreth 에지 검출
// 입력 : 영상 f(j,i), 0<=j<=M-1, 0<=i<=N-1, 가우시안의 표준편차 σ
// 출력 : 에지 영상 b(j,i), 0<=j<=M-1, 0<=i<=N-1 //에지는 1, 비에지는 0인 이진 영상
// σ 크기의 가우시안으로 입력 영상 f를 스무딩한다.
// 결과 영상에 라플라시안 연산자를 적용하여 2차 미분을 구한다.
// 결과 영상에서 영교차를 찾아 에지로 설정하고, 나머지는 비에지로 설정한다.

// 3-2 Marr-Hildreth 에지 검출(LOG필터 사용)
// 입력 : 영상 f(j,i), 0<=j<=M-1, 0<=i<=N-1, 가우시안의 표준편차 σ
// 출력 : 에지 영상 b(j,i), 0<=j<=M-1, 0<=i<=N-1 // 에지는 1, 비에지는 0인 이진 영상
// σ 크기의 LOG 필터를 입력 영상 f에 적용한다.
// 결과 영상에서 영교차를 찾아 에지로 설정하고, 나머지는 비에지로 설정한다.

// LOG Filter
// ∇^2G(y,x) = ((y^2 + x^2 - 2*σ^2) / σ^4) * G(y,x)

// 3-3 캐니 에지 검출(스케치 버전)
// 입력 : 영상 f(j,i), 0<=j<=M-1, 0<=i<=N-1, 가우시안의 표준편차 σ
// 출력 : 에지 영상 e(j,i), 0<=j<=M-1, 0<=i<=N-1 // 에지는 1, 비에지는 0인 이진 영상
// 입력 영상 f에 σ 크기의 가우시안 스무딩을 적용한다.
// 결과 영상에 소벨 연산자를 적용하여 에지 강도와 에지 방향 맵을 구한다.
// 비최대 억제를 적용하여 얇은 두께 에지 뱁을 만든다.
// 이력 임계값을 적용하여 거짓 긍정을 제거한다.

// 3-4 캐니 에지 검출
// 입력 : 영상 f(j,i), 0<=j<=M-1, 0<=i<=N-1, 가우시안의 표준편차 σ, 이력 임계값 Thigh, Tlow
// 출력 : 에지 영상 e(j,i), 0<=j<=M-1, 0<=i<=N-1 // 에지는 1, 비에지는 0인 이진 영상
// f에 크기  σ인 가우시안을 적용하여 g를 얻는다.
// g에 소벨 연산자를 적욧ㅇ하여, 에지 강도맵 S와 에지 방향 맵 D를 얻는다. // D는 8-방향 양자화
//
// //5~9행:비최대 억제
// for(y=1 to M-2)
//  for(x=1 to N-2){
//   (y1,x1)과 (y2,x2)를 (y,x)의 두 이웃 화소라 하자. // [그림 3-17] 참고
//   if(S(y,x)<=S(y1,x1) or S(y,x)<=S(y2,x2))S(y,x)=0; //비최대 억제
// }
//
// //12~16행:이력 임계값을 이용한 에지 추적
// e(y,x) = 0, 0<=y<=M-1,0<=x<=N-1;
// visited(y,x)=0,0<=y<=M-1,0<=x<=N-1; // 모든 화소가 아직 방문 안됨을 표시
// for(y=1 to M-2)
//  for(x=1 to N-2)
//   if(S(y,x)>Thigh and visited(y,x)=0) follow_edge(y,x);
//
// // 에지를 추적하는 재귀함수(배열은 모두 전역변수라 가정)
// function flow_edge(y,x) {
//  visited(y,x) = 1;
//  e(y,x) = 1;
//  for(y,x)의 8이웃 (ny,nx) 각각에 대해)
//   if(S(ny,nx>Tlow and visited(y,x)= follow_edge(ny,nx)
// }

// 3-5 SPTA(껍질을 한 번 벗기는 연산)
// 입력 : 에지 영상 e(j,i), 0<=j<=M-1, 0<=i<=N-1 // 에지는 1, 비에지는 0인 이진 영상
// 출력 : 한 두께 얇아진 에지 영상 eout(j,i), 0<=j<=M-1, 0<=i<=N-1
// e를 eout에 복사한다.
// for(j=1 to M-2)
//  for(i=1 to N-2){
//   if(e(j,i)=1 and
//    ((n0' and(n4 and(n5 or n6 or n2 or n3) and (n6 or n'7)and(n2 or n'1))) // n0=비에지, s0=참
//    or (n'4 and (n0 and (n1 or n2 or n6 or n7)and(n2 or n'3)and(n6 or n'5))) // n4=비에지, s4=참
//    or (n'2 and(n6 and(n7 or n0 or n4 or n5)and(n0 or n'1)and(n4 or n'3))) // n2=비에지, s2=참
//    or (n'6 and (n2 and (n3 or n4 or n0 or n1)and(n4 or n'5)and(n0 or n'7))))) // n6=비에지, s6=참
//    eout(j,i)=0;
// }

// 3-6 에지 토막 검출
// 입력 : 에지 영상 e(j,i), 0<=j<=M-1, 0<=i<=N-1
// 출력 : segment(k), 1<=k<=n(n개의 에지 토막)
// Q=Φ; // 큐를 생성하고 공집합으로 초기화
// for(j=1 to M-2)
//  for(i=1 to N-2){
//   (j,i)의 전환 횟수를 세어 c라 한다.
//   if(c=1 or c>=3) // 끝점 또는 분기점
//    for(dir=0 to 7) // 8-방향을 조사하여 추적 방향을 정하고 큐에 저장
//     if(dir 방향의 이웃 화소 (y,x)가 e(y,x)=1) anddQueue(Q,(j,i,dir));
//  }
//
// // 큐에 들어있는 요소에서 에지 추적을 시작(에지 토막 추적 시작)
// n=0; // 에지 토막의 개수
// visited(j,i)=0, 0<=j<=M-1, 0<=i<=N-1; // 이중 추적을 방지하기 위해 사용(방문하면 1로 바꿈)
// while(Q≠Φ) {
//  (y,x,dir)=popQueue(Q);
//  (y,x)의 dir방향의 이웃을 (cy,cx)라 하자.
//  if visited(cy,cs) continue; // 반대쪽에서 이미 추적했음.
//  n++;
//  segments(n)을 생성하고 (y,x)와 (cy,cx)라 하자.
//  visited(y,x)=visited(cy,cx) = 1; // 방문했음을 표시
//  if((cy,cx)가 끝점 또는 분기점) continue; // 두 점으로 구성되는 짧은 에지 토막임
//  while(true){
//   (cy,cx)의 dir 방향의 전방 화소를 조사한다. // [그림 3-26]에서 f표시된 화소들
//   if(끝점 또는 분기점 (y,x)가 있으면){
//    segments(n)에 (y,x)를 추가한다.
//    visited(y,x)=1;
//    break; // segments(n)의 추적이 끝났음
//   }
//   else { // 전환 횟수가 2인 통과점
//    전방 화소 중 e(y,x)=1인 화소를 찾는다.
//    segments(n)에 (y,x)를 추가한다.
//    visited(y,x)=1;
//    dir을 (cy,cx)에서 (y,x)로 진행하는 방향으로 갱신하다.
//    (cy,cx)=(y,x); // 앞으로 이동
//   }
//  }
// }
