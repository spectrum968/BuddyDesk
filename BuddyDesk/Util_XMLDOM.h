/********************************************************************
*	File Name	: Util_XMLDOM.h
*	Version		: 
*	Purpose		: Encapsulation for XML base on DOM
*	Create Date	: 10/9/2005 1:59:51 PM
*	Author		: Van Zhao
*	Copyright (C) MorningStar Corporation.  All Rights Reserved. 
********************************************************************/
#pragma once

#ifndef _AFX
#include <atlcomtime.h>
#endif

#import <msxml3.dll> high_method_prefix("_") raw_method_prefix("")

//XML typedef base on DOM.
using namespace MSXML2;
typedef MSXML2::IXMLDOMDocument2Ptr			XmlDocument;
typedef MSXML2::IXMLDOMNodePtr				XmlNode;
typedef MSXML2::IXMLDOMNodeListPtr			XmlNodeList;
typedef MSXML2::IXMLDOMElementPtr			XmlElement;
typedef MSXML2::IXMLDOMAttributePtr			XmlAttribute;
typedef MSXML2::IXMLDOMNamedNodeMapPtr		XmlMap;
typedef MSXML2::IXMLDOMParseErrorPtr		ParseErrorPtr;
typedef MSXML2::IXMLDOMDocumentFragmentPtr	XmlFragment;
typedef MSXML2::DOMNodeType					XmlNodeType;
typedef MSXML2::IXSLTemplatePtr				XslTemplate;
typedef MSXML2::IXSLProcessorPtr			XslProcess;
typedef MSXML2::IXMLHTTPRequestPtr			XmlHttpRequest;

const _variant_t varEmpty;

namespace Buddy
{
	namespace Util
	{
		//Description	: The CXml* serial classes wrapped XML operations base on DOM. There are some useful typedefs about the MSXML2 namespace defined in Util_ShareDefine.h.
		class CXmlDocument;

		//Description	: The CXmlNode class provides methods that represent the core functionality of any node.
		class CXmlNode
		{
		protected:
			//A smart pointer to IXMLDOMNodePtr.
			XmlNode m_pNode;

		protected:
			CXmlNode GetSubNode(const CString & refcstrPath)
			{
				try{return m_pNode != NULL? static_cast<CXmlNode>(this->Find(refcstrPath)) : CXmlNode();}
				catch(_com_error &){return CXmlNode();}
			}
			
			CXmlNode GetAttributeNode(const CString& attributeName)
			{
				try{return m_pNode != NULL && m_pNode->attributes != NULL? 
						static_cast<CXmlNode>(m_pNode->attributes->_getNamedItem(_bstr_t(attributeName)))
						: CXmlNode();}
				 catch(_com_error &){return CXmlNode();}
			}

			COleDateTime CheckValidDateTime(const COleDateTime& refcDateTime, const COleDateTime &refcDefVal)
			{return refcDateTime.GetStatus() == COleDateTime::DateTimeStatus::valid? refcDateTime : refcDefVal;}

		public:
			//Enumeration representing the type of the node
			enum NodeType
			{
				nodeElement = 1,
				nodeAttribute = 2,
				nodeText = 3,
				nodeCData = 4,
				nodeEntityRef = 5,
				nodeEntity = 6,
				nodePI = 7,
				nodeComment = 8,
				nodeDocument = 10,
				nodeFragment = 11,
				nodeNotation =12,
			};
			//constructors
			CXmlNode(void);
			CXmlNode(const CXmlNode & node) {m_pNode=node.m_pNode;}
			CXmlNode(const XmlNode & pNode) {m_pNode=pNode;}
			CXmlNode(const CXmlDocument & doc);
			virtual ~CXmlNode(void);

			//Description	:An XML representation of this node and all its descendants. 
			//Return value	:A CString object containing XML representation.
			CString GetXml(void)		{return (LPCTSTR)m_pNode->xml;}

