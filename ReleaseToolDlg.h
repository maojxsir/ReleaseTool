// ReleaseToolDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "CDirectory.h"

// CReleaseToolDlg 对话框
class CReleaseToolDlg : public CDialog
{
// 构造
public:
	CReleaseToolDlg(CWnd* pParent = NULL);	// 标准构造函数
	

	CString m_ReleaseVerFilePath ;
	CString m_CheckVerPath;
	CString m_ReleaseSourcePath;

	DWORD m_dwVerNum ; 
	
	CDirectory m_CDTDir;


// 对话框数据
	enum { IDD = IDD_RELEASETOOL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnPrvCheck();
	afx_msg void OnBnClickedBtnSet();
	afx_msg void OnBnClickedBtnStart();
	afx_msg void OnBnClickedBtnCheck();
	afx_msg void OnBnClickedBtnSave();
	afx_msg void OnBnClickedBtnExit();
	afx_msg void OnBnClickedBtnOpenfile();
	afx_msg void OnEnSetfocusEditOpenfile();
	CEdit m_EditStutas;
};
