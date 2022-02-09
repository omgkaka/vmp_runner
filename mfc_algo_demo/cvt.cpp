/*
首先myinc被添加到附加包含目录
若注释掉#include "stdafx.h"又与工程开启预编译头冲突
则可以工程设置强制包含stdafx.h同命令行/FI"stdafx.h"
*/

//#include "stdafx.h"

#include "cvt.h"
#include "dbgprint.h"
#include <locale>
#include <algorithm>

#pragma warning(disable:4996)

const wchar_t* const g_pwszHexMap{ L"0123456789ABCDEF" };
const char* const g_pszHexMap{ "0123456789ABCDEF" };

wchar_t* ANSIToUnicode(const char* str)
{
	int textlen;
	wchar_t* result;
	textlen = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0);
	result = (wchar_t*)malloc((textlen + 1) * sizeof(wchar_t));
	if (result)
	{
		memset(result, 0, (textlen + 1) * sizeof(wchar_t));
		MultiByteToWideChar(CP_ACP, 0, str, -1, (LPWSTR)result, textlen);
	}
	return result;
}

char* UnicodeToANSI(const wchar_t* str)
{
	char* result;
	int textlen;
	textlen = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
	result = (char*)malloc((textlen + 1) * sizeof(char));
	if (result)
	{
		memset(result, 0, sizeof(char) * (textlen + 1));
		WideCharToMultiByte(CP_ACP, 0, str, -1, result, textlen, NULL, NULL);
	}
	return result;
}

wchar_t* UTF8ToUnicode(const char* str)
{
	int textlen;
	wchar_t* result;
	textlen = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
	result = (wchar_t*)malloc((textlen + 1) * sizeof(wchar_t));
	if (result)
	{
		memset(result, 0, (textlen + 1) * sizeof(wchar_t));
		MultiByteToWideChar(CP_UTF8, 0, str, -1, (LPWSTR)result, textlen);
	}
	return result;
}

char* UnicodeToUTF8(const wchar_t* str)
{
	char* result;
	int textlen;
	textlen = WideCharToMultiByte(CP_UTF8, 0, str, -1, NULL, 0, NULL, NULL);
	result = (char*)malloc((textlen + 1) * sizeof(char));
	if (result)
	{
		memset(result, 0, sizeof(char) * (textlen + 1));
		WideCharToMultiByte(CP_UTF8, 0, str, -1, result, textlen, NULL, NULL);
	}
	return result;
}
/*宽字符转换为多字符Unicode - ANSI*/
char* w2m(const wchar_t* wcs)
{
	size_t len;
	char* buf;
	len = wcstombs(NULL, wcs, 0);
	if (len == 0)
		return NULL;
	buf = (char*)malloc(sizeof(char) * (len + 1));
	if (buf)
	{
		memset(buf, 0, sizeof(char) * (len + 1));
		len = wcstombs(buf, wcs, len + 1);
	}
	return buf;
}
/*多字符转换为宽字符ANSI - Unicode*/
wchar_t* m2w(const char* mbs)
{
	size_t len;
	wchar_t* buf;
	len = mbstowcs(NULL, mbs, 0);
	if (len == 0)
		return NULL;
	buf = (wchar_t*)malloc(sizeof(wchar_t) * (len + 1));
	if (buf)
	{
		memset(buf, 0, sizeof(wchar_t) * (len + 1));
		len = mbstowcs(buf, mbs, len + 1);
	}
	return buf;
}

char* ANSIToUTF8(const char* str)
{
	return UnicodeToUTF8(ANSIToUnicode(str));
}

char* UTF8ToANSI(const char* str)
{
	return UnicodeToANSI(UTF8ToUnicode(str));
}

wchar_t* A2W_(const char* _char)
{
	if (!_char) return nullptr;

	int length = MultiByteToWideChar(CP_ACP, 0, _char, -1, NULL, 0);
	wchar_t* ret = new wchar_t[length + 1]();
	MultiByteToWideChar(CP_ACP, 0, _char, -1, (LPWSTR)ret, length);
	return ret;
}

char* W2A_(const wchar_t* tchar)
{
	if (!tchar) return nullptr;

	int length = wcslen(tchar) + 1;
	char* ret = new char[length]();
	WideCharToMultiByte(1, 0, (LPCWCH)tchar, -1, ret, length, NULL, NULL);
	return ret;
}

//将16进制字符串转换成对应字节集
void hexstr2bin_(BYTE* pbDest, BYTE* pbSrc)
{
	char h1, h2;
	BYTE s1, s2;
	size_t i;
	size_t nLen = strlen((const char*)pbSrc) / 2;
	for (i = 0; i < nLen; i++)
	{
		h1 = pbSrc[2 * i];
		h2 = pbSrc[2 * i + 1];

		s1 = toupper(h1) - 0x30;
		if (s1 > 9)
			s1 -= 7;

		s2 = toupper(h2) - 0x30;
		if (s2 > 9)
			s2 -= 7;

		pbDest[i] = s1 * 16 + s2;
	}
}

