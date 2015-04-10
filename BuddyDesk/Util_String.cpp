/********************************************************************
*	File Name	: Util_String.cpp
*	Version		: 
*	Purpose		: The definition of some public functions about string operation.
*	Create Date	: 10/8/2005 3:23:30 PM
*	Author		: Van Zhao
*	Copyright (C) MorningStar Corporation.  All Rights Reserved. 
********************************************************************/
#include "StdAfx.h"
#include <atlrx.h>
#include <atlutil.h>
#include "comutil.h"
#include "util_string.h"
#include <errno.h>
#include <iosfwd>
#include "atlcommon.h"
#include "commonconst.h"
#include <set>
#ifdef _AFX
#include <afxpriv.h>
#endif 

using namespace Buddy::Util;
using namespace std;

//To convert string to Int.
int CStrUtil::ToInt(const CString &str)
{
	return int(ToLong(str));
}

long CStrUtil::HexToLongEx(const CString &str)
{
	if ( str.IsEmpty() )
		return 0;

	LPTSTR endptr;
	LPCTSTR startptr = str;
	errno = 0;
	long result = _tcstol(startptr, &endptr, 16);
	if (errno == ERANGE || startptr == endptr)
	{
		RaiseError(ERANGE, _T("error range"));
	}
	return result;
}

//To convert string to Long.
long CStrUtil::ToLong(const CString &str)
{
	return ToLong2(str,0);
}

long CStrUtil::ToLong2(const CString &str,long lDef)
{
	if ( str.IsEmpty() )
		return lDef;

	try
	{
		return ToLongEx(str);
	}
	catch(long /*ec*/)
	{
		return lDef;
	}
}

long CStrUtil::ToLongEx(const CString &str)
{
	if (str.IsEmpty() )
		return 0;

	LPTSTR endptr;
	LPCTSTR startptr = str;
	errno = 0;
	long result = _tcstol(startptr, &endptr, 0);
	if( result == 0)	//maybe ambiguity number base, try 10 base again.
		result = _tcstol(startptr, &endptr, 10);
	if (errno == ERANGE || startptr == endptr)
	{
		RaiseError(ERANGE, _T("error range"));
	}
	return result;
}

// isn't int == long on windows?
// in case number of shares of one particular holding is too large
long long CStrUtil::ToLongLong(const CString& str, long long llDef)
{
	try{
		return ToLongLongEx(str);
	}
	catch(...)
	{
		return llDef;
	}
}

long long CStrUtil::ToLongLongEx(const CString& str)
{
	if (str.IsEmpty() )
		RaiseError(EINVAL, _T("error input is empty"));

	LPTSTR endptr;
	LPCTSTR startptr = str;
	errno = 0;
	long long result = _tcstoi64(startptr, &endptr, 0);
	if( result == 0)	//maybe ambiguity number base, try 10 base again.
		result = _tcstoi64(startptr, &endptr, 10);
	if (errno == ERANGE || startptr == endptr)
	{
		RaiseError(ERANGE, _T("error range"));
	}
	return result;
}

//To convert string to Double.
double CStrUtil::ToDouble(const CString &str)
{
	return ToDouble2(str,0L);
}
double CStrUtil::ToDouble2(const CString &str, double dDef)
{
	double dRes = ToDoubleEx(str);
	if (CommonConst::IsNullDouble(dRes))
		dRes = dDef;
	return dRes;
}

double CStrUtil::ToDoubleEx(const CString &str)
{
	LPTSTR endptr;
	LPCTSTR startptr = str;
	errno = 0;
	double result = _tcstod(startptr, &endptr);
	if (errno == ERANGE || startptr == endptr)
		result =  CommonConst::cstNullDouble;
	return result;
}
//To convert string to Bool.
bool CStrUtil::ToBool(const CString &str)
{
	// Convert "1", "true", "t", "yes", "y"  to true and otherwise false.
	if( str.CompareNoCase(_T("1"))==0 || str.CompareNoCase(_T("TRUE"))==0 || str.CompareNoCase(_T("t"))==0 ||
		str.CompareNoCase(_T("y"))==0 || str.CompareNoCase(_T("yes"))==0)
		return true;
	else
		return false;
}

//To convert string to Bool.
bool CStrUtil::ToBoolDef(const CString &str,bool bDefault)
{
	if (str.IsEmpty())
		return bDefault;
	else
		return ToBool(str);
}
//Converts a given integer to a string.
CString CStrUtil::FormatInt(const int value, const int base)
{
	//Todo: different base, FormatDouble...
	CString str;
	str.Format(_T("%d"), value);
	return str;
}

//Description	:Converts a given double to a string.
CString CStrUtil::FormatDouble(double value, int num)
{
	TCHAR cVal[512];
	TCHAR cFormat[64];

	if(!_finite(value))
		return _T("");
	if(num>128)	num = 128;

#if _MSC_VER < 1400 // previous compiler before vs2005
	_stprintf(cFormat, _T("%%.%df"), num);
	_stprintf(cVal, cFormat, value);
#else
	_stprintf_s(cFormat, _T("%%.%df"), num);
	_stprintf_s(cVal, cFormat, value);
#endif
	return CString(cVal);
}

double CStrUtil::VariantToDouble(_variant_t value)
{
	if(value.vt==VT_R4||value.vt==VT_R8)
	{
		return value.dblVal;
	}
	else if(value.vt == VT_DATE)
	{
		return value.date;
	}
	else
	{
		return  ToDoubleEx(CString(value));
	}
}

CString CStrUtil::FormatVariant(_variant_t value)
{
	if(value.vt==VT_R4||value.vt==VT_R8)
	{
		if(value.dblVal> CommonConst::cstMaxDouble|| value.dblVal<CommonConst::cstMinDouble)
		{
			return CommonConst::cstEmptyStr;
		}
		else
		{
			double dVal = ToDoubleEx(FormatDouble(value.dblVal, 128));
			if (CommonConst::IsNullDouble(dVal))
				return CommonConst::cstEmptyStr;
			TCHAR cVal[256]={0};
#if _MSC_VER < 1400 // previous compiler before vs2005
			_stprintf(cVal, _T("%f"), dVal);
#else
			_stprintf_s(cVal,256, _T("%f"), dVal);
#endif			
			return CString(cVal);
		}
	}
	else if(value.vt==VT_DATE)
	{
		CString strVal;
		COleDateTime dt = value.date;
		if(dt.GetStatus()==dt.valid && value.date>=2.0)
		{
			try{
				strVal = dt.Format(CommonConst::cstUSDateFmt);
			}catch(...)
			{
				;
			}
		}
		return strVal;
	}
	else
	{
		return CString(value);
	}
}

