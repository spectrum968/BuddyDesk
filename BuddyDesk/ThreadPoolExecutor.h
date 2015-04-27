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
      ��ʼ���̳߳أ�����minThreads���߳� 
    **/  
    bool Init(unsigned int minThreads, unsigned int maxThreads, unsigned int maxPendingTaskse);

    /** 
      ִ����������ǰ�����б�û����������������뵽�����б�����true 
      ����ǰ�����б����ˣ�����ǰ�߳�����С������߳��������������߳�ִ�д����񣬷���true 
      ����ǰ�����б����ˣ�����ǰ�߳�������������߳����������������񣬷���false 
    **/  
    bool Execute(IRunnable * pRunnable);  

    /** 
      ��ֹ�̳߳أ�����ֹ�������� 
      Ȼ��ȴ�ֱ�������б�Ϊ�գ� 
      Ȼ��������С�߳�����Ϊ0�� 
      �ȴ�ֱ���߳�����Ϊ�գ� 
      ����������е����� 
    **/  
    void Terminate();  

    /** 
      �����̳߳��е�ǰ���߳����� 
    **/  
    unsigned int GetThreadPoolSize();  

	/** 
      ��ȡ�����б��е������������б�Ϊ�գ�����NULL 
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