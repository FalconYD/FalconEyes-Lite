#include "stdafx.h"
#include "MatColor.h"
#include "MatDibFeature.h"

void RGB_TO_HSI(double R, double G, double B, double& H, double& S, double& I)
{
	I = (R + G + B) / 3.;
	if ((R == G) && (G == B)) // 그레이 스케일인 경우
	{
		S = 0;
		H = 0;
	}
	else
	{
		double min_rgb;
		double temp;

		min_rgb = std::min(std::min(R, G), B);
		S = 1 - (min_rgb / I);
		temp = ((R - G) + (R - B)) / (2 * sqrt((R - G)*(R - G) + (R - B)*(G - B)));

		H = acos(temp) * 180 / CV_PI;

		if (B > G)
			H = 360 - H;

		H /= 360.;
	}
}

void HSI_TO_RGB(double H, double S, double I, double& R, double& G, double& B)
{
	if (I == 0.0) // 검정색
	{
		R = 0.0;
		G = 0.0;
		B = 0.0;
		return;
	}

	if (S == 0.0)// 그레이 스케일
	{
		R = I;
		G = I;
		B = I;
		return;
	}

	H *= 360;

	if (H <= 120)
	{
		B = I * (1 - S);
		R = I * (1 + S * cos(H*CV_PI / 180) / cos((60 - H)*CV_PI / 180));
		R = limit(R, 0., 1.);
		G = 3 * I - (R + B);
	}
	else if (H <= 240)
	{
		H -= 120;

		R = I * (1 - S);
		G = I * (1 + S * cos(H*CV_PI / 180) / cos((60 - H)*CV_PI / 180));
		G = limit(G, 0., 1.);
		B = 3 * I - (R + G);
	}
	else
	{
		H -= 240;

		G = I * (1 - S);
		B = I * (1 + S * cos(H*CV_PI / 180) / cos((60 - H)*CV_PI / 180));
		B = limit(B, 0., 1.);
		R = 3 * I - (G + B);
	}
}

//------------------------------
// R, G, B, Y, U V 의 범위는 [0, 255]
//------------------------------
void RGB_TO_YUV(BYTE R, BYTE G, BYTE B, BYTE& Y, BYTE& U, BYTE& V)
{
	Y = (BYTE)limit(0.299*R + 0.587*G + 0.144*B + 0.5);
	U = (BYTE)limit(-0.169*R - 0.331*G + 0.500*B + 128 + 0.5);
	V = (BYTE)limit(0.500*R - 0.419*G - 0.081*B + 128 + 0.5);
}

//------------------------------
// R, G, B, Y, U, V 의 범위는 [0, 255]
//------------------------------
void YUV_TO_RGB(BYTE Y, BYTE U, BYTE V, BYTE& R, BYTE& G, BYTE& B)
{
	R = (BYTE)limit(Y + 1.4075*(V - 128) + 0.5);
	G = (BYTE)limit(Y - 0.3455*(U - 128) - 0.7169*(V - 128) + 0.5);
	B = (BYTE)limit(Y + 1.7790*(U - 128) + 0.5);
}

void MatColorSplitRGB(Mat& matColor, Mat& matR, Mat& matG, Mat& matB)
{
	int w = matColor.cols;
	int h = matColor.rows;
	int size = matColor.cols * matColor.rows * matColor.channels();

	matR = Mat::zeros(Size(w, h), CV_8UC1);
	matG = Mat::zeros(Size(w, h), CV_8UC1);
	matB = Mat::zeros(Size(w, h), CV_8UC1);
	int i = 0, j = 0;
	for (i = 0; i < size; i+=3)
	{
		matR.data[j] = matColor.data[i+2];
		matG.data[j] = matColor.data[i+1];
		matB.data[j] = matColor.data[i+0];
		j++;
	}
}

void MatColorSplitHSI(Mat& matColor, Mat& matH, Mat& matS, Mat& matI)
{
	int w = matColor.cols;
	int h = matColor.rows;
	int size = matColor.cols * matColor.rows * matColor.channels();

	matH = Mat::zeros(Size(w, h), CV_8UC1);
	matS = Mat::zeros(Size(w, h), CV_8UC1);
	matI = Mat::zeros(Size(w, h), CV_8UC1);

	double r, g, b, hh, ss, ii;
	int i = 0, j = 0;
	for (i = 0; i < size; i+=3)
	{
		r = matColor.data[i] / 255.;
		g = matColor.data[i+1] / 255.;
		b = matColor.data[i+2] / 255.;

		RGB_TO_HSI(r, g, b, hh, ss, ii);

		matH.data[j] = static_cast<BYTE>(limit(hh * 255 + 0.5));
		matS.data[j] = static_cast<BYTE>(limit(ss * 255 + 0.5));
		matI.data[j] = static_cast<BYTE>(limit(ii * 255 + 0.5));
		j++;
	}
}

