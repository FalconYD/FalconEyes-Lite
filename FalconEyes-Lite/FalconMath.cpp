#include "stdafx.h"
#include "define_Structure.h"

#include "FalconMath.h"

void g_fn_CrossCorr(unsigned char* input1, unsigned char* input2, short len1, short len2, unsigned char* output)
{
	int nStep1, nStep2;
	int nRangePlus = N_CORRRANGE;
	int nRangeMinus = -1 * nRangePlus;
	unsigned short ntmp1, ntmp2;

	if (len1 <= 0 || len2 <= 0)
		return;

	for (nStep1 = nRangeMinus; nStep1 < nRangePlus; ++nStep1)
	{
		ntmp1 = ntmp2 = 0;
		for (nStep2 = nRangePlus; nStep2 < len2 - nRangePlus; ++nStep2)
		{
			ntmp1 = input1[nStep2] * input2[nStep2 + nStep1];
			ntmp1 >>= 15;
			ntmp2 = ntmp2 + ntmp1;
		}
		output[nStep1 + nRangePlus] = ntmp2 >> 5;
	}
}

void g_fn_CrossCorr(unsigned char* input1, unsigned char* input2, short len1, short len2, unsigned int* output)
{
	int nStep1, nStep2;
	int nRangePlus = N_CORRRANGE;
	int nRangeMinus = -1 * nRangePlus;
	int ntmp1, ntmp2;

	if (len1 <= 0 || len2 <= 0)
		return;

	for (nStep1 = nRangeMinus; nStep1 < nRangePlus; ++nStep1)
	{
		ntmp1 = ntmp2 = 0;
		for (nStep2 = nRangePlus; nStep2 < len2 - nRangePlus; ++nStep2)
		{
			ntmp1 = input1[nStep2] * input2[nStep2 + nStep1];
			ntmp1 >>= 7;
			ntmp2 = ntmp2 + ntmp1;
		}
		output[nStep1 + nRangePlus] += ntmp2 >> 5;
	}
}
double g_fn_Get2PointLength(int x1, int y1, int x2, int y2)
{
	return static_cast<double>(sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2)));
}

double g_fn_Get2PointLength(Point pnt1, Point pnt2)
{
	return static_cast<double>(sqrt(pow(pnt1.x - pnt2.x, 2) + pow(pnt1.y - pnt2.y, 2)));
}

/**	
@brief	세점을 통과하는 원의 중심 (외접원)
@return	원 중심 위치
@param	세점
@remark	
-		
@author	선경규(Kyeong Kyu - Seon)
@date	2018/12/10  15:24
*/
Point2f g_fn_GetCircleCenter(Point2f pnt1, Point2f pnt2, Point2f pnt3)
{
	Point2f result;
	double A = pnt2.x - pnt1.x;
	double B = pnt2.y - pnt1.y;
	double C = pnt3.x - pnt1.x;
	double D = pnt3.y - pnt1.y;
	double E = A * (pnt1.x + pnt2.x) + B * (pnt1.y + pnt2.y);
	double F = C * (pnt1.x + pnt3.x) + D * (pnt1.y + pnt3.y);
	double G = 2.*(A*D - B * C);
	if (G != 0.)
	{
		result.x = (float)((D*E - B * F) / G);
		result.y = (float)((A*F - C * E) / G);
		return result;
	}
	return Point2f(-1.0, -1.0);
}

// least mean squre
Mat g_fn_fittingLine_LMS(Mat ROImatImage, LINE_PARAM& stLine)
{
	
	Mat matOut = ROImatImage.clone();
	int nStep = 0;
	int nStepX = 0;
	int nStepY = 0;
	int nVecSize = 0;
	double dAvgX = 0.0;
	double dAvgY = 0.0;
	std::vector<Point> vecPoint;
		

	for (nStepY = 0; nStepY < ROImatImage.rows; nStepY++)
	{
		for (nStepX = 0; nStepX < ROImatImage.cols; nStepX++)
		{
			if (ROImatImage.data[nStepY * ROImatImage.cols + nStepX] > 0)
				vecPoint.push_back(Point(nStepX, nStepY));
		}
	}
	nVecSize = static_cast<int>(vecPoint.size());
	for (nStep = 0; nStep < nVecSize; nStep++)
	{
		dAvgX += vecPoint[nStep].x / (double)nVecSize;
		dAvgY += vecPoint[nStep].y / (double)nVecSize;
	}
	double temp1 = 0.0;
	double temp2 = 0.0;
	for (nStep = 0; nStep < nVecSize; nStep++)
	{
		temp1 += (vecPoint[nStep].x - dAvgX) * (vecPoint[nStep].y - dAvgY);
		temp2 += pow(vecPoint[nStep].x - dAvgX, 2);
	}
	stLine.a = temp1 / temp2;
	stLine.b = dAvgY - stLine.a * dAvgX;

	double dTemp = 0.0;
	for (nStepX = 0; nStepX < matOut.cols; nStepX++)
	{
		dTemp = stLine.a * nStepX + stLine.b;
		if (dTemp > ROImatImage.rows)
			break;
		if (dTemp < 0)
			continue;
		matOut.data[(int)(dTemp)* matOut.cols + nStepX] = 128;
	}

	for (nStepY = 0; nStepY < matOut.rows; nStepY++)
	{
		dTemp = (nStepY - stLine.b) / stLine.a;
		if (dTemp > ROImatImage.cols)
			break;
		if (dTemp < 0)
			continue;
		matOut.data[nStepY * matOut.cols + (int)dTemp] = 128;
	}

	return matOut;
}

