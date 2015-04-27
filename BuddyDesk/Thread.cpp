#include "stdafx.h"
#include "Thread.h"  
  
CThread::CThread(void) :   
m_pRunnable(NULL),  
m_bRun(false)  
{  
}  
  
CThread::~CThread(void)  
{  
}  
  
CThread::CThread(IRunnable * pRunnable) :   
m_strThreadName(""),  
m_pRunnable(pRunnable),  
m_bRun(false)  
{  
}  
  
CThread::CThread(const char * ThreadName, IRunnable * pRunnable) :   
m_strThreadName(ThreadName),  
m_pRunnable(pRunnable),  
m_bRun(false)  
{  
}  
  
CThread::CThread(const CString& strThreadName, IRunnable * pRunnable) :   
m_strThreadName(strThreadName),  
m_pRunnable(pRunnable),  
m_bRun(false)  
{  
}  
  
bool CThread::Start(bool bSuspend)  
{  
    if(m_bRun)  
    {  
        return true;  
    }  
    if(bSuspend)  
    {  
        m_handle = (HANDLE)_beginthreadex(NULL, 0, StaticThreadFunc, this, CREATE_SUSPENDED, &m_ThreadID);  
    }  
    else  
    {  
        m_handle = (HANDLE)_beginthreadex(NULL, 0, StaticThreadFunc, this, 0, &m_ThreadID);  
    }  
    m_bRun = (NULL != m_handle);  
    return m_bRun;  
}  
  
void CThread::Run()  
{  
    if(!m_bRun)  
    {  
        return;  
    }  
    if(NULL != m_pRunnable)  
    {  
        m_pRunnable->Run();  
    }  
    m_bRun = false;  
}  
  
void CThread::Join(int timeout)  
{  
    if(NULL == m_handle || !m_bRun)  
    {  
        return;  
    }  
    if(timeout <= 0)  
    {  
        timeout = INFINITE;  
    }  
    ::WaitForSingleObject(m_handle, timeout);  
}  
  
void CThread::Resume()  
{  
    if(NULL == m_handle || !m_bRun)  
    {  
        return;  
    }  
    ::ResumeThread(m_handle);  
}  
  
void CThread::Suspend()  
{  
    if(NULL == m_handle || !m_bRun)  
    {  
        return;  
    }  
    ::SuspendThread(m_handle);  
}  
  
bool CThread::Terminate(unsigned long ExitCode)  
{  
    if(NULL == m_handle || !m_bRun)  
    {  
        return true;  
    }  
    if(::TerminateThread(m_handle, ExitCode))  
    {  
        ::CloseHandle(m_handle);  
        return true;  
    }  
    return false;  
}  
  
unsigned int CThread::GetThreadID()  
{  
    return m_ThreadID;  
}  
  
CString& CThread::GetThreadName()  
{  
    return m_strThreadName;  
}  
  
void CThread::SetThreadName(const CString& ThreadName)  
{  
    m_strThreadName = ThreadName;  
}  
  
void CThread::SetThreadName(const char * ThreadName)  
{  
    if(NULL == ThreadName)  
    {  
        m_strThreadName = _T("");  
    }  
    else  
    {  
        m_strThreadName = ThreadName;  
    }  
}  
  
unsigned int CThread::StaticThreadFunc(void * arg)  
{  
    CThread * pThread = (CThread *)arg;  
    pThread->Run();  
    return 0;  
}  
