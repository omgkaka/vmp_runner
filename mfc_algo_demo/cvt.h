#ifndef    _CVTAXXXBXXXC_H_
#define    _CVTAXXXBXXXC_H_

//#include <stdio.h>  
#include <iostream>
#include <atlstr.h>
#include <wchar.h>
#include <string>
#include <vector>
#include <deque>

#include <sstream>


#ifdef _UNICODE
#define t_sscanf swscanf
#define t_sscanf_s swscanf_s
#define t_sprintf swprintf 
#define t_sprintf_s swprintf_s
#else
#define t_sscanf sscanf
#define t_sscanf_s sscanf_s
#define t_sprintf sprintf 
#define t_sprintf_s sprintf_s
#endif 


#pragma warning(disable:4996)

//定义虚拟机保护的宏
#define   VMProtectBegin \
    _asm _emit 0xEB \
    _asm _emit 0x10 \
    _asm _emit 0x56 \
    _asm _emit 0x4D \
    _asm _emit 0x50 \
    _asm _emit 0x72 \
    _asm _emit 0x6F \
    _asm _emit 0x74 \
    _asm _emit 0x65 \
    _asm _emit 0x63 \
    _asm _emit 0x74 \
    _asm _emit 0x20 \
    _asm _emit 0x62 \
    _asm _emit 0x65 \
    _asm _emit 0x67 \
    _asm _emit 0x69 \
    _asm _emit 0x6E \
    _asm _emit 0x00

#define  VMProtectEnd \
    __asm _emit 0xEB \
    __asm _emit 0x0E \
    __asm _emit 0x56 \
    __asm _emit 0x4D \
    __asm _emit 0x50 \
    __asm _emit 0x72 \
    __asm _emit 0x6F \
    __asm _emit 0x74 \
    __asm _emit 0x65 \
    __asm _emit 0x63 \
    __asm _emit 0x74 \
    __asm _emit 0x20 \
    __asm _emit 0x65 \
    __asm _emit 0x6E \
    __asm _emit 0x64 \
    __asm _emit 0x00



wchar_t* ANSIToUnicode(const char* str);
char* UnicodeToANSI(const wchar_t* str);
wchar_t* UTF8ToUnicode(const char* str);
char* UnicodeToUTF8(const wchar_t* str);
char* w2m(const wchar_t* wcs);
wchar_t* m2w(const char* mbs);
char* ANSIToUTF8(const char* str);
char* UTF8ToANSI(const char* str);
WCHAR* A2W_(const char* _char);
char* W2A_(const wchar_t* tchar);
void hexstr2bin_(BYTE* pbDest, BYTE* pbSrc);
BYTE* hexstr2bin(char* pbSrc);
BYTE* hexstr2bin_end00(char* pbSrc);
void bin2hexstr_(BYTE* pbDest, BYTE* pbSrc, size_t nLen);
BYTE* bin2hexstr(BYTE* pbSrc, size_t nLen);
char* Reverse(char* s);
char* Reverse_(char* s);
std::vector<std::string> split(std::string str, std::string pattern);

BYTE* readfiletobuf(LPCSTR sPath, int& size);//小文件用
uint8_t* readfiletobuf(char* sPath, size_t& size);

//win下SEH版本
BOOL file2buf(LPCSTR sPath, BYTE*& pout, ULONG& size);
//非win下版本
bool file2buf_(char* sPath, uint8_t*& pout, uint32_t& size);
BOOL buf2file(char* path, char* data, ULONG len);
BOOL buf2file(const std::string& path, const std::string& data, ULONG len);
bool buf2file(char* path, char* data, size_t len);




BYTE* en_xor(BYTE* pbin, size_t len, BYTE* pkey);
BYTE* de_xor(BYTE* pbin, size_t len, BYTE* pkey);
void* memset_(void* pvBuffer, size_t iValue, size_t nBufferLen);

void print_string_hex(const char* comment, unsigned char* str, size_t len);
void hex_dump(unsigned char* ptr, size_t len);

std::string replace_str(std::string& str, const std::string& to_replaced, const std::string& newchars);
std::string string_replace(std::string& strBig, const std::string& strsrc, const std::string& strdst);

//原字符串转换大小写
void string_lower(std::string& s);
void string_upper(std::string& s);

//原地址转小写  改变了参数字符串
char* strToLower(char* str);

