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
      ��ʼ�����߳� 
      @arg bSuspend ��ʼ����ʱ�Ƿ���� 
    **/  
    bool Start(bool bSuspend = false);

    /** 
      ���е��̺߳���������ʹ����������д�˺��� 
    **/  
    virtual void Run();

    /** 
      ��ǰִ�д˺����̵߳ȴ��߳̽��� 
      @arg timeout �ȴ���ʱʱ�䣬���Ϊ�������ȴ�����ʱ�� 
    **/  
    void Join(int timeout = -1);  
    /** 
      �ָ�������߳� 
    **/  
    void Resume();  
    /** 
      �����߳� 
    **/  
    void Suspend();  
    /** 
      ��ֹ�̵߳�ִ�� 
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