			//Description	:Retrieves the node type in string form.
			//Return value	:A CString object containing the version of the node type.
			CString GetTypeString(void)	{return (LPCTSTR)m_pNode->nodeTypeString;}

			//Description	:Retrieves the node type in enumeration form.
			//Return value	:A enumeration representing the type of the node.
			XmlNodeType GetType(void)	{return m_pNode->nodeType;}

			//Description	:Retrieves the parent node (for nodes that can have parents).
			//Return value	:A XmlNode(IXMLDOMNodePtr) that representing parent node if successed, otherwise is NULL.
			XmlNode GetParent(void)		{return m_pNode->parentNode;}

			//Description	:Retrieves the Uniform Resource Identifier (URI) for the namespace.
			//Return value	:A CString containing the URI of namespace.
			CString GetNamespaceURI(void)			{return (LPCTSTR)m_pNode->namespaceURI;}

			//Description	:Retrieves the namespace prefix of the node.
			//Return value	:A CString containing namespace prefix, if no prefix is specified, CString is empty.
			CString GetPrefix(void) {return (LPCTSTR)m_pNode->prefix;}

			//Description	:Retrieves the qualified name of the node. .
			//Return value	:A CString containing node name, which varies depending on the node type. 
			CString GetName(void)	{return (LPCTSTR)m_pNode->nodeName;}

			//Description	:Retrieves the text content of the node in string form.
			//Return value	:A CString containing node text and its descendants, which varies depending on the node type.
			CString GetValue(void)	{return (LPCTSTR)m_pNode->text;}

			/// Added by Anor qiu on Aug 21, 2006.
			//Description	:Contains the text associated with the node.
			//Return value	:A CString containing node text and its descendants, which varies depending on the node type.
			CString GetNodeValue(void){return (LPCTSTR)(_bstr_t)m_pNode->nodeValue;}
			///}}

			_variant_t	GetData(void);

			double GetDouble(const double defVal = 0.0);
			COleDateTime	GetDateTime(const COleDateTime &defVal);
			char	GetChar(const char defVal = 0);
			unsigned char	GetUChar(const unsigned char defVal = 0);
			short	GetShort(const short defVal = 0);
			unsigned short GetUShort(const unsigned short defVal = 0);
			long	GetLong(const long defVal = 0);
			unsigned long	GetULong(const unsigned long defVal = 0);
			LONGLONG	GetLongLong(const LONGLONG defVal = 0);
			ULONGLONG	GetULongLong(const ULONGLONG defVal = 0);

			//Description	:Retrieves value of the specified attribute of the node in string form.
			//attributeName	:The name of attribution want to get value.
			//Return value	:If succeeds, CString containing text content of specified attribute. If failed, CString is empty.
			CString		GetAttribute(const CString& attributeName, bool bNoTrim = false); //bNoTrim -- added by Anor qiu on Aug 21, 2006.
			
			BOOL GetAttribute(const CString& attName,CString &attValue);

			//Description	:Retrieves value of the specified attribute of the node in _variant_t form.
			//attributeName	:The name of attribution want to get value.
			//Return value	:A _variant_t object containing text content of specified attribute if succeed, otherwise _variant_t is empty.
			_variant_t	GetAttributeData(const CString& attributeName, const _variant_t defVar = varEmpty);
			
			double GetAttributeDouble(const CString& attributeName, const double defVal = 0.0)
			{return GetAttributeNode(attributeName).GetDouble(defVal);}

			COleDateTime GetAttributeDateTime(const CString& attributeName, const COleDateTime &defVal)
			{return GetAttributeNode(attributeName).GetDateTime(defVal);}

			char GetAttributeChar(const CString& attributeName, const char defVal = 0)
			{return GetAttributeNode(attributeName).GetChar(defVal);}

			unsigned char GetAttributeUChar(const CString& attributeName, const unsigned char defVal = 0)
			{return GetAttributeNode(attributeName).GetUChar(defVal);}

			short GetAttributeShort(const CString& attributeName, const short defVal = 0)
			{return GetAttributeNode(attributeName).GetShort(defVal);}

