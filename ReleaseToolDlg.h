// ReleaseToolDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "CDirectory.h"

// CReleaseToolDlg �Ի���
class CReleaseToolDlg : public CDialog
{
// ����
public:
	CReleaseToolDlg(CWnd* pParent = NULL);	// ��׼���캯��
	

	CString m_ReleaseVerFilePath ;
	CString m_CheckVerPath;
	CString m_ReleaseSourcePath;

	DWORD m_dwVerNum ; 
	
	CDirectory m_CDTDir;


// �Ի�������
	enum { IDD = IDD_RELEASETOOL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
