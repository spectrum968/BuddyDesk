#pragma once
#ifndef __WORKER__
#define __WORKER__

#include "ThreadPoolExecutor.h"

class CThreadPoolExecutor;

class CWorker :
	public CThread
{
public:
	 CWorker(CThreadPoolExecutor* pThreadPool, IRunnable * pFirstTask = NULL);
	~CWorker(void);
	void Run();

    private:  
        CThreadPoolExecutor * m_pThreadPool;  
        IRunnable * m_pFirstTask;  
        volatile bool m_bRun;  
};


#endif //__WORKER

