#include "StdAfx.h"
#include "Litmt.h"

namespace Buddy {

//synchronous object base class
CLitSyncObject::CLitSyncObject(LPCTSTR pstrName, enmSyncObjectType objectType) : m_strName(pstrName), m_objectType(objectType)
{
	m_hObject = NULL;
}

CLitSyncObject::~CLitSyncObject()
{
	if (m_hObject != NULL)
	{
		::CloseHandle(m_hObject);
		m_hObject = NULL;
	}
}

BOOL CLitSyncObject::Lock(DWORD dwTimeout)
{
	DWORD dwRet = ::WaitForSingleObject(m_hObject, dwTimeout);
	if (dwRet == WAIT_OBJECT_0 || dwRet == WAIT_ABANDONED)
		return TRUE;
	else
		return FALSE;
}

//class CLitSemaphore
CLitSemaphore::CLitSemaphore(LONG lInitialCount, LONG lMaxCount,
	LPCTSTR pstrName, LPSECURITY_ATTRIBUTES lpsaAttributes)
	:  CLitSyncObject(pstrName, enmSyncObjectSemaphore)
{
	ATLASSERT(lMaxCount > 0);
	ATLASSERT(lInitialCount <= lMaxCount);

	m_hObject = ::CreateSemaphore(lpsaAttributes, lInitialCount, lMaxCount,
		pstrName);
	if (m_hObject == NULL)
	{
		RaiseError(cstMTErrorCode,_T("create Semaphore object failed."));
	}
}

CLitSemaphore::~CLitSemaphore()
{
}

BOOL CLitSemaphore::Unlock(LONG lCount, LPLONG lpPrevCount)
{
	return ::ReleaseSemaphore(m_hObject, lCount, lpPrevCount);
}

// CLitMutex
CLitMutex::CLitMutex() : CLitSyncObject(NULL, enmSyncObjectMutex)
{
	m_bUsedhandle = FALSE;
	m_hObject = ::CreateMutex(NULL, FALSE, NULL);
	if (m_hObject == NULL)
	{
		RaiseError(cstMTErrorCode,_T("create Mutex object failed."));
	}
}

CLitMutex::CLitMutex(BOOL bInitiallyOwn, LPCTSTR pstrName,
	LPSECURITY_ATTRIBUTES lpsaAttribute)
	: CLitSyncObject(pstrName, enmSyncObjectMutex)
{
	m_bUsedhandle = FALSE;
	m_hObject = ::CreateMutex(lpsaAttribute, bInitiallyOwn, pstrName);
	if (m_hObject == NULL)
	{
		RaiseError(cstMTErrorCode,_T("create Mutex object failed."));
	}

	//judgement if the handle is a used handle
	if(pstrName)
	{
		if(::GetLastError() == ERROR_ALREADY_EXISTS)
		{
			m_bUsedhandle = TRUE;
		}
	}
}

CLitMutex::~CLitMutex()
{
}

BOOL CLitMutex::Unlock()
{
	return ::ReleaseMutex(m_hObject);
}

// CLitEvent
CLitEvent::CLitEvent(BOOL bInitiallyOwn, BOOL bManualReset, LPCTSTR pstrName,
	LPSECURITY_ATTRIBUTES lpsaAttribute)
	: CLitSyncObject(pstrName, enmSyncObjectEvent)
{
	m_hObject = ::CreateEvent(lpsaAttribute, bManualReset,
		bInitiallyOwn, pstrName);
	if (m_hObject == NULL)
	{
		RaiseError(cstMTErrorCode,_T("create Event object failed."));
	}
}

CLitEvent::~CLitEvent()
{
}

// CLitSingleLock

CLitSingleLock::CLitSingleLock(CLitSyncObject* pObject, BOOL bInitialLock, LPCTSTR strFunction)
{
	ATLASSERT(pObject != NULL);

	if(pObject == NULL)
	{
		RaiseError(cstMTErrorCode,_T("argument error."));
	}
#ifdef _ENABLE_DEBUG_LOCK		
	if(strFunction) m_strLockFunction = strFunction;
#endif
	m_pObject = pObject;
	m_hObject = pObject->m_hObject;
	m_bAcquired = FALSE;

	if (bInitialLock)
		Lock();
}

BOOL CLitSingleLock::Lock(DWORD dwTimeOut)
{
	ATLASSERT(m_pObject != NULL || m_hObject != NULL);
	ATLASSERT(!m_bAcquired);
#ifdef _ENABLE_DEBUG_LOCK
	if(!m_strLockFunction.IsEmpty())
		AtlTrace(_T("begin to lock for %s at %s with object %08X\n"), m_strLockFunction,COleDateTime::GetCurrentTime().Format(_T("%M:%S")),this);
#endif
	m_bAcquired = m_pObject->Lock(dwTimeOut);
	return m_bAcquired;
}

BOOL CLitSingleLock::Unlock()
{
	ATLASSERT(m_pObject != NULL);
	if (m_bAcquired)
		m_bAcquired = !m_pObject->Unlock();

	// successfully unlocking means it isn't acquired
#ifdef _ENABLE_DEBUG_LOCK
	if(!m_strLockFunction.IsEmpty())
	AtlTrace(_T("end to lock for %s at %s with object %08X\n"), m_strLockFunction,COleDateTime::GetCurrentTime().Format(_T("%M:%S")),this);
#endif
	return !m_bAcquired;
}

BOOL CLitSingleLock::Unlock(LONG lCount, LPLONG lpPrevCount /* = NULL */)
{
	ATLASSERT(m_pObject != NULL);
	if (m_bAcquired)
		m_bAcquired = !m_pObject->Unlock(lCount, lpPrevCount);

	// successfully unlocking means it isn't acquired
#ifdef _ENABLE_DEBUG_LOCK
	AtlTrace(_T("end to lock for %s\n"), m_strLockFunction);
#endif
	return !m_bAcquired;
}

// CLitMultiLock
CLitMultiLock::CLitMultiLock(CLitSyncObject* pObjects[], DWORD dwCount,
	BOOL bInitialLock)
{
	ATLASSERT(dwCount > 0 && dwCount <= MAXIMUM_WAIT_OBJECTS);
	ATLASSERT(pObjects != NULL);
	
	if(pObjects == NULL)
	{
		RaiseError(cstMTErrorCode,_T("argument error."));
	}
		
	m_ppObjectArray = pObjects;
	m_dwCount = dwCount;

	// as an optimization, skip allocating array if
	// we can use a small, preallocated bunch of handles

	if (m_dwCount > _countof(m_hPreallocated))
	{
		ATL::CAutoVectorPtr<HANDLE> spHandleArray(new HANDLE[m_dwCount]);
		ATL::CAutoVectorPtr<BOOL> spLockedArray(new BOOL[m_dwCount]);
		m_pHandleArray = spHandleArray.Detach();
		m_bLockedArray = spLockedArray.Detach();
	}
	else
	{
		m_pHandleArray = m_hPreallocated;
		m_bLockedArray = m_bPreallocated;
	}

	// get list of handles from array of objects passed
	for (DWORD i = 0; i <m_dwCount; i++)
	{
		if(pObjects[i] == NULL)
		{
			RaiseError(cstMTErrorCode,_T("argument error."));
		}
			
		// can't wait for critical sections
		ATLASSERT(pObjects[i]->m_objectType != enmSyncObjectCriticalSection);

		m_pHandleArray[i] = pObjects[i]->m_hObject;
		m_bLockedArray[i] = FALSE;
	}

	if (bInitialLock)
		Lock();
}

CLitMultiLock::~CLitMultiLock()
{
	Unlock();
	if (m_pHandleArray != m_hPreallocated)
	{
		delete[] m_bLockedArray;
		delete[] m_pHandleArray;
	}
}

DWORD CLitMultiLock::Lock(DWORD dwTimeOut ,	BOOL bWaitForAll , DWORD dwWakeMask)
{
	DWORD dwResult;
	if (dwWakeMask == 0)
		dwResult = ::WaitForMultipleObjects(m_dwCount,
			m_pHandleArray, bWaitForAll, dwTimeOut);
	else
		dwResult = ::MsgWaitForMultipleObjects(m_dwCount,
			m_pHandleArray, bWaitForAll, dwTimeOut, dwWakeMask);

	DWORD dwUpperBound = (DWORD)WAIT_OBJECT_0 + m_dwCount;
	if (dwResult >= WAIT_OBJECT_0 && dwResult < dwUpperBound)
	{
		if (dwUpperBound >= m_dwCount && dwUpperBound >= WAIT_OBJECT_0)
		{
			if (bWaitForAll)
			{
				for (DWORD i = 0; i < m_dwCount; i++)
					m_bLockedArray[i] = TRUE;
			}
			else
			{
				ATLASSERT((dwResult >= WAIT_OBJECT_0) && ((dwResult - WAIT_OBJECT_0) <= dwResult));
				if ((dwResult >= WAIT_OBJECT_0) && ((dwResult - WAIT_OBJECT_0) <= dwResult))
					m_bLockedArray[dwResult - WAIT_OBJECT_0] = TRUE;
			}
		}
	}
	return dwResult;
}

BOOL CLitMultiLock::Unlock()
{
	for (DWORD i=0; i < m_dwCount; i++)
	{
		if (m_bLockedArray[i])
			m_bLockedArray[i] = !m_ppObjectArray[i]->Unlock();
	}
	return TRUE;
}

BOOL CLitMultiLock::Unlock(LONG lCount, LPLONG lpPrevCount /* =NULL */)
{
	BOOL bGotOne = FALSE;
	for (DWORD i=0; i < m_dwCount; i++)
	{
		if (m_bLockedArray[i])
		{
			CLitSemaphore* pSemaphore = static_cast<CLitSemaphore *>(m_ppObjectArray[i]);
			if (pSemaphore != NULL)
			{
				bGotOne = TRUE;
				m_bLockedArray[i] = !m_ppObjectArray[i]->Unlock(lCount, lpPrevCount);
			}
		}
	}

	return bGotOne;
}

} //end namespace