Mat g_fn_fittingLine_RANSAC(Mat ROImatImage, LINE_PARAM& stLine)
{
	const int MAX_ANGLE = 18000;
	const int MAX_AXIS = 40000;
	int nStep = 0;
	int arrX[MAX_AXIS];
	int arrY[MAX_AXIS];
	int arrAngle[MAX_ANGLE];
	Mat matOut = ROImatImage.clone();
	double temp;
	memset(arrAngle, 0x00, sizeof(arrAngle[0]) * MAX_ANGLE);
	memset(arrX, 0x00, sizeof(arrX[0]) * MAX_AXIS);
	memset(arrY, 0x00, sizeof(arrY[0]) * MAX_AXIS);
	std::vector<Point> vecPoint;
	
	int nStepX = 0;
	int nStepY = 0;
	
	for (nStepY = 0; nStepY < ROImatImage.rows; nStepY++)
	{
		for (nStepX = 0; nStepX < ROImatImage.cols; nStepX++)
		{
			if (ROImatImage.data[nStepY * ROImatImage.cols + nStepX] > 0)
				vecPoint.push_back(Point(nStepX, nStepY));
		}
	}

	int nIndex1, nIndex2;
	double dTempAngle, dTempY;
	Point pnt1, pnt2, pntTmp;
	double dPickDis;
	int nLoopCount = N_SAMPLINGCOUNT;
	for (int nStep = 0; nStep < nLoopCount; nStep++)
	{
		nIndex1 = g_fn_GetRandomNumber(static_cast<int>(vecPoint.size() - 1));
		do
		{
			nIndex2 = g_fn_GetRandomNumber(static_cast<int>(vecPoint.size() - 1));
		} while (nIndex1 == nIndex2);
		pnt1 = vecPoint[nIndex1];
		pnt2 = vecPoint[nIndex2];
		dPickDis = sqrt(pow(pnt1.x - pnt2.x, 2) + pow(pnt1.y - pnt2.y, 2));

		if (pnt1.x - pnt2.x == 0 || pnt1.y - pnt2.y == 0 || dPickDis < 3)
		{
			nStep--;
			continue;
		}
		if (pnt1.y > pnt2.y)
		{
			pntTmp = pnt1;
			pnt1 = pnt2;
			pnt2 = pntTmp;
		}
		dTempAngle = (pnt2.x - pnt1.x) / sqrt(pow(pnt1.y - pnt2.y, 2) + pow(pnt1.x - pnt2.x, 2));
		dTempAngle = std::acos(dTempAngle) * MAX_ANGLE / M_PI;
		ASSERT(dTempAngle != NAN);
		arrAngle[static_cast<int>(dTempAngle)]++;

		dTempY = (pnt1.y - std::tan(dTempAngle * (double)M_PI / MAX_ANGLE) * pnt1.x);
		if (dTempAngle >= 45 / 180.0 * MAX_ANGLE && dTempAngle < 135 / 180.0 * MAX_ANGLE)
		{
			dTempY = dTempY / std::tan(dTempAngle * (double)M_PI / MAX_ANGLE);
			dTempY = dTempY + MAX_AXIS / 2.0;
			arrX[(int)dTempY]++;
		}
		else
		{
			dTempY = dTempY + MAX_AXIS / 2.0;
			arrY[(int)dTempY]++;
		}
	}

	// pick Max Value
	int nMaxA = 0, nMaxY = 0, nMaxX = 0;
	double dAngle = 0, dY = 0, dX = 0;
	for (nStep = 0; nStep < MAX_ANGLE; nStep++)
	{
		if (nMaxA < arrAngle[nStep])
		{
			dAngle = nStep / 100.0;
			nMaxA = arrAngle[nStep];
		}
	}

	for (nStep = 0; nStep < MAX_AXIS; nStep++)
	{
		if (nMaxY < arrY[nStep])
		{
			dY = (nStep - MAX_AXIS / 2.0);
			nMaxY = arrY[nStep];
		}
	}

	for (nStep = 0; nStep < MAX_AXIS; nStep++)
	{
		if (nMaxX < arrX[nStep])
		{
			dX = (nStep - MAX_AXIS / 2.0);
			nMaxX = arrX[nStep];
		}
	}

	if (dAngle >= 45 && dAngle < 135)
	{
		dY = std::tan(dAngle * CV_PI / 180.0) * dX;
	}

	// Draw Line
	
	stLine.a = dAngle;
	stLine.b = dY;
	for (nStepX = 0; nStepX < matOut.cols; nStepX++)
	{
		temp = (std::tan(dAngle * CV_PI / 180.0) * nStepX + dY);
		if (temp > ROImatImage.rows)
			break;
		if (temp < 0)
			continue;
		matOut.data[(int)(temp)* matOut.cols + nStepX] = 128;
	}

	for (nStepY = 0; nStepY < matOut.rows; nStepY++)
	{
		temp = (nStepY - dY) / std::tan(dAngle * CV_PI / 180.0);
		if (temp > ROImatImage.cols)
			break;
		if (temp < 0)
			continue;
		matOut.data[nStepY * matOut.cols + (int)temp] = 128;
	}
	
	return matOut;
}

