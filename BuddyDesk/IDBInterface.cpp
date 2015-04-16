#include "StdAfx.h"
#include "IDBInterface.h"
#include "Log.h"
#include <iostream>
using namespace std;
//tstring IDBInterface::m_strConnectString=_T("");
bool IDBInterface::bThreadRet=true;
bool IDBInterface::Connect()
{        
	bool bRet = false;
	CString strConnectString;
	GetProperties(_T("filedsn"), strConnectString);
	if (!strConnectString.IsEmpty())
	{
		strConnectString = _T("FileDSN=") + strConnectString;
	}
	else
	{
		GetProperties(_T("connectstring"), strConnectString);
		m_strConnectString=strConnectString;
	}
	//do 
	//{
		try
		{
			for (int i=0;i<m_conNum;i++)
			{
				m_vecConnectPtr[i]->Open(strConnectString.GetBuffer(),_T(""), _T(""),adConnectUnspecified);
				m_vecConnectPtr[i]->PutCommandTimeout(3000);
			}
			bRet = true;
		}
		catch (_com_error &e)
		{
			CString strLog;
			strLog.AppendFormat(strLog, _T("Can not connect to Database: %s\r\n Description:%s\r\n Connect String:%s"), e.ErrorMessage(), e.Description(), strConnectString);
			CLog::Log(_T("IDBInterface"), strLog);
		} 
	//} while (!bRet);
	return bRet;
}

bool IDBInterface::DisConnect()
{      
	bool bRet=true;
	
	CString strConnectString;
	GetProperties(_T("filedsn"), strConnectString);
	if (!strConnectString.IsEmpty())
	{
		strConnectString = _T("FileDSN=") + strConnectString;
	}
	else
	{
		GetProperties(_T("connectstring"), strConnectString);
	}
	CString strLog;
	strLog.Format(_T("Disconnect db: %s"), strConnectString);
	CLog::Log(_T("IDBInterface"), strLog);

	try
	{
		for (int i=0;i<m_conNum;i++)
		{
			if (m_vecConnectPtr[i]!=NULL)
			{
				m_vecConnectPtr[i].Release();
			}

		}
		if (m_pRecordset!=NULL)
		{
			m_pRecordset.Release();
		}
	}
	catch (_com_error &e)
	{
		CString strLog;
		strLog += _T("Failed to release connection ");
		strLog+=_T("\r\n  Connect String:")+m_strConnectString;
		strLog += e.ErrorMessage();

		CLog::Log(_T("IDBInterface"), strLog);
		bRet=false;
	} 
	catch (...)
	{
		CString strLog;
		strLog += _T("Failed to release connection ");
		strLog+=_T("\r\n  Connect String:")+m_strConnectString;
		CLog::Log(_T("IDBInterface"), strLog);
		bRet=false;
	}   
	return bRet;
}


bool IDBInterface::ExecuteNoneQuery(const CString& strSQL)
{
/*
	vector<CString> vecSQL;
	if(!strSQL.empty())
	{
		vecSQL.push_back(strSQL);
		return ExecuteNoneQuery(vecSQL);
	}
	return true;
*/
	bool bRet=true;
	CString strQuery = strSQL;
	try
	{        
		m_vecConnectPtr[0]->Execute(strQuery.GetBuffer(),NULL,adCmdText|adExecuteNoRecords);   
	}
	catch (_com_error &e)
	{
		CString strLog;
		strLog += _T("Failed to execute SQL: ");
		strLog += _T("\r\n ErrorMessage:");
		strLog +=e.ErrorMessage();
		strLog += _T("\r\n Description:");
		strLog.AppendFormat(_T("%s"), e.Description());
		strLog += _T("\r\n SQL String:") + strSQL;
		strLog+=_T("\r\n  Connect String:")+m_strConnectString;

		CLog::Log(_T("IDBInterface"), strLog);
		bRet=false;
	} 
	catch (...)
	{
		CString strLog;
		strLog += _T("Failed to execute SQL: ");         
		strLog += _T("\r\n SQL String:") + strSQL;
		strLog+=_T("\r\n  Connect String:")+m_strConnectString;

		CLog::Log(_T("IDBInterface"), strLog);
		bRet=false;
	}   
	return bRet;

}

