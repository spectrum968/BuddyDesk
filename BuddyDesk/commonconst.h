/*
	this file define consts used for varies project, not include any project special const define
*/
#pragma once

#include <limits>
namespace CommonConst {

const double  cstNullDouble = std::numeric_limits<double>::infinity();
const long    cstNullLong   = std::numeric_limits<long>::infinity();//Todo: In fact, no infinity for integer or long type. infinity() will return 0.
const double  cstDoubleNaN = std::numeric_limits<double>::quiet_NaN();
const CString cstEmptyStr   = _T("");

const CString cstUSDateFmt   = _T("%Y-%m-%d");

//timeout timesoan define for cache
const COleDateTimeSpan cstCacheTimeout(0,8,0,0);
const COleDateTimeSpan cstCacheTimeout1Day(0,24,0,0);
const COleDateTimeSpan cstCacheTimeout30Days(0,720,0,0);



const double  cstMaxDouble = 100000000000000000.0;
const double  cstMinDouble = -100000000000000000.0;
const double  cstDelta = 0.000001;

static bool IsNullDouble(double value) { return !_finite(value);}
static bool IsNullLong(long value) {return (value == std::numeric_limits<long>::infinity());}
static bool IsDoubleZero(double fVal) {return fVal > -cstDelta && fVal < cstDelta;}
static bool IsDoubleEqual(double fValA, double fValB) {return (fValA - fValB > -cstDelta && fValA - fValB < cstDelta);}
static bool IsDoubleMoreThan(double fVal1, double fVal2) {return fVal1 - cstDelta > fVal2;}
static bool IsDoubleLessThan(double fVal1, double fVal2) {return fVal1 + cstDelta < fVal2;}
static bool IsDoubleMoreThanOrEqual(double fVal1, double fVal2) {return IsDoubleMoreThan(fVal1, fVal2) || IsDoubleEqual(fVal1, fVal2);}
static bool IsDoubleLessThanOrEqual(double fVal1, double fVal2) {return IsDoubleLessThan(fVal1, fVal2) || IsDoubleEqual(fVal1, fVal2);}
};