			unsigned short GetAttributeUShort(const CString& attributeName, const unsigned short defVal = 0)
			{return GetAttributeNode(attributeName).GetUShort(defVal);}

			long GetAttributeLong(const CString& attributeName, const long defVal = 0)
			{return GetAttributeNode(attributeName).GetLong(defVal);}

			unsigned long	GetAttributeULong(const CString& attributeName, const unsigned long defVal = 0)
			{return GetAttributeNode(attributeName).GetULong(defVal);}

			LONGLONG	GetAttributeLongLong(const CString& attributeName, const LONGLONG defVal = 0)
			{return GetAttributeNode(attributeName).GetLongLong(defVal);}

			ULONGLONG	GetAttributeULongLong(const CString& attributeName, const ULONGLONG defVal = 0)
			{return GetAttributeNode(attributeName).GetULongLong(defVal);}

			//Description	:Assigns the text content of the node.
			//value			:The text content of the node in CString form. The text represents the node and its descendants.
			//Return value	:None.
			void SetValue(const CString& value);

			//Description	:Assigns the value of the node.
			//value			:the value of the node in _variant_t form.
			//Return value	:None.
			void SetData(const _variant_t value);

			//Description	:Assigns the value of the specified attribute of the node in string form.
			//attributeName	:The name of attribution want to get value.
			//value			:The text content of the attribute in CString form.
			//Return value	:None
			void SetAttribute(const CString& attributeName,const CString& value);

			//Description	:Assigns the value of the specified attribute of the node in _variant_t form.
			//attributeName	:The name of attribution want to get value.
			//value			:The text content of the attribute in _variant_t form.
			//Return value	:None
			void SetAttribute(const CString& attributeName, const _variant_t value);


			//Description	:Retrieves a node list containing the children (for nodes that can have children).
			//Return value	:A XmlNodeList(IXMLDOMNodeListPtr) that represents a list of children in the node.
			XmlNodeList GetChilds(void) {return m_pNode->childNodes;}

			//Description	:Retrieves the list of attributes for this node.
			//Return value	:A XmlMap(IXMLDOMNamedNodeMapPtr) that is retrieved for nodes that can return attributes
			XmlMap		GetAttributes(void) {return m_pNode->attributes;}

			//Description	:Remove a named attribution
			//strAttrName	:name of attribution which want to remove
			void RemoveAttribute(const CString& strAttrName);

			//Description	:Retrieves the first child of this node.
			//Return value	:A XmlNode(IXMLDOMNodePtr) that representing the first child node.
			XmlNode First(void)	{return m_pNode->firstChild;}

			//Description	:Retrieves the last child node.
			//Return value	:A XmlNode(IXMLDOMNodePtr) that representing the last child node.
			XmlNode Last(void)	{return m_pNode->lastChild;}

			//Description	:Retrieves the next sibling of this node in the parent's child list.
			//Return value	:A XmlNode(IXMLDOMNodePtr) that representing the next sibling child node.
			XmlNode Next(void)	{return m_pNode->nextSibling;}

			//Description	:Retrieves the previous sibling of this node in the parent's child list.
			//Return value	:A XmlNode(IXMLDOMNodePtr) that representing the previous  sibling child node.
			XmlNode Prev(void)	{return m_pNode->previousSibling;}

			//Description	:Applies the specified pattern-matching operation to find the first matching node.
			//xpath			:A string specifying an XPath expression. 
			//Return value	:A XmlNode(IXMLDOMNodePtr) that representing the first node that is selected by the given pattern-matching operation. If no nodes match the expression, returns NULL
			XmlNode Find(const CString& xpath);

			XmlNode Find(LPCTSTR xpath) {return m_pNode->_selectSingleNode(_bstr_t(xpath));}

			//Description	:Applies the specified pattern-matching operation to this node's context and returns the list of matching nodes. 
			//xpath			:A string specifying an XPath expression. 
			//Return value	:A XmlNodeList(IXMLDOMNodeListPtr) that represents the list of nodes selected by applying the given pattern-matching operation. If no nodes are selected, returns NULL.
			XmlNodeList Select(const CString& xpath);

