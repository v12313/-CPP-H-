
// ɨ��CPP��H�ļ�.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// Cɨ��CPP��H�ļ�App: 
// �йش����ʵ�֣������ ɨ��CPP��H�ļ�.cpp
//

class Cɨ��CPP��H�ļ�App : public CWinApp
{
public:
	Cɨ��CPP��H�ļ�App();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern Cɨ��CPP��H�ļ�App theApp;