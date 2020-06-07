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

// Computer Vision �˰��� ���� Part
//
// 2-1 ��� ���󿡼� ������׷� ���
// �Է� : ��� ���� f(j,i), 0<=j<=M-1, 0<=i<=N-1
// ��� : ������׷� h(I)�� ���� ������׷� h'(I), 0<=I<=L-1
//
// for(I=0 to L-1)	h(I)=0; //�ʱ�ȭ
// for(j=0 to M-1)
//  for(i=0 to N-1) // f�� ȭ�� (j,i) ������ ����
//   h(f(j,i))++;
// for(I=0 to L-1)
//  h'(I) = h(I)/(M*N); // ����ȭ�Ѵ�.
void g_fn_Histogram(unsigned char* pSrc, size_t nWidth, size_t nHeight, unsigned int* pHistogram);

// 2-2 2���� ������׷� ���(HS����)
// �Է� : H�� Sä�� ���� fH(j,i), fS(j,i), 0<=j<=M-1, 0<=i<=N-1
// ��� : ������׷� h(j,i)�� ���� ������׷� ^h(j,i), 0<=j,i<=q-1 // L�ܰ踦 q�ܰ�� ����ȭ
//
// h(j,i), 0<=j,i<=q-1�� 0���� �ʱ�ȭ �Ѵ�.
// for(j=0 to M-1)
//  for(i=0 to N-0) // ȭ�� (j,i) ������ ����
//   h(quantize(fH(j,i)), quantize(fS(j,i)))++;
// for(j=0 to q-1)
//  for(i=0 to q-1)
//   ^h(j,i)=h(j,i)/(M*N); // ����ȭ
void g_fn_Histogram2DCal_HS(unsigned char* pSrcH, unsigned char* pSrcS, size_t nWidth, size_t nHeight, unsigned int* pHistogram);

//  hr(j,i) = min(^hm(j,i)/^hi(j,i), 1.0), 0<=j,i <= q-1  ----- (2.4)
//�� quantize : ����ȭ, Scaling
// 2-3 ������׷� ������
// �Է� : H�� Sä�� ���� gH(j,i), gS(j,i), 0<=j<=M-1, 0<=i<=N-1 // ���� �����Ϸ��� ����
//       �� ������׷� ^hm(j,i), 0<=j,i<=q-1
// ��� : ���ɼ� �� o(j,i), 0<=j<=M-1, 0<=i<=N-1
//
// ���� gH, gS�� [�˰��� 2-2]�� �����Ͽ� ���� ������׷� ^hi�� �����.
// �� (2.4)�� �̿��Ͽ� ^hr�� ���Ѵ�.
// for(j=0 to M-1)
//  for(i=0 to N-1)
//   o(j,i)=^hr(quantize(gH(j,i)), quantize(gS(j,i))); // ������
void g_fn_InverseProjectionHistogram(unsigned char* pSrcH, unsigned char* pSrcS, unsigned char* pSrcHisto, size_t nWidth, size_t nHeight, unsigned char* pDst);

// b(j,i) = { 1, f(j,i) >= T                    ----- (2.5)
//			{ 0, f(j,i) <  T
//
// T = argmax vbetween(t) _t��{0,1,...,L-1}        ----(2.8)
// vbetween(t) = w0(t)(1-w0(t))(u0(t)-u1(t))^2
//
// �ʱ갪(t=0) : w0(0) = ^h(0), u0(0) = 0
// ��ȯ��(t>0) :
//  w0(t) = w0(t-1) + ^h(t)                     ----- (2.9)
//  u0(t) = (w0(t-1)u0(t-1) + t^h(t)) / w0(t)
//  u1(t) = (u - w0(t)u0(t)) / (1 - w0(t))
//
// 2-4 ���� �˰��� ȿ������ ����
// �Է� : ���� f(j,i), 0<=j<=M-1, 0<=i<=N-1
// ��� : ���� ���� b(j,i), 0<=j<=M-1, 0<=i<=N-1
//
// [�˰��� 2-1]�� �̿��Ͽ� f�� ���� ������׷� ^h�� �����.
// �� (2.9)�� �ʱ������� �̿��Ͽ� w0(0)�� u0(0)�� ����Ѵ�.
// for(t=1 to L-1){
//  �� (2.9)�� ��ȯ���� �̿��Ͽ� w0(t), u0(t), u1(t)�� ����Ѵ�.
//  �� (2.8)�� �̿��Ͽ� vbetween(t)�� ����Ѵ�.
// }
// ���� for �������� ����ū vbetween(t)�� ���� t�� �Ӱ谪T�� ���Ѵ�.
// �� (2.5)�� f�� ����ȭ�Ͽ� b�� �����.
void g_fn_OtsuAlgorithm(unsigned char* pSrc, size_t nWidth, size_t nHeight, unsigned char* pDst);