Mat g_fn_fittingCircle_RANSAC(Mat ROImatImage, Point& pnt, int& radius)
{
	Mat Output = ROImatImage.clone();
	std::vector<cv::Point> vecPnt;
	std::vector<int> vecCnt;
	std::vector<CIRCLE_RANSAC_RESULT> vecResult;
	CIRCLE_RANSAC_RESULT stResult;
	int nStepX, nStepY, nRandom;
	size_t nStep = 0;
	UINT nMinRadius = ROImatImage.cols, nMaxRadius = 0;
	int nMinDistance = 5;
	vecResult.clear();
	// Get Point
	for (nStepY = 0; nStepY < ROImatImage.rows; nStepY++)
	{
		uchar* pixel = ROImatImage.ptr<uchar>(nStepY);
		for (nStepX = 0; nStepX < ROImatImage.cols; nStepX++)
		{
			if (pixel[nStepX] > 254)
				vecPnt.push_back(Point(nStepX, nStepY));
		}
	} // ~Get Point
	nStep = 0;
	Point pntA, pntB, pntC, pntResult;
	size_t nVecSize = vecPnt.size();

	srand((UINT)time(0));
	if (nVecSize == 0)
		return Output;
	while (nStep < N_SAMPLINGCOUNT)
	{
		nRandom = rand() % nVecSize;
		pntA = vecPnt[nRandom];

		do
		{
			nRandom = rand() % nVecSize;
			pntB = vecPnt[nRandom];
		} while (pntA == pntB);

		do
		{
			nRandom = rand() % nVecSize;
			pntC = vecPnt[nRandom];
		} while (pntC == pntB || pntC == pntA);

		if (abs(pntA.x - pntB.x) < nMinDistance)
			continue;
		if (abs(pntB.x - pntC.x) < nMinDistance)
			continue;
		if (abs(pntC.x - pntA.x) < nMinDistance)
			continue;

		if (abs(pntA.y - pntB.y) < nMinDistance)
			continue;
		if (abs(pntB.y - pntC.y) < nMinDistance)
			continue;
		if (abs(pntC.y - pntA.y) < nMinDistance)
			continue;
		pntResult = g_fn_GetCircleCenter(pntA, pntB, pntC);
		vecResult.push_back(CIRCLE_RANSAC_RESULT(pntResult, (UINT)g_fn_Get2PointLength(pntResult, pntA)));

		if (vecResult[nStep].nRadius <= nMinRadius && vecResult[nStep].nRadius 
			< (ROImatImage.cols > ROImatImage.rows ? static_cast<unsigned int>(ROImatImage.cols) : static_cast<unsigned int>(ROImatImage.rows)))
			nMinRadius = vecResult[nStep].nRadius;
		if (vecResult[nStep].nRadius >= nMaxRadius && vecResult[nStep].nRadius 
			< (ROImatImage.cols > ROImatImage.rows ? static_cast<unsigned int>(ROImatImage.cols) : static_cast<unsigned int>(ROImatImage.rows)))
			nMaxRadius = vecResult[nStep].nRadius;
		nStep++;
	}
	int npCountRadius[2000];
	int nAbsRadius = nMaxRadius - nMinRadius, nTempValue, nMaxCntRadius;
	if (nMaxRadius - nMinRadius <= 0)
		return Output;
	nStep = 0;

	memset(npCountRadius, 0, sizeof(int) * 2000);
	// 반지름 Count
	while (nStep < vecResult.size())
	{
		if (vecResult[nStep].nRadius 
			< (ROImatImage.cols > ROImatImage.rows ? static_cast<unsigned int>(ROImatImage.cols) : static_cast<unsigned int>(ROImatImage.rows)))
		{
			npCountRadius[vecResult[nStep].nRadius - nMinRadius]++;
		}
		nStep++;
	}

	nTempValue = 0;
	for (nStep = 0; nStep < nAbsRadius; nStep++)
	{
		if (nTempValue <= npCountRadius[nStep])
		{
			nMaxCntRadius = static_cast<int>(nStep + nMinRadius);
			nTempValue = npCountRadius[nStep];
		}
	}

	int nCnt = 0;
	for (nStep = 0; nStep < vecResult.size(); nStep++)
	{
		if (vecResult[nStep].nRadius < static_cast<unsigned int> (nMaxCntRadius) && vecResult[nStep].nRadius >= static_cast<unsigned int>(nMaxCntRadius * 0.95))
		{
			stResult.pntCenter += vecResult[nStep].pntCenter;
			stResult.nRadius += vecResult[nStep].nRadius;
			nCnt++;
		}
	}
	stResult.nRadius = nMaxCntRadius;
	if (nCnt != 0)
	{
		stResult.pntCenter /= nCnt;
		//stResult.nRadius /= nCnt;
	}
	else
		return Output;

	if (Output.channels() == 1)
		cvtColor(Output, Output, COLOR_GRAY2BGR);

	circle(Output, stResult.pntCenter, stResult.nRadius, Scalar(0, 255, 0));
	line(Output, stResult.pntCenter + Point(-30, 0), stResult.pntCenter + Point(30, 0), Scalar(0, 0, 255));
	line(Output, stResult.pntCenter + Point(0, -30), stResult.pntCenter + Point(0, 30), Scalar(0, 0, 255));
	char chTmp[256] = { 0, };
	sprintf_s(chTmp, "%d R", stResult.nRadius);
	cv::putText(Output, chTmp, stResult.pntCenter + Point(50, 50), cv::HersheyFonts::FONT_HERSHEY_SIMPLEX, 1.0, Scalar(255, 0, 0));
	pnt = stResult.pntCenter;
	radius = stResult.nRadius;
	return Output;
}

// A(x1,y1) B(x2,y2)
// y = ax + b
// a = (y2-y1)/(x2-x1)
// y = (y2-y1)/(x2-x1)x+b
// b = y-(y2-y1)/(x2-x1)x
//   = y1-(y1-y1)/(x2-x1)x1
// y = (y2-y1)/(x2-x1)x + (y1-(y2-y1)/(x2-x1)x1)
LINE_PARAM g_fn_GetLineParam(Point2f pnt1, Point2f pnt2)
{
	LINE_PARAM stLineParam;
	// 기울기가 무한대일때 처리. x절편 설정.
	if (abs(pnt1.x - pnt2.x) == 0)
	{
		stLineParam.a = 0;
		stLineParam.b = 0;
		stLineParam.x = pnt1.x;
	}
	else
	{
		stLineParam.a = (pnt2.y - pnt1.y) / (double)(pnt2.x - pnt1.x);
		stLineParam.b = pnt1.y - stLineParam.a * pnt1.x;
		stLineParam.x = 0;
		// x절편 = -(b/a)
	}
	
	return stLineParam;
}

// 방정식 1 : y = a1x + b1
// 방정식 2 : y = a2x + b2
// 교점은 두방정식 연립.
// 교점은 (-(b1 - b2)/(a1-a2), a1(-(1-b2)/(a1-a2))+b1)
Point2f g_fn_GetLineIntersectionPoint(LINE_PARAM param1, LINE_PARAM param2)
{
	Point2f pnt;
	if (param1.x == 0)
		if (param2.x == 0)
			pnt.x = static_cast<float>((param2.b - param1.b) / (param1.a - param2.a));
		else
			pnt.x = static_cast<float>(param2.x);
	else
		pnt.x = static_cast<float>(param1.x);

	//pnt.y = param1.a * ((param2.b - 1) / (param1.a - param2.a)) + param2.b;
	pnt.y = static_cast<float>(param1.a * pnt.x + param1.b);
	return pnt;
}

