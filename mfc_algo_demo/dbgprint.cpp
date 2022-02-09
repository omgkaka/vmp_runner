//#include "stdafx.h"
#include "dbgprint.h"

#pragma warning (disable: 4996) 

void DbgPrintA(LPCSTR lpszFormat, ...)
{
	va_list  args;
	char* szBuffer = new char[0x4000]();
	va_start(args, lpszFormat);
	wvsprintfA(szBuffer, lpszFormat, args);
	OutputDebugStringA(szBuffer);
	va_end(args);
	delete[] szBuffer;
}
void DbgPrintW(LPCWSTR lpszFormat, ...)
{
	va_list   args;
	wchar_t* szBuffer = new wchar_t[0x4000]();
	va_start(args, lpszFormat);
	wvsprintfW(szBuffer, lpszFormat, args);
	OutputDebugStringW(szBuffer);
	va_end(args);
	delete[] szBuffer;
}

void DbgPrintHex(char* data, int len)
{
	char* buf = new char[0xFFFF]();
	for (int i = 0; i < len; i++)
	{
		char test[8] = { 0 };
		if (i % 8 == 0 && i != 0 && i % 16 != 0) sprintf(test, "  %02X", (BYTE)data[i]);
		else sprintf(test, " %02X", (BYTE)data[i]);
		strcat(buf, test);
		if ((i + 1) % 16 == 0 && i != 0) strcat(buf, "\r\n");
		else if (i >= (len - 1)) strcat(buf, "\r\n");
	}
	DbgPrintA(buf);
	delete[] buf;
}


BOOL GetOS_Version(DWORD& dwMajorVer, DWORD& dwMinorVer, DWORD& dwBuildNumber)
{
	uintptr_t PEB;
#ifdef _WIN64
	PEB = __readgsqword(0x60);
	dwMajorVer = *((INT*)(PEB + 0x118));
	dwMinorVer = *((INT*)(PEB + 0x11C));
	dwBuildNumber = *((INT*)(PEB + 0x120));
#else
	PEB = __readfsdword(0x30);
	dwMajorVer = *((INT*)(PEB + 0xA4));
	dwMinorVer = *((INT*)(PEB + 0xA8));
	dwBuildNumber = *((INT*)(PEB + 0xAC));
#endif // _WIN64

	dwBuildNumber &= 0xffff;
	return TRUE;
}

BOOL GetOS_Version_(DWORD& dwMajorVer, DWORD& dwMinorVer, DWORD& dwBuildNumber)
{
	BOOL bRet = FALSE;
	HMODULE hModNtdll = NULL;
	if (hModNtdll = ::LoadLibraryW(L"ntdll.dll"))
	{
		typedef void (WINAPI* pfRTLGETNTVERSIONNUMBERS)(DWORD*, DWORD*, DWORD*);
		pfRTLGETNTVERSIONNUMBERS pfRtlGetNtVersionNumbers;
		pfRtlGetNtVersionNumbers = (pfRTLGETNTVERSIONNUMBERS)::GetProcAddress(hModNtdll, "RtlGetNtVersionNumbers");
		if (pfRtlGetNtVersionNumbers)
		{
			pfRtlGetNtVersionNumbers(&dwMajorVer, &dwMinorVer, &dwBuildNumber);
			dwBuildNumber &= 0x0ffff;
			bRet = TRUE;
		}
		::FreeLibrary(hModNtdll);
		hModNtdll = NULL;
	}
	return bRet;
}

BOOL IsWow64()
{
	typedef BOOL(WINAPI* LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);
	LPFN_ISWOW64PROCESS fnIsWow64Process;
	BOOL bIsWow64 = FALSE;
	fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(GetModuleHandle(TEXT("kernel32")), "IsWow64Process");
	if (NULL != fnIsWow64Process)
	{
		fnIsWow64Process(GetCurrentProcess(), &bIsWow64);
	}
	return bIsWow64;
}


bool EnableDebugPrivilege()
{
	HANDLE hToken;
	LUID sedebugnameValue;
	TOKEN_PRIVILEGES tkp;
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) {
		return false;
	}
	if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &sedebugnameValue)) {
		__try {
			if (hToken) {
				CloseHandle(hToken);
			}
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {};
		return false;
	}
	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Luid = sedebugnameValue;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	if (!AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof(tkp), NULL, NULL)) {
		__try {
			if (hToken) {
				CloseHandle(hToken);
			}
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {};
		return false;
	}
	return true;
}



//https://blog.csdn.net/xboxmicro/article/details/19422443
/*
Operating system	Version number
Windows 10					10.0*
Windows Server 2019			10.0*
Windows Server 2016			10.0*
Windows 8.1					6.3*
Windows Server 2012 R2		6.3*
Windows 8					6.2
Windows Server 2012			6.2
Windows 7					6.1
Windows Server 2008 R2		6.1
Windows Server 2008			6.0
Windows Vista				6.0
Windows Server 2003 R2		5.2
Windows Server 2003			5.2
Windows XP 64_Bit Edition	5.2
Windows XP					5.1
Windows 2000				5.0
*/