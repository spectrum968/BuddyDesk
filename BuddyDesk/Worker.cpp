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
  ִ������Ĺ����̡߳� 
  ��ǰû������ʱ�� 
  �����ǰ�߳�����������С�߳������������̣߳� 
  ����ִ��������򣬽��߳�����ͷŵ� 
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
