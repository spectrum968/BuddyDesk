/********************************************************************
*	File Name	: Util_XMLDOM.cpp
*	Version		: 
*	Purpose		: Encapsulation for XML base on DOM
*	Create Date	: 10/9/2005 2:02:01 PM
*	Author		: Van Zhao
*	Copyright (C) MorningStar Corporation.  All Rights Reserved. 
********************************************************************/

#include "StdAfx.h"
#include "util_xmldom.h"

using namespace Buddy::Util;

////////////////////////////////////////////////////////////////////////////////////
//CXmlNode
CXmlNode::CXmlNode(void)
{
	m_pNode=NULL;
}

CXmlNode::CXmlNode(const CXmlDocument & doc)
{
	m_pNode = doc.GetRoot();
}

CXmlNode::~CXmlNode()
{
}

CString CXmlNode::GetAttribute(const CString& attributeName, bool bNoTrim)
{
	CXmlNode node;
	node=m_pNode->attributes->_getNamedItem(_bstr_t(attributeName));
	if(node.IsValid())
	{
		/// Modified by Anor qiu on Aug 21, 2006.
		//return node.GetValue();
		return bNoTrim? node.GetNodeValue() : node.GetValue();
		///}}
	}
	else
	{
		return _T("");
	}
}

BOOL CXmlNode::GetAttribute(const CString& attName,CString &attValue)
{
	try
	{
		BOOL ret = FALSE;
		CXmlNode node = m_pNode->attributes->_getNamedItem(_bstr_t(attName));
		if(node.IsValid())
		{
			attValue = node.GetValue();
			ret = TRUE;
		}
		return ret;
	}
	catch(_com_error &)
	{
		return FALSE;
	}
}

_variant_t CXmlNode::GetData(void)
{
	return _variant_t(m_pNode->text);
}

double CXmlNode::GetDouble(const double defVal)
{
	try
	{
		// modified by jimmy 07.08.2006, because varDest.ChangeType(VT_R8, strval) will be fail for regional change.
		LPTSTR endptr;
		LPCTSTR lpVal = (_bstr_t)GetData();
		errno = 0;
		double dVal = _tcstod(lpVal, &endptr);
		if (errno == ERANGE || lpVal == endptr)
		{
			return defVal;
		}
		else
		{
			return dVal;
		}
		//return (double)GetData();
	}
	catch(_com_error &)
	{
		return defVal;
	}
}

COleDateTime	CXmlNode::GetDateTime(const COleDateTime &defVal)
{
	try
	{
		//return (COleDateTime)GetData();
		return CheckValidDateTime((COleDateTime)GetData(), defVal);
	}
	catch(_com_error &)
	{
		return defVal;
	}
}

char	CXmlNode::GetChar(const char defVal)
{
	try
	{
		return (char)GetData();
	}
	catch(_com_error &)
	{
		return defVal;
	}
}

unsigned char	CXmlNode::GetUChar(const unsigned char defVal)
{
	try
	{
		return (unsigned char)GetData();
	}
	catch(_com_error &)
	{
		return defVal;
	}
}

short	CXmlNode::GetShort(const short defVal)
{
	try
	{
		return (short)GetData();
	}
	catch(_com_error &)
	{
		return defVal;
	}
}

unsigned short CXmlNode::GetUShort(const unsigned short defVal)
{
	try
	{
		return (short)GetData();
	}
	catch(_com_error &)
	{
		return defVal;
	}
}

long	CXmlNode::GetLong(const long defVal)
{
	try
	{
		return (long)GetData();
	}
	catch(_com_error &)
	{
		return defVal;
	}
}

unsigned long	CXmlNode::GetULong(const unsigned long defVal)
{
	try
	{
		return (unsigned long)GetData();
	}
	catch(_com_error &)
	{
		return defVal;
	}
}

LONGLONG	CXmlNode::GetLongLong(const LONGLONG defVal)
{
	try
	{
		return (__int64)long(GetData());
	}
	catch(_com_error &)
	{
		return defVal;
	}
}

ULONGLONG	CXmlNode::GetULongLong(const ULONGLONG defVal)
{
	try
	{
		return (unsigned __int64)long(GetData());
	}
	catch(_com_error &)
	{
		return defVal;
	}
}