// 2-5 ���� ä��(4-���Ἲ ����)
// �Է� : ��������b(j,i), 0<=j<=M-1, 0<=i<=N-1
// ��� : ��ȣ�� �ű� ���� I(j,i), 0<=j<=M-1, 0<=i<=N-1
// b�� I�� �����Ѵ�. �̶� 0�� 0,1�� -1�� �����Ѵ�. // -1�� ���� ��ȣ�� �Ⱥٿ����� ǥ��
// I�� ���, ��j=0, j=M-1,i=0,i=N-1�� ȭ�Ҹ� 0���� �����Ѵ�. // ���� �ٱ����� ������ ���� ����
// label=1;
// for(j=1 to M-2)
//  for(i=1 to N-2){
//   if(I(j,i)=-1){
//    flood_fill4(I,j,i,label);
//    label++;
//   }
//  }
//
// //4-���Ἲ ���� ä�� �Լ�
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

// 2-6 ���� ä��(�޸� ���� ���� ����)
// �Է� : ���� ���� b(j,i), 0<=j<=M-1, 0<=i<=N-1
// ��� : ��ȣ�� �ű� ���� I(j,i), 0<=j<=M-1, 0<=i<=N-1
// b�� I�� �����Ѵ�. �̶� 0�� 0,1�� -1�� �����Ѵ�. // -1�� ���� ��ȣ�� �Ⱥٿ����� ǥ��
// I�� ���, �� j=0, j=M-1, i=0, i=N-1�� ȭ�Ҹ� 0���� �����Ѵ�. // ���� �ٱ����� ������ �� ����
// label=1;
// for(j=1 to M-2)
//  for(i=1 to N-2){
//   if(I(j,i)=-1){
//    efficient_floodfill4(I,j,i,label);
//    label++;
//   }
//  }
//
// //�޸𸮸� ���� ����ϴ� ȿ������ 4-���Ἲ ���� ä�� �Լ�
// function efficient_floodfill(4(I,j,i,label){
//  Q=��; // �� ť Q�� �����Ѵ�.
//  push(Q,(j,i));
//  while(Q != ��){
//   (y,x) = pop(Q); // Q���� ���Ҹ� �ϳ� ������.
//   if(I(y,x) = -1){
//    left=right=x;
//    while(I(y,left-1)=-1) left--; // ���� ��ó�� ������ ���� ã�´�.
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

// ǥ 2-1 ���� ��ȯ�� ���� ���� ���
// ��ȯ			�������H				����
//
//					  1  0  0
// �̵�  T(ty,tx) =	( 0  1  0 )		y�������� ty, x�������� tx��ŭ �̵�
//					  ty tx 1
//
//				 cos�� -sin�� 0
// ȸ��  R(��) = ( sin�� cos��  0 )		������ �߽����� �ð�������� �踸ŭ ȸ��
//				   0    0   1
//
//					 sy  0  0
// ũ��  S(sy,sx) = ( 0  sx  0 )		y�������� sy, x�������� sx��ŭ Ȯ��
//					 0   0  1
//
//					 1  0  0			   1  hx 0
// ����� Shy(hy) = ( hy 1  0 ) Shx(hx) = ( 0  1  0 )   Shy : y�������� hy��ŭ �����
//					 0  0  1			   0  0  1	   Shx : x�������� hx��ŭ �����
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

// 2-7 ���� ���� ��ȯ
// �Է� : ���� fsource(j,i), 0<=j<=M-1, 0<=i<=N-1, ��ȯ ��� H
// ��� : ���� ��ȯ�� ���� ftarget(j,i), 0<=j<=M-1, 0<=i<=N-1
// for(j=0 to M-1)
//  for(i=0 to N-1){
//   (j,i)�� H�� �����Ͽ� ��ȯ�� �� (j', i')�� ���Ѵ�. �Ǽ��� �ݿø��Ͽ� ������ �����.
//   ftarget(j', i')=fsource(j,i); // ���� ������ ��� ���� �����Ѵ�.
// }

