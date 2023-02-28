
// Project.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once
//#define WM_MY_SELECT WM_USER+0x100
#define WM_SELECT WM_USER+0x101
#define WM_MODE WM_USER+0x110
#define WM_RANK_MSG WM_USER+0X111

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号

// CProjectApp: 
// 有关此类的实现，请参阅 Project.cpp
//

class CProjectApp : public CWinApp
{
public:
	CProjectApp();

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CProjectApp theApp;