			//Description	:Appends the supplied new child as the last child of this node.
			//node			:The new child node to be appended.
			//Return value	:A XmlNode(IXMLDOMNodePtr) representing the new child node appended to the list. 
			XmlNode Append(const CXmlNode &node);
			XmlNode Append(const CString  &strXml);

			//Description	:Inserts a child node to the left of the specified node or at the end of the list.
			//node			:The new child node to be appended.
			//refNode		:The address of the reference node. If this parameter is NULL, node is inserted at the end of the child list. 
			//nodeRef		:The reference node.
			//Return value	:A XmlNode(IXMLDOMNodePtr) containing the child node that was successfully inserted. 
			XmlNode Insert(const CXmlNode &node,const _variant_t &refNode);
			XmlNode Insert(const CXmlNode &node,const CXmlNode &nodeRef);

			//Description	:Removes the specified child node from this node.
			//child			:The child node to be removed from this node.
			//Return value	:A XmlNode(IXMLDOMNodePtr) representing the removed child node. 
			XmlNode RemoveChild(const CXmlNode & child);

			//Description	:Removes the all children nodes from this node.
			//Return value	:None. 
			void RemoveAllChildren();

			//Description	:Replaces the old child node with the supplied new child node and returns the specified old child node.
			//oldChild		:The old child node.
			//newChild		:The new child node.
			//Return value	:A XmlNode(IXMLDOMNodePtr) containing the old child that is replaced.
			XmlNode ReplaceChild(const CXmlNode & oldChild,const CXmlNode & newChild);

			//Description	:Creates a new node that is an exact clone of this node.
			//beep			:Indicates whether to recursively clone all nodes that are descendants of this node. If true, create a clone of the complete tree below this node. If false, clone this node and its attributes only.
			//Return value	:A XmlNode(IXMLDOMNodePtr) containing a newly created clone node. 
			XmlNode Clone(bool deep);


			//Description	:Tests validity of this node.
			//Return value	:Reture true if this node is valid, otherwise is false.
			bool IsValid(void)	{return m_pNode!=NULL;}

			//Description	:Tests the parsed status of the node and child nodes.
			//Return value	:True if this node and all descendants have been parsed; False if any descendants remain to be parsed. 
			bool IsParsed(void) {return m_pNode->parsed != 0;}

			//Description	:Retrieves a Boolean value indicating whether this node has children.
			//Return value	:True if this node has children, otherwise is false.
			bool HasChild(void);

			//Description	:Determines whether this node has specified element.
			//element		:A string buffer containing the path of element.
			//Return value	:True if this node has specified element, otherwise is false.
			bool HasElement(LPCTSTR element);

			//Description	:Determines whether this node has specified attribute.
			//attribute		:A string buffer containing the path of element.
			//Return value	:True if this node has specified element, otherwise is false.
			bool HasAttribute(LPCTSTR attribute);

			void SetNodeValue(const CString& xpath, const CString& value);
			void SetNodeAttribute(const CString& xpath, const CString& attributeName, const CString& value);

			//check if this object is null
			bool IsNull() {return (m_pNode == NULL);}
			// Operations
			XmlNode GetNode(void) const	{return m_pNode;}
			CXmlNode & operator= (const CXmlNode & rhl) {this->m_pNode=rhl.GetNode();return *this;}
			CXmlNode & operator= (const XmlNode & rhl) {this->m_pNode=rhl;return *this;}

			CString GetNodeValue(LPCTSTR xpath);
			CString GetNodeXml(LPCTSTR xpath);
			_variant_t GetNodeData(LPCTSTR xpath);

			double GetNodeDouble(LPCTSTR xpath,const double defVal = 0.0)
			{return GetSubNode(xpath).GetDouble(defVal);}

