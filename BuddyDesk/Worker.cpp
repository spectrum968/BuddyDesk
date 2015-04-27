#include "stdafx.h"
#include "Worker.h"


CWorker::CWorker(CThreadPoolExecutor * pThreadPool, IRunnable * pFirstTask) :
	m_pThreadPool(pThreadPool),
	m_pFirstTask(pFirstTask),
	m_bRun(true)
{
}


CWorker::~CWorker(void)
{
}

/** 
  执行任务的工作线程。 
  当前没有任务时， 
  如果当前线程数量大于最小线程数量，减少线程， 
  否则，执行清理程序，将线程类给释放掉 
**/  
void CWorker::Run()  
{  
    IRunnable * pTask = NULL;  
    while(m_bRun)  
    {  
        if(NULL == m_pFirstTask)  
        {  
            pTask = m_pThreadPool->GetTask();  
        }  
        else  
        {  
            pTask = m_pFirstTask;  
            m_pFirstTask = NULL;  
        }  
  
        if(NULL == pTask)  
        {  
			EnterCriticalSection(&(m_pThreadPool->GetThreadPoolLock()));  
			if(m_pThreadPool->GetThreadPoolSize() > m_pThreadPool->GetMinThreads())  
            {  
				ThreadPoolItr itr = m_pThreadPool->GetThreadPool().find(this);  
				if(itr != m_pThreadPool->GetThreadPool().end())  
                {  
					m_pThreadPool->GetThreadPool().erase(itr);  
					m_pThreadPool->GetThreadPool().insert(this);  
                }  
                m_bRun = false;  
            }  
            else  
            {  
				ThreadPoolItr itr = m_pThreadPool->GetTrashThread().begin();  
				while(itr != m_pThreadPool->GetTrashThread().end())  
                {  
                    (*itr)->Join();  
                    delete (*itr);  
					m_pThreadPool->GetTrashThread().erase(itr);  
					itr = m_pThreadPool->GetTrashThread().begin();  
                }  
            }  
			LeaveCriticalSection(&(m_pThreadPool->GetThreadPoolLock()));  
            continue;  
        }  
        else  
        {  
            pTask->Run();  
            pTask = NULL;  
        }  
    }  
}  