bool IDBInterface::ExecuteNoneQuery(const vector<CString>& vecSQL)
{
	if(vecSQL.size()==0)
	{
		return true;
	}
	bThreadRet=true;
	int m_size=vecSQL.size();
	int splitsize=m_size/m_conNum;
	int m_curCount=0;
	int curbeg=0;
	int curend=splitsize+m_size%m_conNum;
	int tmpend=curbeg;
	HANDLE hThread[10];
	if (splitsize>0)
	{
		for (int i=0;i<m_conNum;i++)
		{
			threadparam* pPram;
			pPram =  new threadparam();
			pPram->pConnection=m_vecConnectPtr[i];
			pPram->pVecStr=&vecSQL;
			pPram->begPos=tmpend;            
			pPram->strConnectString=m_strConnectString;
			if (i==0)
			{
				pPram->endPos=splitsize+m_size%m_conNum;
			}
			else
			{
				pPram->endPos=tmpend+splitsize;
			}
			tmpend=pPram->endPos;
			hThread[i] = (HANDLE)_beginthreadex( NULL, 0, fun, pPram, 0, NULL );	
		}	
	}
	else
	{
		threadparam* pPram;
		pPram =  new threadparam();
		pPram->pConnection=m_vecConnectPtr[0];
		pPram->pVecStr=&vecSQL;
		pPram->begPos=curbeg;
		pPram->endPos=curend;
		pPram->strConnectString=m_strConnectString;
		hThread[0] = (HANDLE)_beginthreadex( NULL, 0, fun, pPram, 0, NULL );
	}
	if (splitsize>0)
	{
		WaitForMultipleObjects(m_conNum,hThread,TRUE,INFINITE);
		for (int i=0;i<m_conNum;i++)
		{
			CloseHandle( hThread[i]);
		}
	}
	else
	{
		WaitForSingleObject(hThread[0],INFINITE);
		CloseHandle( hThread[0]);
	}
	return bThreadRet;
}