			COleDateTime GetNodeDateTime(LPCTSTR xpath,const COleDateTime &defVal)
			{return GetSubNode(xpath).GetDateTime(defVal);}

			char GetNodeChar(LPCTSTR xpath,const char defVal = 0)
			{return GetSubNode(xpath).GetChar(defVal);}

			unsigned char GetNodeUChar(LPCTSTR xpath,const unsigned char defVal = 0)
			{return GetSubNode(xpath).GetUChar(defVal);}

			short GetNodeShort(LPCTSTR xpath,const short defVal = 0)
			{return GetSubNode(xpath).GetShort(defVal);}\

			unsigned short GetNodeUShort(LPCTSTR xpath,const unsigned short defVal = 0)
			{return GetSubNode(xpath).GetUShort(defVal);}

			long GetNodeLong(LPCTSTR xpath,const long defVal = 0)
			{return GetSubNode(xpath).GetLong(defVal);}\

			unsigned long GetNodeULong(LPCTSTR xpath,const unsigned long defVal = 0)
			{return GetSubNode(xpath).GetULong(defVal);}

			LONGLONG GetNodeLongLong(LPCTSTR xpath,const LONGLONG defVal = 0)
			{return GetSubNode(xpath).GetLongLong(defVal);}

			ULONGLONG GetNodeULongLong(LPCTSTR xpath,const ULONGLONG defVal = 0)
			{return GetSubNode(xpath).GetULongLong(defVal);}

			CString GetNodeAttribute(LPCTSTR xpath,LPCTSTR attrname);
			_variant_t GetNodeAttributeData(LPCTSTR xpath,LPCTSTR attrname,const _variant_t defVar = varEmpty);
			
			double GetNodeAttributeDouble(LPCTSTR xpath,LPCTSTR attrname, const double defVal = 0.0)
			{return GetSubNode(xpath).GetAttributeNode(attrname).GetDouble(defVal);}

			COleDateTime GetNodeAttributeDateTime(LPCTSTR xpath,LPCTSTR attrname, const COleDateTime & defVal)
			{return GetSubNode(xpath).GetAttributeNode(attrname).GetDateTime(defVal);}

			char GetNodeAttributeChar(LPCTSTR xpath,LPCTSTR attrname, const char defVal = 0)
			{return GetSubNode(xpath).GetAttributeNode(attrname).GetChar(defVal);}

			unsigned char GetNodeAttributeUChar(LPCTSTR xpath,LPCTSTR attrname, const unsigned char defVal = 0)
			{return GetSubNode(xpath).GetAttributeNode(attrname).GetUChar(defVal);}

			short GetNodeAttributeShort(LPCTSTR xpath,LPCTSTR attrname, const short defVal = 0)
			{return GetSubNode(xpath).GetAttributeNode(attrname).GetShort(defVal);}

			unsigned short GetNodeAttributeUShort(LPCTSTR xpath,LPCTSTR attrname, const unsigned short defVal = 0)
			{return GetSubNode(xpath).GetAttributeNode(attrname).GetUShort(defVal);}

			long GetNodeAttributeLong(LPCTSTR xpath,LPCTSTR attrname, const long defVal = 0)
			{return GetSubNode(xpath).GetAttributeNode(attrname).GetLong(defVal);}

			unsigned long GetNodeAttributeULong(LPCTSTR xpath,LPCTSTR attrname, const unsigned long defVal = 0)
			{return GetSubNode(xpath).GetAttributeNode(attrname).GetULong(defVal);}

			LONGLONG GetNodeAttributeLongLong(LPCTSTR xpath,LPCTSTR attrname, const LONGLONG  defVal = 0)
			{return GetSubNode(xpath).GetAttributeNode(attrname).GetLongLong(defVal);}

			LONGLONG GetNodeAttributeULongLong(LPCTSTR xpath,LPCTSTR attrname, const ULONGLONG  defVal = 0)
			{return GetSubNode(xpath).GetAttributeNode(attrname).GetULongLong(defVal);}


