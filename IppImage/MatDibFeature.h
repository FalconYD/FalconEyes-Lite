#pragma once
#include "define_Class.h"
#include "MatDib.h"
#include "MatDibExt.h"

void MatEdgeCanny(MatDib& Src, MatDib& Dst, float sigma, float th_low, float th_high);
void MatHoughLine(MatDib& Src, std::vector<MatLineParam>& lines, int threshold = 60);
void MatDrawLine(MatDib& Dst, MatLineParam line, BYTE c);
void MatDrawLine(MatDib& img, int x1, int y1, int x2, int y2, BYTE c);
void MatHarrisCorner(MatDib& Src, std::vector<Point>& corners, double th);

