#pragma once
#include "MatDib.h"

void DibInverse(MatDib& src);
void DibBrightness(MatDib& src, int n);
void DibConstrast(MatDib& src, int n);
void DibGammaCorrection(MatDib& src, float gamma);
void DibHistogram(MatDib& src, float histo[256]);
void DibHistogramStretching(MatDib& src);
void DibHistogramEqualization(MatDib& src);

BOOL DibAdd(MatDib& dst, MatDib src1, MatDib src2);
BOOL DibAvg(MatDib& dst, MatDib src1, MatDib src2);
BOOL DibDiff(MatDib& dst, MatDib src1, MatDib src2);
BOOL DibAnd(MatDib& dst, MatDib src1, MatDib src2);
BOOL DibOR(MatDib& dst, MatDib src1, MatDib src2);
BOOL DibSub(MatDib& dst, MatDib src1, MatDib src2);

void DibToGray(MatDib src, MatDib& dst);
void DibToGrayCV(MatDib src, MatDib& dst);

void DibBitPlane(MatDib src, MatDib& dst, int nBit);
void DibFilterMean(MatDib src, MatDib& dst, int size);
void DibFilterMedianMean(MatDib src, MatDib& dst, int ksize);
void DibFilterWeightMean(MatDib src, MatDib& dst);
void DibFilterGaussian(MatDib src, float** pDst, float sigma);
void DibFilterGaussian_CV(MatDib src, MatDib& dst, float sigma);
void DibFilterBilateral(MatDib src, MatDib& dst, int d, float sigmacolor, float sigmaspace);
void DibFilterLaplacian(MatDib src, MatDib& dst);
void DibFilterUnsharpMask(MatDib src, MatDib& dst);
void DibFilterHighboost(MatDib src, MatDib& dst, float alpha);
void DibFilterDiffusion(MatDib src, MatDib& dst, float lambda, float k, int iter);

void DibTranslate(MatDib src, MatDib& dst, int sx, int sy);
void DibResizeNearest(MatDib src, MatDib& dst, int nw, int nh);
void DibResizeBilinear(MatDib src, MatDib& dst, int nw, int nh);
void DibResizeCubic(MatDib src, MatDib& dst, int nw, int nh);
double cubic_interpolation(double v1, double v2, double v3, double v4, double d);

void DibNoiseGaussian(MatDib src, MatDib& dst, int amount);
void DibNoiseSaltPepper(MatDib src, MatDib& dst, int amount);

void DibRotate(MatDib src, MatDib& dst, double angle);
void DibRotate90(MatDib src, MatDib& dst);
void DibRotate180(MatDib src, MatDib& dst);
void DibRotate270(MatDib src, MatDib& dst);

void DibMatMirror(MatDib src, MatDib& dst);
void DibMatFlip(MatDib src, MatDib& dst);

void DibMatDFT(MatDib src, MatDib& dst);
void DibEdgeRoberts(MatDib src, MatDib& dst);
void DibEdgePrewitt(MatDib src, MatDib& dst);
void DibEdgeSobel(MatDib src, MatDib& dst);