BYTE* hexstr2bin(char* pbSrc)
{
	BYTE* pbDest = NULL;
	if (pbSrc)
	{
		char h1, h2;
		BYTE s1, s2;
		int i;
		int nLen = strlen(pbSrc) / 2;
		pbDest = new BYTE[nLen]();
		for (i = 0; i < nLen; i++)
		{
			h1 = pbSrc[2 * i];
			h2 = pbSrc[2 * i + 1];

			s1 = toupper(h1) - 0x30;
			if (s1 > 9)
				s1 -= 7;

			s2 = toupper(h2) - 0x30;
			if (s2 > 9)
				s2 -= 7;

			pbDest[i] = (BYTE)(s1 * 16 + s2);
		}
	}
	return pbDest;
}

BYTE* hexstr2bin_end00(char* pbSrc)
{
	BYTE* pbDest = NULL;
	if (pbSrc)
	{
		char h1, h2;
		BYTE s1, s2;
		int i;
		int nLen = strlen(pbSrc) / 2;
		pbDest = new BYTE[nLen + 1]();
		for (i = 0; i < nLen; i++)
		{
			h1 = pbSrc[2 * i];
			h2 = pbSrc[2 * i + 1];

			s1 = toupper(h1) - 0x30;
			if (s1 > 9)
				s1 -= 7;

			s2 = toupper(h2) - 0x30;
			if (s2 > 9)
				s2 -= 7;

			pbDest[i] = (BYTE)(s1 * 16 + s2);
		}
		pbDest[nLen] = '\0';
	}
	return pbDest;
}

//将内存地址处数据转16进制字符串，参数传入待转的字节数量。
void bin2hexstr_(BYTE* pbDest, BYTE* pbSrc, size_t nLen)
{
	char ddl, ddh;
	size_t i;
	for (i = 0; i < nLen; i++)
	{
		ddh = 48 + pbSrc[i] / 16;
		ddl = 48 + pbSrc[i] % 16;
		if (ddh > 57) ddh = ddh + 7;
		if (ddl > 57) ddl = ddl + 7;
		pbDest[i * 2] = ddh;
		pbDest[i * 2 + 1] = ddl;
	}
	pbDest[nLen * 2] = '\0';
}

BYTE* bin2hexstr(BYTE* pbSrc, size_t nLen)//返回值记得free
{
	BYTE* pbDest = NULL;
	if (pbSrc)
	{
		char ddl, ddh;
		size_t i;
		pbDest = (BYTE*)new char[nLen * 2 + 1];
		for (i = 0; i < nLen; i++)
		{
			ddh = 48 + pbSrc[i] / 16;
			ddl = 48 + pbSrc[i] % 16;
			if (ddh > 57) ddh = ddh + 7;
			if (ddl > 57) ddl = ddl + 7;
			pbDest[i * 2] = ddh;
			pbDest[i * 2 + 1] = ddl;
		}
		pbDest[nLen * 2] = '\0';
	}
	return pbDest;
}

//原地逆序
char* Reverse(char* s)
{
	// p指向字符串头部
	char* p = s;
	// q指向字符串尾部
	char* q = s;
	while (*q)
		++q;
	q--;
	// 交换并移动指针，直到p和q交叉
	while (q > p)
	{
		char t = *p;
		*p++ = *q;
		*q-- = t;
	}
	return s;
}

// 使用异或操作对字符串s进行逆序
char* Reverse_(char* s)
{
	char* r = s;
	//令p指向字符串最后一个字符
	char* p = s;
	while (*(p + 1) != '\0')
		++p;
	// 使用异或操作进行交换
	while (p > s)
	{
		*p = *p ^ *s;
		*s = *p ^ *s;
		*p = *p-- ^ *s++;
	}

	return r;
}

//字符串分割函数
std::vector<std::string> split(std::string str, std::string pattern)
{
	std::string::size_type pos;
	std::vector<std::string> result;
	str += pattern;//扩展字符串以方便操作
	size_t size = str.size();

	for (size_t i = 0; i < size; i++)
	{
		pos = str.find(pattern, i);
		if (pos < size)
		{
			std::string s = str.substr(i, pos - i);
			result.push_back(s);
			i = pos + pattern.size() - 1;
		}
	}
	return result;
}

//成功返回非空
BYTE* readfiletobuf(LPCSTR sPath, int& size)
{
	size_t pSize = 0;
	unsigned char* pBuffer = NULL;
	FILE* fp = fopen(sPath, "rb");
	if (fp)
	{
		if (fseek(fp, 0, SEEK_END) == 0)
		{
			pSize = ftell(fp);
			if (pSize != -1)
			{
				if (fseek(fp, 0, SEEK_SET) == 0)
				{
#ifdef NDEBUG
					pBuffer = new(std::nothrow) unsigned char[pSize + 2]();//加2字节0000  方便文本文件读取转char* 自带\0000截断			
#else
					pBuffer = new unsigned char[pSize + 2]();//加2字节0000  方便文本文件读取转char* 自带\0000截断
#endif 

					if (pBuffer)
					{
						memset(pBuffer, 0, pSize + 2);
						pSize = fread(pBuffer, sizeof(unsigned char), pSize, fp);
						size = pSize;
					}
				}
			}
		}
		fclose(fp);
	}
	return pBuffer;
}