/**	
@brief	CheckCrossLine
@return	사각형 내부 교점
@param	사각형 4점
@remark	
- 사각형 4점간의 교점을 구한다.
- 비교 17번 수행
@author	선경규(Kyeong Kyu - Seon)
@date	2019/6/10  13:29
*/
Point2f g_fn_CheckCrossLine(Point2f pnt1, Point2f pnt2, Point2f pnt3, Point2f pnt4)
{
	LINE_PARAM stLine1, stLine2;
	Point2f pntReturn;
	double nMinX = 99999, nMaxX = 0, nMinY = 99999, nMaxY = 0;
	if (pnt1.x < nMinX) nMinX = pnt1.x;
	if (pnt2.x < nMinX) nMinX = pnt2.x;
	if (pnt3.x < nMinX) nMinX = pnt3.x;
	if (pnt4.x < nMinX) nMinX = pnt4.x;

	if (pnt1.x > nMaxX) nMaxX = pnt1.x;
	if (pnt2.x > nMaxX) nMaxX = pnt2.x;
	if (pnt3.x > nMaxX) nMaxX = pnt3.x;
	if (pnt4.x > nMaxX) nMaxX = pnt4.x;

	if (pnt1.y < nMinY) nMinY = pnt1.y;
	if (pnt2.y < nMinY) nMinY = pnt2.y;
	if (pnt3.y < nMinY) nMinY = pnt3.y;
	if (pnt4.y < nMinY) nMinY = pnt4.y;

	if (pnt1.y > nMaxY) nMaxY = pnt1.y;
	if (pnt2.y > nMaxY) nMaxY = pnt2.y;
	if (pnt3.y > nMaxY) nMaxY = pnt3.y;
	if (pnt4.y > nMaxY) nMaxY = pnt4.y;

	stLine1 = g_fn_GetLineParam(pnt1, pnt2);
	stLine2 = g_fn_GetLineParam(pnt2, pnt3);
	pntReturn = g_fn_GetLineIntersectionPoint(stLine1, stLine2);
	if (pntReturn.x > nMinX && pntReturn.x < nMaxX && pntReturn.y > nMinY && pntReturn.y < nMaxY)
		if(pntReturn != pnt1 && pntReturn != pnt2 && pntReturn != pnt3 && pntReturn != pnt4)
			return pntReturn;

	stLine2 = g_fn_GetLineParam(pnt3, pnt4);
	pntReturn = g_fn_GetLineIntersectionPoint(stLine1, stLine2);
	if (pntReturn.x > nMinX && pntReturn.x < nMaxX && pntReturn.y > nMinY && pntReturn.y < nMaxY)
		if (pntReturn != pnt1 && pntReturn != pnt2 && pntReturn != pnt3 && pntReturn != pnt4)
			return pntReturn;

	stLine2 = g_fn_GetLineParam(pnt1, pnt4);
	pntReturn = g_fn_GetLineIntersectionPoint(stLine1, stLine2);
	if (pntReturn.x > nMinX && pntReturn.x < nMaxX && pntReturn.y > nMinY && pntReturn.y < nMaxY)
		if (pntReturn != pnt1 && pntReturn != pnt2 && pntReturn != pnt3 && pntReturn != pnt4)
			return pntReturn;

	stLine2 = g_fn_GetLineParam(pnt2, pnt4);
	pntReturn = g_fn_GetLineIntersectionPoint(stLine1, stLine2);
	if (pntReturn.x > nMinX && pntReturn.x < nMaxX && pntReturn.y > nMinY && pntReturn.y < nMaxY)
		if (pntReturn != pnt1 && pntReturn != pnt2 && pntReturn != pnt3 && pntReturn != pnt4)
			return pntReturn;

	stLine2 = g_fn_GetLineParam(pnt1, pnt3);
	pntReturn = g_fn_GetLineIntersectionPoint(stLine1, stLine2);
	if (pntReturn.x > nMinX && pntReturn.x < nMaxX && pntReturn.y > nMinY && pntReturn.y < nMaxY)
		if (pntReturn != pnt1 && pntReturn != pnt2 && pntReturn != pnt3 && pntReturn != pnt4)
			return pntReturn;

	stLine1 = g_fn_GetLineParam(pnt2, pnt3);
	stLine2 = g_fn_GetLineParam(pnt3, pnt4);
	pntReturn = g_fn_GetLineIntersectionPoint(stLine1, stLine2);
	if (pntReturn.x > nMinX && pntReturn.x < nMaxX && pntReturn.y > nMinY && pntReturn.y < nMaxY)
		if (pntReturn != pnt1 && pntReturn != pnt2 && pntReturn != pnt3 && pntReturn != pnt4)
			return pntReturn;

	stLine2 = g_fn_GetLineParam(pnt1, pnt4);
	pntReturn = g_fn_GetLineIntersectionPoint(stLine1, stLine2);
	if (pntReturn.x > nMinX && pntReturn.x < nMaxX && pntReturn.y > nMinY && pntReturn.y < nMaxY)
		if (pntReturn != pnt1 && pntReturn != pnt2 && pntReturn != pnt3 && pntReturn != pnt4)
			return pntReturn;

	stLine2 = g_fn_GetLineParam(pnt1, pnt3);
	pntReturn = g_fn_GetLineIntersectionPoint(stLine1, stLine2);
	if (pntReturn.x > nMinX && pntReturn.x < nMaxX && pntReturn.y > nMinY && pntReturn.y < nMaxY)
		if (pntReturn != pnt1 && pntReturn != pnt2 && pntReturn != pnt3 && pntReturn != pnt4)
			return pntReturn;

	stLine2 = g_fn_GetLineParam(pnt2, pnt4);
	pntReturn = g_fn_GetLineIntersectionPoint(stLine1, stLine2);
	if (pntReturn.x > nMinX && pntReturn.x < nMaxX && pntReturn.y > nMinY && pntReturn.y < nMaxY)
		if (pntReturn != pnt1 && pntReturn != pnt2 && pntReturn != pnt3 && pntReturn != pnt4)
			return pntReturn;


	stLine1 = g_fn_GetLineParam(pnt3, pnt4);
	stLine2 = g_fn_GetLineParam(pnt1, pnt4);
	pntReturn = g_fn_GetLineIntersectionPoint(stLine1, stLine2);
	if (pntReturn.x > nMinX && pntReturn.x < nMaxX && pntReturn.y > nMinY && pntReturn.y < nMaxY)
		if (pntReturn != pnt1 && pntReturn != pnt2 && pntReturn != pnt3 && pntReturn != pnt4)
			return pntReturn;

	stLine2 = g_fn_GetLineParam(pnt2, pnt4);
	pntReturn = g_fn_GetLineIntersectionPoint(stLine1, stLine2);
	if (pntReturn.x > nMinX && pntReturn.x < nMaxX && pntReturn.y > nMinY && pntReturn.y < nMaxY)
		if (pntReturn != pnt1 && pntReturn != pnt2 && pntReturn != pnt3 && pntReturn != pnt4)
			return pntReturn;

	stLine2 = g_fn_GetLineParam(pnt1, pnt3);
	pntReturn = g_fn_GetLineIntersectionPoint(stLine1, stLine2);
	if (pntReturn.x > nMinX && pntReturn.x < nMaxX && pntReturn.y > nMinY && pntReturn.y < nMaxY)
		if (pntReturn != pnt1 && pntReturn != pnt2 && pntReturn != pnt3 && pntReturn != pnt4)
			return pntReturn;

	stLine1 = g_fn_GetLineParam(pnt1, pnt4);
	stLine2 = g_fn_GetLineParam(pnt2, pnt4);
	pntReturn = g_fn_GetLineIntersectionPoint(stLine1, stLine2);
	if (pntReturn.x > nMinX && pntReturn.x < nMaxX && pntReturn.y > nMinY && pntReturn.y < nMaxY)
		if (pntReturn != pnt1 && pntReturn != pnt2 && pntReturn != pnt3 && pntReturn != pnt4)
			return pntReturn;

	stLine2 = g_fn_GetLineParam(pnt1, pnt3);
	pntReturn = g_fn_GetLineIntersectionPoint(stLine1, stLine2);
	if (pntReturn.x > nMinX && pntReturn.x < nMaxX && pntReturn.y > nMinY && pntReturn.y < nMaxY)
		if (pntReturn != pnt1 && pntReturn != pnt2 && pntReturn != pnt3 && pntReturn != pnt4)
			return pntReturn;

	stLine1 = g_fn_GetLineParam(pnt2, pnt4);
	stLine2 = g_fn_GetLineParam(pnt1, pnt3);
	pntReturn = g_fn_GetLineIntersectionPoint(stLine1, stLine2);
	if (pntReturn.x > nMinX && pntReturn.x < nMaxX && pntReturn.y > nMinY && pntReturn.y < nMaxY)
		if (pntReturn != pnt1 && pntReturn != pnt2 && pntReturn != pnt3 && pntReturn != pnt4)
			return pntReturn;

	pntReturn.x = -1.0;
	pntReturn.y = -1.0;
	return pntReturn;
}