_variant_t CXmlNode::GetAttributeData(const CString& attributeName,const _variant_t defVar)
{
	CXmlNode node;
	node=m_pNode->attributes->_getNamedItem(_bstr_t(attributeName));
	if(node.IsValid())
	{
		return node.GetData();
	}
	else
	{
		return defVar;
	}
}

/*
double CXmlNode::GetAttributeDouble(const CString& attributeName, const double defVal)
{
	try
	{
		// modified by jimmy 07.08.2006, because varDest.ChangeType(VT_R8, strval) will be fail for regional change.
		LPTSTR endptr;
		LPCTSTR lpVal = (_bstr_t)GetAttributeData(attributeName,defVal);
		errno = 0;
		double dVal = _tcstod(lpVal, &endptr);
		if (errno == ERANGE || lpVal == endptr)
		{
			return defVal;
		}
		else
		{
			return dVal;
		}

		//return (double)GetAttributeData(attributeName,defVal);
	}
	catch (_com_error &) 
	{
		return defVal;
	}
}

COleDateTime CXmlNode::GetAttributeDateTime(const CString& attributeName, const COleDateTime & defVal)
{
	try
	{
		return (COleDateTime)GetAttributeData(attributeName,_variant_t(defVal));
	}
	catch (_com_error &) 
	{
		return defVal;
	}
}

char CXmlNode::GetAttributeChar(const CString& attributeName, const char defVal)
{
	try
	{
		return (char)GetAttributeData(attributeName,defVal);
	}
	catch(_com_error &)
	{
		return defVal;
	}
}

unsigned char CXmlNode::GetAttributeUChar(const CString& attributeName, const unsigned char defVal)
{
	try
	{
		return (unsigned char)GetAttributeData(attributeName,defVal);
	}
	catch(_com_error &)
	{
		return defVal;
	}
}

short CXmlNode::GetAttributeShort(const CString& attributeName, const short defVal)
{
	try
	{
		return (short)GetAttributeData(attributeName,defVal);
	}
	catch(_com_error &)
	{
		return defVal;
	}
}

unsigned short CXmlNode::GetAttributeUShort(const CString& attributeName, const unsigned short defVal)
{
	try
	{
		return (unsigned short)GetAttributeData(attributeName,defVal);
	}
	catch(_com_error &)
	{
		return defVal;
	}
}

long CXmlNode::GetAttributeLong(const CString& attributeName, const long defVal)
{
	try
	{
		return (long)GetAttributeData(attributeName,defVal);
	}
	catch(_com_error &)
	{
		return defVal;
	}
}

unsigned long CXmlNode::GetAttributeULong(const CString& attributeName, const unsigned long defVal)
{
	try
	{
		return (unsigned long)GetAttributeData(attributeName,defVal);
	}
	catch(_com_error &)
	{
		return defVal;
	}
}

LONGLONG	CXmlNode::GetAttributeLongLong(const CString& attributeName, const LONGLONG defVal)
{
	try
	{
		long dVal = (long)defVal;
		return (__int64) long (GetAttributeData(attributeName,_variant_t(dVal)) ) ;
	}
	catch(_com_error &)
	{
		return defVal;
	}
}

ULONGLONG	CXmlNode::GetAttributeULongLong(const CString& attributeName, const ULONGLONG defVal)
{
	try
	{
		unsigned long dVal = (unsigned long)defVal;
		return (unsigned __int64)( unsigned long (GetAttributeData(attributeName,_variant_t(dVal))) );
	}
	catch(_com_error &)
	{
		return defVal;
	}
}
//*/

void CXmlNode::SetAttribute(const CString& attributeName, const CString& value)
{
	SetAttribute(attributeName,_variant_t((LPCTSTR)value));
}

void CXmlNode::SetAttribute(const CString& attributeName, const _variant_t value)
{
	CXmlNode node;
	node=m_pNode->attributes->_getNamedItem(_bstr_t(attributeName));
	if(node.IsValid())
	{
		node.SetData(value);
	}
	else
	{
		CXmlDocument myDoc = this->GetOwnerDocument();
		if(myDoc.IsValid())
		{
			CXmlNode attr = myDoc.CreateNode(attributeName,nodeAttribute);
			attr.SetValue(value);
			XmlNode attrNode;
			m_pNode->attributes->setNamedItem(attr.GetNode(),&attrNode);
		}
	}
}


void CXmlNode::SetData(const _variant_t value)
{
	m_pNode->text=_bstr_t(value);
}