//strrstr函数用来从一个字符串尾部开始查找另一个字符串出现的第一个位置，这个函数的宽字符版本
static inline const wchar_t* wcsrstr(_In_z_ const wchar_t* _String, _In_z_ const wchar_t* _SubString)
{
	const wchar_t* pLast = _String + wcslen(_String);
	const wchar_t* pFind = NULL;
	for (; pLast >= _String; pLast--)
	{
		if (*pLast == *_SubString)
		{
			if (wcsncmp(pLast, _SubString, wcslen(_SubString)) == 0)
			{
				pFind = pLast;
			}
		}
		if (pFind)
			break;
	}
	return pFind;
}


std::string w2string(wchar_t* w);

class LPCTSTR_CLASS
{
public:
	explicit LPCTSTR_CLASS(std::string str);
	~LPCTSTR_CLASS();

public:
	LPCTSTR ptr;
};


static inline std::wstring a2w(std::string ch, unsigned int CodePage = CP_ACP)
{
	if (ch.empty())return L"";
	std::wstring ret;
	DWORD dwOutSize = 0;
	dwOutSize = MultiByteToWideChar(CodePage, 0, ch.c_str(), -1, NULL, 0);

	ret.resize(dwOutSize - 1);
	MultiByteToWideChar(CodePage, 0, ch.c_str(), ch.size(), &ret[0], dwOutSize);

	return ret;
}

static inline std::string w2a(std::wstring wch, unsigned int CodePage = CP_ACP)
{
	std::string ret;
	DWORD dwOutSize = 0;
	dwOutSize = WideCharToMultiByte(CodePage, 0, wch.c_str(), -1, NULL, 0, NULL, FALSE);

	char* pwText = 0;
	pwText = new char[dwOutSize];
	pwText[dwOutSize - 1] = '\0';

	WideCharToMultiByte(CodePage, 0, wch.c_str(), wch.size(), pwText, dwOutSize, NULL, FALSE);

	ret = pwText;
	if (pwText)delete[]pwText;

	return ret;
}

//A字符串原地址转16进制 成功返回长度  失败返回-1,不足偶数前补0
static inline int strtohex(char* str)
{
	int len = 0;
	char* ptr = str, buffer[2] = { 0 };

	/* if std::string length is even, first char is a full number */
	if (strlen(str) % 2)
	{
		if (sscanf(ptr, "%01x", buffer) != 1) return -1;
		*ptr++ = *buffer;
		len = 1;
	}

	while (*ptr)
	{
		if (sscanf(ptr, "%02x", str + len++) != 1) return -1;
		ptr += 2;
	}

	return len;
}



/*
//*******************************************************************************************
//取自hexctrl
//快速查找数组
extern  const wchar_t* const g_pwszHexMap;
extern  const char* const g_pszHexMap;

//Converts dwSize bytes of ull to WCHAR std::string.
void UllToWchars(ULONGLONG ull, wchar_t* pwsz, size_t dwSize, bool fAsHex = true);

//Converts char* std::string to unsigned long number. Basically it's a strtoul() wrapper.
//Returns false if conversion is imposible, true otherwise.
bool CharsToUl(const char* pcsz, unsigned long& ul);

//Converts wstring to ULONGLONG or LONGLONG depending on template type argument.
template<typename T>inline bool wstr2num(std::wstring_view wstr, T& tData, int iBase = 0);

//Wide std::string to Multibyte std::string convertion.
std::string WstrToStr(std::wstring_view wstr);

//Multibyte std::string to wide std::string.
std::wstring StrToWstr(std::string_view str);

//Converts every two numeric chars to one respective hex character: "56"->V(0x56), "7A"->z(0x7A)
bool StrToHex(const std::string& strFrom, std::string& strToHex);

//*****************************************************************************************

*/

//c++ 11 自带    iota递增
template<class ForwardIterator, class T>
void iota_inc(ForwardIterator first, ForwardIterator last, T value)
{
	while (first != last) {
		*first++ = value;
		++value;
	}
}

template<class ForwardIterator, class T>
void iota_dec(ForwardIterator first, ForwardIterator last, T value)
{
	while (first != last) {
		*first++ = value;
		--value;
	}
}

/*
#include <numeric>

std::vector<int> ivec(10);
std::iota(ivec.begin(), ivec.end(), 0);
// ivec: 0,1,2,3,4,5,6,7,8,9

// 这个递增可不止局限于数值哟
std::vector<std::vector<int>::iterator> iter_vec(ivec.size());
std::iota(iter_vec.begin(), iter_vec.end(), ivec.begin());

for (auto it : iter_vec)
	std::cout << *it << " ";
// out: 0,1,2,3,4,5,6,7,8,9

*/