int g_fn_GetRandomNumber(int nMax)
{
	std::random_device rd;
	std::mt19937_64 rnd(rd());
	std::uniform_int_distribution<int> range(0, nMax);
	return range(rnd);
}

Mat g_fn_fittingElipse(Mat ROImatImage, ELLIPSE_PARAM& stEllipse)
{
	Mat Output = ROImatImage.clone();
	std::vector<cv::Point> vecPnt;
	int nStepX, nStepY;

	for (nStepY = 0; nStepY < ROImatImage.rows; nStepY++)
	{
		for (nStepX = 0; nStepX < ROImatImage.cols; nStepX++)
		{
			if (ROImatImage.data[nStepY * ROImatImage.cols + nStepX] > 0)
			{
				vecPnt.push_back(Point(nStepX, nStepY));
			}
		}
	}
	int nVecCount = static_cast<int>(vecPnt.size());
	Mat matJ = Mat::zeros(Size(5, nVecCount), CV_64F);
	Mat matY = Mat::zeros(Size(5, nVecCount), CV_64F);
	for (nStepY = 0; nStepY < nVecCount; nStepY++)
	{
		matJ.at<double>(nStepY, 0) = static_cast<double>(vecPnt[nStepY].x * vecPnt[nStepY].y);
		matJ.at<double>(nStepY, 1) = static_cast<double>(vecPnt[nStepY].y * vecPnt[nStepY].y);
		matJ.at<double>(nStepY, 2) = static_cast<double>(vecPnt[nStepY].x);
		matJ.at<double>(nStepY, 3) = static_cast<double>(vecPnt[nStepY].y);
		matJ.at<double>(nStepY, 4) = static_cast<double>(1);
		matY.at<double>(nStepY, 0) = static_cast<double>(-1 * vecPnt[nStepY].x * vecPnt[nStepY].x);
	}

	// 최적의 해 의사역행렬 (pseudo inv)
	// J의 역행렬이 있어야함. (문제의 대부분 역행렬이 존재)
	// JX = Y
	// X = (Jt * J)^-1*J * Y
	Mat matX = (matJ.t() * matJ).inv()* matJ.t() * matY;
	double dA, dB, dC, dD, dE, dF;
	dA = 1;
	dB = matX.at<double>(0, 0);
	dC = matX.at<double>(1, 0);
	dD = matX.at<double>(2, 0);
	dE = matX.at<double>(3, 0);
	dF = matX.at<double>(4, 0);

	double dTheta, dCx, dCy, dCu, dW, dH;
	dTheta = 0.5 * atan(dB / (1 - dC));
	dCx = (2 * dC * dD - dB * dE) / (dB * dB - 4 * dA * dC);
	dCy = (2 * dA * dE - dB * dD) / (dB * dB - 4 * dA * dC);
	dCu = dA*dCx*dCx + dB*dCx*dCy + dC*dCy*dCy - dF;
	dW = sqrt(dCu / (dA*cos(dTheta)*cos(dTheta) + dB * cos(dTheta)*sin(dTheta) + dC*sin(dTheta)*sin(dTheta)));
	dH = sqrt(dCu / (dA*sin(dTheta)*sin(dTheta) - dB * cos(dTheta)*sin(dTheta) + dC*cos(dTheta)*cos(dTheta)));

	stEllipse.angle = dTheta;
	stEllipse.cx = dCx;
	stEllipse.cy = dCy;
	stEllipse.w = dW;
	stEllipse.h = dH;
	cvtColor(Output, Output, ColorConversionCodes::COLOR_GRAY2BGR);
	
	// X(t) = Xc + A * cos(t) * cos(th) - B * sin(t) * sin(th)
	// Y(t) = Yc + A * cos(t) * sin(th) + B * sin(t) * cos(th)
	// Xc : 타원의 중심 x좌표
	// Yc : 타원의 중심 y좌표
	// t : 0 ~ 360도
	// th : 타원의 회전각
	// A : x축에 대한 타원의 반경
	// B : y축에 대한 타원의 반경
	ellipse(Output, Point(static_cast<int>(dCx), static_cast<int>(dCy)), Size(static_cast<int>(dW), static_cast<int>(dH)), dTheta * 180 / M_PI, 0, 360, Scalar(0, 255, 0));
	line(Output, Point(static_cast<int>(dCx - 5), static_cast<int>(dCy)), Point(static_cast<int>(dCx + 5), static_cast<int>(dCy)), Scalar(0, 0, 255));
	line(Output, Point(static_cast<int>(dCx), static_cast<int>(dCy - 5)), Point(static_cast<int>(dCx), static_cast<int>(dCy + 5)), Scalar(0, 0, 255));

	line(Output, Point(static_cast<int>(dCx), static_cast<int>(dCy)), Point(static_cast<int>(dCx + dW), static_cast<int>(dCy)), Scalar(255, 0, 0));
	line(Output, Point(static_cast<int>(dCx), static_cast<int>(dCy)), Point(static_cast<int>(dCx), static_cast<int>(dCy + dH)), Scalar(255, 0, 0));
	return Output;
}