			XmlDocument GetOwnerDocument();
		};
		//The CXmlDocument class represents the top level of XML documents. 
		class CXmlDocument : public CXmlNode
		{
		private:
			//A smart pointer to IXMLDOMNodePtr.
			XmlDocument m_pDoc;
			CString		m_szUrl;
			bool        m_bFreeThread;
		public:
			//Constructors.
			//bFreeThread=false	:A boolean value indicating whether create a free-thread DOM.
			CXmlDocument(bool bFreeThread=false);
			CXmlDocument(const XmlDocument & pDoc) { m_pDoc = pDoc;m_pNode = pDoc;}
			CXmlDocument(const CXmlDocument & doc) { m_pDoc = doc.GetDocument();m_pNode = doc.GetNode();}
			virtual ~CXmlDocument(void);

			BOOL Initialize();
			void SetPreserveWhiteSpace(bool bPreserve);

			//Description	:Loads an XML document with the contents of the file at the specified location.
			//url			:String containing a URL that specifies the location of the XML file.
			//Return value	:Returns True if the load succeeded, and False if the load failed.
			bool Load(LPCTSTR url);
			CString GetNameSpaces();
			//Description	:Loads the supplied string into an XML document.
			//xmlString		:Containing the XML string to load into this XML document object. 
			//Return value	:True if the XML load succeeded. If the load failed, returns false.
			bool LoadXml(const _bstr_t &xmlString);
			bool LoadXml(LPCWSTR xmlString);
			bool LoadXml(LPCSTR xmlString);

			//Description	:Saves an XML document on its original location.
			//Return value	:True if succeeded, otherwise is false.
			bool Save(void);

			//Description	:Saves an XML document to the specified location.
			//filename		:Containing file name.
			//Return value	:True if succeeded, otherwise is false.
			bool SaveAs(LPCTSTR filename);

			//Description	:Stops download and parsing and discards any portion of the XML tree already built.
			//Return value	:None.
			void Abort(void);

			void Close();
			//Description	:
			//nodeXsl		:A valid XML document or DOM node that consists of XSLT elements that direct the transformation of this node. 
			//Return value	:A CString object containing the product of the transformation of this XML document based on the XSLT style sheet. 
			CString Transform(const CXmlNode &nodeXsl);
			bool TransformNodeToObject(CXmlDocument*pNode,CXmlDocument*pOutDOM);


			//Description	:Creates a node using the supplied type, name, and namespace.
			//name			:String containing the value for the new node's name.
			//nodeType		:Enumeration representing the type of new node
			//nameSpace		:String defining the namespace URI of new node
			//Return value	:Returns the newly created node.
			XmlNode CreateNode(const CString & name,enum NodeType nodeType=nodeElement,const CString & nameSpace=_T(""));

			//Description	:Creates an empty XmlFragment object.
			//Return value	:The newly empty XmlFragment
			XmlFragment CreateFragment(void) {return m_pDoc->_createDocumentFragment();}


			//Description	:Retrieves the root element of the document.
			//Return value	:A XmlNode(IXMLDOMNodePtr) representing the root of the XML document tree. 
			XmlNode	GetRoot(void) const {return m_pDoc->documentElement;}


			//Description	:Retrieves the URL for this XML document.
			//Return value	:CString containing the URL of XML document.
			CString GetUrl(void) {return (LPCTSTR)m_pDoc->url;}

			CString GetDocUrl(void) {return m_szUrl; }

			//Description	:Retrieves the readyState of this XML document.
			//Return value	:The value that indicates the instantiation and download state of the XML document object. It can be one of the following: 
			//LOADING		(1) 
			//LOADED		(2) 
			//INTERACTIVE	(3) 
			//COMPLETED		(4) 
			long	GetReadyState(void) {return m_pDoc->readyState;}

			//Description	:Retrieves the default properties.
			//propertyName	:String containing name of property.This name is case-sensitive.
			//Return value	:The string value of the property.
			CString GetProperty(const CString &propertyName);

