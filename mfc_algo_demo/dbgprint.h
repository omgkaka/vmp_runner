#ifndef __DBGPRINT_H___XXXXXXXXXX14785296355555
#define __DBGPRINT_H___XXXXXXXXXX14785296355555

#include <atlstr.h>

void DbgPrintA(LPCSTR lpszFormat, ...);
void DbgPrintW(LPCWSTR lpszFormat, ...);
void DbgPrintHex(char* data, int len);

BOOL GetOS_Version(DWORD& dwMajorVer, DWORD& dwMinorVer, DWORD& dwBuildNumber);
BOOL GetOS_Version_(DWORD& dwMajorVer, DWORD& dwMinorVer, DWORD& dwBuildNumber);

BOOL IsWow64();
bool EnableDebugPrivilege();

#endif // !__DBGPRINT_H___XXXXXXXXXX14785296355555