Mat g_fn_fittingElipse_RANSAC(Mat ROImatImage, ELLIPSE_PARAM& stEllipse)
{
	Mat Output = ROImatImage.clone();
	std::vector<cv::Point> vecPnt;
	int nStepX, nStepY;

	for (nStepY = 0; nStepY < ROImatImage.rows; nStepY++)
	{
		for (nStepX = 0; nStepX < ROImatImage.cols; nStepX++)
		{
			if (ROImatImage.data[nStepY * ROImatImage.cols + nStepX] > 0)
			{
				vecPnt.push_back(Point(nStepX, nStepY));
			}
		}
	}
	double dA, dB, dC, dD, dE, dF;
	std::map<double, int> mapB, mapC, mapD, mapE, mapF;
	Mat matX, matJ, matY;
	int nVecCount = static_cast<int>(vecPnt.size());
	int nRandom;
	std::vector<int> vecNum;
	const int N_PICKPOINT = 25;
	for (nStepX = 0; nStepX < N_SAMPLINGCOUNT; nStepX++)
	{
		vecNum.clear();
		if (!matJ.empty())
			matJ.release();
		matJ = Mat::zeros(Size(5, N_PICKPOINT), CV_64F);

		if (!matY.empty())
			matY.release();
		matY = Mat::zeros(Size(5, N_PICKPOINT), CV_64F);

		for (nStepY = 0; nStepY < N_PICKPOINT; nStepY++)
		{
			nRandom = g_fn_GetRandomNumber(nVecCount - 1);
			for (int i = 0; i < vecNum.size(); i++)
			{
				while (vecNum[i] == nRandom)
				{
					nRandom = g_fn_GetRandomNumber(nVecCount - 1);
				}
			}
			vecNum.push_back(nRandom);
			matJ.at<double>(nStepY, 0) = static_cast<double>(vecPnt[nRandom].x * vecPnt[nRandom].y);
			matJ.at<double>(nStepY, 1) = static_cast<double>(vecPnt[nRandom].y * vecPnt[nRandom].y);
			matJ.at<double>(nStepY, 2) = static_cast<double>(vecPnt[nRandom].x);
			matJ.at<double>(nStepY, 3) = static_cast<double>(vecPnt[nRandom].y);
			matJ.at<double>(nStepY, 4) = static_cast<double>(1);
			matY.at<double>(nStepY, 0) = static_cast<double>(-1 * vecPnt[nRandom].x * vecPnt[nRandom].x);
		}

		// 최적의 해 의사역행렬 (pseudo inv)
		// J의 역행렬이 있어야함. (문제의 대부분 역행렬이 존재)
		// JX = Y
		// X = (Jt * J)^-1*J * Y
		matX = (matJ.t() * matJ).inv()* matJ.t() * matY;

		mapB[round(matX.at<double>(0, 0) * 1000) / 1000]++;
		mapC[round(matX.at<double>(1, 0) * 1000) / 1000]++;
		mapD[round(matX.at<double>(2, 0) * 1000) / 1000]++;
		mapE[round(matX.at<double>(3, 0) * 1000) / 1000]++;
		mapF[round(matX.at<double>(4, 0) * 1) / 1]++;
	}
	double dMax = -9999999.0; 
	int nMax = 0;

	dA = 1;
	nMax = 0;
	for (auto it = mapB.begin(); it != mapB.end(); it++)
	{
		if (it->second >= nMax)
		{
			nMax = it->second;
			dB = it->first;
		}
	}
	nMax = 0;
	for (auto it = mapC.begin(); it != mapC.end(); it++)
	{
		if (it->second >= nMax)
		{
			nMax = it->second;
			dC = it->first;
		}
	}
	nMax = 0;
	for (auto it = mapD.begin(); it != mapD.end(); it++)
	{
		if (it->second >= nMax)
		{
			nMax = it->second;
			dD = it->first;
		}
	}
	nMax = 0;
	for (auto it = mapE.begin(); it != mapE.end(); it++)
	{
		if (it->second >= nMax)
		{
			nMax = it->second;
			dE = it->first;
		}
	}
	nMax = 0;
	for (auto it = mapF.begin(); it != mapF.end(); it++)
	{
		if (it->second >= nMax)
		{
			nMax = it->second;
			dF = it->first;
		}
	}

	double dTheta, dCx, dCy, dCu, dW, dH;
	dTheta = 0.5 * atan(dB / (1 - dC));
	dCx = (2 * dC * dD - dB * dE) / (dB * dB - 4 * dA * dC);
	dCy = (2 * dA * dE - dB * dD) / (dB * dB - 4 * dA * dC);
	dCu = dA*dCx*dCx + dB*dCx*dCy + dC*dCy*dCy - dF;
	dW = sqrt(abs(dCu / (dA*cos(dTheta)*cos(dTheta) + dB * cos(dTheta)*sin(dTheta) + dC*sin(dTheta)*sin(dTheta))));
	dH = sqrt(abs(dCu / (dA*sin(dTheta)*sin(dTheta) - dB * cos(dTheta)*sin(dTheta) + dC*cos(dTheta)*cos(dTheta))));

	stEllipse.angle = dTheta;
	stEllipse.cx = dCx;
	stEllipse.cy = dCy;
	stEllipse.w = dW;
	stEllipse.h = dH;
	cv::cvtColor(Output, Output, ColorConversionCodes::COLOR_GRAY2BGR);

	// X(t) = Xc + A * cos(t) * cos(th) - B * sin(t) * sin(th)
	// Y(t) = Yc + A * cos(t) * sin(th) + B * sin(t) * cos(th)
	// Xc : 타원의 중심 x좌표
	// Yc : 타원의 중심 y좌표
	// t : 0 ~ 360도
	// th : 타원의 회전각
	// A : x축에 대한 타원의 반경
	// B : y축에 대한 타원의 반경
	cv::ellipse(Output, Point(static_cast<int>(dCx), static_cast<int>(dCy)), Size(static_cast<int>(dW), static_cast<int>(dH)), dTheta * 180 / M_PI, 0, 360, Scalar(0, 255, 0));
	cv::line(Output, Point(static_cast<int>(dCx - 5), static_cast<int>(dCy)), Point(static_cast<int>(dCx) + 5, static_cast<int>(dCy)), Scalar(0, 0, 255));
	cv::line(Output, Point(static_cast<int>(dCx), static_cast<int>(dCy - 5)), Point(static_cast<int>(dCx), static_cast<int>(dCy + 5)), Scalar(0, 0, 255));

	cv::line(Output, Point(static_cast<int>(dCx), static_cast<int>(dCy)), Point(static_cast<int>(dCx + dW), static_cast<int>(dCy)), Scalar(255, 0, 0));
	cv::line(Output, Point(static_cast<int>(dCx), static_cast<int>(dCy)), Point(static_cast<int>(dCx), static_cast<int>(dCy + dH)), Scalar(255, 0, 0));
	return Output;
}

// Computer Vision 알고리즘 구현 Part