std::vector<std::string> SplitString(const  std::string& s, const std::string& delim = ",");
std::vector<std::string>* SplitString_(const  std::string& s, const std::string& delim = ",");

std::vector<CString> SplitString(CString& str, char pattern);
std::vector<CString> SplitString_(CString& str, char pattern);
std::vector<CString> SplitString(CString& str, char pattern[]);

std::vector<BYTE> hex2bin(CString& arg_src);


//多字符集下
//std::string-->CStringA
//CString strCStemp = strS.c_str();//注意，可写在同一行

//unicode下
//注意：std::string-- > CString时，不可以写在同一行：
//CString strCStemp = strS.c_str();//ERROR
//下面正确
/*
CString strCStemp;
strCStemp = strS.c_str();//std::string-->CString
*/



/**
 * Replaces the provided substring with the replacement std::string in the origin std::string.
 * The replacement may be an empty std::string.
 *
 * @param origin the std::string to manipulate
 * @param toReplace std::string that will be replaced
 * @param replacement std::string that will be inserted instead of toReplace
 * @return The origin std::string with the defined replacements as a new std::string.
 */
std::string replaceSubstrings(const std::string& origin, const std::string& toReplace, const std::string& replacement);

/**
 * Searches for a substring byte by byte and returns its start position byte number in the original std::string.
 * If the toSearch std::string is not contained in the origin std::string origin.size() is returned. Only the first
 * substring that is contained, will be considered.
 * @param origin The std::string to search in
 * @param toSearcfh the std::string to search in the origin.
 * @return byte position of the start of the substring in the origin std::string or origin.size() if not contained
 */
size_t searchForSubstring(const std::string& origin, const std::string& toSearch);

/**
 * Returns if the two strings match in the number of given bytes.
 * Strings do not necessarily need to be null terminated, but in case they are
 * not the result may be inaccurate.
 * @param one std::string to compare
 * @param two std::string to compare
 * @return true if each byte matches
 */
bool matches(const char* one, const char* two, size_t bytes);

//***************************************************************************************************************************************
	//sunday


BYTE* sunday_find(BYTE* psrc, ULONGLONG len_src, BYTE* pdst, ULONGLONG len_dst);
//返回找到所有的指针首地址
std::vector<BYTE*> sunday_findall(BYTE* psrc, ULONGLONG len_src, BYTE* pdst, ULONGLONG len_dst);

//快速strstr
int sunday_strStr(char* psrc, char* pdst);
//***************************************************************************************************************************************

//取指定子串左边  默认区分大小写
std::string getstrL(std::string src, std::string substr, int bpos = 0, bool nocase = false);

std::string getstrR(std::string src, std::string substr, int bpos = 0, bool nocase = false);

std::string midstr(std::string src, std::string lstr, std::string rstr);

std::string num2str_0(int arg_num, int arg_len = 6);

std::string rsubstr(const std::string& src_str, const size_t len);

// 删除左侧空格
std::string& trimleft(std::string& str);

// 删除右侧空格
std::string& trimright(std::string& str);

// 删除首尾空格
std::string& trim(std::string& str);

//取自进程全路径
std::string  getselfpath();

//模板int转hex
template <class T, class U>
U* Int2Hex(T lnumber, U* buffer)
{
	const char* ref = "0123456789ABCDEF";
	T hNibbles = (lnumber >> 4);

	unsigned char* b_lNibbles = (unsigned char*)&lnumber;
	unsigned char* b_hNibbles = (unsigned char*)&hNibbles;

	U* pointer = buffer + (sizeof(lnumber) << 1);

	*pointer = 0;
	do {
		*--pointer = ref[(*b_lNibbles++) & 0xF];
		*--pointer = ref[(*b_hNibbles++) & 0xF];
	} while (pointer > buffer);

	return buffer;
}

/*

char buffer[100] = { 0 };
Int2Hex(305419896ULL, buffer);//returns "0000000012345678"
Int2Hex(305419896UL, buffer);//returns "12345678"
Int2Hex((short)65533, buffer);//returns "FFFD"
Int2Hex((char)18, buffer);//returns "12"

wchar_t buffer[100] = { 0 };
Int2Hex(305419896ULL, buffer);//returns L"0000000012345678"
Int2Hex(305419896UL, buffer);//returns L"12345678"
Int2Hex((short)65533, buffer);//returns L"FFFD"
Int2Hex((char)18, buffer);//returns L"12"

*/