// 2-8 �Ĺ� ���� ��ȯ
// �Է� : ���� fsource(j,i), 0<=j<=M-1, 0<=i<=N-1, ��ȯ ��� H
// ��� : ���� ��ȯ�� ���� ftarget(j,i), 0<=j<=M-1, 0<=i<=N-1
// for(j=0 to M-1)
//  for(i=0 to N-1){
//   (j,i)�� H^-1�� �����Ͽ� ��ȯ�� �� (j', i')�� ���Ѵ�. �Ǽ��� �ݿø��Ͽ� ������ �����.
//   ftarget(j,i)=fsource(j',i'); // ���� ������ ��� ���� �����Ѵ�.
// }
void g_fn_HomogeneousTranform(unsigned char* pSrc, int Width, int Height, unsigned char* pDst, HOMOGENEOUSMODE mode, int x, int y, double theta, bool bPrev = false);

// G(y,x) = (1 / 2*pi*��^2)*e^(-((y^2+x^2)/2*��^2))

// 3-1 Marr-Hildreth ���� ����
// �Է� : ���� f(j,i), 0<=j<=M-1, 0<=i<=N-1, ����þ��� ǥ������ ��
// ��� : ���� ���� b(j,i), 0<=j<=M-1, 0<=i<=N-1 //������ 1, ������ 0�� ���� ����
// �� ũ���� ����þ����� �Է� ���� f�� �������Ѵ�.
// ��� ���� ���ö�þ� �����ڸ� �����Ͽ� 2�� �̺��� ���Ѵ�.
// ��� ���󿡼� �������� ã�� ������ �����ϰ�, �������� ������ �����Ѵ�.

// 3-2 Marr-Hildreth ���� ����(LOG���� ���)
// �Է� : ���� f(j,i), 0<=j<=M-1, 0<=i<=N-1, ����þ��� ǥ������ ��
// ��� : ���� ���� b(j,i), 0<=j<=M-1, 0<=i<=N-1 // ������ 1, ������ 0�� ���� ����
// �� ũ���� LOG ���͸� �Է� ���� f�� �����Ѵ�.
// ��� ���󿡼� �������� ã�� ������ �����ϰ�, �������� ������ �����Ѵ�.

// LOG Filter
// ��^2G(y,x) = ((y^2 + x^2 - 2*��^2) / ��^4) * G(y,x)

// 3-3 ĳ�� ���� ����(����ġ ����)
// �Է� : ���� f(j,i), 0<=j<=M-1, 0<=i<=N-1, ����þ��� ǥ������ ��
// ��� : ���� ���� e(j,i), 0<=j<=M-1, 0<=i<=N-1 // ������ 1, ������ 0�� ���� ����
// �Է� ���� f�� �� ũ���� ����þ� �������� �����Ѵ�.
// ��� ���� �Һ� �����ڸ� �����Ͽ� ���� ������ ���� ���� ���� ���Ѵ�.
// ���ִ� ������ �����Ͽ� ���� �β� ���� ���� �����.
// �̷� �Ӱ谪�� �����Ͽ� ���� ������ �����Ѵ�.

// 3-4 ĳ�� ���� ����
// �Է� : ���� f(j,i), 0<=j<=M-1, 0<=i<=N-1, ����þ��� ǥ������ ��, �̷� �Ӱ谪 Thigh, Tlow
// ��� : ���� ���� e(j,i), 0<=j<=M-1, 0<=i<=N-1 // ������ 1, ������ 0�� ���� ����
// f�� ũ��  ���� ����þ��� �����Ͽ� g�� ��´�.
// g�� �Һ� �����ڸ� ���ꤷ�Ͽ�, ���� ������ S�� ���� ���� �� D�� ��´�. // D�� 8-���� ����ȭ
//
// //5~9��:���ִ� ����
// for(y=1 to M-2)
//  for(x=1 to N-2){
//   (y1,x1)�� (y2,x2)�� (y,x)�� �� �̿� ȭ�Ҷ� ����. // [�׸� 3-17] ����
//   if(S(y,x)<=S(y1,x1) or S(y,x)<=S(y2,x2))S(y,x)=0; //���ִ� ����
// }
//
// //12~16��:�̷� �Ӱ谪�� �̿��� ���� ����
// e(y,x) = 0, 0<=y<=M-1,0<=x<=N-1;
// visited(y,x)=0,0<=y<=M-1,0<=x<=N-1; // ��� ȭ�Ұ� ���� �湮 �ȵ��� ǥ��
// for(y=1 to M-2)
//  for(x=1 to N-2)
//   if(S(y,x)>Thigh and visited(y,x)=0) follow_edge(y,x);
//
// // ������ �����ϴ� ����Լ�(�迭�� ��� ���������� ����)
// function flow_edge(y,x) {
//  visited(y,x) = 1;
//  e(y,x) = 1;
//  for(y,x)�� 8�̿� (ny,nx) ������ ����)
//   if(S(ny,nx>Tlow and visited(y,x)= follow_edge(ny,nx)
// }

