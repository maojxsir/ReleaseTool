// ReleaseTool.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CReleaseToolApp:
// �йش����ʵ�֣������ ReleaseTool.cpp
//

class CReleaseToolApp : public CWinApp
{
public:
	CReleaseToolApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CReleaseToolApp theApp;