//To determine if string is start with sub-string.
bool CStrUtil::IsStartWith(const CString &strSour, const CString &strSub)
{
	return (0 == strSour.Find(strSub));
}
//To retrieve a value that indicates the hash code value for the CString.
ULONG	CStrUtil::GetHashCode(const CString &str)
{
	return CStringElementTraits<CString>::Hash(str);
}

//To convert BSTR to Ascii String
LPSTR CStrUtil::BSTRToASTR(const WCHAR* bstrString)
{
	int nBufSize = 0;
	LPSTR pszTempBuf = NULL;
	if(bstrString != NULL)
	{
		nBufSize = WideCharToMultiByte(CP_ACP,0,bstrString,lstrlenW(bstrString),NULL,0,NULL,NULL);
		if(nBufSize > 0)
		{
			pszTempBuf = new char [nBufSize + 64];
			if(pszTempBuf != NULL)
			{
				nBufSize = WideCharToMultiByte(CP_ACP,0,bstrString,lstrlenW(bstrString),pszTempBuf,nBufSize+1,NULL,NULL);
				if(nBufSize <= 0)
				{
					delete[] pszTempBuf;
					pszTempBuf = NULL;
				}
				else pszTempBuf[nBufSize] = 0;
			}
		}
	}
	return pszTempBuf;
}


// helper function for decoding
BYTE CStrUtil::MakeByte(TCHAR ch1, TCHAR ch2)
{
	BYTE bt1 = 0, bt2 = 0;

	switch (ch2)
	{
	case _T('0'):
		bt2 = 0x00;
		break;
	case _T('1'):
		bt2 = 0x01;
		break;
	case _T('2'):
		bt2 = 0x02;
		break;
	case _T('3'):
		bt2 = 0x03;
		break;
	case _T('4'):
		bt2 = 0x04;
		break;
	case _T('5'):
		bt2 = 0x05;
		break;
	case _T('6'):
		bt2 = 0x06;
		break;
	case _T('7'):
		bt2 = 0x07;
		break;
	case _T('8'):
		bt2 = 0x08;
		break;
	case _T('9'):
		bt2 = 0x09;
		break;
	case _T('A'):
		bt2 = 0x0A;
		break;
	case _T('B'):
		bt2 = 0x0B;
		break;
	case _T('C'):
		bt2 = 0x0C;
		break;
	case _T('D'):
		bt2 = 0x0D;
		break;
	case _T('E'):
		bt2 = 0x0E;
		break;
	case _T('F'):
		bt2 = 0x0F;
		break;
	}

	switch (ch1)
	{
	case _T('0'):
		bt1 = 0x00;
		break;
	case _T('1'):
		bt1 = 0x10;
		break;
	case _T('2'):
		bt1 = 0x20;
		break;
	case _T('3'):
		bt1 = 0x30;
		break;
	case _T('4'):
		bt1 = 0x40;
		break;
	case _T('5'):
		bt1 = 0x50;
		break;
	case _T('6'):
		bt1 = 0x60;
		break;
	case _T('7'):
		bt1 = 0x70;
		break;
	case _T('8'):
		bt1 = 0x80;
		break;
	case _T('9'):
		bt1 = 0x90;
		break;
	case _T('A'):
		bt1 = 0xA0;
		break;
	case _T('B'):
		bt1 = 0xB0;
		break;
	case _T('C'):
		bt1 = 0xC0;
		break;
	case _T('D'):
		bt1 = 0xD0;
		break;
	case _T('E'):
		bt1 = 0xE0;
		break;
	case _T('F'):
		bt1 = 0xF0;
		break;
	}

	BYTE btFinal = bt2 | bt1;

	return  btFinal;  

}

WCHAR* CStrUtil::DecodeFromUTF8(LPCTSTR szSource)
{
	ATLASSERT(szSource != NULL);

	size_t nMax = szSource==NULL?0:_tcslen(szSource);

	WCHAR* pwszBuf = new WCHAR[nMax + 1];
	memset(pwszBuf,0,sizeof(WCHAR) * (nMax + 1));

	BYTE z, y, x, w, v, u;
	int nIndex = 0;

	for (size_t n = 0; n < nMax; ++n)
	{
		WORD ch = (WORD)szSource[n];

		if (ch != _T('%'))
		{
			pwszBuf[nIndex++] = ch;
			continue;
		}

		if (n >= nMax - 2) break; // something is wrong
		z = MakeByte(szSource[n+1], szSource[n+2]);

		if (z < 127)
		{
			ch = z;
			n = n + 2;
		}
		else if (z >= 192 && z <= 223)
		{
			// character is two bytes
			if (n >= nMax - 5) break; // something is wrong
			y = MakeByte(szSource[n+4], szSource[n+5]);
			ch = (TCHAR)( (z-192)*64 + (y-128) );
			n = n + 5;
		}
		else if (z >= 224 && z <= 239)
		{
			// character is three bytes
			if (n >= nMax - 8) break; // something is wrong
			y = MakeByte(szSource[n+4], szSource[n+5]);
			x = MakeByte(szSource[n+7], szSource[n+8]);
			ch = ( (z-224)*4096 + (y-128)*64 + (x-128) );
			n = n + 8;
		}
		else if (z >= 240 && z <= 247)
		{
			// character is four bytes
			if (n >= nMax - 11) break; // something is wrong
			y = MakeByte(szSource[n+4], szSource[n+5]);
			x = MakeByte(szSource[n+7], szSource[n+8]);
			w = MakeByte(szSource[n+10], szSource[n+11]);
			ch = (TCHAR)( (z-240)*262144 + (y-128)*4096 + 
				(x-128)*64 + (w-128) );
			n = n + 11;
		}
		else if (z >= 248 && z <= 251)
		{
			// character is four bytes
			if (n >= nMax - 14) break; // something is wrong
			y = MakeByte(szSource[n+4], szSource[n+5]);
			x = MakeByte(szSource[n+7], szSource[n+8]);
			w = MakeByte(szSource[n+10], szSource[n+11]);
			v = MakeByte(szSource[n+13], szSource[n+14]);
			ch = (TCHAR)( (z-248)*16777216 + (y-128)*262144 + 
				(x-128)*4096 + (w-128)*64 + (v-128) );
			n = n + 14;
		}
		else if (z >= 252 && z <= 253)
		{
			// character is four bytes
			if (n >= nMax - 17) break; // something is wrong
			y = MakeByte(szSource[n+4], szSource[n+5]);
			x = MakeByte(szSource[n+7], szSource[n+8]);
			w = MakeByte(szSource[n+10], szSource[n+11]);
			v = MakeByte(szSource[n+13], szSource[n+14]);
			u = MakeByte(szSource[n+16], szSource[n+17]);
			ch = (TCHAR)( (z-252)*1073741824 + (y-128)*16777216 + 
				(x-128)*262144 + (w-128)*4096 + (v-128)*64 + (u-128) );
			n = n + 17;
		}
		pwszBuf[nIndex++] = ch;

	}

	return pwszBuf;
}