void CXmlNode::SetValue(const CString& value)
{
	m_pNode->text=_bstr_t(value);
}

XmlNode CXmlNode::Find(const CString& xpath)
{
	return m_pNode->_selectSingleNode(_bstr_t(xpath));
}

XmlNodeList CXmlNode::Select(const CString& xpath)
{
	return m_pNode->_selectNodes(_bstr_t(xpath));
}

bool CXmlNode::HasElement(LPCTSTR element)
{
	return (Find(element)!=NULL);
}

bool CXmlNode::HasAttribute(LPCTSTR attribute)
{
	CString xpath = _T("@");
	xpath+=attribute;
	return (Find(xpath)!=NULL);
}
bool CXmlNode::HasChild(void)
{
	return m_pNode->_hasChildNodes() != 0;
}

XmlNode CXmlNode::Append(const CXmlNode &node)
{
	return m_pNode->_appendChild(node.GetNode());
}

XmlNode CXmlNode::Append(const CString  &strXml)
{
	CXmlDocument doc;
	if(doc.LoadXml(strXml))
	{
		return Append(doc.GetRoot());
	}
	else
		return NULL;
}

XmlNode CXmlNode::Insert(const CXmlNode &node, const CXmlNode &nodeRef)
{
	CComVariant varRef((IDispatch*)nodeRef.GetNode());
	return m_pNode->_insertBefore(node.GetNode(),varRef);
}

XmlNode CXmlNode::Insert(const CXmlNode &node, const _variant_t &refNode)
{
	return m_pNode->_insertBefore(node.GetNode(),refNode);
}

XmlNode CXmlNode::RemoveChild(const CXmlNode & child)
{
	return m_pNode->_removeChild(child.GetNode());
}

void CXmlNode::RemoveAllChildren()
{
	XmlNode node = NULL;
	m_pNode->get_firstChild(&node);

	while( node)
	{
		XmlNode sibing = NULL;
		node->get_nextSibling(&sibing);
		m_pNode->_removeChild(node);
		node = sibing;
	}
}

XmlNode CXmlNode::ReplaceChild(const CXmlNode & oldChild,const CXmlNode & newChild)
{
	return m_pNode->_replaceChild(newChild.GetNode(),oldChild.GetNode());
}

XmlNode CXmlNode::Clone(bool deep) 
{
	return m_pNode->_cloneNode(deep ? VARIANT_TRUE : VARIANT_FALSE);
}

CString CXmlNode::GetNodeValue(LPCTSTR xpath)
{
	CXmlNode node = this->Find(xpath);
	if(node.IsNull())
	{
		return _T("");
	}
	else
	{
		return node.GetValue();
	}
}

CString CXmlNode::GetNodeXml(LPCTSTR xpath)
{
	CXmlNode node = this->Find(xpath);
	if(node.IsNull())
	{
		return _T("");
	}
	else
	{
		return node.GetXml();
	}
}

_variant_t CXmlNode::GetNodeData(LPCTSTR xpath)
{
	CXmlNode node = this->Find(xpath);
	if(node.IsNull())
	{
		_variant_t var;
		var.Clear();
		return var;
	}
	else
	{
		return node.GetData();
	}
}

CString CXmlNode::GetNodeAttribute(LPCTSTR xpath,LPCTSTR attrname)
{
	CXmlNode node = this->Find(xpath);
	if(node.IsNull())
	{
		return _T("");
	}
	else
	{
		return node.GetAttribute(attrname);
	}
}

_variant_t CXmlNode::GetNodeAttributeData(LPCTSTR xpath,LPCTSTR attrname,const _variant_t defVar)
{
	CXmlNode node = this->Find(xpath);
	if(node.IsNull())
	{
		return defVar;
	}
	else
	{
		return node.GetAttributeData(attrname, defVar); //Using the default value specified by a client
	}
}