// 3-5 SPTA(������ �� �� ����� ����)
// �Է� : ���� ���� e(j,i), 0<=j<=M-1, 0<=i<=N-1 // ������ 1, ������ 0�� ���� ����
// ��� : �� �β� ����� ���� ���� eout(j,i), 0<=j<=M-1, 0<=i<=N-1
// e�� eout�� �����Ѵ�.
// for(j=1 to M-2)
//  for(i=1 to N-2){
//   if(e(j,i)=1 and
//    ((n0' and(n4 and(n5 or n6 or n2 or n3) and (n6 or n'7)and(n2 or n'1))) // n0=����, s0=��
//    or (n'4 and (n0 and (n1 or n2 or n6 or n7)and(n2 or n'3)and(n6 or n'5))) // n4=����, s4=��
//    or (n'2 and(n6 and(n7 or n0 or n4 or n5)and(n0 or n'1)and(n4 or n'3))) // n2=����, s2=��
//    or (n'6 and (n2 and (n3 or n4 or n0 or n1)and(n4 or n'5)and(n0 or n'7))))) // n6=����, s6=��
//    eout(j,i)=0;
// }

// 3-6 ���� �丷 ����
// �Է� : ���� ���� e(j,i), 0<=j<=M-1, 0<=i<=N-1
// ��� : segment(k), 1<=k<=n(n���� ���� �丷)
// Q=��; // ť�� �����ϰ� ���������� �ʱ�ȭ
// for(j=1 to M-2)
//  for(i=1 to N-2){
//   (j,i)�� ��ȯ Ƚ���� ���� c�� �Ѵ�.
//   if(c=1 or c>=3) // ���� �Ǵ� �б���
//    for(dir=0 to 7) // 8-������ �����Ͽ� ���� ������ ���ϰ� ť�� ����
//     if(dir ������ �̿� ȭ�� (y,x)�� e(y,x)=1) anddQueue(Q,(j,i,dir));
//  }
//
// // ť�� ����ִ� ��ҿ��� ���� ������ ����(���� �丷 ���� ����)
// n=0; // ���� �丷�� ����
// visited(j,i)=0, 0<=j<=M-1, 0<=i<=N-1; // ���� ������ �����ϱ� ���� ���(�湮�ϸ� 1�� �ٲ�)
// while(Q����) {
//  (y,x,dir)=popQueue(Q);
//  (y,x)�� dir������ �̿��� (cy,cx)�� ����.
//  if visited(cy,cs) continue; // �ݴ��ʿ��� �̹� ��������.
//  n++;
//  segments(n)�� �����ϰ� (y,x)�� (cy,cx)�� ����.
//  visited(y,x)=visited(cy,cx) = 1; // �湮������ ǥ��
//  if((cy,cx)�� ���� �Ǵ� �б���) continue; // �� ������ �����Ǵ� ª�� ���� �丷��
//  while(true){
//   (cy,cx)�� dir ������ ���� ȭ�Ҹ� �����Ѵ�. // [�׸� 3-26]���� fǥ�õ� ȭ�ҵ�
//   if(���� �Ǵ� �б��� (y,x)�� ������){
//    segments(n)�� (y,x)�� �߰��Ѵ�.
//    visited(y,x)=1;
//    break; // segments(n)�� ������ ������
//   }
//   else { // ��ȯ Ƚ���� 2�� �����
//    ���� ȭ�� �� e(y,x)=1�� ȭ�Ҹ� ã�´�.
//    segments(n)�� (y,x)�� �߰��Ѵ�.
//    visited(y,x)=1;
//    dir�� (cy,cx)���� (y,x)�� �����ϴ� �������� �����ϴ�.
//    (cy,cx)=(y,x); // ������ �̵�
//   }
//  }
// }
