#include "stdafx.h"
#include "lib_SystemInfo.h"
// #include <wbemidl.h>
// #include <iostream>
// #pragma comment(lib, "wbemuuid.lib")


lib_SystemInfo::lib_SystemInfo()
{
	m_pWmiServices = nullptr;
	m_pWmiLocator = nullptr;
}


lib_SystemInfo::~lib_SystemInfo()
{
	Close();
}

bool lib_SystemInfo::Connect(const wchar_t *host, const wchar_t* pszName, const wchar_t* pszPwd)
{
	Close();

	HRESULT result;

	result = CoInitialize(NULL);
	if (result < 0)
	{
		return false;
	}

	result = CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_DEFAULT, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, NULL);
	if (result < 0)
	{
		CoUninitialize();
		return false;
	}

	result = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, (LPVOID *)&m_pWmiLocator);
	if (result < 0)
	{
		CoUninitialize();
		return false;
	}

	if (host == 0 || pszName == 0 || pszPwd == 0)
	{
		result = m_pWmiLocator->ConnectServer(_bstr_t(L"ROOT\\CIMV2"), NULL, NULL, 0, NULL, 0, 0, &m_pWmiServices);
		if (FAILED(result)) {
			CoUninitialize();
			return false;
		}
	}
	else
	{
		wchar_t buffer[100];
		swprintf_s(buffer, L"\\\\%s\\ROOT\\CIMV2", host);
		result = m_pWmiLocator->ConnectServer(_bstr_t(buffer), _bstr_t(pszName), _bstr_t(pszPwd), 0, NULL, 0, 0, &m_pWmiServices);
		if (result < 0)
		{
			CoUninitialize();
			return false;
		}
	}

	result = CoSetProxyBlanket(m_pWmiServices, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL, RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE);
	if (result < 0)
	{
		CoUninitialize();
		return false;
	}

	m_bIsInit = true;

	return true;
}

void lib_SystemInfo::Close()
{
	if (!m_bIsInit)
		return;

	if (m_pWmiServices) {
		m_pWmiServices->Release();
		m_pWmiServices = NULL;
	}

	if (m_pWmiLocator) {
		m_pWmiLocator->Release();
		m_pWmiLocator = NULL;
	}

	CoUninitialize();
}

bool lib_SystemInfo::Run(const wchar_t* query, RowSet& rs)
{
	if (!m_bIsInit) {
		return false;
	}
	rs.clear();
	HRESULT                 ret;
	IEnumWbemClassObject *  pEnumerator = NULL;

	ret = m_pWmiServices->ExecQuery(L"WQL", _bstr_t(query), WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, NULL, &pEnumerator);
	if (FAILED(ret)) {
		return false;
	}
	if (pEnumerator)
	{
		IWbemClassObject *      clsObj[32];
		ULONG                   uReturn = 0;

		ret = pEnumerator->Next(WBEM_INFINITE, 32, clsObj, &uReturn);
		if (uReturn > 0)
		{
			rs.resize(uReturn);
			for (ULONG i = 0; i < uReturn; ++i)
			{
				Row& r = rs[i];
				VARIANT vtProp;
				VariantInit(&vtProp);

				ret = clsObj[i]->BeginEnumeration(WBEM_FLAG_NONSYSTEM_ONLY);
				if (FAILED(ret))
				{
					clsObj[i]->Release();
					continue;
				}
				CIMTYPE	pvtType; VARIANT pVal; BSTR pstrName;
				// 각각의 Property들을 Map에 할당
				while (clsObj[i]->Next(0, &pstrName, &pVal, &pvtType, NULL) == WBEM_S_NO_ERROR)
				{
					// 백터&배열 지원 안함
					if ((pVal.vt & 0xf000) == 0)
					{
						if (pVal.vt != VT_NULL)
						{
							_bstr_t str = pVal;
							r[pstrName] = str;
						}
						else
						{
							r[pstrName] = L"";
						}
					}
					else
					{
						r[pstrName] = L"";
					}
					SysFreeString(pstrName);
					VariantClear(&pVal);
				}
				clsObj[i]->Release();
			}
		}
		pEnumerator->Release();
		if (uReturn == 0)
			return false;
	}
	return true;
}

bool lib_SystemInfo::GetClassProperties(const wchar_t * wmiClass, RowSet& rs)
{
	std::wstring wquery;

	wquery += L"SELECT * FROM ";
	wquery += wmiClass;

	return Run(wquery.c_str(), rs);
}