void g_fn_Histogram(unsigned char* pSrc, size_t nWidth, size_t nHeight, unsigned int* pHistogram)
{
	const int GRAYLEVELMAX = 256;
	size_t i, j;
	for (i = 0; i < GRAYLEVELMAX; i++)
	{
		pHistogram[i] = 0;
	}

	for (j = 0; j < nHeight; j++)
	{
		for (i = 0; i < nWidth; i++)
		{
			pHistogram[pSrc[j * nWidth + i]]++;
		}
	}
	for (j = 0; j < GRAYLEVELMAX; j++)
	{
		pHistogram[j] = static_cast<unsigned int>(pHistogram[j] / nHeight* nWidth);
	}
}

void g_fn_Histogram2DCal_HS(unsigned char* pSrcH, unsigned char* pSrcS, size_t nWidth, size_t nHeight, unsigned int* pHistogram)
{
	const double HISTOGRAMQLEVEL = 64.0;
	const double GRAYLEVELMAX = 256.0;
	size_t i, j;

	for (i = 0; i < HISTOGRAMQLEVEL; i++)
	{
		for (j = 0; j < HISTOGRAMQLEVEL; j++)
		{
			pHistogram[static_cast<int>(j*HISTOGRAMQLEVEL + i)] = 0;
		}
	}

	// 화소 (j,i) 각각에 대해
	for (j = 0; j < nHeight; j++)
	{
		for (i = 0; i < nWidth; i++)
		{
			pHistogram[static_cast<int>(pSrcH[j*nWidth + i] * HISTOGRAMQLEVEL / GRAYLEVELMAX) * nWidth
				+ static_cast<int>(pSrcS[j*nWidth + i] * HISTOGRAMQLEVEL / GRAYLEVELMAX)]++;
		}
	}

	// 정규화
	for (j = 0; j < nHeight; j++)
	{
		for (i = 0; i < nWidth; i++)
		{
			pHistogram[j * nWidth + i] = static_cast<unsigned int>(pHistogram[j*nWidth + i] / (nWidth * nHeight));
		}
	}
}

void g_fn_InverseProjectionHistogram(unsigned char* pSrcH, unsigned char* pSrcS, unsigned char* pSrcHisto, size_t nWidth, size_t nHeight, unsigned char* pDst)
{
	const double HISTOGRAMQLEVEL = 64.0;
	const double GRAYLEVELMAX = 256.0;
	size_t i, j;
	unsigned int* pHistoi = new unsigned int[static_cast<int>(HISTOGRAMQLEVEL * HISTOGRAMQLEVEL)];
	unsigned int* pHistor = new unsigned int[static_cast<int>(HISTOGRAMQLEVEL * HISTOGRAMQLEVEL)];
	g_fn_Histogram2DCal_HS(pSrcH, pSrcS, nWidth, nHeight, pHistoi);

	for (j = 0; j < HISTOGRAMQLEVEL; j++)
	{
		for (i = 0; i < HISTOGRAMQLEVEL; i++)
		{
			pHistor[static_cast<int>(j * HISTOGRAMQLEVEL + i)] =
				static_cast<int>(std::min(pSrcHisto[static_cast<int>(j * HISTOGRAMQLEVEL + i)] / static_cast<double>(pHistoi[static_cast<int>(j * HISTOGRAMQLEVEL + i)]), 1.0));
		}
	}

	for (j = 0; j < nHeight; j++)
	{
		for (i = 0; i < nWidth; i++)
		{
			pDst[j * nWidth + i] = pHistor[static_cast<int>((pSrcS[j * nWidth + i] * HISTOGRAMQLEVEL / GRAYLEVELMAX) * HISTOGRAMQLEVEL
				+ pSrcH[j * nWidth + i] * HISTOGRAMQLEVEL / GRAYLEVELMAX)];
		}
	}

	delete pHistoi;
	pHistoi = nullptr;
	delete pHistor;
	pHistor = nullptr;
}

void g_fn_OtsuAlgorithm(unsigned char* pSrc, size_t nWidth, size_t nHeight, unsigned char* pDst)
{
	const int MAX_GRAY_LEVEL = 256;
	int nStep, nStepX, nStepY;
	unsigned int pHisto[MAX_GRAY_LEVEL] = { 0, };
	unsigned char pW0[MAX_GRAY_LEVEL] = { 0, };
	unsigned char pU0[MAX_GRAY_LEVEL] = { 0, };
	unsigned char pU1[MAX_GRAY_LEVEL] = { 0, };
	unsigned char pVbet[MAX_GRAY_LEVEL] = { 0, };
	int nU = 0, nMax = 0, nThreshold = 0;
	g_fn_Histogram(pSrc, nWidth, nHeight, pHisto);

	for (nStep = 0; nStep < MAX_GRAY_LEVEL; nStep++)
	{
		nU += nStep * pHisto[nStep];
	}
	// otsu 계산
	pW0[0] = pHisto[0];
	pU0[0] = 0;
	for (nStep = 1; nStep < MAX_GRAY_LEVEL; nStep++)
	{
		pW0[nStep] = pW0[nStep - 1] + pHisto[nStep];
		pU0[nStep] = (pW0[nStep - 1] * pU0[nStep - 1] + nStep * pHisto[nStep]) / pW0[nStep];
		pU1[nStep] = (nU - pW0[nStep] * pU0[nStep]) / (1 - pW0[nStep]);
		pVbet[nStep] = static_cast<unsigned char>(pW0[nStep] * pow((1 - pW0[nStep])*(pU0[nStep] - pU1[nStep]), 2));
		if (nMax < pVbet[nStep])
		{
			nMax = pVbet[nStep];
			nThreshold = nStep;
		}
	}
	// 이진화
	for (nStepY = 0; nStepY < nHeight; nStepY++)
	{
		for (nStepX = 0; nStepX < nWidth; nStepX++)
		{
			pDst[nStepY * nWidth + nStepX] = (pSrc[nStepY * nWidth + nStepX] <= nThreshold) ? 255 : 0;
		}
	}
}

void g_floodfill4(char* pDst, size_t szStepX, size_t szStepY, size_t nWid, int label)
{
	if (pDst[szStepY * nWid + szStepX] == -1)
	{
		pDst[szStepY * nWid + szStepX] = label;
		g_floodfill4(pDst, szStepX - 1, szStepY, nWid, label);
		g_floodfill4(pDst, szStepX, szStepY - 1, nWid, label);
		g_floodfill4(pDst, szStepX + 1, szStepY, nWid, label);
		g_floodfill4(pDst, szStepX, szStepY + 1, nWid, label);
	}
}