/*
double CXmlNode::GetNodeDouble(LPCTSTR xpath,const double defVal)
{
	try
	{
		// modified by jimmy 07.08.2006, because varDest.ChangeType(VT_R8, strval) will be fail for regional change.
		LPTSTR endptr;
		LPCTSTR lpVal = (_bstr_t)GetNodeData(xpath);
		errno = 0;
		double dVal = _tcstod(lpVal, &endptr);
		if (errno == ERANGE || lpVal == endptr)
		{
			return defVal;
		}
		else
		{
			return dVal;
		}
		//return (double)GetNodeData(xpath);
	}
	catch(_com_error &)
	{
		return defVal;
	}
}

COleDateTime CXmlNode::GetNodeDateTime(LPCTSTR xpath,const COleDateTime &defVal)
{
	try
	{
		return (COleDateTime)GetNodeData(xpath);
	}
	catch(_com_error &)
	{
		return defVal;
	}
}

char CXmlNode::GetNodeChar(LPCTSTR xpath,const char defVal)
{
	try
	{
		return (char)GetNodeData(xpath);
	}
	catch(_com_error &)
	{
		return defVal;
	}
}

unsigned char CXmlNode::GetNodeUChar(LPCTSTR xpath,const unsigned char defVal)
{
	try
	{
		return (unsigned char)GetNodeData(xpath);
	}
	catch(_com_error &)
	{
		return defVal;
	}
}

short CXmlNode::GetNodeShort(LPCTSTR xpath,const short defVal)
{
	try
	{
		return (short)GetNodeData(xpath);
	}
	catch(_com_error &)
	{
		return defVal;
	}
}

unsigned short CXmlNode::GetNodeUShort(LPCTSTR xpath,const unsigned short defVal)
{
	try
	{
		return (unsigned short)GetNodeData(xpath);
	}
	catch(_com_error &)
	{
		return defVal;
	}
}

long CXmlNode::GetNodeLong(LPCTSTR xpath,const long defVal)
{
	try
	{
		return (long)GetNodeData(xpath);
	}
	catch(_com_error &)
	{
		return defVal;
	}
}

unsigned long CXmlNode::GetNodeULong(LPCTSTR xpath,const unsigned long defVal)
{
	try
	{
		return (unsigned long)GetNodeData(xpath);
	}
	catch(_com_error &)
	{
		return defVal;
	}
}

LONGLONG CXmlNode::GetNodeLongLong(LPCTSTR xpath,const LONGLONG defVal)
{
	try
	{
		return (__int64)long(GetNodeData(xpath));
	}
	catch(_com_error &)
	{
		return defVal;
	}
}

ULONGLONG CXmlNode::GetNodeULongLong(LPCTSTR xpath,const ULONGLONG defVal)
{
	try
	{
		return (unsigned __int64)( unsigned long (GetNodeData(xpath)) );
	}
	catch(_com_error &)
	{
		return defVal;
	}
}

double CXmlNode::GetNodeAttributeDouble(LPCTSTR xpath,LPCTSTR attrname, const double defVal)
{
	try
	{
		// modified by jimmy 07.08.2006, because varDest.ChangeType(VT_R8, strval) will be fail for regional change.
		LPTSTR endptr;
		LPCTSTR lpVal = (_bstr_t)GetNodeAttributeData(xpath,attrname,defVal);
		errno = 0;
		double dVal = _tcstod(lpVal, &endptr);
		if (errno == ERANGE || lpVal == endptr)
		{
			return defVal;
		}
		else
		{
			return dVal;
		}
		//return (double)GetNodeAttributeData(xpath,attrname,defVal);
	}
	catch(_com_error &)
	{
		return defVal;
	}
}

COleDateTime CXmlNode::GetNodeAttributeDateTime(LPCTSTR xpath,LPCTSTR attrname, const COleDateTime & defVal)
{
	try
	{
		return (COleDateTime)GetNodeAttributeData(xpath,attrname,_variant_t(defVal));
	}
	catch(_com_error &)
	{
		return defVal;
	}
}

char CXmlNode::GetNodeAttributeChar(LPCTSTR xpath,LPCTSTR attrname, const char defVal)
{
	try
	{
		return (char)GetNodeAttributeData(xpath,attrname,defVal);
	}
	catch(_com_error &)
	{
		return defVal;
	}
}

unsigned char CXmlNode::GetNodeAttributeUChar(LPCTSTR xpath,LPCTSTR attrname, const unsigned char defVal)
{
	try
	{
		return (unsigned char)GetNodeAttributeData(xpath,attrname,defVal);
	}
	catch(_com_error &)
	{
		return defVal;
	}
}

short CXmlNode::GetNodeAttributeShort(LPCTSTR xpath,LPCTSTR attrname, const short defVal)
{
	try
	{
		return (short)GetNodeAttributeData(xpath,attrname,defVal);
	}
	catch(_com_error &)
	{
		return defVal;
	}
}

unsigned short CXmlNode::GetNodeAttributeUShort(LPCTSTR xpath,LPCTSTR attrname, const unsigned short defVal)
{
	try
	{
		return (unsigned short)GetNodeAttributeData(xpath,attrname,defVal);
	}
	catch(_com_error &)
	{
		return defVal;
	}
}

long CXmlNode::GetNodeAttributeLong(LPCTSTR xpath,LPCTSTR attrname, const long defVal)
{
	try
	{
		return (long)GetNodeAttributeData(xpath,attrname,defVal);
	}
	catch(_com_error &)
	{
		return defVal;
	}
}

unsigned long CXmlNode::GetNodeAttributeULong(LPCTSTR xpath,LPCTSTR attrname, const unsigned long defVal)
{
	try
	{
		return (unsigned long)GetNodeAttributeData(xpath,attrname,defVal);
	}
	catch(_com_error &)
	{
		return defVal;
	}
}

LONGLONG CXmlNode::GetNodeAttributeLongLong(LPCTSTR xpath,LPCTSTR attrname, const LONGLONG  defVal)
{
	try
	{
		return (__int64)long(GetNodeAttributeData(xpath,attrname,_variant_t((long)defVal)));
	}
	catch(_com_error &)
	{
		return defVal;
	}
}

LONGLONG CXmlNode::GetNodeAttributeULongLong(LPCTSTR xpath,LPCTSTR attrname, const ULONGLONG  defVal)
{
	try
	{
		return (unsigned __int64)long(GetNodeAttributeData(xpath,attrname,_variant_t((long)defVal)) );
	}
	catch(_com_error &)
	{
		return defVal;
	}
}
//*/