//To convert all unsafe characters to escape sequences.
CString CStrUtil::EncodeURL(const CString& strSource)
{
	CStringA strDestination(_T(""));
	//
	//int nCnt = AtlUnicodeToUTF8(strSource, dwLength, NULL, 0);//
	CStringA strA = CT2A(strSource,CP_UTF8);
	DWORD dwLength = strA.GetLength();
	int nCnt = strA.GetLength();
	strDestination.GetBufferSetLength(nCnt*3);
	
	if ( !AtlEscapeUrl(strA, strDestination.GetBuffer(), &dwLength, nCnt*3 + 1, ATL_URL_ENCODE_PERCENT) )
	{
		strDestination.ReleaseBuffer(0);
	}
	else
	{
		strDestination.ReleaseBuffer();
	}
	CString strTemp = CA2T(strDestination,CP_UTF8);
	return strTemp;	
}

//To convert format such as %uXXXX (UTF-8) encode url from MultiBytes words
CString CStrUtil::FilterMultiBytes(const CString& strSource)
{
	CString str = strSource;
	int nPos = -1;
	while( ( nPos = str.Find(_T("%u"))) >= 0)
	{
		CString strMid = str.Mid(nPos+2,4);
		CString strSub = _T("0x") + strMid;

		int n = 0;
		
		if (StrToIntEx(strSub,STIF_SUPPORT_HEX,&n))
		{
			WCHAR szBuf[3]={0};
			szBuf[0] = n & 0x0000FFFF; 
			szBuf[1] = n & 0xFFFF0000;
			strSub = szBuf;
			str.Replace(_T("%u")+strMid,strSub);
		}
	}
	return str;
}

//To convert escaped characters back to their original values.
CString CStrUtil::DecodeURL(const CString& strSour)
{
	CStringA strDestination;

//#ifdef _UNICODE
//	WCHAR* pBuf = DecodeFromUTF8(FilterMultiBytes(strSour));
//	strDestination = pBuf;
//	delete []pBuf;
//#else	
//	DWORD dwLength = strSour.GetLength();
//	//TCHAR *pBuff = new TCHAR[dwLength + 1];
//	strDestination.GetBufferSetLength(dwLength);
//	if (AtlUnescapeUrl(strSour, strDestination.GetBuffer(), &dwLength, dwLength + 1))
//	{
//		strDestination.ReleaseBuffer();
//	}		
//	else
//	{
//		strDestination.ReleaseBuffer(0);
//	}
//#endif		

	CString strTemp = FilterMultiBytes(strSour);
	CStringA strA = CT2A(strTemp,CP_UTF8);
	DWORD dwLength = strA.GetLength();
	//TCHAR *pBuff = new TCHAR[dwLength + 1];
	strDestination.GetBufferSetLength(dwLength);
	if (AtlUnescapeUrl(strA, strDestination.GetBuffer(), &dwLength, dwLength + 1))
	{
		strDestination.ReleaseBuffer();
	}		
	else
	{
		strDestination.ReleaseBuffer(0);
	}

	strTemp = CA2T(strDestination,CP_UTF8);
	return strTemp;	
}

//The following codes about XML encoding/decoding are imitated the implement codes in Direct
//which were base on STL and not supported UNICODE.

//To convert all unsafe characters to escape sequences.
CString	CStrUtil::EncodeXML(const CString& strSour)
{
	TCHAR * XML_ENCODING_TABLE[]= 
	{
		//	0,   1,   2,   3,    4,   5,   6,   7,    8,   9,   A,   B,    C,   D,   E,   F,
		0,   0,   0,   0,    0,   0,   0,   0,    0,   0,   0,   0,    0,   0,   0,   0,		// 0
		0,   0,   0,   0,    0,   0,   0,   0,    0,   0,   0,   0,    0,   0,   0,   0,		// 1
		0,   0,   _T("&quot;"),   0,    0,   0,   _T("&amp;"),   _T("&apos;"),    0,   0,   0,   0,    0,   0,   0,   0,	// 2
		0,   0,   0,   0,    0,   0,   0,   0,    0,   0,   0,   0,    _T("&lt;"),   0,   _T("&gt;"),   0,		// 3
	};

	CString strResult(_T(""));
	TCHAR tc;
	//wchar_t tc;
	TCHAR *str;
	int nLength = strSour.GetLength();
	for(int i=0; i<nLength; i++)
	{
		tc = strSour.GetAt(i);
		if( tc < 0x40 && (str=XML_ENCODING_TABLE[tc])!=NULL)	//Encode reserved characters according to encodging table
		{
			strResult += str;
		}
		else
		{
			strResult += tc;	//Normal characters
		}
	}
	return strResult;
}

char CStrUtil::WCToChar( TCHAR c)
{
	char cRet=0;
	char abuf[16];
	abuf[0]=0;
	abuf[1]=0;
#ifdef UNICODE
	TCHAR mbuf[16] = _T("1");
	mbuf[0] = c;
	#if _MSC_VER < 1400 // previous compiler before vs2005
		if(wcstombs(abuf,mbuf,1)==1)
			cRet = abuf[0];
	#else
		size_t nConvertedChars;
		if(wcstombs_s(&nConvertedChars, abuf, mbuf,1)==0)
			cRet = abuf[0];
	#endif
#else 
	cRet = c;
#endif	
	return cRet;
}

