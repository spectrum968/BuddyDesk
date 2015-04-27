#include "stdafx.h"
#include "ThreadPoolExecutor.h"

CThreadPoolExecutor* CThreadPoolExecutor::m_pInstance = NULL;

CThreadPoolExecutor::CThreadPoolExecutor(void) :   
m_bRun(false),  
m_bEnableInsertTask(false)  
{  
    InitializeCriticalSection(&m_csTasksLock);  
    InitializeCriticalSection(&m_csThreadPoolLock);  
}
CThreadPoolExecutor::~CThreadPoolExecutor(void)  
{  
    Terminate();  
    DeleteCriticalSection(&m_csTasksLock);  
    DeleteCriticalSection(&m_csThreadPoolLock);  
}
CThreadPoolExecutor* CThreadPoolExecutor::GetIntance()
{
	if (m_pInstance == NULL)
		m_pInstance = new CThreadPoolExecutor();

	return m_pInstance;
}
bool CThreadPoolExecutor::Init(unsigned int minThreads, unsigned int maxThreads, unsigned int maxPendingTasks)  
{  
    if(minThreads == 0)  
    {  
        return false;  
    }  
    if(maxThreads < minThreads)  
    {  
        return false;  
    }  
    m_minThreads = minThreads;  
    m_maxThreads = maxThreads;  
    m_maxPendingTasks = maxPendingTasks;  
    unsigned int i = m_ThreadPool.size();  
    for(; i<minThreads; i++)  
    {  
        //创建线程  
        CWorker * pWorker = new CWorker(this);  
        if(NULL == pWorker)  
        {  
            return false;  
        }  
        EnterCriticalSection(&m_csThreadPoolLock);  
        m_ThreadPool.insert(pWorker);  
        LeaveCriticalSection(&m_csThreadPoolLock);  
        pWorker->Start();  
    }  
    m_bRun = true;  
    m_bEnableInsertTask = true;  
    return true;  
}
bool CThreadPoolExecutor::Execute(IRunnable * pRunnable)  
{  
    if(!m_bEnableInsertTask)  
    {  
        return false;  
    }  
    if(NULL == pRunnable)  
    {  
        return false;  
    }  
    if(m_Tasks.size() >= m_maxPendingTasks)  
    {  
        if(m_ThreadPool.size() < m_maxThreads)  
        {  
            CWorker * pWorker = new CWorker(this, pRunnable);  
            if(NULL == pWorker)  
            {  
                return false;  
            }  
            EnterCriticalSection(&m_csThreadPoolLock);  
            m_ThreadPool.insert(pWorker);  
            LeaveCriticalSection(&m_csThreadPoolLock);  
            pWorker->Start();  
        }  
        else  
        {  
            return false;  
        }  
    }  
    else  
    {  
        EnterCriticalSection(&m_csTasksLock);  
		m_Tasks.push_back((CThread*)pRunnable);  
        LeaveCriticalSection(&m_csTasksLock);  
    }  
    return true;  
}
IRunnable * CThreadPoolExecutor::GetTask()  
{  
    IRunnable * Task = NULL;  
    EnterCriticalSection(&m_csTasksLock);  
    if(!m_Tasks.empty())  
    {  
        Task = m_Tasks.front();  
        m_Tasks.pop_front();  
    }  
    LeaveCriticalSection(&m_csTasksLock);  
    return Task;  
} 
unsigned int CThreadPoolExecutor::GetThreadPoolSize()  
{  
    return m_ThreadPool.size();  
}
void CThreadPoolExecutor::Terminate()  
{  
    m_bEnableInsertTask = false;  
    while(m_Tasks.size() > 0)  
    {  
        Sleep(1);  
    }  
    m_bRun = false;  
    m_minThreads = 0;  
    m_maxThreads = 0;  
    m_maxPendingTasks = 0;  
    while(m_ThreadPool.size() > 0)  
    {  
        Sleep(1);  
    }  
    EnterCriticalSection(&m_csThreadPoolLock);  
    ThreadPoolItr itr = m_TrashThread.begin();  
    while(itr != m_TrashThread.end())  
    {  
        (*itr)->Join();  
        delete (*itr);  
        m_TrashThread.erase(itr);  
        itr = m_TrashThread.begin();  
    }  
    LeaveCriticalSection(&m_csThreadPoolLock);  
}  