//i要转化的十进制整数，width转化后的宽度，位数不足则补0
std::string dec2hex(int i, int width);

/*
转换一个全小写的十六进制字符串到32位整数，原理是从字符串末尾开始转换
0x4750524d1a  超过32位长度将只转换低32位--->0x50524d1a
*/
uint32_t  hexstr_to_dword(char* str);

//模板函数：将std::string类型变量转换为常用的数值类型（此方法具有普遍适用性） 这种到数值没有进制转换！ 008011---> 8011 
template <class T>
T stringToNum(std::string& str) {
	istringstream iss(str);
	T num;
	iss >> num;
	return num;
}

//带格式字符串到整数 
uint32_t string_to_dword(std::string& arg_str, const char* fmt);


//备用
// Split a std::string in a vector by a delimiter
std::vector<std::string> string_split(std::string str, std::string delimiter);

// Split a wide std::string in a vector by a delimiter
std::vector<std::wstring> string_split(std::wstring str, std::wstring delimiter);

// Small wrapper function for std::string.find()
bool string_find(std::string haystack, std::string needle);

// Small wrapper function for wstring.find()
bool string_find(std::wstring haystack, std::wstring needle);

// Small wrapper function for checking if a std::string starts with another std::string
bool string_starts_with(std::string str, std::string prefix);

// Small wrapper function for checking if a wide std::string starts with another wide std::string
bool string_starts_with(std::wstring str, std::wstring prefix);

// Check if a std::string ends with another std::string
bool string_ends_with(std::string str, std::string suffix);

// Check if a wide std::string ends with another wide std::string
bool string_ends_with(std::wstring str, std::wstring suffix);

//直接修改的就是参数1 str,返回的引用也直接是str  !!!
bool string_replace_once(std::string& str, const std::string& from, const std::string& to);
std::string& string_replaceAll(std::string& str, const std::string& from, const std::string& to);

//提供一个不修改源字符串的替换版本
std::string string_replaceAll_(const std::string& str, const std::string& from, const std::string& to);

//尾部加0  到文本
std::string string_from_bin(const std::vector<BYTE>& arg_bin);
std::string string_from_bin(BYTE* p,int len);


#endif




//1.1使用函数模板+ostringstream

//使用函数模板将基本数据类型（整型、字符型、实型、布尔型）转换成std::string。

// template<typename T> std::string to_str(T val)
// {
// 	ostringstream oss;
// 	oss<<val;
// 	return oss.str();
// }
// cout<<toString(14.2)<<endl;  //实型->std::string：输出14.2
// cout<<toString(12301)<<endl; //整型->std::string：输出12301
// cout<<toString(123456789785)<<endl;//长整型->std::string：输出123456789785
// cout<<toString(true)<<endl;  //布尔型->std::string：输出1
// 

//1.2使用标准库函数std::to_string()

//std命令空间下有一个C++标准库函数std::to_string()，可用于将数值类型转换为std::string。使用时需要include头文件<string>。

//	函数原型申明如下：
// 
// std::string to_string (int val);
// std::string to_string (long val);
// std::string to_string (long long val);
// std::string to_string (unsigned val);
// std::string to_string (unsigned long val);
// std::string to_string (unsigned long long val);
// std::string to_string (float val);
// std::string to_string (double val);
// std::string to_string (long double val);
// 

//2.std::string转换为数值类型
//	2.1使用函数模板+ istringstream
// #include <iostream>  
// #include <sstream>    //使用stringstream需要引入这个头文件  
// using namespace std;  

//模板函数：将std::string类型变量转换为常用的数值类型（此方法具有普遍适用性）  
// template <class Type>  
// Type stringToNum(const std::string& str){  
// 	istringstream iss(str);  
// 	Type num;  
// 	iss >> num;  
// 	return num;      
// }  

// std::string str("00801");  
//cout << stringToNum<int>(str) << endl;  
// 

// int n = 10;
// TCHAR sz[8] = { 0 };
// _stprintf(sz, TEXT("%02X"), n);

//std::string to char* if need modify char*  else just use str.c_str()
/*
std::string str="aaaaaaaaaaaaa";
char *cstr = new char[str.length() + 1];
strcpy(cstr, str.c_str());
// do stuff
delete[] cstr;
*/