			//Description	:Retrieves information about the last parsing error.
			//Return value	:A CString object containing information of last parsing error.
			CString GetParseError(void);

			//Description	:Sets second-level properties on the DOM object. 
			//propertyName	:The name of the property to be set.
			//propertyValue	:The value of the specified property. 
			//Return value	:None
			void	SetProperty(const CString &propertyName,const CString &propertyValue);

			//Description	:Specifies if asynchronous download is permitted.
			//Return value	:None
			void	SetAsync(bool bAsync){m_pDoc->async = bAsync;}

			XmlDocument	GetDocument(void) const {return m_pDoc;}
			// Operations
			CXmlDocument & operator= (const CXmlDocument & rhl) {this->m_pDoc=rhl.GetDocument();this->m_pNode=rhl.GetNode();return *this;}
			CXmlDocument & operator= (const XmlDocument & rhl) {this->m_pDoc=rhl;this->m_pNode=rhl;return *this;}

			//check if this document is null
			bool IsNull() {return (m_pDoc == NULL);}

			HRESULT	SetXmlDocument(VARIANT* pXmlDocument, bool& bLoginPage);
			HRESULT SetXmlDocument(VARIANT* pXmlDocument, bool& bLoginPage,bool bCheckLogin);
			HRESULT SetXmlDocument(VARIANT* pXmlDocument, bool& bLoginPage,bool bCheckLogin, bool& bErrXml);
			HRESULT SetXmlDocument(VARIANT* pXmlDocument);

			static LPCWSTR XmlStringAdapt(LPCWSTR xmldata);
			static LPCSTR XmlStringAdapt(LPCSTR xmldata);

			bool SetNodeAttribute(const CXmlNode& node, const CString& attributeName, const CString& value);
			HRESULT SetNodeValueEx(const CString& strPath, const CString& value,const CString& attributeName = _T(""));
			CString GetNodeValueEx(const CString& strPath, const CString& attributeName = _T(""),bool bGetXML=false);
			//bool SetNodeAttribute(const CXmlNode& node, const CString& attributeName, const _variant_t value);
		};
		
		//Description	:The CXmlNodeList class is wrapped operations on CXmlNode collection. It supports to indexed access.
		class CXmlNodeList
		{
		private:
			//A smart pointer to IXMLDOMNodeListPtr.
			XmlNodeList m_pNodeList;

		public:
			//Constructors
			CXmlNodeList(void){m_pNodeList=NULL;};
			CXmlNodeList(const XmlNodeList & pNodeList) {m_pNodeList=pNodeList;}
			CXmlNodeList(const CXmlNodeList & nodeList) {m_pNodeList=nodeList.GetNodeList();}
			virtual ~CXmlNodeList(void){};

			//Description	:Gets delimited string that concatenate node value
			//Return value	:A CString containing joined node value
			CString GetDelimitedString(const CString &delimiter);

			//Description	:Gets length of nodelist.
			//Return value	:Length of this nodelist.
			long GetLength(void);

			//Description	:Resets nodelist
			//Return value	:True if succeed, otherwise false.
			bool Reset(void);

			//Description	:Gets next node from node list
			//Return value	:XmlNode to the next node in the collection. Returns Null if there is no next node.
			XmlNode Next(void);

			//Description	:Random accesses node list number of elements
			//Return value	:Returns an XmlNode object. Returns NULL if the index is out of range.
			XmlNode GetItem(long index);
			
			//check if this object is null or has no node
			bool IsNull() {return (m_pNodeList == NULL);}
			bool IsEmpty() {return (m_pNodeList == NULL || m_pNodeList->length==0);}

