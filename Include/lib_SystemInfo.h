#pragma once
#include <wbemidl.h>
#include <iostream>
#pragma comment(lib, "wbemuuid.lib")
class lib_SystemInfo
{
public:
	typedef std::map<std::wstring, std::wstring> Row;
	typedef std::vector<Row> RowSet;
private:
	IWbemServices* m_pWmiServices;
	IWbemLocator* m_pWmiLocator;
	bool m_bIsInit;
public:
	lib_SystemInfo();
	~lib_SystemInfo();

	bool Connect(const wchar_t *host, const wchar_t* pszName, const wchar_t* pszPwd);
	void Close();
	bool Run(const wchar_t* query, RowSet& rowset);
	bool GetClassProperties(const wchar_t* wmiClass, RowSet& rowset);
};