/*
《C++标准函数库》中说的
有三个函数可以将字符串的内容转换为字符数组和C—std::string
1.data(), 返回没有”\0“的字符串数组
2, c_str()，返回有”\0“的字符串数组
3，copy()
.................................................................
int 转 CString：
CString.Format("%d", int);
...............................
std::string 转 CString
CString.format("%s", std::string.c_str());
用c_str()确实比data()要好.
.......................................
char* 转 CString
CString.format("%s", char*);
CString strtest;
char * charpoint;
charpoint = "give std::string a value";
strtest = charpoint; //直接付值
...................................................................
CString 转 int
CString  ss = "1212.12";
int temp = atoi(ss); //atoi _atoi64或atol

将字符转换为整数，可以使用atoi、_atoi64或atol。
int int_chage = atoi((lpcstr)ss);
或：
CString str = "23";
UINT uint;
sscanf(str, "%d", uint);
..............................
std::string 转 int
..............................
char* 转 int
#include <stdlib.h>

int atoi(const char *nptr);
long atol(const char *nptr);
long long atoll(const char *nptr);
long long atoq(const char *nptr);
...................................................................
CString 转 std::string
std::string s(CString.GetBuffer());
GetBuffer()后一定要ReleaseBuffer(), 否则就没有释放缓冲区所占的空间.
..........................................
int 转 std::string
..........................................
char* 转 std::string
std::string s(char *);
你的只能初始化，在不是初始化的地方最好还是用assign().
...................................................................
CString 转 char *
CString strtest = "wwwwttttttt";
charpoint = strtest.GetBuffer(strtest.GetLength());
CString转换 char[100]
char a[100];
CString str("aaaaaa");
strncpy(a, (LPCTSTR)str, sizeof(a));
CString  str = "aaa";
char*  ch;
ch = (char*)(LPCTSTR)str;
..........................................
int 转 char *
在stdlib.h中有个函数itoa()
itoa的用法：
itoa(i, num, 10);
i 需要转换成字符的数字
num 转换后保存字符的变量
10 转换数字的基数（进制）10就是说按照10进制转换数字。还可以是2，8，16等等你喜欢的进制类型
原形：char *itoa(int value, char* std::string, int radix);
实例：
#include "stdlib.h"
#include "stdio.h"
main()
{
	int i = 1234;
	char s[5];
	itoa(i, s, 10);
	printf("%s", s);
	getchar();
}
..........................................
std::string 转 char *
char *p = std::string.c_str();

std::string aa("aaa");
char *c = aa.c_str();
std::string mngName；
char t[200];
memset(t, 0, 200);
strcpy(t, mngName.c_str());
...................................................................
标准C里没有std::string, char *= = char[] == std::string
可以用CString.Format("%s", char *)这个方法来将char *转成CString。要把CString转成char *，用操
作符（LPCSTR）CString就可以了。
cannot convert from 'const char *' to 'char *'
const char *c = aa.c_str();
std::string.c_str()只能转换成const char *





FILE * fopen(const char * path,const char * mode);
	-- path: 文件路径，如："F:\Visual Stdio 2012\test.txt"
	-- mode: 文件打开方式，例如：
			 "r" 以只读方式打开文件，该文件必须存在。
			 "w" 打开只写文件，若文件存在则文件长度清为0，即该文件内容会消失。若文件不存在则建立该文件。
			"w+" 打开可读写文件，若文件存在则文件长度清为零，即该文件内容会消失。若文件不存在则建立该文件。
			 "a" 以附加的方式打开只写文件。若文件不存在，则会建立该文件，如果文件存在，写入的数据会被加到文件尾，即文件原先的内容会被保留。（EOF符保留）
			 "a+" 以附加方式打开可读写的文件。若文件不存在，则会建立该文件，如果文件存在，写入的数据会被加到文件尾后，即文件原先的内容会被保留。（原来的EOF符不保留）
			"wb" 只写打开或新建一个二进制文件，只允许写数据。
			"wb+" 读写打开或建立一个二进制文件，允许读和写。
			 "ab" 追加打开一个二进制文件，并在文件末尾写数据。
			 "ab+"读写打开一个二进制文件，允许读，或在文件末追加数据。
	--返回值: 文件顺利打开后，指向该流的文件指针就会被返回。如果文件打开失败则返回NULL，并把错误代码存在errno中。


	1 size_t fwrite(const void* buffer, size_t size, size_t count, FILE* stream);
2     -- buffer:指向数据块的指针
3     -- size:每个数据的大小，单位为Byte(例如：sizeof(int)就是4)
4     -- count:数据个数
5     -- stream:文件指针


1 size_t fread(void *buffer, size_t size, size_t count, FILE *stream);
2   -- buffer:指向数据块的指针
3   -- size:每个数据的大小，单位为Byte(例如：sizeof(int)就是4)
4   -- count:数据个数
5   -- stream:文件指针




1.string与cstring有什么区别
是C++标准库头文件，包含了拟容器class std::string的声明（不过class string事实上只是basic_string的typedef），用于字符串操作。
是C标准库头文件<string.h>的C++标准库版本，包含了C风格字符串（NUL即’\0’结尾字符串）相关的一些类型和函数的声明，例如strcmp、strchr、strstr等。和<string.h>的最大区别在于，其中声明的名称都是位于std命名空间中的，而不是后者的全局命名空间。
看定义就知道了，string是新标准，定义了namespace std;而cstring虽然也是新标，但是定义中包含的是string.h。

string中可以进行+ = += >等运算，而cstring中不能进行相关运算。

#include //不可以定义string s；可以用到strcpy等函数

using namespace std;

#include //可以定义string s；可以用到strcpy等函数
using namesapce std;

#include <string.h> //不可以定义string s；可以用到strcpy等函数

2.
1）文件cstring，和string.h对应，c++版本的头文件，包含比如strcpy之类的字符串处理函数
2）文件string.h，和cstring对应，c版本的头文件，包含比如strcpy之类的字符串处理函数
3）文件string，包含std::string的定义，属于STL范畴
4）CString，MFC里的的字符串类

string.h是C语言中字符串操作函数的头文件
cstring是c++对C语言中的strcpy之类的函数申明，包含cstring之后，就可以在程序中使用C语言风格的strcpy之类的函数。

string是c++语言中string类模板的申明
CString是MFC中定义的字符串类，MFC中很多类及函数都是以CString为参数的，另外CString类重载了（LPCSTR）运算符，所以如果你在MFC下面使用CString类，就可以直接用CString类做为参数来调用需要一个C语言风格字符串的win api函数，编译器会自动调用(LPCSTR)成员函数完成从CString到一个C风格字符串的转换。如果你在MFC下使用C++语言中标准的 string类，那么在调用需要C语言风格的字符串为参数的win api时，你必须显示调用sting.c_str()成员函数，来完成同样的转换，也就是说在使用MFC里，如果用CString类，会比sting类方便那么一点点。

3.
(1).首先说cstring与string.h:
cstring和string.h其实里面都是C标准库提供的东西，某些实现中cstring的内容
就是:
namespace std
{
#include <string.h>
}
cstring是C++的组成部分，它可以说是把C的string.h的升级版，但它不是C的组成部分。
所以如果你用的是C++，那么请用cstring,如果你用的是C请用string.h。

(2).string与cstring:
一般一个C++库老的版本带“.h”扩展名的库文件，比如iostream.h，在新标准后的标准库中都有一个不带“.h”扩展名的相对应，区别除了后者的好多改进之外，还有一点就是后者的东东都塞进了“std”名字空间中。
string，它是C++定义的std::string所使用的文件，是string类的头文件，属于STL范畴。它有很多对字符串操作的方法。

4.string.h是C++标准化（1998年）以前的C++库文件，在标准化过程中，为了兼容以前，标准化组织将所有这些文件都进行了新的定义，加入到了标准库中，加入后的文件名就新增了一个"c"前缀并且去掉了.h的后缀名，所以string.h头文件成了cstring头文件。但是其实现却是相同的或是兼容以前的。相当于标准库组织给它盖了个章，说“你也是我的标准程序库的一份子了”

5.cstring代表的是string.h，但是被封装到了std里面，譬如调用strlen函数，需要写成std::strlen(yourstr)才行，这个使用方法比较符合C++的标准要求string就是C++标准库里面的string模板（确切地说应该是一个特化的模板），但是他同样包含了C风格字符串操作函数的定义（应该是通过包含string.h实现的）string.h就不需要使用名字空间了，这个是C风格字符串操作的一个函数库，strlen，strcpy，strcat，strcmp……都在这里面了，不过既然是C风格的库，当然不需要namespace支持了。

//非编译单元抑制警告方法
//For Visual Studio, the command line argument is /wd4101. Note there is not the normal : between the flag and the number, and you can't do a comma separated list of numbers. For other compilers it might be /nowarn:4101 instead.
#pragma warning( push )
#pragma warning( disable : WarningCode)
// code with warning
#pragma warning( pop ) 

*/