bool CStrUtil::AppendChar( CString &str, int asii)
{
	if(asii<=0)
		return false;    			
	char abuf[16];

	abuf[0]=(char)((BYTE)asii);
	abuf[1]=0;
	CString strTemp;
#ifdef UNICODE
	TCHAR ubuf[16]=_T("");

#if _MSC_VER < 1400 // previous compiler before vs2005
	if(mbstowcs(ubuf,abuf,1)==1)
		str +=(LPCTSTR)ubuf;	
	else 
		return false;
#else
	size_t nConvertedChars;
	if(mbstowcs_s(&nConvertedChars, ubuf, 16, abuf,1)==0)

		str +=(LPCTSTR)ubuf;	
	else 
		return false;
#endif

#else 
	str += abuf;
#endif	
	return true;
}
//To convert escaped characters back to their original values.
CString	CStrUtil::DecodeXML(const CString& strSour)
{
	CString strResult(_T(""));
	TCHAR tc;
	int nLength = strSour.GetLength();
	for( int i=0; i<nLength; i++)
	{
		tc = strSour.GetAt(i);
		if( tc == _T('&') )
		{
			int curPos = 0;
			if( ( curPos = strSour.Find(_T(';'), i)) != -1)
			{
				CString token = strSour.Mid( i+1 , curPos - i);
				if (token.Compare(_T("quot;"))==0)	//decode reserved characters
				{
					strResult += _T('\"');
					i = curPos;
				}
				else if (token.Compare(_T("amp;"))==0)
				{
					strResult += _T('&');
					i = curPos;
				}
				else if (token.Compare(_T("lt;"))==0)
				{
					strResult+=_T('<');
					i = curPos;
				}
				else if (token.Compare(_T("gt;"))==0)
				{
					strResult+=_T('>');
					i = curPos;
				}
				else if (token.Compare(_T("apos;"))==0)
				{
					strResult+=_T('\'');
					i = curPos;
				}
				else if(token.Find(_T("#x"))==0)
				{
					CString sTemp = token.Mid(2,token.GetLength()-3);
					if(sTemp.GetLength()>0)
					{
						int asii=0;
#if _MSC_VER < 1400 // previous compiler before vs2005
						_stscanf(sTemp.GetBuffer(), _T("%x"), &asii);
#else
						_stscanf_s(sTemp.GetBuffer(), _T("%x"), &asii);
#endif
						if(!AppendChar(strResult, asii))
							ATLASSERT(FALSE);
						i = curPos;
					}
				}
				else 
				{
					// Not implemented for other entities
					strResult += tc;
				}
				
			}	//end if find(';')
			else
			{
				strResult += strSour.Mid(i);	//None pattern (&...;) in strSour, break it.
				break;
			}
		}
		else	//end if '&'
		{
			strResult += tc;
		}
	}	//end for(...)
	return strResult;
}

CString CStrUtil::GetFilePath(const CString &strFilePath)
{
	TCHAR szDir[_MAX_DIR];
	TCHAR szDrive[_MAX_DRIVE];
#if _MSC_VER < 1400 // previous compiler before vs2005
	_tsplitpath(strFilePath, szDrive, szDir, NULL, NULL);
#else
	_tsplitpath_s(strFilePath, szDrive,_MAX_DRIVE, szDir,_MAX_DIR, NULL, 0, NULL,0);
#endif

	return  CString(szDrive) + CString(szDir);
}

CString CStrUtil::GetFileExt(const CString &strFilePath)
{
	TCHAR szExt[_MAX_EXT];
#if _MSC_VER < 1400 // previous compiler before vs2005
	_tsplitpath(strFilePath, NULL, NULL, NULL, szExt);
#else
	_tsplitpath_s(strFilePath, NULL, 0, NULL,0, NULL,0, szExt,_MAX_EXT);
#endif
	return CString(szExt);
}

CString CStrUtil::GetFileTitle(const CString &strFilePath)
{
	TCHAR szFname[_MAX_FNAME];
#if _MSC_VER < 1400 // previous compiler before vs2005
	_tsplitpath(strFilePath, NULL, NULL, szFname, NULL);
#else
	_tsplitpath_s(strFilePath, NULL, 0, NULL, 0, szFname, _MAX_FNAME, NULL,0);
#endif
	return  CString(szFname);
}

CString CStrUtil::GetFileDirAndName(const CString &strFilePath)
{
	TCHAR szDir[_MAX_DIR];
	TCHAR szFname[_MAX_FNAME];
	TCHAR szExt[_MAX_EXT];
#if _MSC_VER < 1400 // previous compiler before vs2005
	_tsplitpath(strFilePath, NULL , szDir, szFname, szExt);
#else
	_tsplitpath_s(strFilePath, NULL , 0, szDir, _MAX_DIR, szFname, _MAX_FNAME, szExt, _MAX_EXT);
#endif

	CString Dir = szDir;
	Dir.TrimLeft(_T("\\"));
	return  Dir + szFname + szExt;
}

CString CStrUtil::GetFileName(const CString &strFilePath)
{
	TCHAR szExt[_MAX_EXT];
	TCHAR szName[_MAX_FNAME];
#if _MSC_VER < 1400 // previous compiler before vs2005
	_tsplitpath(strFilePath, NULL, NULL, szName, szExt);
#else
	_tsplitpath_s(strFilePath, NULL, 0, NULL,0, szName, _MAX_FNAME, szExt,_MAX_EXT);
#endif
	return CString(szName) + CString(szExt);
}

CString CStrUtil::GetDrive(const CString &strFilePath)
{
	TCHAR szDrive[_MAX_DRIVE];
#if _MSC_VER < 1400 // previous compiler before vs2005
	_tsplitpath(strFilePath, szDrive, NULL, NULL, NULL);
#else
	_tsplitpath_s(strFilePath, szDrive, _MAX_DRIVE, NULL,0, NULL,0, NULL,0);
#endif
	return szDrive;
}

CString	CStrUtil::LoadMultiLanguageString(const UINT nIndex)
{
	//just a demo
	CString message;
	message.Format(_T("Error Code: %d.\n"), nIndex); //-V576
	return message;
}

CString	CStrUtil:: EncodeSpliter(CString Input)
{
	int len = Input.GetLength();
	CString Output;
	for (int i = 0; i < len; i++) 
	{
		TCHAR c = Input[i];
		switch (c) {
			case L'|':
				Output += _T("%4c");
				break;
			case L';':
				Output += _T("%3b");
				break;
			case L'%':
				Output += _T("%25");
				break;
			default:
				Output += c;
				break;
		}
	}
	return Output;
}
//To convert escaped characters back to their original values.
CString	CStrUtil::DecodeSpliter(const CString& strSour)
{
	CString strResult(_T(""));
	TCHAR tc;
	int nLength = strSour.GetLength();
	int curPos = 0;
	for( int i=0; i<nLength; i++)
	{
		tc = strSour.GetAt(i);
		if( tc == _T('%') )
		{
			CString token = strSour.Mid( i , 3);
			if (token.Compare(_T("%25"))==0)	//decode reserved characters
			{
				strResult += _T('%');
				i += 2;
			}
			else if (token.Compare(_T("%4c"))==0)	//decode reserved characters
			{
				strResult += _T('|');
				i += 2;
			}
			else if (token.Compare(_T("%3b"))==0)	//decode reserved characters
			{
				strResult += _T(';');
				i += 2;
			}
			else
				strResult += tc;
		}
		else
			strResult += tc;
	}
	return strResult;
}

int CStrUtil::TokenizeString(const CString & source,vector<CString> &vecStr,LPCTSTR delimiter)
{
	int at = 0;
	CString temp = source.Tokenize(delimiter,at);
	while(!temp.IsEmpty())
	{
		vecStr.push_back(temp);
		temp = source.Tokenize(delimiter,at);
	}
	return (int)vecStr.size();
}