void MatColorSplitYUV(Mat& matColor, Mat& matY, Mat& matU, Mat& matV)
{
	int w = matColor.cols;
	int h = matColor.rows;
	int size = matColor.cols * matColor.rows * matColor.channels();

	matY = Mat::zeros(Size(w, h), CV_8UC1);
	matU = Mat::zeros(Size(w, h), CV_8UC1);
	matV = Mat::zeros(Size(w, h), CV_8UC1);

	int i = 0, j = 0;
	for (i = 0; i < size; i+=3)
	{
		RGB_TO_YUV(matColor.data[i], matColor.data[i+1], matColor.data[i+2], matY.data[j], matU.data[j], matV.data[j]);
		j++;
	}
}

BOOL MatColorCombineRGB(Mat& matR, Mat& matG, Mat& matB, Mat& matColor)
{
	int w = matR.cols;
	int h = matR.rows;
	int size = matR.cols * matR.rows * matR.channels();

	if (matG.cols != w || matG.rows != h ||
		matB.cols != w || matB.rows != h)
		return FALSE;

	matColor = Mat::zeros(Size(w, h), CV_8UC3);
	int j = 0;
	for (int i = 0; i < size; i++)
	{
		matColor.data[j] = matR.data[i];
		matColor.data[j + 1] = matG.data[i];
		matColor.data[j + 2] = matB.data[i];
		j+=3;
	}

	return TRUE;
}

BOOL MatColorCombineHSI(Mat& matH, Mat& matS, Mat& matI, Mat& matColor)
{
	int w = matH.cols;
	int h = matH.rows;
	int size = matH.cols * matH.rows * matH.channels();

	if (matS.cols != w || matS.rows != h ||
		matI.cols != w || matI.rows != h)
		return FALSE;

	matColor = Mat::zeros(Size(w, h), CV_8UC3);
	int j = 0;
	double r, g, b, hh, ss, ii;
	for (int i = 0; i < size; i++)
	{
		hh = matH.data[i] / 255.;
		ss = matS.data[i] / 255.;
		ii = matI.data[i] / 255.;

		HSI_TO_RGB(hh, ss, ii, r, g, b);
		matColor.data[j] = static_cast<BYTE>(limit(r * 255 + 0.5));
		matColor.data[j + 1] = static_cast<BYTE>(limit(g * 255 + 0.5));
		matColor.data[j + 2] = static_cast<BYTE>(limit(b * 255 + 0.5));
		j+=3;
	}

	return TRUE;
}

BOOL MatColorCombineYUV(Mat& matY, Mat& matU, Mat& matV, Mat& matColor)
{
	int w = matY.cols;
	int h = matY.rows;
	int size = matY.cols * matY.rows * matY.channels();

	if (matU.cols != w || matU.rows != h ||
		matV.cols != w || matV.rows != h)
		return FALSE;

	matColor = Mat::zeros(Size(w, h), CV_8UC3);
	int j = 0;
	for (int i = 0; i < size; i++)
	{
		YUV_TO_RGB(matY.data[i], matU.data[i], matV.data[i], matColor.data[j], matColor.data[j + 1], matColor.data[j + 2]);
		j+=3;
	}

	return TRUE;
}

void MatColorEdge(Mat& matSrc, Mat& matEdge)
{
	Mat matY, matU, matV;
	MatColorSplitYUV(matSrc, matY, matU, matV);

	Mat edgeY, edgeU, edgeV;
	MatDib tmpDib;
	tmpDib.SetMatImg(matY);
	DibEdgePrewitt(tmpDib, tmpDib);
	edgeY = tmpDib.GetMatImg();

	tmpDib.SetMatImg(matU);
	DibEdgePrewitt(tmpDib, tmpDib);
	edgeU = tmpDib.GetMatImg();

	tmpDib.SetMatImg(matV);
	DibEdgePrewitt(tmpDib, tmpDib);
	edgeV = tmpDib.GetMatImg();

	int w = matSrc.cols;
	int h = matSrc.rows;
	//int size = matSrc.cols * matSrc.rows * matSrc.channels();
	int size = matSrc.cols * matSrc.rows;

	matEdge = Mat::zeros(Size(w, h), CV_8UC1);

	double dist;
	for (int i = 0; i < size; i++)
	{
		dist = (edgeY.data[i] * edgeY.data[i]) + (0.5*edgeU.data[i]) * (0.5*edgeU.data[i]) + (0.5*edgeV.data[i]) * (0.5*edgeV.data[i]);
		matEdge.data[i] = static_cast<BYTE>(limit(sqrt(dist)));
	}
}