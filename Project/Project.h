
// Project.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once
//#define WM_MY_SELECT WM_USER+0x100
#define WM_SELECT WM_USER+0x101
#define WM_MODE WM_USER+0x110
#define WM_RANK_MSG WM_USER+0X111

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������

// CProjectApp: 
// �йش����ʵ�֣������ Project.cpp
//

class CProjectApp : public CWinApp
{
public:
	CProjectApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CProjectApp theApp;