CString CStrUtil::PickupDelimitedString(const CString& src, LPCTSTR delimiter,long nPos)
{
	CString temp;
	CString delimiters(delimiter);
	TCHAR ch;
	if (nPos <0)
		return _T("");
	int count = 0,at = 0,len = src.GetLength();
	while(at<len)
	{
		ch = src[at++];
		if(delimiters.Find(ch)>=0)
		{
			if (nPos == count)
				return temp;
			++count;
			temp.Empty();
		}
		else temp += ch;
	}
	if(!temp.IsEmpty()&&(nPos == count))
	{
		return temp;
	}
	return _T("");
}

int CStrUtil::SplitStringEx(const CString& src, vector<CString> &vecStr,const CString & delimiter,bool bRemovEmpty)
{
	int delimitlen = delimiter.GetLength();
	int nPos = src.Find(delimiter);
	if (nPos <0)
	{
		vecStr.push_back(src);
		return 1;
	}
	int nStart = 0;
	CString strPartStr;
	while(nPos >= 0)
	{
		strPartStr = src.Mid(nStart,nPos - nStart);
		if (!(bRemovEmpty&&strPartStr.IsEmpty()))
		{
			vecStr.push_back(strPartStr);
		}
		
		nStart = nPos + delimitlen;
		nPos = src.Find(delimiter,nStart);
	}
	if (nStart <= src.GetLength())
	{
		strPartStr = src.Mid(nStart);
		if (!(bRemovEmpty&&strPartStr.IsEmpty()))
		{
			vecStr.push_back(strPartStr);
		}
	}
	return vecStr.size();
}

int CStrUtil::SplitString(const CString& src, vector<CString> &vecStr,LPCTSTR delimiter,bool bRemovEmpty)
{
	CString temp;
	CString delimiters(delimiter);
	TCHAR ch;
	int count = 0,at = 0,len = src.GetLength();
	while(at<len)
	{
		ch = src[at++];
		if(delimiters.Find(ch)>=0)
		{
			if (!bRemovEmpty)
			{
				vecStr.push_back(temp);
				++count;
			}
			else if(!temp.IsEmpty())
			{
				vecStr.push_back(temp);
				++count;
			}
			temp.Empty();
		}
		else temp += ch;
	}
	if(!temp.IsEmpty())
	{
		vecStr.push_back(temp);
		++count;
	}
	return count;
}

void CStrUtil::JoinString(const vector<CString> &vecStr,CString &result,LPCTSTR delimiter)
{
	result.Empty();
	for(int i = 0;i<(int)vecStr.size();++i)
	{
		if(!result.IsEmpty()) result.Append(delimiter);
		result.Append(vecStr[i]);
	}
}

#ifdef _AFXDLL
int CStrUtil::SplitString(const CString & source,CStringArray &aryStr,LPCTSTR delimiter,bool bRemovEmpty)
{
	vector<CString> vecStr;
	int nCount = SplitString(source,vecStr, delimiter,bRemovEmpty);
	for(int i=0; i<nCount; i++)
		aryStr.Add(vecStr[i]);
	return nCount;

	//int at = 0;
	//CString temp = source.Tokenize(delimiter,at);
	//while(!temp.IsEmpty())
	//{
	//	aryStr.Add(temp);
	//	temp = source.Tokenize(delimiter,at);
	//}
	//return (int)aryStr.GetCount();
}
#endif

bool CStrUtil::IsValidNumericString(LPCTSTR source)
{
	if ( source == NULL || lstrlen(source) == 0 ) return false;

	//// check if it's numeric #
	CAtlRegExp<> reNumeric;
	//// five match groups: scheme, authority, path, query, fragment
	REParseError status = reNumeric.Parse(_T("^[-+]?[0-9]*[.]?[0-9]+([eE][-+]?[0-9]+)?$"));
	if (REPARSE_ERROR_OK != status)
	{
		//	// Unexpected error.
		//	CAWDException* e = new CAWDException(CAWDException::EXCEPTION_STR2LONG, __AWDFILE__, __AWDLINE__);
		//	throw e;
		ATLASSERT(FALSE);
		ATLTRACE(_T("failed to parse regex\n"));
		return false;
	}

	CAtlREMatchContext<> mcNumeric;
	if (!reNumeric.Match(source,&mcNumeric))
	{
		//	CAWDException* e = new CAWDException(CAWDException::EXCEPTION_STR2LONG, __AWDFILE__, __AWDLINE__);
		//	throw e;
		return false;
	}
	return true;
}

bool CStrUtil::IsValidFilePath(LPCTSTR source)
{
	if ( source == NULL || lstrlen(source) == 0 ) return false;

	//// check if it's numeric #
	CAtlRegExp<> reFilePath;
	//// five match groups: scheme, authority, path, query, fragment
	REParseError status = reFilePath.Parse(_T("^[a-zA-Z]:"));  //\\(?:[^\\/:*?\"<>|\r\n]+\\)*[^\\/:*?\"<>|\r\n]*$
	if (REPARSE_ERROR_OK != status)
	{
		//	// Unexpected error.
		//	CAWDException* e = new CAWDException(CAWDException::EXCEPTION_STR2LONG, __AWDFILE__, __AWDLINE__);
		//	throw e;
		ATLASSERT(FALSE);
		ATLTRACE(_T("failed to parse regex\n"));
		return false;
	}

	CAtlREMatchContext<> mcNumeric;
	if (!reFilePath.Match(source,&mcNumeric))
	{
		//	CAWDException* e = new CAWDException(CAWDException::EXCEPTION_STR2LONG, __AWDFILE__, __AWDLINE__);
		//	throw e;
		return false;
	}
	return true;
}

bool CStrUtil::EncodeBase64(const BYTE *buf,int buflen,CString &outString)
{
	CStringA strA;
	int outlen = buflen*2;
	if(Base64Encode(buf,buflen,strA.GetBuffer(outlen),&outlen, ATL_BASE64_FLAG_NONE))
	{
		strA.ReleaseBuffer(outlen);
		outString = CA2T(strA);
		return true;
	}
	return false;
}

bool CStrUtil::DecodeBase64(LPCTSTR source,BYTE *buf,int &buflen)
{
	CStringA strA;
	strA = CT2A(source);
	return Base64Decode(strA,strA.GetLength(),buf,&buflen) == TRUE;	//just for warning C4800.
}


COLORREF CStrUtil::StringToColor(LPCTSTR s) 
{
	DWORD hex=0;
	int nRet = 0;
#if _MSC_VER < 1400 // previous compiler before vs2005
	nRet = _stscanf(s,_T("%x"), &hex);
#else
	nRet = _stscanf_s(s,_T("%x"), &hex);
#endif
	if(nRet==1)
	{
		int r = (hex & 0xff0000) >> 16;
		int g = (hex & 0x00ff00) >> 8;
		int b = (hex & 0x0000ff);
		return RGB(r, g, b);
	}
	else
	{
		return RGB(0,0,0);
	}
}

