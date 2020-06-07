#pragma once

#include "MatDib.h"
#include <vector>


class MatLabelInfo
{
public:
	std::vector<Point> pixels;
	int cx, cy;
	int minx, miny, maxx, maxy;
public:
	MatLabelInfo() : cx(0), cy(0), minx(INT_MAX), miny(INT_MAX), maxx(0), maxy(0)
	{
		pixels.clear();
	}
};

void MatBinarization(MatDib& Src, MatDib& Dst, int threshold);
int MatBinarizationIterative(MatDib& Src);
int MatLabeling(Mat& matSrc, std::vector<MatLabelInfo>& labels);
int MatLabeling(MatDib& dibSrc, std::vector<MatLabelInfo>& labels);
void MatContourTracing(MatDib& dibSrc, int sx, int sy, std::vector<Point>& cp);
void MatMorphologyErosion(Mat& matSrc, Mat& matDst);
void MatMorphologyDilation(Mat& matSrc, Mat& matDst);
void MatMorphologyOpening(Mat& matSrc, Mat& matDst);
void MatMorphologyClosing(Mat& matSrc, Mat& matDst);
void MatMorphologyGrayErosion(Mat& matSrc, Mat& matDst);
void MatMorphologyGrayDilation(Mat& matSrc, Mat& matDst);
void MatMorphologyGrayOpening(Mat& matSrc, Mat& matDst);
void MatMorphologyGrayClosing(Mat& matSrc, Mat& matDst);

void MatBinarizationCVOtsu(MatDib& Src, MatDib& Dst);
void MatBinarizationCVAdaptive(MatDib& Src, MatDib& Dst);

void MatFourierDescriptor(MatDib& dibSrc, int sx, int syu, int percent, std::vector<Point>& cp);

void MatInvariantMoments(MatDib& dibSrc, double m[7]);
double MatGeometricMoment(MatDib& dibSrc, int p, int q);

bool MatZernikeMoments(MatDib& dibSrc, int n, int m, double& zr, double& zi);
Point MatTemplatematching(MatDib& dibSrc, MatDib& dibTmpl, int* arrayMap);
Point MatTemplatematching_Optimize(MatDib& dibSrc, MatDib& dibTmpl, int* arrayMap);