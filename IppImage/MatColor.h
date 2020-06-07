#pragma once
#include "MatDib.h"

void RGB_TO_HSI(double R, double G, double B, double& H, double& S, double& I);
void HSI_TO_RGB(double H, double S, double I, double& R, double& G, double& B);
void RGB_TO_YUV(BYTE R, BYTE G, BYTE B, BYTE& Y, BYTE& U, BYTE& V);
void YUV_TO_RGB(BYTE Y, BYTE U, BYTE V, BYTE& R, BYTE& G, BYTE& B);
void MatColorSplitRGB(Mat& matColor, Mat& matR, Mat& matG, Mat& matB);
void MatColorSplitHSI(Mat& matColor, Mat& matH, Mat& matS, Mat& matI);
void MatColorSplitYUV(Mat& matColor, Mat& matY, Mat& matU, Mat& matV);
BOOL MatColorCombineRGB(Mat& matR, Mat& matG, Mat& matB, Mat& matColor);
BOOL MatColorCombineHSI(Mat& matH, Mat& matS, Mat& matI, Mat& matColor);
BOOL MatColorCombineYUV(Mat& matY, Mat& matU, Mat& matV, Mat& matColor);
void MatColorEdge(Mat& matSrc, Mat& matEdge);