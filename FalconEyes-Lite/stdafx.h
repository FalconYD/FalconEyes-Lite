
// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������ 
// ��� �ִ� ���� �����Դϴ�.

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // �Ϻ� CString �����ڴ� ��������� ����˴ϴ�.

// MFC�� ���� �κа� ���� ������ ��� �޽����� ���� ����⸦ �����մϴ�.
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC �ٽ� �� ǥ�� ���� ����Դϴ�.
#include <afxext.h>         // MFC Ȯ���Դϴ�.


#include <afxdisp.h>        // MFC �ڵ�ȭ Ŭ�����Դϴ�.


#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // Internet Explorer 4 ���� ��Ʈ�ѿ� ���� MFC �����Դϴ�.
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // Windows ���� ��Ʈ�ѿ� ���� MFC �����Դϴ�.
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC�� ���� �� ��Ʈ�� ���� ����
#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>
#include <locale>
#include <codecvt>
#include <functional>
#include <random>
#include "opencv2/opencv.hpp"
using namespace cv;

#include "../IppImage/MatDib.h"
#include "../IPPImage/MatDibExt.h"
#include "../IppImage/MatDibFourier.h"
#include "../IppImage/MatDibSegment.h"
#include "../IppImage/MatDibFeature.h"
#include "../IppImage/MatColor.h"
#include "../IppImage/MatAvi.h"
#include "../IppImage/MatFullSearch.h"
#include "../Include/OScopeCtrl.h"
#include "../Include/BarChart.h"
#include "FalconMath.h"
#include "FalconTimer.h"

#include "define_Structure.h"
#include "define_Fuction.h"
#include "define_StaticValues.h"
#include "define_Class.h"


#include <afxcontrolbars.h>
#include <afxwin.h>
#include <afxcontrolbars.h>

#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


#ifdef _WIN64
#pragma comment(lib,"../lib/CS_CPP_Interface.lib")
#else
#pragma comment(lib,"../lib/x86/CS_CPP_Interface.lib")
#endif