CString CStrUtil::HexEncode(LPCTSTR src)
{
	CStringA strA = CT2AEX<>(src,CP_UTF8);
	if(strA.IsEmpty()) return _T("");
	CStringA strOut;
	int outLen;
	outLen = AtlHexEncodeGetRequiredLength(strA.GetLength());
	if(AtlHexEncode((const BYTE *)strA.GetBuffer(),strA.GetLength(),(LPSTR)strOut.GetBuffer(outLen),&outLen))
	{
		strOut.ReleaseBuffer(outLen);
		return (LPCTSTR)CA2TEX<>((LPCSTR)strOut,CP_UTF8);
	}
	else
	{
		return _T("");
	}
}

CString CStrUtil::HexDecode(LPCTSTR src)
{
	CStringA strA = CT2AEX<>(src,CP_UTF8);
	if(strA.IsEmpty()) return _T("");
	CStringA strOut;
	int outLen;
	outLen = AtlHexDecodeGetRequiredLength(strA.GetLength());
	if(AtlHexDecode((LPCSTR)strA,strA.GetLength(),(BYTE *)strOut.GetBuffer(outLen),&outLen))
	{
		strOut.ReleaseBuffer(outLen);
		return (LPCTSTR)CA2TEX<>((LPCSTR)strOut,CP_UTF8);
	}
	else
	{
		return _T("");
	}
}

CString CStrUtil::GetGUID()
{
	GUID guid;
	::CoCreateGuid(&guid);
	ATLASSERT(guid!=GUID_NULL);
	CString cstr;
	cstr.Empty();
	cstr.Format(_T("%08X"),guid.Data1);
	cstr.AppendFormat(_T("%04X"),guid.Data2);
	cstr.AppendFormat(_T("%04X"),guid.Data3);
	for(int i=0;i<8;i++)
	{
		cstr.AppendFormat(_T("%02X"),guid.Data4[i]);
	}
	return cstr;
}

//CString CStrUtil::GetReadWritePermissionText(int nPermission)
//{
//	bool bRead = ((iView & nPermission) == iView) && ((iList & nPermission) == iList);
//	bool bWrite = ((iEdit & nPermission) == iEdit) && ((iDelete & nPermission) == iDelete);
//	if ( bRead && bWrite)
//	{
//		return _T("Read/Write");
//	}
//	else if (bRead)
//	{
//		return _T("Read Only");
//	}
//	else
//	{
//		return _T("");
//	}
//}

//check a secId, if it is a custom fund return true. Note that AG/CF are all custom fund
bool CStrUtil::IsCustomFund(const CString &strSecId)
{
	CString str = CStrUtil::GetUniverse(strSecId);
	return  IsUserFundHandleUniv(str);
}


bool CStrUtil::IsUserFundHandleUniv(const CString &strUniv)
{
	static set<CString>  setCustomFund;

	if (setCustomFund.size() == 0)
	{
		setCustomFund.insert(_T("AG"));
		setCustomFund.insert(_T("CF"));
		setCustomFund.insert(_T("SG"));
		setCustomFund.insert(_T("BM"));
		setCustomFund.insert(_T("MD"));
		setCustomFund.insert(_T("CN"));
		setCustomFund.insert(_T("MM"));
		setCustomFund.insert(_T("UA"));
		setCustomFund.insert(_T("SY"));
		setCustomFund.insert(_T("AO"));
		setCustomFund.insert(_T("AC"));
	}

	return setCustomFund.find(strUniv)  != setCustomFund.end() ? true : false;
	
}

//check a seccurityId, if it is a CF/AG and account.
bool CStrUtil::IsUserPortfolioUniv(const CString &strUniv)
{
	return (strUniv.CompareNoCase(_T("AG"))==0 ||strUniv.CompareNoCase(_T("CF"))==0
			 || strUniv.CompareNoCase(_T("SG"))==0 ||strUniv.CompareNoCase(_T("BM"))==0
			|| strUniv.CompareNoCase(_T("MD"))==0 ||strUniv.CompareNoCase(_T("CN"))==0	
			||strUniv.CompareNoCase(_T("MM"))==0 || strUniv.CompareNoCase(_T("AC"))==0
			||strUniv.CompareNoCase(_T("ACCOUNT"))==0|| strUniv.CompareNoCase(_T("CUSTBM"))==0
			|| strUniv.CompareNoCase(_T("UA"))==0||strUniv.CompareNoCase(_T("SY"))==0
			||strUniv.CompareNoCase(_T("AO"))==0/*||strUniv.CompareNoCase(_T("AC"))==0*/);
}

//check a seccurityId, if it is a CF/AG and account.
bool CStrUtil::IsUserPortfolio(const CString &strSecId)
{
	CString str = CStrUtil::GetUniverse(strSecId);

	return IsUserPortfolioUniv(str);
}

//check a secId, if it is an aggregate return true
bool CStrUtil::IsAggregate(const CString &strSecId)
{
	CString str = CStrUtil::GetUniverse(strSecId);
	return (str.CompareNoCase(_T("AG")) == 0 ) ? true : false;
}

bool CStrUtil::IsCustomFundUniv(const CString & strUniverse)
{
	return (strUniverse.CompareNoCase(_T("UA")) == 0 || 
				 strUniverse.CompareNoCase(_T("AG")) == 0 ||
				 strUniverse.CompareNoCase(_T("CF")) == 0) ? true : false;
}

bool CStrUtil::IsCFUniv(const CString & strUniverse)
{
	return (strUniverse.CompareNoCase(_T("UA")) == 0 || 
				 strUniverse.CompareNoCase(_T("CF")) == 0) ? true : false;
}


CString CStrUtil::FormatSecId(const CString &strSecId, const CString &strUniverse)
{
	CString str;
	str.Format(_T("%s;%s"), strSecId, strUniverse);
	return str;
}

//check a secId, if it is default cash, return true
bool CStrUtil::IsCash(const CString &strSecId)
{
	return IsCashUniverse(CStrUtil::GetUniverse(strSecId));
}

//check a universe string, if it is a cash universe
bool CStrUtil::IsCashUniverse(const CString &strUniverse)
{
	const static TCHAR *_MapCashUniverse[] = 
	{
		{_T("CB")},
		{_T("CC")},
		{_T("CD")},
		{_T("CH")},
		{_T("CL")},
		{_T("CO")},
		{_T("CP")},
		{_T("CQ")},
		{_T("CR")},
		{_T("CS")},
		{_T("CT")},
		{_T("CU")},
		{_T("CV")},
		{_T("CX")}
	};
	if(strUniverse.IsEmpty())
		return false;

	int count =  sizeof(_MapCashUniverse)/sizeof(TCHAR *);
	for(int i=0; i< count; i++)
	{
		if(strUniverse==_MapCashUniverse[i])
			return true;
	}
	return false;
}

