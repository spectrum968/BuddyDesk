#pragma once

//for all atl COM project reference

#include <atlbase.h>
#include <atlcom.h>
#include <atlwin.h>
#include <atltypes.h>
#include <atlctl.h>
#include <atlhost.h>
#include <atlutil.h>
#include <atlcomtime.h>
#include <comdef.h>
#include <comutil.h>

using namespace ATL;

typedef CComCritSecLock<CComAutoCriticalSection> AutoCriticalSessionLock;

#ifndef _RAISEERROR_T
#define _RAISEERROR_T
#define __TFILE__ _T( ## __FILE__ ## )
#define RaiseError(ec, ex) {AtlTrace(_T("exception at %d in %s since [%d] %s\n"), __LINE__, __TFILE__, ec, ex);throw (long)ec;}
#endif