			XmlNodeList GetNodeList(void) const {return m_pNodeList;}
			//Operators:
			CXmlNodeList & operator= (const CXmlNodeList & rhl) {this->m_pNodeList=rhl.GetNodeList();return *this;}
			CXmlNodeList & operator= (const XmlNodeList & rhl) {this->m_pNodeList=rhl;return *this;}
		};

		
		//Description	: The CXmlMap class provides methods that operate the collection of attribute nodes.
		class CXmlMap
		{
		private:
			//A smart pointer to IXMLDOMNamedNodeMapPtr.
			XmlMap m_pMap;
		public:
			//Constrcutors
			CXmlMap(void){m_pMap=NULL;};
			CXmlMap(const XmlMap & pMap) {m_pMap=pMap;}
			CXmlMap(const CXmlMap & map) {m_pMap=map.GetMap();}
			virtual ~CXmlMap(void){};

			//Description	:Gets length of collection.
			//Return value	:Length of this collection.
			long	GetLength(void) {return m_pMap->length;}

			//Description	:Retrieves the attribute with the specified name.
			//nodeName		:Name of node.
			//Return value	:CString containing text of specified item.
			CString GetNamedItem(LPCTSTR nodeName) {return (LPCTSTR)_bstr_t(m_pMap->_getNamedItem(nodeName)->nodeValue);}

			//Description	:Retrieves the node's value of specified item by index;
			//Return value	:Specified node's value in _variant_t form.
			_variant_t GetItemData(long index) {return m_pMap->Getitem(index)->nodeValue;}

			//Description	:Retrieves the node's value of specified item by index;
			//Return value	:Specified node's value in CString form.
			CString GetItemValue(long index) {return (LPCTSTR)_bstr_t(m_pMap->Getitem(index)->nodeValue);}

			//Description	:Random accesses node collection by index
			//Return value	:Returns an XmlNode object. Returns NULL if the index is out of range.
			XmlNode GetItem(long index) {return m_pMap->Getitem(index);}

			//Description	:Gets next node from node collection
			//Return value	:XmlNode to the next node in the collection. Returns Null if there is no next node.
			XmlNode GetNext(void) {return m_pMap->_nextNode();}

			//Description	:Resets CXmlMap
			//Return value	:True if succeed, otherwise false.
			void Reset(void){m_pMap->_reset();}

			//Description	:Assigns a new value to specified node.
			//name			:The name of the attribute to remove from the collection.
			//value			:The new value of specified node.
			//Return value	:None
			void SetValue(LPCTSTR name,LPCTSTR value) {m_pMap->_getNamedItem(name)->nodeValue = _bstr_t(value);}

			//Description	:Adds the supplied node to the collection.
			//node			:The new node containing the attribute to be added to the collection.
			//Return value	:None.
			void SetNode(const XmlNode &node) {m_pMap->_setNamedItem(node);}

			//Description	:Removes an attribute from the collection.
			//name			:The name of the attribute to remove from the collection.
			//Return value	:None.
			void RemoveNode(LPCTSTR name) {m_pMap->_removeNamedItem(_bstr_t(name));}

			//check if this object is null
			bool IsNull() {return (m_pMap==NULL);}

			//check if this object is empty
			bool IsEmpty() {return (m_pMap==NULL || m_pMap->length==0);}

			//Operators:
			XmlMap GetMap(void) const { return m_pMap;}
			CXmlMap & operator= (const CXmlMap & rhl) {this->m_pMap=rhl.GetMap();return *this;}
			CXmlMap & operator= (const XmlMap & rhl) {this->m_pMap=rhl;return *this;}
		};
		//Description	:The CXmlFragment class is a lightweight object that is useful for tree insert operations.
		class CXmlFragment : public CXmlNode
		{
		public:
			//Constructors:
			CXmlFragment(void){};
			CXmlFragment(const XmlFragment & pFragment) {m_pNode=pFragment;}
			CXmlFragment(const CXmlFragment & fragment) {m_pNode=fragment.GetFragment();}
			virtual ~CXmlFragment(void){};

			//Operators:
			XmlFragment GetFragment(void) const { return m_pNode;}
			CXmlFragment & operator= (const CXmlFragment & rhl) {this->m_pNode=rhl.GetFragment();return *this;}
			CXmlFragment & operator= (const XmlFragment & rhl) {this->m_pNode=rhl;return *this;}
		};
	}
}