unsigned  IDBInterface::fun(void* pParam)
{
	int m_curCount=0;
	if(pParam==NULL)
		return 0;
	threadparam* pthreadParam=(threadparam*)pParam;
	int curbeg=pthreadParam->begPos;
	int curend=pthreadParam->endPos;
	bool bHaveNetWorkError(false);
	while (true)
	{
		if (curbeg<curend)
		{
			try
			{
				CString strSql = (*(pthreadParam->pVecStr))[curbeg];
				pthreadParam->pConnection->Execute(strSql.GetBuffer(),NULL,adCmdText|adExecuteNoRecords); 			
			}
			catch (_com_error &e)
			{
				bThreadRet=false;
				CString strLog;
				strLog += _T("Failed to execute SQL: ");
				strLog += _T("\r\n ErrorCode:");
				CString errorcode;
				errorcode.Format(_T("%d"), (int)e.Error());
				strLog +=errorcode;
				strLog += _T("\r\n ErrorMessage:");
				strLog +=e.ErrorMessage();
				strLog += _T("\r\n Description:");
				strLog.AppendFormat(_T("%s"), e.Description());
				strLog += _T("\r\n SQL String:") + (*(pthreadParam->pVecStr))[curbeg];
				strLog+=_T("\r\n  Connect String:")+pthreadParam->strConnectString;

				CLog::Log(_T("IDBInterface"), strLog);
				if(strLog.Find(_T("network error"))!= -1 ||
					strLog.Find(_T("Connection failure"))!= -1)
				{
					for(int i=1;i<=3;++i)
					{
						bThreadRet=true;
						Sleep(i*1000);
						
						CString strRecLog;
						strRecLog.Format(_T("RECONNECT NOW,Meet network error, reconnect %d times..."), i);
						CLog::Log(_T("IDBInterface"), strRecLog);
						try
						{
							if(pthreadParam->pConnection->GetState()!=adStateClosed)
							{
								try
								{
									pthreadParam->pConnection->Close();
								}						
								catch (_com_error &e)
								{
									CString strCloseDBLog;
									strCloseDBLog += _T("Close Database Failed Before Reconnect: ");
									strCloseDBLog += _T("\r\n ErrorMessage:");
									strCloseDBLog += e.ErrorMessage();
									strCloseDBLog += _T("\r\n Description:");
									strCloseDBLog.AppendFormat(_T("%s"), e.Description());
									strCloseDBLog += _T("\r\n Connect String:") +  pthreadParam->strConnectString;

									CLog::Log(_T("IDBInterface"), strCloseDBLog);
									bThreadRet=false;
									continue;
								}
							}
							pthreadParam->pConnection->Open(pthreadParam->strConnectString.GetBuffer(),_T(""), _T(""),adConnectUnspecified);
							pthreadParam->pConnection->PutCommandTimeout(3000);
						}
						catch (_com_error &e)
						{
							CString strRecLog;
							strRecLog += _T("Reconnect to Database Failed: ");
							strRecLog += _T("\r\n ErrorMessage:");
							strRecLog += e.ErrorMessage();
							strRecLog += _T("\r\n Description:");
							strRecLog.AppendFormat(_T("%s"), e.Description());
							strRecLog += _T("\r\n Connect String:") + pthreadParam->strConnectString;

							CLog::Log(_T("IDBInterface"), strRecLog);
							bThreadRet=false;
							continue;
						}
						if(pthreadParam->pConnection->GetState()==adStateOpen)
						{
							CLog::Log(_T("IDBInterface"), _T("Reconnect to Database success,SQL will be ReExecuted\r\n"));
							try
							{
								CString strSql = (*(pthreadParam->pVecStr))[curbeg];
								pthreadParam->pConnection->Execute(strSql.GetBuffer(),NULL,adCmdText|adExecuteNoRecords); 							
							}
							catch(_com_error &e)
							{
								CString strFailLog;
								strFailLog += _T("Failed to execute SQL After Reconnect: ");
								strFailLog += _T("\r\n ErrorCode:");
								CString errorcode;
								errorcode.Format(_T("%d"), (int)e.Error());
								strFailLog +=errorcode;
								strFailLog += _T("\r\n ErrorMessage:");
								strFailLog +=e.ErrorMessage();
								strFailLog += _T("\r\n Description:");
								strFailLog.AppendFormat(_T("%s"), e.Description());
								strFailLog += _T("\r\n SQL String:") + (*(pthreadParam->pVecStr))[curbeg];
								strFailLog+=_T("\r\n  Connect String:")+pthreadParam->strConnectString;
								CLog::Log(_T("IDBInterface"), strFailLog);
								bThreadRet=false;
							}
							//exit reconnect									
							break;
						}
					}
				}
			}
			catch (...)
			{
				bThreadRet=false;
				CString strFailedLog;
				strFailedLog += _T("Failed to execute SQL: ");         
				strFailedLog += _T("\r\n SQL String:") + (*(pthreadParam->pVecStr))[curbeg];
				strFailedLog+=_T("\r\n  Connect String:")+pthreadParam->strConnectString;

				CLog::Log(_T("IDBInterface"), strFailedLog);
			}				
			curbeg++;			
		}
		else
		{
			break;
		}
	}
		//if(!bHaveNetWorkError)
		//{
		//	try
		//	{
		//		pthreadParam->pConnection->CommitTrans();
		//	}         
		//	catch (_com_error &e)
		//	{
		//		if (CanWriteLog(enLogError))
		//		{
		//			tstring strLog;
		//			strLog += _T("Failed to CommitTrans: ");
		//			strLog += e.ErrorMessage();
		//			strLog+=_T("\r\n  Connect String:")+pthreadParam->strConnectString;
		//			CImportLog::WriteLog(ERROR_IMPORTDB_SQLEXCEPTION, strLog, enLogError);
		//		}
		//		bThreadRet=false;

		//	} 
		//	catch (...)
		//	{
		//		if (CanWriteLog(enLogError))
		//		{
		//			tstring strLog;
		//			strLog += _T("Failed to CommitTrans: ");
		//			strLog+=_T("\r\n  Connect String:")+pthreadParam->strConnectString;
		//			CImportLog::WriteLog(ERROR_IMPORTDB_SQLEXCEPTION, strLog, enLogError);
		//		}	
		//		bThreadRet=false;
		//	}				
		//}
		////have NetWorkError
		//else
		//{
  //         for(int i=1;i<=3;++i)
		//	{
  //             Sleep(i*1000);
		//		if (CanWriteLog(enLogError))
		//		{
		//			tstring strLog;
		//			CTSStrUtil::Format(strLog, _T("Meet network error, reconnect %d..."),i);
		//			CImportLog::WriteLog(_T("RECONNECT NOW:"), strLog, enLogError);
		//		}
		//		try
		//		{
		//			pthreadParam->pConnection->Release();
		//		
		//			pthreadParam->pConnection->Open(pthreadParam->strConnectString.c_str(),_T(""), _T(""),adConnectUnspecified);
		//			pthreadParam->pConnection->PutCommandTimeout(3000);
		//		}
		//		catch (_com_error &e)
		//		{
		//			tstring strLog;
		//			strLog += _T("Reconnect to Database Failed: ");
		//			strLog += e.ErrorMessage();
		//			strLog += _T("\r\n Description:");
		//			strLog +=e.Description();
		//			strLog += _T("\r\n Connect String:") + pthreadParam->strConnectString;
		//			CImportLog::WriteLog(ERROR_IMPORTDB_SQLEXCEPTION, strLog, enLogError);
		//			continue;
		//		}
		//		if(pthreadParam->pConnection->GetState()&adStateOpen)
		//		{

		//			int curbeg=pthreadParam->begPos;
		//			int curend=pthreadParam->endPos;
		//			while(curbeg<curend)
		//			{
		//				try
		//				{
		//					pthreadParam->pConnection->Execute((*(pthreadParam->pVecStr))[curbeg].c_str(),NULL,adCmdText|adExecuteNoRecords); 							
		//				}
		//				catch(_com_error &e)
		//				{
		//					tstring strLog;
		//					strLog += _T("Failed to execute SQL After Reconnect: ");
		//					strLog += _T("\r\n ErrorCode:");
		//					tstring errorcode;
		//					CTSStrUtil::Format(errorcode,_T("%d"),(int)e.Error());
		//					strLog +=errorcode;
		//					strLog += _T("\r\n ErrorMessage:");
		//					strLog +=e.ErrorMessage();
		//					strLog += _T("\r\n Description:");
		//					strLog +=e.Description();
		//					strLog += _T("\r\n SQL String:") + (*(pthreadParam->pVecStr))[curbeg];
		//					strLog+=_T("\r\n  Connect String:")+pthreadParam->strConnectString;
		//					CImportLog::WriteLog(ERROR_IMPORTDB_SQLEXCEPTION, strLog, enLogError);
		//				}
		//				curbeg++;
		//			}
		//			//exit reconnect
		//			bThreadRet=true;
		//			break;
		//		}
		//    }
		//}
  //  }

	delete pthreadParam;
	_endthreadex( 0 );	
	return 0;

}

