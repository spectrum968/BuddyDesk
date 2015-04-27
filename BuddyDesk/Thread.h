#pragma once
#include "Runnable.h"

class CThread :
	public IRunnable
{
private:
	explicit CThread(const CThread& t);

public:
	CThread(void);
	CThread(IRunnable* pRunable);
	CThread(const char* ThreadName, IRunnable* pRunnable=NULL);
	CThread(const CString& strThreadName, IRunnable* pRunnable=NULL);
	~CThread(void);

    /** 
      开始运行线程 
      @arg bSuspend 开始运行时是否挂起 
    **/  
    bool Start(bool bSuspend = false);

    /** 
      运行的线程函数，可以使用派生类重写此函数 
    **/  
    virtual void Run();

    /** 
      当前执行此函数线程等待线程结束 
      @arg timeout 等待超时时间，如果为负数，等待无限时长 
    **/  
    void Join(int timeout = -1);  
    /** 
      恢复挂起的线程 
    **/  
    void Resume();  
    /** 
      挂起线程 
    **/  
    void Suspend();  
    /** 
      终止线程的执行 
    **/  
    bool Terminate(unsigned long ExitCode);  
  
    unsigned int GetThreadID();  
    CString& GetThreadName();  
    void SetThreadName(const CString& strThreadName);  
    void SetThreadName(const char * ThreadName);  

private:  
    static unsigned int WINAPI StaticThreadFunc(void * arg);  
  
private:  
    HANDLE m_handle;
    IRunnable * const m_pRunnable;  
    unsigned int m_ThreadID;  
    CString m_strThreadName;  
    volatile bool m_bRun;  

};