//成功返回非空
uint8_t* readfiletobuf(char* sPath, size_t& size)
{
	size_t pSize = 0;
	unsigned char* pBuffer = NULL;
	FILE* fp = fopen(sPath, "rb");
	if (fp)
	{
		if (fseek(fp, 0, SEEK_END) == 0)
		{
			pSize = ftell(fp);
			if (pSize != -1)
			{
				if (fseek(fp, 0, SEEK_SET) == 0)
				{
#ifdef NDEBUG
					pBuffer = new(std::nothrow) unsigned char[pSize + 2]();//加2字节0000  方便文本文件读取转char* 自带\0000截断			
#else
					pBuffer = new unsigned char[pSize + 2]();//加2字节0000  方便文本文件读取转char* 自带\0000截断
#endif 
					if (pBuffer)
					{
						memset(pBuffer, 0, pSize + 2);
						pSize = fread(pBuffer, sizeof(uint8_t), pSize, fp);
						size = pSize;
					}
				}
			}
		}
		fclose(fp);
	}
	return pBuffer;
}


//win 下SEH版本
BOOL file2buf(LPCSTR sPath, BYTE*& pout, ULONG& size)
{
	BOOL bret = FALSE;
	unsigned long pSize = 0;
	unsigned char* pBuffer = NULL;
	FILE* fp = fopen(sPath, "rb");
	if (fp)
	{
		fseek(fp, 0, SEEK_END);
		pSize = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		__try
		{
			pBuffer = new unsigned char[pSize + 2]();//加2字节0000  方便文本文件读取转char* 自带\0000截断
			memset(pBuffer, 0, pSize + 2);
		}
		__except (1)
		{
			MessageBox(NULL, TEXT("内存不足"), TEXT("异常提示"), MB_ICONERROR);
			return false;
		}
		if (pBuffer)
		{
			pSize = fread(pBuffer, sizeof(unsigned char), pSize, fp);
			size = pSize;
			pout = pBuffer;
			bret = TRUE;
		}
		fclose(fp);
	}
	return bret;
}

//非win下版本
bool file2buf_(char* sPath, uint8_t*& pout, uint32_t& size)
{
	bool bret = false;
	unsigned long pSize = 0;
	unsigned char* pBuffer = NULL;
	FILE* fp = fopen(sPath, "rb");
	if (fp)
	{
		fseek(fp, 0, SEEK_END);
		pSize = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		try
		{
			pBuffer = new unsigned char[pSize + 2]();//加2字节0000  方便文本文件读取转char* 自带\0000截断
			memset(pBuffer, 0, pSize + 2);
			pSize = fread(pBuffer, sizeof(unsigned char), pSize, fp);
			size = pSize;
			pout = pBuffer;
			bret = true;
		}
		catch (const std::exception&)
		{
			;
		}
		fclose(fp);
	}
	return bret;
}


BOOL buf2file(char* path, char* data, ULONG len)
{
	FILE* fp = fopen(path, "wb+");
	if (!fp)
	{
		DbgPrintA("buf2file  fopen failed  err=%d\n", GetLastError());
		return FALSE;
	}
	ULONG wlen = fwrite(data, 1, len, fp);
	fclose(fp);
	DbgPrintA("fwrite return value=%d\n", wlen);
	return wlen == len;
}

BOOL buf2file(const std::string& path, const std::string& data, ULONG len)
{
	return buf2file((char*)path.c_str(), (char*)data.c_str(), data.length());
}

bool buf2file(char* path, char* data, size_t len)
{
	FILE* fp = fopen(path, "wb+");
	if (fp)
	{
		size_t wlen = fwrite(data, 1, len, fp);
		fclose(fp);
		if (wlen == len)
		{
			return true;
		}
	}
	return false;
}



#define shift_len       23    //字符位移