bool IDBInterface::ExecuteSQL( const CString& strSQL)
{
	bool bRet = true;
	CString strExecuteSQL = strSQL;
	try
	{
		m_pRecordset->Open(strExecuteSQL.GetBuffer(), _variant_t((IDispatch*)m_vecConnectPtr[0], true),adOpenDynamic, adLockOptimistic, adConnectUnspecified);
		//m_pRecordset->Open(strSQL.c_str(), _variant_t((IDispatch*)m_vecConnectPtr[0], true),adOpenStatic, adLockOptimistic, adConnectUnspecified);
	}
	catch (_com_error &e)
	{
		CString strLog;
		strLog += _T("Failed to execute SQL: ");
		strLog += _T("\r\n ErrorMessage:");
		strLog +=e.ErrorMessage();
		strLog += _T("\r\n Description:");
		strLog.AppendFormat(_T("%s"), e.Description());
		strLog += _T("\r\n SQL String:") + strExecuteSQL;
		strLog+=_T("\r\n  Connect String:")+m_strConnectString;
		CLog::Log(_T("IDBInterface"), strLog);
		if(strLog.Find(_T("network error"))!= -1 ||
			strLog.Find(_T("Connection failure"))!= -1)
		{
			int i=1;
			for(i;i<=3;++i)
			{
				bRet=true;
				Sleep(i*1000);
				CString strRecLog;
				strRecLog.Format(_T("RECONNECT NOW,Meet network error, reconnect %d times..."),i);
				CLog::Log(_T("IDBInterface"), strRecLog);
				if(m_vecConnectPtr[0]->GetState()!=adStateClosed)
				{
					try
					{
						m_vecConnectPtr[0]->Close();
					}
					catch (_com_error &e)
					{
						CString strCloseLog;
						strCloseLog += _T("Close Database Failed Before Reconnect: ");
						strCloseLog += _T("\r\n ErrorMessage:");
						strCloseLog += e.ErrorMessage();
						strCloseLog += _T("\r\n Description:");
						strCloseLog.AppendFormat(_T("%s"), e.Description());
						strCloseLog += _T("\r\n Connect String:") + m_strConnectString;
						
						CLog::Log(_T("IDBInterface"), strCloseLog);
						bRet=false;
						continue;
					}
				}
				//m_vecConnectPtr[0]->Release();
				try
				{
					m_vecConnectPtr[0]->Open(m_strConnectString.GetBuffer(),_T(""), _T(""),adConnectUnspecified);
					m_vecConnectPtr[0]->PutCommandTimeout(3000);
				}
				catch (_com_error &e)
				{
					CString strFailedLog;
					strFailedLog += _T("Reconnect to Database Failed: ");
					strFailedLog += _T("\r\n ErrorMessage:");
					strFailedLog += e.ErrorMessage();
					strFailedLog += _T("\r\n Description:");
					strFailedLog.AppendFormat(_T("%s"), e.Description());
					strFailedLog += _T("\r\n Connect String:") + m_strConnectString;
					
					CLog::Log(_T("IDBInterface"), strFailedLog);
					bRet=false;
					continue;
				}

			
				if(m_vecConnectPtr[0]->GetState()==adStateOpen)
				{
					CLog::Log(_T("IDBInterface"), _T("Reconnect to Database success,SQL will be ReExecuted\r\n"));
					try
					{
						m_pRecordset->Open(strExecuteSQL.GetBuffer(), _variant_t((IDispatch*)m_vecConnectPtr[0], true),adOpenDynamic, adLockOptimistic, adConnectUnspecified);
					}
					catch (_com_error &e)
					{
						CString strFailedLog;
						strFailedLog += _T("Failed to execute SQL After Reconnect: ");
						strFailedLog += _T("\r\n ErrorMessage:");
						strFailedLog +=e.ErrorMessage();
						strFailedLog += _T("\r\n Description:");
						strFailedLog.AppendFormat(_T("%s"), e.Description());
						strFailedLog += _T("\r\n SQL String:") + strExecuteSQL;
						strFailedLog+=_T("\r\n  Connect String:")+m_strConnectString;

						CLog::Log(_T("IDBInterface"), strFailedLog);
						bRet=false;
						continue ;
					}   
					break;
				}
			}  
			if(i==4)
			{
				CLog::Log(_T("IDBInterface"), _T("Reconnect 3 times Failed"));
				bRet=false;
			}
		}
		else
		{
			bRet= false;
		}	    
	} 
	catch (...)
	{
	   
		CString strLog;
		strLog += _T("Failed to execute SQL: ");
		strLog += _T("\r\n SQL String:") + strExecuteSQL;
		strLog+=_T("\r\n  Connect String:")+m_strConnectString;
		
		CLog::Log(_T("IDBInterface"), strLog);
		bRet= false;
	}
	return bRet;
}