bool CStrUtil::IsUserDefineSecurity(const CString &strSecId)
{
	if (GetBaldSecId(strSecId).GetLength() >= 36 && !IsCustomFund(strSecId))
		return true;
	return false;
}

//this method is not very correct, but we can use it in some case.
bool CStrUtil::IsMorningstarSecurity(const CString &strSecId)
{
	if (GetBaldSecId(strSecId).GetLength() < 36)
		return true;
	return false;
}

//get the secid not witch the universe from a secid
CString CStrUtil::GetBaldSecId(const CString &strSecId)
{
	int idx;
	if((idx=strSecId.Find(_T(";")))>0)
	{
		CString str = strSecId.Mid(0, idx);		
		return str;
	}
	return strSecId;
}
//get the universe from a secid
CString CStrUtil::GetUniverse(const CString &strSecId)
{
	CString str;
	int idx;
	if((idx=strSecId.Find(_T(";")))>0)
	{
		str = strSecId.Mid(idx +1);
		if((idx=str.Find(_T(";")))>0)
		{
			str = str.Mid(0, idx);	
		}
	}
	return str;
}

//Compare two secid if is same security then return ture
bool CStrUtil::IsSameSecId(const CString &strSecId1, const CString &strSecId2)
{
	int nLen1,nLen2;	
	if((nLen1=strSecId1.Find(_T(";")))<0)
	{
		nLen1 = strSecId1.GetLength();	
	}
	if((nLen2=strSecId2.Find(_T(";")))<0)
	{
		nLen2 = strSecId2.GetLength();	
	}
	if(nLen1!=nLen2)
		return false;

	for(int i=0; i<nLen1;i++)
		if(strSecId1[i]!=strSecId2[i])
			return false;
	return true;
}

//check a universe string, if it is a fund universe
bool CStrUtil::IsFundUniverse(const CString &strUniverse)
{
	const static TCHAR *_MapFundUniverse[] = 
	{
		{_T("FO")},
		{_T("XI")},
		{_T("V1")},
		{_T("SA")},
		{_T("ETF")}
	};
	int count =  sizeof(_MapFundUniverse)/sizeof(TCHAR *);
	for(int i=0; i< count; i++)
	{
		if(strUniverse==_MapFundUniverse[i])
			return true;
	}
	return false;
}

bool CStrUtil::ExitStr(const TCHAR *_strArray[], int nCount, const CString &strKey)
{
	for(int i= 0; i<nCount; i++)
	{		
		if(strKey.Compare(_strArray[i])==0)
			return true;
	}
	return false;
}

CString CStrUtil::AddUrlParam(CString &UrlStr,CString ParamName,CString ParamValue)
{
	CString strTmp;
	strTmp.Format(_T("%s="),ParamName);
	if(UrlStr.Find(strTmp) == -1) 
	{
		if(UrlStr.Find(_T("http://"))>=0 ||UrlStr.Find(_T("https://"))>=0)
		{
			//LONG nLastPos = UrlStr.ReverseFind(_T('/'));
			//CString strLast = UrlStr.Mid(nLastPos+1);
			LONG n = UrlStr.Find(_T("?"));
			if (n>=0)
				UrlStr.AppendFormat(_T("&%s=%s"),ParamName,ParamValue);
			else
				UrlStr.AppendFormat(_T("?%s=%s"),ParamName,ParamValue);
		}
		else 
			UrlStr.AppendFormat(_T("&%s=%s"),ParamName,ParamValue);
	}
	return UrlStr;
}

CString CStrUtil::AddUrlParam(CString &UrlStr,CString ParamStr)
{
	CString strTmp;
	if(UrlStr.Find(ParamStr) == -1) 
	{
		if(UrlStr.Find(_T("http://"))>=0 ||UrlStr.Find(_T("https://"))>=0)
		{
			//LONG nLastPos = UrlStr.ReverseFind(_T('/'));
			//CString strLast = UrlStr.Mid(nLastPos+1);
			LONG n = UrlStr.Find(_T("?"));
			if (n>=0)
				UrlStr.AppendFormat(_T("&%s"),ParamStr);
			else
				UrlStr.AppendFormat(_T("?%s"),ParamStr);
		}
		else 
			UrlStr.AppendFormat(_T("&%s"),ParamStr);
	}
	return UrlStr;
}

void CStrUtil::CheckDirectory(LPCTSTR strDirectory)
{
	CString strDir = strDirectory;
	//remove ending / if exists
	if(strDir.Right(1)=="\\")
		strDir=strDir.Left(strDir.GetLength()-1);
	// base case . . .if directory exists
	if(GetFileAttributes(strDir) != -1)
		return;
	// recursive call, one less directory
	int nFound = strDir.ReverseFind('\\');
	if (nFound == -1)
		return;
	CheckDirectory(strDir.Left(nFound));
	// actual work
	CreateDirectory(strDir,NULL);
}

CString CStrUtil::GetTempFile(LPCTSTR kind,LPCTSTR specDir)
{
	CString tempDir;
	tempDir = specDir;
	//check the temporary dir exists
	if(!::PathFileExists(tempDir))
	{
		TCHAR tempPath[MAX_PATH+1];
		GetTempPath(MAX_PATH,tempPath);
		tempDir = tempPath;
	}
	TCHAR tempFile[MAX_PATH+1];
	GetTempFileName(tempDir,kind,0, tempFile);
	return tempFile;
}

#ifdef _AFX

bool CStrUtil::LoadString(UINT nStringRID, CString& strGetString, LPCTSTR szFailReturn/*=NULL*/)
{
	bool bret = false;

	enum{ emDefaultSize = 256 };
	CString strString;
	int nCopied = AfxLoadString(nStringRID, strString.GetBuffer(emDefaultSize), emDefaultSize);

	if( nCopied > 0 )
	{
		int nStrLen = emDefaultSize;
		while( nStrLen-1 == nCopied )
		{
			// change buffer size
			if( nStrLen < 4096 )
			{
				nStrLen *= 2;
			}
			else
			{
				nStrLen += 1024;
			}
			
			nCopied = AfxLoadString(nStringRID, strString.GetBuffer(nStrLen), nStrLen);
		}// end of while
	}

	if( nCopied > 0 )
	{
		strString.ReleaseBuffer(nCopied);
		strGetString = strString;
		bret = true;
	}
	else if( NULL != szFailReturn )
	{
		strGetString = szFailReturn;
	}
	else
	{
		strGetString.Empty();
	}

	return bret;
}

#endif