//字符串异或
BYTE* en_xor(BYTE* pbin, size_t len, BYTE* pkey)
{
	if (!pbin || !pkey) return nullptr;

	BYTE* pout = nullptr;
	int lenkey = strlen((char*)pkey);
	__try
	{
		pout = new BYTE[len + 1]();
		for (size_t i = 0; i < len; i++)
		{
			for (size_t j = 0; j < lenkey; j++)
			{
				pout[i] = (pbin[i] + shift_len) ^ pkey[j];
			}
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		;
	}
	return pout;
}

BYTE* de_xor(BYTE* pbin, size_t len, BYTE* pkey)
{
	if (!pbin || !pkey) return nullptr;

	BYTE* pout = nullptr;
	__try
	{
		pout = new BYTE[len + 1]();
		for (size_t i = 0; i < len; i++)
		{
			for (size_t j = 0; j < strlen((char*)pkey); j++)
			{
				pout[i] = (BYTE)(pbin[i] ^ pkey[j]);
				pout[i] = pout[i] - shift_len;
			}
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		;
	}
	return pout;
}

/* Public Function Definitions ***********************************************/
void* memset_(void* pvBuffer, size_t iValue, size_t nBufferLen)
{
	SIZE_T nBufferIndex = 0;

	/* Sets the buffer */
	for (nBufferIndex = 0; nBufferIndex < nBufferLen; nBufferIndex++)
	{
		((PBYTE)pvBuffer)[nBufferIndex] = (BYTE)iValue;
	}

	return pvBuffer;
}

void print_string_hex(const char* comment, unsigned char* str, size_t len)
{
	unsigned char* c;

	printf("%s", comment);
	for (c = str; c < str + len; c++) {
		printf("0x%02x ", *c & 0xff);
	}

	printf("\n");
}


void hex_dump(unsigned char* ptr, size_t len)
{
	size_t i;
	for (i = 0; i < len; i++) {
		if (i != 0 && (i & 0xf) == 0) {
			fprintf(stderr, "\n");
		}
		fprintf(stderr, "%02hhx", ptr[i]);
	}
	fprintf(stderr, "\n");
}


std::string string_replace(std::string& strBig, const std::string& strsrc, const std::string& strdst)
{
	CStringA cstr = strBig.c_str();
	CStringA src = strsrc.c_str();
	CStringA dst = strdst.c_str();
	cstr.Replace(src, dst);
	std::string str = cstr.GetBuffer();
	return str;
}

std::string replace_str(std::string& str, const std::string& to_replaced, const std::string& newchars)
{
	return string_replace(str, to_replaced, newchars);
}



void string_lower(std::string& s)
{
	int len = s.size();
	for (int i = 0; i < len; i++) {
		if (s[i] >= 'A' && s[i] <= 'Z') {
			s[i] += 32;//+32转换为小写
			//s[i]=s[i]-'A'+'a';
		}
	}
}

void string_upper(std::string& s)
{
	int len = s.size();
	for (int i = 0; i < len; i++) {
		if (s[i] >= 'a' && s[i] <= 'z') {
			s[i] -= 32;//+32转换为小写
			//s[i]=s[i]-'a'+'A';
		}
	}
}

char* strToLower(char* str)
{
	char* ptr = str;
	while (*ptr) {
		if (*ptr >= 'A' && *ptr <= 'Z') {
			*ptr |= 32;  // 转换为小写
		}
		ptr++;
	}
	return str;
}

/*
宽字节字符集（Unicode）、多字节字符集（Multi_Byte） 自适应：
CString 转 std::string

CString cstrTest = _T("test");
std::string strTest;
std::string = CT2A(cstrTest.GetString());
//____________________________________
std::string 转 CString

std::string strTest="test";
CString cstrTest;
cstrTest= CA2T(strTest.c_str());
*/


std::string w2string(wchar_t* w)
{
	wchar_t* wText = w;
	DWORD dwNum = WideCharToMultiByte(CP_OEMCP, NULL, wText, -1, NULL, 0, NULL, FALSE);//取得长度	
	char* psText = new char[dwNum];//psText为char*的临时数组，作为赋值给std::string的中间变量
	WideCharToMultiByte(CP_OEMCP, NULL, wText, -1, psText, dwNum, NULL, FALSE);// WideCharToMultiByte的再次运用
	std::string str = psText;// std::string赋值
	delete[] psText;
	return str;
}

LPCTSTR_CLASS::LPCTSTR_CLASS(std::string str)
{
#ifdef _UNICODE
	ptr = A2W_(str.c_str());
#else
	int len = str.length() + 1;
	ptr = new TCHAR[len]();
	memcpy_s((void*)ptr, len, str.c_str(), len);
#endif
}

LPCTSTR_CLASS::~LPCTSTR_CLASS()
{
	delete[] ptr;
}

/*

#undef min
#undef max

void UllToWchars(ULONGLONG ull, wchar_t* pwsz, size_t dwSize, bool fAsHex)
{
	if (fAsHex)
	{
		//Converts dwSize bytes of ull to wchar_t*.
		for (size_t i = 0; i < dwSize; i++)
		{
			pwsz[i * 2] = g_pwszHexMap[((ull >> ((dwSize - 1 - i) << 3)) >> 4) & 0x0F];
			pwsz[i * 2 + 1] = g_pwszHexMap[(ull >> ((dwSize - 1 - i) << 3)) & 0x0F];
		}
	}
	else
	{
		std::wstring wstrFormat;
		switch (dwSize)
		{
		case 2:
			wstrFormat = L"%05llu";
			break;
		case 3:
			wstrFormat = L"%08llu";
			break;
		case 4:
			wstrFormat = L"%010llu";
			break;
		case 5:
			wstrFormat = L"%013llu";
			break;
		case 6:
			wstrFormat = L"%015llu";
			break;
		case 7:
			wstrFormat = L"%017llu";
			break;
		case 8:
			wstrFormat = L"%019llu";
			break;
		}
		swprintf_s(pwsz, 32, wstrFormat.data(), ull);
	}
}


bool CharsToUl(const char* pcsz, unsigned long& ul)
{
	char* pEndPtr;
	ul = strtoul(pcsz, &pEndPtr, 16);
	return !(ul == 0 && (pEndPtr == pcsz || *pEndPtr != '\0'));
}

template<typename T>bool wstr2num(std::wstring_view wstr, T& tData, int iBase)
{
	wchar_t* pEndPtr;
	if constexpr (std::is_same_v<T, ULONGLONG>)
	{
		tData = std::wcstoull(wstr.data(), &pEndPtr, iBase);
		if ((tData == 0 && (pEndPtr == wstr.data() || *pEndPtr != '\0'))
			|| (tData == ULLONG_MAX && errno == ERANGE))
			return false;
	}
	else
	{
		auto llData = std::wcstoll(wstr.data(), &pEndPtr, iBase);
		if ((llData == 0 && (pEndPtr == wstr.data() || *pEndPtr != '\0'))
			|| ((llData == LLONG_MAX || llData == LLONG_MIN) && errno == ERANGE)
			|| (llData > static_cast<LONGLONG>(std::numeric_limits<T>::max()))
			|| (llData < static_cast<LONGLONG>(std::numeric_limits<T>::min()))
			)
			return false;
		tData = static_cast<T>(llData);
	}

	return true;
}
//Explicit instantiations of templated func in .cpp.
template bool wstr2num<CHAR>(std::wstring_view wstr, CHAR& t, int iBase);
template bool wstr2num<UCHAR>(std::wstring_view wstr, UCHAR& t, int iBase);
template bool wstr2num<SHORT>(std::wstring_view wstr, SHORT& t, int iBase);
template bool wstr2num<USHORT>(std::wstring_view wstr, USHORT& t, int iBase);
template bool wstr2num<LONG>(std::wstring_view wstr, LONG& t, int iBase);
template bool wstr2num<ULONG>(std::wstring_view wstr, ULONG& t, int iBase);
template bool wstr2num<LONGLONG>(std::wstring_view wstr, LONGLONG& t, int iBase);
template bool wstr2num<ULONGLONG>(std::wstring_view wstr, ULONGLONG& t, int iBase);

std::string WstrToStr(std::wstring_view wstr)
{
	int iSize = WideCharToMultiByte(CP_UTF8, 0, wstr.data(), static_cast<int>(wstr.size()), nullptr, 0, nullptr, nullptr);
	std::string str(iSize, 0);
	WideCharToMultiByte(CP_UTF8, 0, wstr.data(), static_cast<int>(wstr.size()), &str[0], iSize, nullptr, nullptr);

	return str;
}

std::wstring StrToWstr(std::string_view str)
{
	int iSize = MultiByteToWideChar(CP_UTF8, 0, str.data(), static_cast<int>(str.size()), nullptr, 0);
	std::wstring wstr(iSize, 0);
	MultiByteToWideChar(CP_UTF8, 0, str.data(), static_cast<int>(str.size()), &wstr[0], iSize);
	return wstr;
}




bool StrToHex(const std::string& strFrom, std::string& strToHex)
{
	size_t dwIterations = strFrom.size() / 2 + strFrom.size() % 2;
	std::string strTmp;
	for (size_t i = 0; i < dwIterations; i++)
	{
		std::string strToUL; //String to hold currently extracted two letters.

		if (i + 2 <= strFrom.size())
			strToUL = strFrom.substr(i * 2, 2);
		else
			strToUL = strFrom.substr(i * 2, 1);

		unsigned long ulNumber;
		if (!CharsToUl(strToUL.data(), ulNumber))
			return false;

		strTmp += static_cast<unsigned char>(ulNumber);
	}
	strToHex = std::move(strTmp);
	strToHex.shrink_to_fit();

	return true;
}
*/


//分割文本
std::vector<std::string> SplitString(const  std::string& s, const std::string& delim)
{
	std::vector<std::string> elems;
	size_t pos = 0;
	size_t len = s.length();
	size_t delim_len = delim.length();
	if (delim_len == 0) return elems;
	while (pos < len)
	{
		int find_pos = s.find(delim, pos);
		if (find_pos < 0)
		{
			elems.push_back(s.substr(pos, len - pos));
			break;
		}
		elems.push_back(s.substr(pos, find_pos - pos));
		pos = find_pos + delim_len;
	}
	return elems;
}

std::vector<std::string>* SplitString_(const  std::string& s, const std::string& delim)
{
	std::vector<std::string>* pelems = new std::vector<std::string>();
	size_t pos = 0;
	size_t len = s.length();
	size_t delim_len = delim.length();
	if (delim_len == 0) return pelems;
	while (pos < len)
	{
		int find_pos = s.find(delim, pos);
		if (find_pos < 0)
		{
			pelems->push_back(s.substr(pos, len - pos));
			break;
		}
		pelems->push_back(s.substr(pos, find_pos - pos));
		pos = find_pos + delim_len;
	}
	return pelems;
}


std::vector<CString> SplitString(CString& str, char pattern)
{
	std::vector<CString> strArray;
	CString strTemp = TEXT("");
	char c;
	for (int i = 0; i < str.GetLength(); i++)
	{
		c = str[i];
		if (c == pattern)
		{
			strArray.push_back(strTemp);
			strTemp = TEXT("");
		}
		else
		{
			strTemp += c;
		}
	}
	strArray.push_back(strTemp);
	return strArray;
}

std::vector<CString> SplitString_(CString& str, char pattern)
{
	std::vector<CString> strArray;
	CString strTemp;
	strTemp = str;
	int iPos = 0;
	while (iPos != -1)
	{
		iPos = strTemp.Find(pattern);
		if (iPos == -1)
		{
			break;
		}
		strArray.push_back(strTemp.Left(iPos));
		strTemp = strTemp.Mid(iPos + 1, strTemp.GetLength());
	}
	strArray.push_back(strTemp);
	return strArray;
}

std::vector<CString> SplitString(CString& str, char pattern[])
{
	int nPattern = strlen(pattern);
	std::vector<CString> strArray;
	CString strTemp;
	char c;
	BOOL bFind = FALSE;
	for (int i = 0; i < str.GetLength(); i++)
	{
		c = str[i];
		for (int j = 0; j < nPattern; j++)
		{
			if (c == pattern[j])
			{
				if (strTemp != TEXT(""))
				{
					strArray.push_back(strTemp);
				}
				bFind = TRUE;
				break;
			}
			bFind = FALSE;
		}
		if (bFind)
		{
			strTemp = TEXT("");
		}
		else
		{
			strTemp += c;
		}
	}
	if (strTemp != TEXT(""))
	{
		strArray.push_back(strTemp);
	}
	return strArray;
}

//使用：
//char s[] = { ' ', ',', '\0' };
//CStringArray strPolygon = *SplitString(strTemp, s);


std::vector<BYTE> hex2bin(CString& arg_src)
{
	int len = arg_src.GetLength() / 2;
	std::vector<BYTE>  bvec(len);
	for (int iSrc = 0, iDst = 0; iDst < len; iSrc += 2, iDst++)
	{
		CString szSub = arg_src.Mid(iSrc, 2);
		bvec[iDst] = (BYTE)_tcstoul(szSub, NULL, 16);
	}
	return bvec;
}

std::string replaceSubstrings(const std::string& origin, const std::string& toReplace, const std::string& replacement) {
	size_t position = searchForSubstring(origin, toReplace);

	if (position == origin.size()) {
		return origin;
	}
	else {
		const std::string& remainingString = origin.substr(position + toReplace.size(), origin.size());
		return origin.substr(0, position) +
			replacement +
			replaceSubstrings(remainingString, toReplace, replacement);
	}
}

size_t searchForSubstring(const std::string& origin, const std::string& toSearch) {
	// boundary checks first
	size_t bytesToCompare = toSearch.size();
	if (bytesToCompare < origin.size()) {
		// search from front to back
		for (size_t index = 0; index < origin.size(); index++) {
			if (matches(origin.substr(index, bytesToCompare).c_str(), toSearch.c_str(), bytesToCompare)) {
				return index;
			}
		}
	}
	return origin.size();
}

bool matches(const char* one, const char* two, size_t bytes) {
	if (bytes > 0) {
		if (*one == *two) {
			return matches(one + 1, two + 1, bytes - 1);
		}
		else {
			return false;
		}
	}
	return true;
}


//sunday
//返回找到的首个指针
BYTE* sunday_find(BYTE* psrc, ULONGLONG len_src, BYTE* pdst, ULONGLONG len_dst)
{
	BYTE* pret = nullptr;
	size_t sbox[256] = { 0 };
	size_t* shift = sbox;
	size_t i;

	for (i = 0; i < 256; i++)
	{
		*(shift + i) = len_dst + 1;
	}
	for (i = 0; i < len_dst; i++)
	{
		*(shift + (unsigned char)(*(pdst + i))) = len_dst - i;//指针比下标快
	}
	size_t limit = len_src - len_dst + 1;
	for (i = 0; i < limit; i += shift[psrc[i + len_dst]])
	{
		if (psrc[i] == *pdst)
		{
			BYTE* match_text = psrc + i + 1;
			size_t match_size = 1;
			do
			{
				if (match_size == len_dst)
				{
					pret = psrc + i;
					return pret;
				}
			} while ((*match_text++) == pdst[match_size++]);
		}
	}
	return pret;
}

//返回找到所有的指针首地址     
std::vector<BYTE*> sunday_findall(BYTE* psrc, ULONGLONG len_src, BYTE* pdst, ULONGLONG len_dst)
{
	std::vector<BYTE*> vec_ret;
	vec_ret.reserve(100);//预留100

	size_t sbox[256] = { 0 };
	size_t* shift = sbox;
	size_t i;

	for (i = 0; i < 256; i++)
	{
		*(shift + i) = len_dst + 1;
	}
	for (i = 0; i < len_dst; i++)
	{
		*(shift + (unsigned char)(*(pdst + i))) = len_dst - i;//指针比下标快
	}
	size_t limit = len_src - len_dst + 1;
	for (i = 0; i < limit; i += shift[psrc[i + len_dst]])
	{
		if (psrc[i] == *pdst)
		{
			BYTE* match_text = psrc + i + 1;
			size_t match_size = 1;
			do
			{
				if (match_size == len_dst)
				{
					//std::cout << "the NO. is " << i << std::endl;
					//DbgPrintHex((char*)psrc + i, 2);
					vec_ret.push_back(psrc + i);
					//这里注意  查找全部当找到一个匹配后  下一轮查找i值应该+1处继续下一轮  不然会出现 FFFFFFFF 中FFFF替换CCDD 全为CCCC的情况
					i += 1;
				}
			} while ((*match_text++) == pdst[match_size++]);
		}
	}
	return vec_ret;
}

//快速strstr
int sunday_strStr(char* psrc, char* pdst)
{
	int len_src = strlen(psrc);
	int len_dst = strlen(pdst);

	int charStep[256] = { 0 };
	for (int i = 0; i < 256; ++i)
		charStep[i] = -1;
	for (int i = 0; i < len_dst; ++i)
		charStep[(int)pdst[i]] = i;

	for (int i = 0; i <= len_src - len_dst;)
	{
		int j = 0;
		while (j < len_dst)
		{
			if (psrc[i] == pdst[j])
			{
				++i;
				++j;
			}
			else
			{
				char* p = psrc + i + len_dst - j;
				if (charStep[(int)*p] == -1)
				{
					i = p - psrc + 1;
				}
				else
				{
					i = p - charStep[(int)*p] - psrc;
				}
				break;
			}
		}
		if (j == len_dst)
		{
			return i - len_dst;
		}
	}
	return -1;
}



std::string getstrL(std::string src, std::string substr, int bpos, bool nocase)
{
	std::string str = "";
	if (src.empty() || substr.empty()) return str;

	if (!nocase)//区分大小写
	{
		int index = src.find(substr, bpos);
		if (index != std::string::npos)
		{
			str = src.substr(0, index);
		}
	}
	else
	{
		std::string upsrc = src;
		std::string upsubstr = substr;
		string_upper(upsrc);
		string_upper(upsubstr);
		int index = upsrc.find(upsubstr, bpos);
		if (index != std::string::npos)
		{
			str = src.substr(0, index);
		}
	}
	return str;
}

std::string getstrR(std::string src, std::string substr, int bpos, bool nocase)
{
	std::string str = "";
	if (src.empty() || substr.empty()) return str;

	if (!nocase)//区分大小写
	{
		int index = src.find(substr, bpos);
		if (index != std::string::npos)
		{
			str = std::string(src.c_str() + index + substr.length());
		}
	}
	else
	{
		std::string upsrc = src;
		std::string upsubstr = substr;
		string_upper(upsrc);
		string_upper(upsubstr);
		int index = upsrc.find(upsubstr, bpos);
		if (index != std::string::npos)
		{
			str = std::string(src.c_str() + index + substr.length());
		}
	}
	return str;
}

std::string midstr(std::string src, std::string lstr, std::string rstr)
{
	std::string  str = "";
	int b = src.find(lstr);
	if (b != std::string::npos)
	{
		int e = src.find(rstr, b);
		if (e != std::string::npos)
		{
			str = src.substr(b + lstr.length(), e - b - lstr.length());
		}
	}
	return str;
}

std::string num2str_0(int arg_num, int arg_len)
{
	std::string  str = "", tmp = "";
	tmp = std::to_string(arg_num);
	int zlen = arg_len - tmp.length();
	for (size_t i = 0; i < zlen; i++)
	{
		str.append("0");
	}
	str.append(tmp);

	return str;
}

std::string rsubstr(const std::string& src_str, const size_t len)
{
	const size_t size(src_str.length());
	if (len >= size)
	{
		return src_str;
	}
	else
	{
		return src_str.substr(size - len, len);
	}
}

// 删除左侧空格
std::string& trimleft(std::string& str)
{
	str.erase(0, str.find_first_not_of(std::ctype_base::space));
	return str;
}

// 删除右侧空格
std::string& trimright(std::string& str)
{
	str.erase(str.find_last_not_of(std::ctype_base::space) + 1);
	return str;
}

// 删除首尾空格
std::string& trim(std::string& str)
{
	return trimleft(trimright(str));
}
//取自进程全路径
std::string  getselfpath()
{
	char exeFullPath[MAX_PATH] = { 0 };
	GetModuleFileNameA(NULL, exeFullPath, MAX_PATH);
	return std::string(exeFullPath);
}



//i要转化的十进制整数，width转化后的宽度，位数不足则补0
std::string dec2hex(int i, int width)
{
	using std::stringstream;

	std::stringstream ioss;     //定义字符串流
	std::string s_temp;         //存放转化后字符
	ioss << std::hex << i;      //以十六制形式输出
	ioss >> s_temp;

	if (width > s_temp.size())
	{
		std::string s_0(width - s_temp.size(), '0');      //位数不够则补0
		s_temp = s_0 + s_temp;                            //合并
	}

	std::string s = s_temp.substr(s_temp.length() - width, s_temp.length());    //取右width位
	return s;
}

uint32_t  hexstr_to_dword(char* str) {
	char* ptr = (char*)str;
	uint32_t tmp;
	uint32_t v = 0;
	uint32_t i = 0;

	while (*ptr) ptr++;

	for (ptr--; ptr >= str && i < 32; ptr--) {
		if (*ptr >= '0' && *ptr <= '9') {
			tmp = *ptr - '0';
		}
		else if (*ptr >= 'a' && *ptr <= 'f') {
			tmp = *ptr - 'a' + 10;
		}
		else {
			break;
		}
		v |= tmp << i;
		i += 4;
	}
	return v;
}

uint32_t string_to_dword(std::string& arg_str, const char* fmt)
{
	uint32_t ret = 0;
	sscanf(arg_str.c_str(), fmt, &ret);
	return ret;
}

//备用
// Split a std::string in a vector by a delimiter
std::vector<std::string> string_split(std::string str, std::string delimiter)
{
	size_t pos_start = 0, pos_end;
	std::string token;
	std::vector<std::string> splitString;

	while ((pos_end = str.find(delimiter, pos_start)) != std::string::npos) {
		token = str.substr(pos_start, pos_end - pos_start);
		pos_start = pos_end + delimiter.length();
		splitString.push_back(token);
	}

	splitString.push_back(str.substr(pos_start));
	return splitString;
}

// Split a wide std::string in a vector by a delimiter
std::vector<std::wstring> string_split(std::wstring str, std::wstring delimiter)
{
	size_t pos_start = 0, pos_end;
	std::wstring token;
	std::vector<std::wstring> splitWString;

	while ((pos_end = str.find(delimiter, pos_start)) != std::string::npos) {
		token = str.substr(pos_start, pos_end - pos_start);
		pos_start = pos_end + delimiter.length();
		splitWString.push_back(token);
	}

	splitWString.push_back(str.substr(pos_start));
	return splitWString;
}

// Small wrapper function for std::string.find()
bool string_find(std::string haystack, std::string needle)
{
	return haystack.find(needle) != std::string::npos;
}

// Small wrapper function for wstring.find()
bool string_find(std::wstring haystack, std::wstring needle)
{
	return haystack.find(needle) != std::string::npos;
}

// Small wrapper function for checking if a std::string starts with another std::string
bool string_starts_with(std::string str, std::string prefix)
{
	return str.rfind(prefix, 0) == 0;

}

// Small wrapper function for checking if a wide std::string starts with another wide std::string
bool string_starts_with(std::wstring str, std::wstring prefix)
{
	return str.rfind(prefix, 0) == 0;
}

// Check if a std::string ends with another std::string
bool string_ends_with(std::string str, std::string suffix)
{
	auto it = suffix.begin();
	return str.size() >= suffix.size() &&
		std::all_of(std::next(str.begin(), str.size() - suffix.size()), str.end(), [&it](const char& c) {
		return c == *(it++);
			});
}

// Check if a wide std::string ends with another wide std::string
bool string_ends_with(std::wstring str, std::wstring suffix)
{
	auto it = suffix.begin();
	return str.size() >= suffix.size() &&
		std::all_of(std::next(str.begin(), str.size() - suffix.size()), str.end(), [&it](const char& c) {
		return c == *(it++);
			});
}

bool string_replace_once(std::string& str, const std::string& from, const std::string& to) {
	size_t start_pos = str.find(from);
	if (start_pos == std::string::npos)
		return false;
	str.replace(start_pos, from.length(), to);
	return true;
}

std::string& string_replaceAll(std::string& str, const std::string& from, const std::string& to) {
	if (from.empty())
		return str;
	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
		str.replace(start_pos, from.length(), to);
		start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
	}
	return str;
}

std::string string_replaceAll_(const std::string& str, const std::string& from, const std::string& to) {
	std::string str_local = str;
	if (from.empty())
		return str_local;
	size_t start_pos = 0;
	while ((start_pos = str_local.find(from, start_pos)) != std::string::npos) {
		str_local.replace(start_pos, from.length(), to);
		start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
	}
	return str_local;
}

std::string string_from_bin(const std::vector<BYTE>& arg_bin)
{
	std::vector<BYTE> bin;
	bin.resize(arg_bin.size()+1,0);
	memcpy(bin.data(), arg_bin.data(), arg_bin.size());
	return std::string((char*)bin.data());
}

std::string string_from_bin(BYTE* p, int len)
{
	std::vector<BYTE> bin;
	bin.resize(len + 1, 0);
	memcpy(bin.data(), p, len);
	return std::string((char*)bin.data());
}
