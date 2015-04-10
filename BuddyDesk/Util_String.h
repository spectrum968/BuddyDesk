/********************************************************************
*	File Name	: Util_String.h
*	Version		: 
*	Purpose		: Some public functions about string operation.
*	Create Date	: 10/8/2005 3:22:05 PM
*	Author		: Van Zhao
*	Copyright (C) MorningStar Corporation.  All Rights Reserved. 
********************************************************************/
#pragma once
#include <vector>
#include <atlenc.h>

using namespace std;
namespace Buddy
{
	namespace Util
	{

	////permission control value
	//const int iView = 1;
	//const int iList = 2;
	//const int iCreate = 4;
	//const int iEdit = 8;
	//const int iDelete = 16;
	//const int iShare = 32;
	//const int iViewDetail = 64;
	//const int iListDetail = 128;
	//const int iCreateDetail = 256;
	//const int iEditDetail = 512;
	//const int iDeleteDetail = 1024;
	//const int iShareDetail = 2048;
	//const int iExport = 4096;
	//const int iExportDetail = 8192;

	//const CString cstReadOnlyText =  _T("Read Only");
	//const CString cstReadWriteText =  _T("Read/Write");

//Description	: CStrUtil is a utilizable class wrapped frequent operations on CString. Its methods are static and used with classname::funcionname.
	class CStrUtil
		{
		public:
			//Data type converter

			//Description	:Converts a given string to an integer.
			//		  str	:String to be converted.
			//Return value	:The string's integer value.
			static	int		ToInt(const CString &str);

			//Description	:Converts a given string to an long integer.
			//		  str	:String to be converted.
			//Return value	:The string's long integer value.
			static	long	ToLongEx(const CString &str);

			static	long	ToLong2(const CString &str,long iDef);
			static	long	ToLong(const CString &str);
			static	long	HexToLongEx(const CString &str);

			//Description	:Converts a given string to an long long integer.
			//		  str	:String to be converted.
			//Return value	:The string's long long integer value.
			static	long long	ToLongLongEx(const CString &str);
			static	long long	ToLongLong(const CString &str, long long llDef = 0);

			//Description	:Converts a given string to an double .
			//		  str	:String to be converted.
			//Return value	:The string's double value.
			static	double	ToDoubleEx(const CString &str);

			static	double	ToDouble2(const CString &str, double dDef);
			static	double	ToDouble(const CString &str);

			//Description	:Converts a given integer to a string.
			//		value	:Integer to be converted.
			//		base	:Converting base, default is 10.
			//Return value	:CString in 10 base..
			static	CString FormatInt(const int value, const int base = 10);

			//Description	:Converts a given double to a string. 
			//		value	:Integer to be converted.
			//		num	:Converting decimal digits, default is 5. the value must less 128
			//Return value	:CString 
			static	CString FormatDouble(double value, int num = 5);

			//Description	:Converts a given variant to a string. to the double it is diffrent form VariantChangeType,it will not effect the system Regional and Language Options setting,
			//               but effect form the setlocale.
			//		value	:Variant to be converted.
			//Return value	:double
			static	double VariantToDouble(_variant_t value);
			//Description	:Converts a given variant to a string. to the double it is diffrent form VariantChangeType,it will not effect the system Regional and Language Options setting,
			//               but effect form the setlocale.
			//		value	:Variant to be converted.
			//Return value	:CString in 10 base..
			static	CString FormatVariant(_variant_t value);

			//Description	:Converts a given string to an bool.
			//		  str	:String to be converted.
			//Return value	:true if str is "1", "true", "t", "yes", "y"  , otherwise is false.
			static	bool	ToBool(const CString &str);
			
			static	bool	ToBoolDef(const CString &str,bool bDefault = false);
			//Decode/Encode funtions

			//Description	:To convert all unsafe characters to escape sequences.
			//strSour		:The URL to be converted.
			//Return value	:A CString object which stored the converted URL.
			static	CString EncodeURL(const CString& strSour);

			//Description	:To convert escaped characters back to their original values.
			//strSour		:The URL to be converted.
			//Return value	:A CString object which stored the converted URL.
			static	CString	DecodeURL(const CString& strSour);
			
			//Description	:To convert all unsafe characters to escape sequences.
			//strSour		:The XML string to be converted.
			//Return value	:A CString object which stored the encoded XML.
			static	CString	EncodeXML(const CString& strSour);

			//Description	:To convert escaped characters back to their original values.
			//strSour		:The XML string to be converted.
			//Return value	:A CString object which stored the decoded XML.
			static	CString	DecodeXML(const CString& strSour);
			
			//Description	:To test a string whether start with another string.
			//strSour		:The source string to be compare.
			//strSub		:The other string used for comparison
			//Return value	:This function returns true if the source string is begin with substring..
			static	bool	IsStartWith(const CString &strSour, const CString &strSub);

			//Description	:To calculate a hash value for the given string.
			//str			:The string to be calculated as key.
			//Return value	:Returns a hash value, calculated using the string's contents.
			static	ULONG	GetHashCode(const CString &str);

			//Description	:Load a string from (...) by nIndex, (...) maybe a file or buffer stored multi-language string table
			//nIndex		:string id
			//Return value	:Error message
			static	CString	LoadMultiLanguageString(const UINT nIndex);

			////////////////////////////////////////////
			//Filename and Directory helper functions  

			//Description	:To get the title of the file
			//strFilePath	:Full file name
			static	CString GetFileTitle(const CString &strFilePath);