HRESULT CStrUtil::ToString(VARIANT vIn, CString& sOut)
{
	if(vIn.vt == VT_BSTR)
	{
		sOut = CW2TEX<>(vIn.bstrVal);
		return S_OK;
	}
	else if(vIn.vt == (VT_ARRAY | VT_UI1))
	{
		SAFEARRAY* pArray = vIn.parray;
		if(pArray)
		{
			int nSize = pArray->rgsabound[0].cElements;
			if ( nSize <= 0 )
				return E_FAIL;

			CHAR* lpa = new CHAR[nSize + 1];
			::memcpy(lpa, (void*)((CHAR*)pArray->pvData), nSize);
			lpa[nSize] = 0;
			sOut = CA2TEX<>(lpa,CP_UTF8);
			delete lpa;
			return S_OK;
		}
	}

	return E_FAIL;
}
 
CString 
CStrUtil::FormatEscapedChars(const CString& strSour)
{
	TCHAR * FORMAT_TABLE[]= 
	{
		//	0,   1,   2,   3,    4,   5,   6,   7,    8,   9,   A,   B,    C,   D,   E,   F,
		0,   0,   0,   0,    0,   0,   0,   0,    0,   0,   0,   0,    0,   0,   0,   0,		// 0
		0,   0,   0,   0,    0,   0,   0,   0,    0,   0,   0,   0,    0,   0,   0,   0,		// 1
		0,   0,   0,   0,    0,   0,   _T("&&"),  0,    0,   0,   0,   0,    0,   0,   0,   0,	// 2
		0,   0,   0,   0,    0,   0,   0,   0,    0,   0,   0,   0,    0,   0,   0,   0,		// 3
	};

	CString strResult(_T(""));
	TCHAR tc;
	//wchar_t tc;
	TCHAR *str;
	int nLength = strSour.GetLength();
	for(int i=0; i<nLength; i++)
	{
		tc = strSour.GetAt(i);
		if( tc < 0x40 && (str=FORMAT_TABLE[tc])!=NULL)	//Encode reserved characters according to encodging table
		{
			strResult += str;
		}
		else
		{
			strResult += tc;	//Normal characters
		}
	}
	return strResult;
}

/****************************************************************************
*
* METHOD    : CheckName
*
* PARAMETER : [LPCTSTR &] lpstrName - original name
			  [int] nMaxLen - maximum length of name, default value -1 means it is not limited	
*             
* RETURN    : [CString] the valid excel file name
* PURPOSE   : Some characters is not supported as the excel file name
* REMARKS   : 
*
***************************************************************************/
CString 
CStrUtil::CheckName( LPCTSTR lpstrName, int nMaxLen/* = -1*/ )
{
	CString sName(lpstrName);

	if (!sName.IsEmpty())
	{
		TCHAR arrayInvalidChar[]	= {_T('\\'), _T('/'), _T('?'), _T('*'), _T('['), _T(']'), _T('<'), _T('>'), _T(':'), _T('|'), _T("\"")};
		int		  nLen					= sizeof(arrayInvalidChar) / sizeof(TCHAR);

		if ( nMaxLen > 0 && sName.GetLength() > nMaxLen )
		{
			sName = sName.Left(31);
		}

		for (int i = 0; i < nLen; ++i)
		{
			sName.Replace(arrayInvalidChar[i], _T(' '));
		}
	}

	return sName;
}

//get ole date support yyyy-mm-dd and mm/dd/yyyy
//also support yy-mm-dd and mm/dd/yy
COleDateTime CStrUtil::GetOleDateWithSep(CString& strDate)
{
	COleDateTime oledate;
	int   nYear(0),   nMonth(0),   nDay(-1);
	strDate.Trim();
	int pos = strDate.Find(_T("-"));
	if (pos != -1)
	{
		if (pos == 4)
			swscanf_s(strDate,_T("%d-%d-%d"),   
			&nYear,   &nMonth,   &nDay);
		else
			swscanf_s(strDate,_T("%d-%d-%d"),   
				&nMonth,   &nDay,   &nYear);
	}
	else
	{
		swscanf_s(strDate,_T("%d/%d/%d"),   
			&nMonth,   &nDay,   &nYear);
	}

	if ( nYear>0 && nYear<51 )
		nYear += 2000;
	else if ( nYear>50 && nYear<100 )
		nYear += 1900;

	if( nYear<1900 || nYear>2050
		|| nMonth<=0 || nMonth > 12
		|| nDay<=0 || nDay>31)
		return oledate;
	oledate.SetDate(nYear,nMonth,nDay);

	return oledate;

}
CString CStrUtil::GUIDToStr(const GUID& guidIn)
{
	const CString GUID_INVALID = _T("CCCCCCCC-CCCC-CCCC-CCCC-CCCCCCCCCCCC");

	if(GUID_NULL == guidIn)
		return _T("");

	CString retStr;
	TCHAR tcTemp[10];

	_ltot_s(guidIn.Data1, tcTemp, 16);
	retStr.Append(AddZero(tcTemp, 8));
	retStr.Append(_T("-"));

	_ltot_s(guidIn.Data2, tcTemp, 16);
	retStr.Append(AddZero(tcTemp, 4));
	retStr.Append(_T("-"));

	_ltot_s(guidIn.Data3, tcTemp, 16);
	retStr.Append(AddZero(tcTemp, 4));
	retStr.Append(_T("-"));

	for(int i=0; i<8; ++i)
	{
		_ltot_s(guidIn.Data4[i], tcTemp, 16);
		retStr.Append(AddZero(tcTemp, 2));
		if(1 == i)
			retStr.Append(_T("-"));
	}

	retStr.MakeUpper();

	if(GUID_INVALID == retStr)
		return _T("");

	return retStr;
}
CString CStrUtil::AddZero(TCHAR tcIn[], const int n)
{
	CString strIn = tcIn;
	int nlen;
	if((nlen=strIn.GetLength()) >= n)
		return strIn;

	do 
	{
		strIn = _T("0") + strIn;
		++nlen;
	} while(nlen<n);

	return strIn;
}
CString CStrUtil::GetStandardDateFormat(const DBTIMESTAMP& dbDate)
{
	COleDateTime oleDate(dbDate.year, dbDate.month, dbDate.day, 
						dbDate.hour, dbDate.minute, dbDate.second);
	return GetStandardDateFormat(oleDate);
}
CString CStrUtil::GetStandardDateFormat( const COleDateTime& oleDate )
{
	//_ASSERT(COleDateTime::valid == oleDate.GetStatus());
	if (COleDateTime::valid == oleDate.GetStatus() && oleDate.m_dt != 0)
	{
		return oleDate.Format(_T("%m/%d/%Y"));
	}
	else
	{
		return _T("01/01/1900");
	}
}