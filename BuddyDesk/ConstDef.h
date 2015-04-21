#pragma once
#include "stdafx.h"

const CString cst_CONFIG_FILE		= _T("BuddyConfig.xml");
const CString cst_EXECUTE_PATH		= _T("execute_path");
const CString cst_TDX_EXPORT_PATH	= _T("export_data_path");
const CString cst_DATA_PATH			= _T("data_path");
const CString cst_SQL_SERVER		= _T("sql_server");
const CString cst_SQL_DB			= _T("sql_db");
const CString cst_TDX_PATH			= _T("tdx_path");

const CString cst_DATA_FOLDER		= _T("vipdoc");
const CString cst_SH				= _T("sh");
const CString cst_SZ				= _T("sz");
const CString cst_DAY_FOLDER		= _T("lday");
const CString cst_DAY_FILE			= _T(".day");
const CString cst_5MIN_FOLDER		= _T("fzline");
const CString cst_5MIN_FILE			= _T(".lc5");
const CString cst_1MIN_FOLDER		= _T("minline");
const CString cst_1MIN_FILE			= _T(".lc1");

enum QuoteType
{
	Undefined = -1,
	ByTrade,
	Min1,
	Min5,
	Min15,
	Min30,
	Min60,
	Day,
	Day5,
	Month,
	HalfYear,
	Year,
};

enum MarketType
{
	Unknown = -1,
	Shanghai,
	Shenzhen,
};

enum StickType
{
	OriginStick = 0,
	MergedStick = 1,
};