			//Description	:return the file extension
			//strFilePath	:Full file name
			static	CString GetFileExt(const CString &strFilePath);

			//Description	:return the name of the file (title + extension)
			//strFilePath	:Full file name
			static	CString GetFileName(const CString &strFilePath);

			//Description	:To get the path of the given file
			//strFilePath	:Full file name
			static	CString GetFilePath(const CString &strFilePath);

			//Description	:To get the directory and the file name from the file path
			//strFilePath	:Full file name
			static	CString GetFileDirAndName(const CString &strFilePath);

			//Description	:Return the (drive:) part from the path
			//strFilePath	:Full file name
			static	CString GetDrive(const CString &strFilePath);

			static int TokenizeString(const CString & source,vector<CString> &vecStr,LPCTSTR delimiter = _T(";| ,\t"));
			static CString PickupDelimitedString(const CString& src, LPCTSTR delimiter = _T(";| ,\t"),long nPos = 0);
			static int SplitString(const CString & src,vector<CString> &vecStr,LPCTSTR delimiter = _T(";| ,\t"),bool bRemovEmpty = false);
			static int SplitStringEx(const CString& src, vector<CString> &vecStr,const CString & delimiter,bool bRemovEmpty  = false);
			static void JoinString(const vector<CString> &vecStr,CString &result,LPCTSTR delimiter = _T("|"));
#ifdef _AFXDLL
			static int SplitString(const CString & source,CStringArray &vecStr,LPCTSTR delimiter = _T(";| ,\t"),bool bRemovEmpty = false);
#endif
			static bool IsValidNumericString(LPCTSTR source);
			static bool IsValidFilePath(LPCTSTR source);
			static CString HexEncode(LPCTSTR src);
			static CString HexDecode(LPCTSTR src);

			static bool EncodeBase64(const BYTE *buf,int buflen,CString &outString);
			static bool DecodeBase64(LPCTSTR source,BYTE *buf,int &buflen);
			static bool WriteBase64(const CString &strFileName, CString &str);

			static COLORREF StringToColor(LPCTSTR s);

			static CString GetGUID();
			//convert value to read-only or read write text
			static CString GetReadWritePermissionText(int nPermission);
			static bool AppendChar( CString &str, int asii);
			static char WCToChar( TCHAR a);
			
			//format a full secid with a secid and a universe.
			static CString FormatSecId(const CString &strSecId, const CString &strUniverse);
			//check a secId, if it is a custom fund return true, note CF/AG are all custom fund
			static bool IsCustomFund(const CString &strSecId);
			static bool IsUserFundHandleUniv(const CString &strUniv);
			static bool IsUserPortfolioUniv(const CString &strUniv);
			//check a seccurityId, if it is a CF/AG and account.
			static bool IsUserPortfolio(const CString &strSecId);
			//check a secId, if it is an aggregate return true
			static bool IsAggregate(const CString &strSecId);
            static bool IsAggregateUniverse(const CString & strUniverse);
            static bool IsCustomFundUniv(const CString & strUniverse);
            static bool IsCFUniv(const CString & strUniverse);
			//check a secId, if it is default cash, return true
			static bool IsCash(const CString &strSecId);
			//check a universe string, if it is a cash universe
			static bool IsCashUniverse(const CString &strUniverse);
			//get the secid not witch the universe from a secid
			static CString GetBaldSecId(const CString &strSecId);
			//get the universe from a secid
			static CString GetUniverse(const CString &strSecId);
			//Compare two secid if is same security then return ture
			static bool IsSameSecId(const CString &strSecId1, const CString &strSecId2);
			//search a string array, if exit strKey than return true.

			//check a universe string, if it is a fund universe
			static bool IsFundUniverse(const CString &strUniverse);
			static bool IsUserDefineSecurity(const CString &strSecId);
			static bool IsMorningstarSecurity(const CString &strSecId);

			static bool ExitStr(const TCHAR *_strArray[], int nCount, const CString &strKey);
			static CString AddUrlParam(CString &UrlStr,CString ParamName,CString ParamValue);
			static CString AddUrlParam(CString &UrlStr,CString ParamStr);
			static void CheckDirectory(LPCTSTR strDirectory);
			static CString GetTempFile(LPCTSTR kind = _T("tmp"),LPCTSTR specDir = NULL);
#ifdef _AFX
			static bool LoadString(UINT nStringRID, CString& strGetString, LPCTSTR szFailReturn=NULL);
#endif
			static HRESULT ToString(VARIANT vIn, CString& sOut);
			static CString	EncodeSpliter(CString Input);
			static CString	DecodeSpliter(const CString& strSour);
			//Description	: Format the escaped characters to depend it from escaping.
			//strSour		:The string to be formatted.
			//Return value	:A CString object which stored the formatted string.
			static CString FormatEscapedChars(const CString& strSour);
			static CString CheckName( LPCTSTR lpstrName, int nMaxLen = -1 );
			static COleDateTime GetOleDateWithSep(CString& strDate);
			static CString GUIDToStr(const GUID& guidIn);
			static CString GetStandardDateFormat(const DBTIMESTAMP& dbDate);
			static CString GetStandardDateFormat(const COleDateTime& oleDate);
		private:	
			static WCHAR* DecodeFromUTF8(LPCTSTR szSource);
			static CString FilterMultiBytes(const CString& strSource);
			static BYTE MakeByte(TCHAR ch1, TCHAR ch2);
			static LPSTR BSTRToASTR(const WCHAR* bstrString);
			static CString AddZero(TCHAR tcIn[], const int n);
		};
	}
}
