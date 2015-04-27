#pragma once
#ifndef __THREAD_POOL_EXECUTOR__  
#define __THREAD_POOL_EXECUTOR__ 

#include "Thread.h"  
#include <set>  
#include <list>  
#include <windows.h>
#include "Worker.h"

class CWorker;

typedef set<CWorker*> ThreadPool;
typedef set<CWorker*>::iterator ThreadPoolItr;
typedef list<CThread*> Tasks; 
typedef list<CThread*>::iterator TaskItr;

class CThreadPoolExecutor
{
private:
	CThreadPoolExecutor(void);
	
public:
	~CThreadPoolExecutor(void);
	static CThreadPoolExecutor* GetIntance();

    /** 
      初始化线程池，创建minThreads个线程 
    **/  
    bool Init(unsigned int minThreads, unsigned int maxThreads, unsigned int maxPendingTaskse);

    /** 
      执行任务，若当前任务列表没有满，将此任务插入到任务列表，返回true 
      若当前任务列表满了，但当前线程数量小于最大线程数，将创建新线程执行此任务，返回true 
      若当前任务列表满了，但当前线程数量等于最大线程数，将丢弃此任务，返回false 
    **/  
    bool Execute(IRunnable * pRunnable);  

    /** 
      终止线程池，先制止塞入任务， 
      然后等待直到任务列表为空， 
      然后设置最小线程数量为0， 
      等待直到线程数量为空， 
      清空垃圾堆中的任务 
    **/  
    void Terminate();  

    /** 
      返回线程池中当前的线程数量 
    **/  
    unsigned int GetThreadPoolSize();  

	/** 
      获取任务列表中的任务，若任务列表为空，返回NULL 
    **/
	IRunnable * GetTask();

public:
	CRITICAL_SECTION& GetTaskLock() {return m_csTasksLock;}
	CRITICAL_SECTION& GetThreadPoolLock() {return m_csThreadPoolLock;}
	bool IsRun() {return m_bRun;}
	bool CanInsertTask() {return m_bEnableInsertTask;}
	unsigned int GetMinThreads() {return m_minThreads;}
	unsigned int GetMaxThreads() {return m_maxThreads;}
	unsigned int GetMaxPendingTasks() {return m_maxPendingTasks;}
	ThreadPool& GetThreadPool() {return m_ThreadPool;}
	ThreadPool& GetTrashThread() {return m_ThreadPool;}
	Tasks& GetTasks() {return m_Tasks;}

private:
    static unsigned int WINAPI StaticThreadFunc(void * arg);

private:
	static CThreadPoolExecutor* m_pInstance;

	ThreadPool m_ThreadPool;
	ThreadPool m_TrashThread;
	Tasks m_Tasks;

	CRITICAL_SECTION m_csTasksLock;
	CRITICAL_SECTION m_csThreadPoolLock;

	volatile bool m_bRun;
	volatile bool m_bEnableInsertTask;
    volatile unsigned int m_minThreads;  
    volatile unsigned int m_maxThreads;  
    volatile unsigned int m_maxPendingTasks;  
};

#endif //__THREAD_POOL_EXECUTOR__