void CXmlNode::SetNodeValue(const CString& xpath, const CString& value)
{
	CXmlNode node = this->Find(xpath);
	if(node.IsNull())
		return;
	else
		node.SetValue(value);
}

void  CXmlNode::SetNodeAttribute(const CString& xpath, const CString& attributeName, const CString& value)
{
	CXmlNode child = this->Find(xpath);
	if(child.IsValid())
	{
		child.SetAttribute(attributeName, value);
	}
}

XmlDocument CXmlNode::GetOwnerDocument()
{
	if(IsValid())
	{
		MSXML2::IXMLDOMDocumentPtr pDoc;
		m_pNode->get_ownerDocument(&pDoc);
		return pDoc;
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////////
//CXmlDocument

CXmlDocument::CXmlDocument(bool bFreeThread):m_bFreeThread(bFreeThread)
{
	Initialize();
}
BOOL CXmlDocument::Initialize()
{
	::CoInitialize(0);
	
	Close();

	HRESULT hr;
	m_pDoc=NULL;
	if(m_bFreeThread)
		hr=m_pDoc.CreateInstance("Msxml2.FreeThreadedDOMDocument");
	else
		hr=m_pDoc.CreateInstance("Msxml2.DOMDocument");

	if(FAILED(hr))
	{
		m_pDoc=NULL;
		ATLTRACE("XML DOM Document can't initialization.");
		return FALSE;
	}
	m_pDoc->_setProperty(_T("SelectionLanguage"), _T("XPath"));
	//m_pDoc->_setProperty(_T("SelectionNamespaces"), _T("xmlns:xsf='http://schemas.microsoft.com/office/infopath/2003/solutionDefinition'"));
	m_pDoc->async = false;
	return TRUE;
}

CXmlDocument::~CXmlDocument()
{
	if(m_pDoc!=NULL)
	{
		m_pDoc.Release();
		m_pDoc=NULL;
	}
}

void CXmlDocument::SetPreserveWhiteSpace(bool bPreserve)
{
	if (m_pDoc!=NULL)
		m_pDoc->preserveWhiteSpace = bPreserve ? VARIANT_TRUE : VARIANT_FALSE;
}

bool CXmlDocument::Load(LPCTSTR url)
{
	if ( m_pDoc ==NULL )
		Initialize();
	ATLASSERT(m_pDoc!=NULL);
	m_pDoc->Putasync(VARIANT_FALSE);
	if(m_pDoc->_load(url))
	{
		m_szUrl = url;
		m_pNode = GetRoot();

		//set default selection namespaces
		CString cstr = GetNameSpaces();
		if(!cstr.IsEmpty())
		{
			m_pDoc->_setProperty(_T("SelectionNamespaces"), (LPCTSTR)cstr);
		}
		return true;
	}
	else
	{
		m_szUrl.Empty();
		m_pNode = NULL;
		return false;
	}
}

bool CXmlDocument::LoadXml(const _bstr_t &xmlString)
{
	if ( m_pDoc ==NULL )
		Initialize();

	ATLASSERT(m_pDoc!=NULL);

	if(xmlString.length() == 0) return false;

	if(m_pDoc->_loadXML(xmlString))
	{
		m_szUrl.Empty();
		m_pNode = GetRoot();
		//set default selection namespaces
		CString cstr = GetNameSpaces();
		if(!cstr.IsEmpty())
		{
			m_pDoc->_setProperty(_T("SelectionNamespaces"), (LPCTSTR)cstr);
		}
		return true;
	}
	else
	{
		m_szUrl.Empty();
		m_pNode = NULL;
		return false;
	}
}

LPCWSTR CXmlDocument::XmlStringAdapt(LPCWSTR xmldata)
{
	if(wcslen(xmldata)>1 && (WORD)xmldata[0]==0xfeff) return (xmldata+1);
	return xmldata;
}

LPCSTR CXmlDocument::XmlStringAdapt(LPCSTR xmldata)
{
	if(strlen(xmldata)>3 && (BYTE)xmldata[0]==0xef && (BYTE)xmldata[1]==0xbb && (BYTE)xmldata[2]==0xbf) return (xmldata+3);
	if(strlen(xmldata)>2 && (BYTE)xmldata[0]==0xff && (BYTE)xmldata[1]==0xfe) return (xmldata+2);
	return xmldata;
}

bool CXmlDocument::LoadXml(LPCWSTR xmlString)
{
	if(xmlString == NULL || wcslen(xmlString) == 0) return false;

	return LoadXml(_bstr_t(CXmlDocument::XmlStringAdapt(xmlString)));
}

bool CXmlDocument::LoadXml(LPCSTR xmlString)
{
	if(xmlString == NULL || strlen(xmlString) == 0) return false;

	return LoadXml(_bstr_t(CXmlDocument::XmlStringAdapt(xmlString)));
}

bool CXmlDocument::Save()
{
	ATLASSERT(m_pDoc!=NULL && !m_szUrl.IsEmpty());

	return SUCCEEDED(m_pDoc->_save(_variant_t(_bstr_t(m_szUrl))));
}

bool CXmlDocument::SaveAs(LPCTSTR filename)
{
	try
	{
		if(SUCCEEDED(m_pDoc->_save(_variant_t(_bstr_t(filename)))))
		{
			m_szUrl = filename;
			return true;
		}
		else
		{
			return false;
		}
	}
	catch(...)//if files is readonly will except
	{
		return false;
	}
}

CString CXmlDocument::Transform(const CXmlNode &nodeXsl)
{
	_bstr_t bsResult;
	bsResult = m_pDoc->_transformNode(nodeXsl.GetNode());
	return (LPCTSTR)bsResult;
}
bool CXmlDocument::TransformNodeToObject(CXmlDocument*pNode,CXmlDocument*pOutDOM)
{
	XmlDocument pXmlDoc;

	HRESULT hr = pXmlDoc.CreateInstance(__uuidof(MSXML2::DOMDocument));
	if ( hr == S_OK )
	{
		CComQIPtr<IDispatch> pDisp = pXmlDoc;
		if ( !pDisp ) return false;

		CComVariant vOut;
		vOut.vt = VT_DISPATCH;
		vOut.pdispVal = pDisp.Detach();
		hr = m_pDoc->transformNodeToObject(pNode->m_pDoc,vOut);
		if ( hr == S_OK )
		{
			bool bLogin=false;
			hr = pOutDOM->SetXmlDocument(&vOut,bLogin);

			return true;
		}
	}

	return false;

}
CString CXmlDocument::GetParseError()
{
	ATLASSERT(m_pDoc!=NULL);

	ParseErrorPtr errPtr=m_pDoc->parseError;
	if(errPtr!=NULL)
	{
		CString cstr;
		cstr.Format(_T("error code %x at line %d position %d, reason: %s, source: %s."),errPtr->errorCode,errPtr->line,errPtr->linepos,(LPCTSTR)errPtr->reason,(LPCTSTR)errPtr->srcText);
		ATLTRACE(cstr);
		return cstr;
	}
	else
	{
		return _T("");
	}
}

CString CXmlDocument::GetNameSpaces()
{
	CString cstr,xpath,name,ns;
	int at;
	long count;
	count = m_pDoc->namespaces->length;
	CXmlNode node;
	for(long i =0;i<count;++i)
	{
		ns = (LPCTSTR)m_pDoc->namespaces->namespaceURI[i];
		xpath.Format(_T("//*[namespace-uri()='%s']"),ns);
		node = this->Find(xpath);
		if(node.IsValid())
		{
			name = node.GetName();
			if((at=name.Find(_T(":"),0))>=0)
			{
				if(!cstr.IsEmpty()) cstr += _T(" ");
				cstr.AppendFormat(_T("xmlns:%s='%s'"),name.Left(at),ns);
			}
		}

	}
	return cstr;
}

XmlNode CXmlDocument::CreateNode(const CString &name,enum NodeType nodeType,const CString &nameSpace)
{
	ATLASSERT(m_pDoc!=NULL);
	if(nameSpace.IsEmpty())
		return m_pDoc->_createNode((long)nodeType,_bstr_t(name),_bstr_t(_T("")));
	else
		return m_pDoc->_createNode((long)nodeType,_bstr_t(name),_bstr_t(nameSpace));
}
void CXmlDocument::Abort(void)
{
	m_pDoc->_abort();
}

CString CXmlDocument::GetProperty(const CString &propertyName)
{
	return (LPCTSTR)_bstr_t(m_pDoc->_getProperty(_bstr_t(propertyName)));
}

void CXmlDocument::SetProperty(const CString &propertyName,const CString &propertyValue)
{
	m_pDoc->_setProperty(_bstr_t(propertyName),_variant_t(propertyValue));
}

HRESULT CXmlDocument::SetXmlDocument(VARIANT* pXmlDocument, bool& bLoginPage)
{
	return SetXmlDocument(pXmlDocument,bLoginPage,true);
}

HRESULT CXmlDocument::SetXmlDocument(VARIANT* pXmlDocument, bool& bLoginPage,bool bCheckLogin)
{
	bool bErrXml;
	return SetXmlDocument(pXmlDocument,bLoginPage,bCheckLogin, bErrXml);
}

HRESULT CXmlDocument::SetXmlDocument(VARIANT* pXmlDocument)
{
	HRESULT hr = S_OK;

	if (pXmlDocument->vt == VT_DISPATCH)
	{
		if (pXmlDocument->pdispVal == NULL)
			return S_FALSE;
		m_pDoc = pXmlDocument->pdispVal;
	}
	else if(pXmlDocument->vt == (VT_ARRAY | VT_UI1))
	{
		SAFEARRAY* pArray = pXmlDocument->parray;

		int nSize = pArray->rgsabound[0].cElements;
		if ( nSize <= 0 )
			return S_FALSE;

		VARIANT_BOOL bResult;
		hr = m_pDoc->load(*pXmlDocument,&bResult);
		if(FAILED(hr) || !bResult)
			return hr;
	}
	else if(pXmlDocument->vt == VT_BSTR)
	{
		VARIANT_BOOL bResult;

		hr = m_pDoc->loadXML(pXmlDocument->bstrVal,&bResult);
		
		if(FAILED(hr) || !bResult) return S_FALSE;
	}
	else
	{
		return E_FAIL;
	}

	m_pNode = GetRoot();
	return S_OK;
}

HRESULT CXmlDocument::SetXmlDocument(VARIANT* pXmlDocument, bool& bLoginPage,bool bCheckLogin, bool& bErrXml)
{
	bCheckLogin;
	bErrXml = false;
	bLoginPage = false;

	CString xpath(_T("//AwsTimeout/Url"));
	CString errpath(_T("//Error"));
	CXmlNode node;

	if (SetXmlDocument(pXmlDocument)==S_OK)
	{
		
		if (m_pNode!=NULL)
		{
			node = Find(xpath);
			if (!node.IsNull())
				bLoginPage = true;

			node = Find(errpath);
			if(!node.IsNull()) 
				bErrXml = true;
		}
	}
	return S_OK;
}
bool CXmlDocument::SetNodeAttribute(const CXmlNode& node, const CString& attributeName, const CString& value)
{
	if(node.GetNode() == NULL)
		return FALSE;

	XmlAttribute pAttr;
	HRESULT hr = m_pDoc->createAttribute((_bstr_t)attributeName, &pAttr);
	if ( FAILED(hr) )
		return FALSE;

	hr = pAttr->put_text((_bstr_t)value);
	if( FAILED(hr) )
		return FALSE;

	XmlMap pNodeMap;
	hr = node.GetNode()->get_attributes(&pNodeMap);
	if ( FAILED(hr) )
		return FALSE;

	XmlNode pNamedItem;
	hr = pNodeMap->setNamedItem(pAttr, &pNamedItem);
	if ( FAILED(hr) )
		return FALSE;

	return TRUE;
}

HRESULT CXmlDocument::SetNodeValueEx(const CString& strPath, const CString& value, const CString& attributeName)
{
	if (m_pNode == NULL)
	{
		return S_FALSE;
	}

	CXmlNode node = this->Find(strPath);
	if (!node.IsValid())
		return S_FALSE;
	if (!attributeName.IsEmpty())
	{
		node.SetAttribute(attributeName,value);
	}
	else
	{
		node.SetValue(value);
	}
	return S_OK;
}

CString CXmlDocument::GetNodeValueEx(const CString& strPath, const CString& attributeName,bool bGetXML)
{
	if (m_pNode == NULL)
		return _T("");

	CXmlNode node = this->Find(strPath);
	if (!node.IsValid())
		return _T("");

	if (!attributeName.IsEmpty())
	{
		return node.GetAttribute(attributeName);
	}
	else
	{
		if( bGetXML )
		{
			return CString(node.GetXml());
		}

		else
			return node.GetValue();
	}
}

void CXmlDocument::Close()
{
	if(m_pDoc)
	{
		m_pNode = NULL;
		m_szUrl.Empty();
		m_pDoc.Release();
		m_pDoc = NULL;
	}
}

void CXmlNode::RemoveAttribute(const CString& strAttrName)
{
	CXmlMap mapAttr = m_pNode->attributes;
	if(!mapAttr.IsEmpty())
		mapAttr.RemoveNode(strAttrName);
}

//bool CXmlDocument::SetNodeAttribute(const CXmlNode& node, const CString& attributeName, const _variant_t value)
//{
//	if(node.GetNode() == NULL)
//		return FALSE;
//
//	XmlAttribute pAttr;
//	HRESULT hr = m_pDoc->createAttribute((_bstr_t)attributeName, &pAttr);
//	if ( FAILED(hr) )
//		return FALSE;
//
//	hr = pAttr->put_nodeValue(value);
//	if( FAILED(hr) )
//		return FALSE;
//
//	XmlMap pNodeMap;
//	hr = node.GetNode()->get_attributes(&pNodeMap);
//	if ( FAILED(hr) )
//		return FALSE;
//
//	XmlNode pNamedItem;
//	hr = pNodeMap->setNamedItem(pAttr, &pNamedItem);
//	if ( FAILED(hr) )
//		return FALSE;
//
//	return TRUE;
//}

////////////////////////////////////////////////////////////////////////////////////
//CXmlNodeList
CString CXmlNodeList::GetDelimitedString(const CString &delimiter)
{
	ATLASSERT(m_pNodeList!=NULL);
	CString cstr;
	CXmlNode node;
	long nLength = m_pNodeList->Getlength();
	for(long i=0;i<nLength;i++)
	{	
		if(i>0) cstr+=delimiter;
		node=m_pNodeList->Getitem(i);
		cstr+=node.GetValue();
	}
	return cstr;
}

long CXmlNodeList::GetLength(void)
{
	ATLASSERT(m_pNodeList!=NULL);
	return m_pNodeList->length;
}
//reset nodelist
bool CXmlNodeList::Reset(void)
{
	ATLASSERT(m_pNodeList!=NULL);
	return SUCCEEDED(m_pNodeList->_reset());
}
//get next node from node list
XmlNode CXmlNodeList::Next(void)
{
	ATLASSERT(m_pNodeList!=NULL);
	return m_pNodeList->_nextNode();
}
//get node list number of elements
XmlNode CXmlNodeList::GetItem(long index) 
{
	ATLASSERT(m_pNodeList!=NULL);
	return m_pNodeList->Getitem(index);
}