void g_fn_LineTracking(unsigned char* pSrc, size_t nWidth, size_t nHeight, char* pDst)
{
	if (pSrc == nullptr)
		return;
	if (pDst == nullptr)
		return;

	size_t nStepX = 0, nStepY = 0;
	int label = 0;
	memset(pDst, 0x00, sizeof(pDst[0]) * nWidth * nHeight);
	for (nStepY = 1; nStepY < nHeight - 1; nStepY++)
	{
		for (nStepX = 1; nStepX < nWidth - 1; nStepX++)
		{
			if (pSrc[nStepY * nWidth + nStepX] > 0)
				pDst[nStepY * nWidth + nStepX] = -1;
			else
				pDst[nStepY * nWidth + nStepX] = 0;
		}
	}
	label = 1;
	for (nStepY = 1; nStepY < nHeight - 1; nStepY++)
	{
		for (nStepX = 1; nStepX < nWidth - 1; nStepX++)
		{
			if (pDst[nStepY * nWidth + nStepX] == -1)
			{
				g_floodfill4(pDst, nStepY, nStepX, nWidth, label);
				label++;
			}
		}
	}
}

void g_efficientfloodfill4(char* pDst, size_t szStepX, size_t szStepY, size_t nWid, int label)
{
	DEFPOINT stPnt;
	size_t szLeft, szRight;
	std::queue<DEFPOINT> que;
	que.push(DEFPOINT(szStepX, szStepY));
	while (!que.empty())
	{
		stPnt = que.front();
		que.pop();
		if (pDst[szStepY * nWid + szStepX] == -1)
		{
			szLeft = szRight = szStepX;
			while (pDst[szStepY * nWid + szLeft - 1] == -1) { szLeft--; }
			while (pDst[szStepY * nWid + szRight + 1] == -1) { szRight++; }
			for (size_t szStep = szLeft; szStep < szRight; szStep++)
			{
				pDst[szStepY * nWid + szStep] = label;
				if (pDst[(szStepY - 1) * nWid + szStep] == -1 && (szStep == szLeft || pDst[(szStepY - 1) * nWid + szStep - 1] != -1))
				{
					que.push(DEFPOINT(szStep, szStepY - 1));
				}
				if (pDst[(szStepY + 1) * nWid + szStep] == -1 && (szStep == szLeft || pDst[(szStepY + 1) * nWid + szStep - 1] != -1))
				{
					que.push(DEFPOINT(szStep, szStepY + 1));
				}
			}
		}
	}
}

void g_fn_LineTrackingEnhanced(unsigned char* pSrc, size_t nWidth, size_t nHeight, char* pDst)
{
	if (pSrc == nullptr)
		return;
	if (pDst == nullptr)
		return;

	size_t nStepX = 0, nStepY = 0;
	int label = 0;
	memset(pDst, 0x00, sizeof(pDst[0]) * nWidth * nHeight);
	for (nStepY = 1; nStepY < nHeight - 1; nStepY++)
	{
		for (nStepX = 1; nStepX < nWidth - 1; nStepX++)
		{
			if (pSrc[nStepY * nWidth + nStepX] > 0)
				pDst[nStepY * nWidth + nStepX] = -1;
			else
				pDst[nStepY * nWidth + nStepX] = 0;
		}
	}
	label = 1;
	for (nStepY = 1; nStepY < nHeight - 1; nStepY++)
	{
		for (nStepX = 1; nStepX < nWidth - 1; nStepX++)
		{
			if (pDst[nStepY * nWidth + nStepX] == -1)
			{
				g_efficientfloodfill4(pDst, nStepY, nStepX, nWidth, label);
				label++;
			}
		}
	}
}

void g_fn_GetHomogeneousMatrix(double* filter, HOMOGENEOUSMODE mode, int x, int y, double theta)
{
	switch (mode)
	{
	case HOMOGENEOUSMODE::EFM_MOVE:
		filter[0] = 1.0;
		filter[1] = 0.0;
		filter[2] = 0.0;

		filter[3] = 0.0;
		filter[4] = 1.0;
		filter[5] = 0.0;

		filter[6] = static_cast<double>(x);
		filter[7] = static_cast<double>(y);
		filter[8] = 1;
		break;
	case HOMOGENEOUSMODE::EFM_ROTATE:
		filter[0] = cos(theta);
		filter[1] = -1 * sin(theta);
		filter[2] = 0.0;

		filter[3] = sin(theta);
		filter[4] = cos(theta);
		filter[5] = 0.0;

		filter[6] = 0.0;
		filter[7] = 0.0;
		filter[8] = 1.0;
		break;
	case HOMOGENEOUSMODE::EFM_SIZE:
		filter[0] = static_cast<double>(y);
		filter[1] = 0.0;
		filter[2] = 0.0;

		filter[3] = 0.0;
		filter[4] = static_cast<double>(x);
		filter[5] = 0.0;

		filter[6] = 0.0;
		filter[7] = 0.0;
		filter[8] = 1.0;
		break;
	case HOMOGENEOUSMODE::EFM_TILT:
		filter[0] = 1.0;
		filter[1] = static_cast<double>(y);
		filter[2] = 0.0;

		filter[3] = static_cast<double>(x);
		filter[4] = 1.0;
		filter[5] = 0.0;

		filter[6] = 0.0;
		filter[7] = 0.0;
		filter[8] = 1.0;
		break;
	default:
		break;
	}
}

void g_fn_HomogeneousTranform(unsigned char* pSrc, int Width, int Height, unsigned char* pDst, HOMOGENEOUSMODE mode, int x, int y, double theta, bool bPrev)
{
	ASSERT(pSrc == nullptr || pDst == nullptr);
	ASSERT(Width < 0 || Height < 0);
	double filter[9] = { 0, };
	g_fn_GetHomogeneousMatrix(filter, mode, x, y, theta);
	int nStepX = 0, nStepY = 0;
	double dDstX = 0.0, dDstY = 0.0;
	for (nStepY = 0; nStepY < Height; nStepY++)
	{
		for (nStepX = 0; nStepX < Width; nStepX++)
		{
			dDstX = nStepY * filter[1] + nStepX * filter[4] + filter[7];
			dDstY = nStepY * filter[0] + nStepX * filter[3] + filter[6];
			// 전방 기하
			if (bPrev)
				pDst[nStepY * Width + nStepX] = pDst[(int)dDstY * Width + (int)dDstX];
			// 후방 기하
			else
				pDst[(int)dDstY * Width + (int)dDstX] = pDst[nStepY * Width + nStepX];
		}
	}
}