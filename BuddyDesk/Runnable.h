#pragma once
#ifndef __RUNABLE_H__  
#define __RUNABLE_H__  

#include "stdafx.h"
#include <Windows.h>
#include <process.h>

interface IRunnable
{
public:
	virtual ~IRunnable(void) {};
	virtual void Run() = 0;
};
#endif //__RUNABLE_H__

