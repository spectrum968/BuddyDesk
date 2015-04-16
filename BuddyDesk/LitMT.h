#pragma once

#if 0
#ifndef _RAISEERROR_T
#define __TFILE__ _T( ## __FILE__ ## )
#define RaiseError(ec, ex) {AtlTrace(_T("exception at %d in %s since [%d] %s\n"), __LINE__, __TFILE__, ec, ex);throw ec;}
#endif
#endif
#include "atlcommon.h"
namespace Buddy { 

#ifndef _ENABLE_DEBUG_LOCK
//#define _ENABLE_DEBUG_LOCK
#endif

class CLitSyncObject;
class CLitSemaphore;
class CLitMutex;
class CLitEvent;
class CLitCriticalSection;

class CLitSingleLock;
class CLitMultiLock;

const DWORD cstMTErrorCode = 80021;

enum enmSyncObjectType
{
	enmSyncObjectEvent,
	enmSyncObjectSemaphore,
	enmSyncObjectMutex,
	enmSyncObjectCriticalSection,
};

class CLitSyncObject
{
// Constructor
public:
	explicit CLitSyncObject(LPCTSTR pstrName, enmSyncObjectType objectType);

// Attributes
public:
	operator HANDLE() const { return m_hObject;}
	HANDLE  m_hObject;

// Operations
	virtual BOOL Lock(DWORD dwTimeout = INFINITE);
	virtual BOOL Unlock() = 0;
	virtual BOOL Unlock(LONG /* lCount */, LPLONG /* lpPrevCount=NULL */) { return TRUE; }

// Implementation
public:
	virtual ~CLitSyncObject();

	friend class CLitSingleLock;
	friend class CLitMultiLock;
protected:
	CString m_strName;
	enmSyncObjectType m_objectType;
};

class CLitSemaphore : public CLitSyncObject
{
public:
	// Constructor
	/* explicit */ 
	CLitSemaphore(LONG lInitialCount = 1, LONG lMaxCount = 1,
		LPCTSTR pstrName=NULL, LPSECURITY_ATTRIBUTES lpsaAttributes = NULL);

// Implementation
public:
	virtual ~CLitSemaphore();
	virtual BOOL Unlock() { return Unlock(1, NULL); }
	virtual BOOL Unlock(LONG lCount, LPLONG lprevCount = NULL);
};

class CLitMutex : public CLitSyncObject
{
public:
	// Constructor
	CLitMutex();
	CLitMutex(BOOL bInitiallyOwn, LPCTSTR lpszName, LPSECURITY_ATTRIBUTES lpsaAttribute = NULL);

// Implementation
public:
	virtual ~CLitMutex();
	BOOL Unlock();
	BOOL IsAlreadyExist() const {return m_bUsedhandle;}
private:
	BOOL m_bUsedhandle;
};

class CLitEvent : public CLitSyncObject
{
private:
   using CLitSyncObject::Unlock;

public:
	//Constructor
	/* explicit */ 
	CLitEvent(BOOL bInitiallyOwn = FALSE, BOOL bManualReset = FALSE,
		LPCTSTR lpszNAme = NULL, LPSECURITY_ATTRIBUTES lpsaAttribute = NULL);

// Operations
public:
	BOOL SetEvent() { ATLASSERT(m_hObject != NULL); return ::SetEvent(m_hObject); }
	BOOL PulseEvent() { ATLASSERT(m_hObject != NULL); return ::PulseEvent(m_hObject); }
	BOOL ResetEvent() { ATLASSERT(m_hObject != NULL); return ::ResetEvent(m_hObject); }
	BOOL Unlock() {return TRUE;}

// Implementation
public:
	virtual ~CLitEvent();
};

class CLitCriticalSection : public CLitSyncObject
{
private:
   using CLitSyncObject::Unlock;

// Constructor
public:
	CLitCriticalSection() : CLitSyncObject(NULL, enmSyncObjectCriticalSection)
	{
		BOOL bSuccess;

		bSuccess = Init();
		if (!bSuccess)
		{
			RaiseError(cstMTErrorCode,_T("create CriticalSection object failed."));
		}
	}

// Attributes
public:
	operator CRITICAL_SECTION*() { return (CRITICAL_SECTION*) &m_sect; }

	CRITICAL_SECTION m_sect;

// Operations
public:
	BOOL Unlock() { ::LeaveCriticalSection(&m_sect); return TRUE; }
	BOOL Lock()
	{	
		__try
		{
			::EnterCriticalSection(&m_sect); 
		}
		__except(STATUS_NO_MEMORY == GetExceptionCode())
		{
			RaiseError(cstMTErrorCode,_T("create lock CriticalSection object failed."));
		}
		return TRUE; 
	}

	BOOL Lock(DWORD dwTimeout) { ATLASSERT(dwTimeout == INFINITE); (void)dwTimeout; return Lock(); }

// Implementation
public:
	virtual ~CLitCriticalSection() { ::DeleteCriticalSection(&m_sect); }

private:
	BOOL Init()
	{
		__try
		{
			::InitializeCriticalSection(&m_sect);
		}
		__except(STATUS_NO_MEMORY == GetExceptionCode())
		{
			return FALSE;
		}

		return TRUE;
	}
};

class CLitSingleLock
{
// Constructors
public:
	explicit CLitSingleLock(CLitSyncObject* pObject, BOOL bInitialLock = FALSE, LPCTSTR strFunction = NULL);

// Operations
public:
	BOOL Lock(DWORD dwTimeOut = INFINITE);
	BOOL Unlock();
	BOOL Unlock(LONG lCount, LPLONG lPrevCount = NULL);
	BOOL IsLocked() { return m_bAcquired; }

// Implementation
public:
	~CLitSingleLock() { Unlock(); }

protected:
	CLitSyncObject* m_pObject;
	HANDLE  m_hObject;
	BOOL    m_bAcquired;
	CString m_strLockFunction;
};

/////////////////////////////////////////////////////////////////////////////
// CMultiLock

class CLitMultiLock
{
// Constructor
public:
	CLitMultiLock(CLitSyncObject* ppObjects[], DWORD dwCount, BOOL bInitialLock = FALSE);

// Operations
public:
	DWORD Lock(DWORD dwTimeOut = INFINITE, BOOL bWaitForAll = TRUE,
		DWORD dwWakeMask = 0);
	BOOL Unlock();
	BOOL Unlock(LONG lCount, LPLONG lPrevCount = NULL);
	BOOL IsLocked(DWORD dwObject)
	{ 
		ATLASSERT(dwObject < m_dwCount);
		return m_bLockedArray[dwObject]; 
	}

// Implementation
public:
	~CLitMultiLock();

protected:
	HANDLE  m_hPreallocated[8];
	BOOL    m_bPreallocated[8];

	CLitSyncObject* const * m_ppObjectArray;
	HANDLE* m_pHandleArray;
	BOOL*   m_bLockedArray;
	DWORD   m_dwCount;
};

};	//end namespace