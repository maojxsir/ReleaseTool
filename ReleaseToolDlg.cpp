// ReleaseToolDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ReleaseTool.h"
#include "ReleaseToolDlg.h"
#include <stdio.h>
#include "CDirectory.h"
#include "Markup.h"

#ifdef _DEBUG
	#define new DEBUG_NEW
	#define T_SAVE_PATH _T("E:\\Program\\ja039CL\\CLRelease\\local\\") 
#else
	#define T_SAVE_PATH _T("T:\\Program\\ja039CL\\CLRelease\\local\\") 
#endif


#define PREV_SYSTEM_VERSION_PATH "C://WINCE600//OSDesigns//WSJA039//WSJA039//Wince600//TT6X0BD_ARMV4I//OAK//files//project.reg"

#define RELEASE_PRODUCT_SYSTEM_VERSION_PATH "C://build_nk//JA039_Program_PRODUCT//AVIC2097ZT_PLT_PRODUCT//AVIC2097ZT//AZT01PLT.HTM"
#define RELEASE_DESKTOP_SYSTEM_VERSION_PATH "C://build_nk//JA039_Program_DESKTOP//AVIC2097ZT_PLT_DESKTOP//AVIC2097ZT//AZT01PLT.HTM"
#define RELEASE_DEVELOP_SYSTEM_VERSION_PATH "C://build_nk//JA039_Program_DEVELOP//AVIC2097ZT_PLT_DEVELOP//AVIC2097ZT//AZT01PLT.HTM"


#define RELEASE_PRODUCT_SOURCE_PATH "C:\\build_nk\\JA039_Program_PRODUCT\\AVIC2097ZT_PLT_PRODUCT\\"
#define RELEASE_DESKTOP_SOURCE_PATH "C:\\build_nk\\JA039_Program_DESKTOP\\AVIC2097ZT_PLT_DESKTOP\\"
#define RELEASE_DEVELOP_SOURCE_PATH "C:\\build_nk\\JA039_Program_DEVELOP\\AVIC2097ZT_PLT_DEVELOP\\"


#define VER_PRODUCT _T("PRODUCT")
#define VER_DESKTOP _T("DESKTOP")
#define VER_DEVELOP _T("DEVELOP")


FILE *fpSysVerFile = NULL ;

char pBuff[2000]={"0"};

char *szpDest = NULL;

int iSize = 0;

char strNewVer[10] = {'0'};

CString cstrNewVer;

CString szPrevOSVerPath , szReleaseOSVerPath;
CString szPrjName;

 /*�ж�һ��·���Ƿ����Ѵ��ڵ�Ŀ¼*/
 BOOL IsDirectory(LPCTSTR pstrPath)
 {
    if (NULL == pstrPath)
     {
        return FALSE;   
	}
 
     /*ȥ��·��ĩβ�ķ�б��*/
     CString strPath = pstrPath;
     if (strPath.Right(1) == _T('\\'))
     {
         strPath.Delete(strPath.GetLength()-1);
     }
 
     CFileFind finder;
     BOOL bRet = finder.FindFile(strPath);
     if (!bRet)
     {
        return FALSE;
     }
 
     /*�жϸ�·���Ƿ���Ŀ¼*/
     finder.FindNextFile();
     bRet = finder.IsDirectory();
     finder.Close();
     return bRet;
}





BOOL CopyFolder(LPCTSTR pstrSrcFolder, LPCTSTR pstrDstFolder)
 {
     if ((NULL == pstrSrcFolder) || (NULL == pstrSrcFolder))
    {
         return FALSE;
     }
 
     /*���ԴĿ¼�Ƿ��ǺϷ�Ŀ¼*/
     if (!IsDirectory(pstrSrcFolder))
     {
         return FALSE;
     }
 
     /*���ļ�����ת��ΪCString���ͣ���ȷ��Ŀ¼��·��ĩβΪ��б��*/
     CString strSrcFolder(pstrSrcFolder);
     if (strSrcFolder.Right(1) != _T('\\'))
     {
         strSrcFolder += _T("\\");
     }
     CString strDstFolder(pstrDstFolder);
     if (strDstFolder.Right(1) != _T("\\"))
     {
         strDstFolder += _T("\\");
     }
 
     /*�����Ŀ¼�����ƣ�ֱ�ӷ��ظ��Ƴɹ�*/
     if (0 == _tcscmp(strSrcFolder, strDstFolder))
     {
         return TRUE;
     }
 
     /*���Ŀ��Ŀ¼������,�򴴽�Ŀ¼*/
     if (!IsDirectory(strDstFolder))
     {
         if (!CreateDirectory(strDstFolder, NULL))
         {
             /*����Ŀ��Ŀ¼ʧ��*/
             return FALSE;
         }
     }
 
     /*����ԴĿ¼�в����ļ���ͨ���*/
     CString strWildcard(strSrcFolder);
     strWildcard += _T("*.*");
 
     /*���ļ����ң��鿴ԴĿ¼���Ƿ����ƥ����ļ�*/
     /*����FindFile�󣬱������FindNextFile���ܻ�ò����ļ�����Ϣ*/
     CFileFind finder;
     BOOL bWorking = finder.FindFile(strWildcard);
 
     while (bWorking)
     {
         /*������һ���ļ�*/
         bWorking = finder.FindNextFile();
 
         /*������ǰĿ¼��.������һ��Ŀ¼��..��*/
         if (finder.IsDots())
        {
             continue;
         }
 
         /*�õ���ǰҪ���Ƶ�Դ�ļ���·��*/
         CString strSrcFile = finder.GetFilePath();
 
         /*�õ�Ҫ���Ƶ�Ŀ���ļ���·��*/
         CString strDstFile(strDstFolder);
         strDstFile += finder.GetFileName();
 
         /*�жϵ�ǰ�ļ��Ƿ���Ŀ¼,*/
         /*�����Ŀ¼���ݹ���ø���Ŀ¼,*/
         /*����ֱ�Ӹ����ļ�*/
         if (finder.IsDirectory())
        {
             if (!CopyFolder(strSrcFile, strDstFile))
             {
                 finder.Close();
                 return FALSE;
             }
         }
         else
         {
             if (!CopyFile(strSrcFile, strDstFile, FALSE))
             {
                 finder.Close();
                 return FALSE;
            }
         }
 
     } /*while (bWorking)*/
 
     /*�ر��ļ�����*/
     finder.Close();
 
     return TRUE;
 } 

/*
BOOL Char2WideChar(char *szVer, wchar_t * pUnicode)
{

	// multi char to wchar

	DWORD dwNum = MultiByteToWideChar (CP_ACP, 0, szVer, -1, NULL, 0);
	pUnicode = new wchar_t[dwNum];
	if(!pUnicode)
	{
		delete []pUnicode;
		return FALSE;
	}  
	MultiByteToWideChar (CP_ACP, 0, szVer, -1, pUnicode, dwNum);

	return TRUE;
}

BOOL WideChar2Char(wchar_t *wText ,char *psText )
{
	
	DWORD dwNum = WideCharToMultiByte(CP_OEMCP,NULL,wText,-1,NULL,0,NULL,FALSE);
	//char *psText;
	psText = new char[dwNum];
	if(!psText)
	{
		delete []psText;
	}
	WideCharToMultiByte (CP_OEMCP,NULL,wText,-1,psText,dwNum,NULL,FALSE);
	delete []psText;
	return TRUE ;
}

*/


// CReleaseToolDlg �Ի���
CReleaseToolDlg::CReleaseToolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CReleaseToolDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_dwVerNum = 0 ;


}

void CReleaseToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_STATUS, m_EditStutas);
}

BEGIN_MESSAGE_MAP(CReleaseToolDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_PRV_CHECK, &CReleaseToolDlg::OnBnClickedBtnPrvCheck)
	ON_BN_CLICKED(IDC_BTN_SET, &CReleaseToolDlg::OnBnClickedBtnSet)
	ON_BN_CLICKED(IDC_BTN_START, &CReleaseToolDlg::OnBnClickedBtnStart)
	ON_BN_CLICKED(IDC_BTN_CHECK, &CReleaseToolDlg::OnBnClickedBtnCheck)
	ON_BN_CLICKED(IDC_BTN_SAVE, &CReleaseToolDlg::OnBnClickedBtnSave)
	ON_BN_CLICKED(IDC_BTN_EXIT, &CReleaseToolDlg::OnBnClickedBtnExit)
	ON_BN_CLICKED(IDC_BTN_OPENFILE, &CReleaseToolDlg::OnBnClickedBtnOpenfile)
	ON_EN_SETFOCUS(IDC_EDIT_OPENFILE, &CReleaseToolDlg::OnEnSetfocusEditOpenfile)
END_MESSAGE_MAP()


// CReleaseToolDlg ��Ϣ�������






BOOL ERCFuncW()
{
	CMarkup xml;

	//if (xml.Load(ERC_PATH))
	//{
	//	xml.ResetMainPos();
	//	if (xml.FindElem(_T("�����¼�")))
	//	{
	//		CString strNum=xml.GetAttrib(_T("TotalNum"));
	//		memset(tmp,0,10);
	//		CString_To_Char(strNum,tmp);
	//		totalEvtNum = atoi(tmp);//��ERC����

	//		xml.IntoElem();
	//		for (int i=0;i<totalEvtNum;i++)
	//		{
	//			if (xml.FindElem(_T("����ʱ��")))
	//			{

	//				int rptflag=0,indxnum=0;;
	//				CString strtime=xml.GetAttrib(_T("time"));

	//				//��ȡ����ʱ��
	//				_stscanf(strtime, _T("%d-%d-%d %d:%d:%d"), &occurTime.wYear,&occurTime.wMonth,&occurTime.wDay, 
	//					&occurTime.wHour,&occurTime.wMinute,&occurTime.wSecond); 
	//				occurTime.wMilliseconds=0;
	//				memset(tmp,0,10);
	//				CString strtime1=xml.GetAttrib(_T("report"));//��ȡ�¼��Ƿ��ϱ�����
	//				CString erctype=xml.GetAttrib(_T("ERCType"));//��ȡ�¼�����������
	//				CString idxtemp=xml.GetAttrib(_T("IndexNum"));//��ȡ�¼��������
	//				CString_To_Char(strtime1,tmp);
	//				rptflag = atoi(tmp);//�Ƿ��Ѿ��ϱ�

	//				CString_To_Char(idxtemp,tmp);
	//				indxnum = atoi(tmp);//�¼��������


	CString Path = TEXT("d:\\Temp\\config.xml") ;
	if (xml.Load(Path))
	{
		if (xml.FindElem(_T("sysconfig")))
		{
			szPrjName = xml.GetAttrib(_T("Project"));

			while (xml.FindElem(_T("Item")))//�����¼�
			{
				CString szType=xml.GetAttrib(_T("Type"));



			//	//totalNum
			//	CString dat=xml.GetAttrib(_T("TotalNum"));

			//	//����ʱ������-time
			//	TCHAR szDateTime[50] = {0};    
			//	_stprintf(szDateTime, _T("%02d-%02d-%02d %02d:%02d:%02d"), time.wYear,time.wMonth, time.wDay, 
			//		time.wHour, time.wMinute, time.wSecond);    
			//	dat = szDateTime;
			//	xml.SetAttrib(_T("time"),dat); //����ʱ������ ���ʱ��

			//	//����ʱ������-�Ƿ��ϱ�
			//	xml.SetAttrib(_T("report"),_T("0")); //����ʱ������ �Ƿ��ϱ���deflaut ��0--no��1-yes

			//	//����ʱ������-�¼�����
			//	xml.SetAttrib(_T("ERCType"),ERC_Type);//����ʱ������ �¼����ͣ�

			//	//����ʱ������-�¼���ţ������ϱ���ѯָ�룩
			//	dat.Empty();
			////	dat.Format(TEXT("%d"),number);
			//	xml.SetAttrib(_T("IndexNum"),dat);

			//	//���� �¼����� �ڵ�
			//	CString strtemp=para->GetAt(0);
			//	xml.AddChildElem(_T("�¼�����"),strtemp);
			//	strtemp.Empty();
			//	int paraNum=(para->GetSize()-1);//!!!!
			//	strtemp.Format(_T("%d"),paraNum);
			//	xml.SetChildAttrib(_T("paranum"),strtemp); //�¼��������� ��������

			//	//�¼��Ĳ���
			//	for (int i=0;i<paraNum;i++)//��Ӳ���
			//	{
			//		strtemp.Empty();
			//		CString str=_T("����");
			//		dat.Format(TEXT("%d"),i);
			//		str+=dat;
			//		strtemp=para->GetAt(i+1);//!!!!
			//		xml.AddChildElem(str,strtemp);	//��Ӳ���
			//	}
	//		xml.Save(_T("\\nandflash\\afn0E.xml"));
				return 1;}
		}
	}
	else
		if ( IDOK ==  AfxMessageBox(_T("��·�������ļ�ʧ�ܣ�"),MB_OK | MB_ICONWARNING) )
		{
			return 0;
		}	
	return 0;
}



BOOL CReleaseToolDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	//m_Cmb_ReleaseVer.AddString(VER_PRODUCT);
	//m_Cmb_ReleaseVer.AddString(VER_DEVELOP);
	//m_Cmb_ReleaseVer.AddString(VER_DESKTOP);
	//m_Cmb_ReleaseVer.SetCurSel( 0 );


////��ʾ״̬��
//	m_StatBar=new   CStatusBarCtrl;
//	RECT     m_Rect;   
//	GetClientRect(&m_Rect);         //��ȡ�Ի���ľ�������
//	m_Rect.top=m_Rect.bottom-20;    //����״̬���ľ�������
//	m_StatBar->Create(WS_BORDER|WS_VISIBLE|CBRS_BOTTOM,m_Rect,this,3);  
//	int nParts[4]= {100, 200, 300,-1};      //�ָ�ߴ�
//	m_StatBar->SetParts(4, nParts); //�ָ�״̬��
//	m_StatBar->SetText(_T("���ǵ�һ��ָʾ��"),0,0); //��һ����������"���ǵ�һ��ָʾ��"
//	m_StatBar->SetText(_T("���ǵڶ���ָʾ��"),1,0); //��������
//	/*Ҳ��ʹ�����·�ʽ����ָʾ������
//	m_StatBar.SetPaneText(0,"���ǵ�һ��ָʾ��",0);
//	����������m_StatBar->SetIcon(3,SetIcon(AfxGetApp()->LoadIcon(IDI_ICON3),FALSE));
//	//�ڵ��ĸ������м���IDΪIDI_ICON3��ͼ��
//	*********************************************/
//	m_StatBar->ShowWindow(SW_SHOW); 
	ERCFuncW();
	
	
	
	


	SetDlgItemText(IDC_EDIT_OPENFILE, _T("    ---���ѡ��Release����---"));

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CReleaseToolDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
//
HCURSOR CReleaseToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CReleaseToolDlg::OnBnClickedBtnPrvCheck()
{
	// TODO: Add your control notification handler code here	
	char szVer[10] = {0};

	fpSysVerFile = fopen(PREV_SYSTEM_VERSION_PATH,"rb+");
	if ( NULL == fpSysVerFile)
	{
		if ( IDOK ==  AfxMessageBox(_T("��ϵͳ�汾��ʧ�ܣ�"),MB_OK | MB_ICONWARNING) )
		{
			return ;
		}	
	}
	iSize = fread( pBuff,sizeof(char), 2000, fpSysVerFile);

	szpDest = strstr(pBuff,"ProductVersion");
	if (NULL == szpDest )
	{
		if ( IDOK ==  AfxMessageBox(_T("ProductVersion Not Found!"),MB_OK | MB_ICONWARNING) )
		{
			fclose(fpSysVerFile);
			return ;
		}	
	}
	else
		szpDest += 25;

	if ( strncpy(szVer, szpDest, 8) )
	{
		szVer[1] = '.';

		// multi char to wchar
		wchar_t * pUnicode;	
		DWORD dwNum = MultiByteToWideChar (CP_ACP, 0, szVer, -1, NULL, 0);
		pUnicode = new wchar_t[dwNum];
		if(!pUnicode)
		{
			delete []pUnicode;
		}  
		MultiByteToWideChar (CP_ACP, 0, szVer, -1, pUnicode, dwNum);


		SetDlgItemText(IDC_EDIT_PRV,pUnicode);
	}
	
}

///////////////////////////////////////////////////////////////////////
void CReleaseToolDlg::OnBnClickedBtnSet()
{
	// TODO: Add your control notification handler code here
	int len = 0;
	char *pTemp = szpDest;
	char *pSour = strNewVer;
	if (NULL == szpDest)
	{
		if ( IDOK  == AfxMessageBox(_T("���ȼ��ϵͳ�汾�ţ�"),MB_OK   | MB_ICONSTOP | MB_DEFBUTTON1 ))
		{
			return;
		}
	}

	len = GetDlgItemText(IDC_EDIT_SET,cstrNewVer);
	if ( len < 8)
	{
		if ( IDOK == AfxMessageBox( _T("����汾�Ŵ���"),MB_OK | MB_ICONSTOP ) )
			return;
	}
	
	//  wchar to multi char
	DWORD dwNum = WideCharToMultiByte(CP_OEMCP,NULL,cstrNewVer,-1,NULL,0,NULL,FALSE);

	WideCharToMultiByte (CP_OEMCP,NULL,cstrNewVer,-1,strNewVer,dwNum,NULL,FALSE);
	if ( '.' == strNewVer[1] )
	{	
		strNewVer[1] = '0';
	}
	while (len--)
	{
		*szpDest++ = * pSour++;
	}
	fseek(fpSysVerFile,0,SEEK_SET);

	fwrite(pBuff, 1, iSize, fpSysVerFile);

	fclose(fpSysVerFile);

	//״̬��ʾ
	len = m_EditStutas.GetWindowTextLength();
	m_EditStutas.SetSel(len, -1);
	m_EditStutas.ReplaceSel(_T("..����ϵͳ�汾��OK \r\n") );


}

void CReleaseToolDlg::OnEnSetfocusEditOpenfile()
{
	// TODO: Add your control notification handler code here

	//��������Ϊ�����ؼ�
	CWnd *wnd = GetFocus ();  
	if (wnd != NULL)  
	{  
		TCHAR str[256];  
		CString ClassName = _T("Button");  
		GetClassName (wnd->m_hWnd, str, 256);  
		if (ClassName == str)  
		{  
			UINT i = wnd->GetDlgCtrlID ();  
			SendMessage (WM_COMMAND, i, (LPARAM)wnd->m_hWnd);  
			return ;  
		}  
	}  
	CWnd *mwnd = GetNextDlgTabItem (wnd);  
	if (mwnd)  
	{  
		mwnd->SetFocus();   
	}  


	//���ļ�������
	TCHAR szFilter[]=_T("files(*.bat)|*.bat|files(*.py)|*.py|�����ļ�(*.*)|*.*||");
	CFileDialog dlg(1,NULL,NULL,NULL,szFilter);

	//����Ĭ��·��
	dlg.m_ofn.lpstrInitialDir  = _T("C://build_nk//") ;

	if(dlg.DoModal()==IDOK) 
	{
		m_ReleaseVerFilePath = dlg.GetPathName();
	}
	//״̬��ʾ
	int len = m_EditStutas.GetWindowTextLength();
	m_EditStutas.SetSel(len, -1);
	m_EditStutas.ReplaceSel(_T("..ѡ��Release �汾Ϊ��") + m_ReleaseVerFilePath + _T("\r\n"));

	//��ʾѡ���ļ�
	int Len = 0,index = 0;
	CString strName;
	Len = m_ReleaseVerFilePath.GetLength();
	index = m_ReleaseVerFilePath.ReverseFind(_T('\\'));
	strName = m_ReleaseVerFilePath.Right(Len - index - 1);
	SetDlgItemText(IDC_EDIT_OPENFILE, strName);	

	//��ȡѡ������
	index = m_ReleaseVerFilePath.ReverseFind(_T('_'));
	strName = m_ReleaseVerFilePath.Right(Len - index - 1);

	index = strName.ReverseFind(_T('.'));
	strName = strName.Left( index );
	if ( !(strName.CompareNoCase(VER_PRODUCT)))
	{
		m_ReleaseSourcePath = RELEASE_PRODUCT_SOURCE_PATH;	
		m_dwVerNum = 1;
	}else if (!(strName.CompareNoCase(VER_DESKTOP)))
	{
		m_ReleaseSourcePath = RELEASE_DESKTOP_SOURCE_PATH ;
		m_dwVerNum = 2;
	}else if (!(strName.CompareNoCase( VER_DEVELOP)))
	{
		m_ReleaseSourcePath = RELEASE_DEVELOP_SOURCE_PATH ;
		m_dwVerNum = 3;
	}
	else{
		if ( IDOK == AfxMessageBox( _T("����汾��������"),MB_OK | MB_ICONSTOP ) )
			return;	
	}


}



///////////////////////////////////////////////////////////////////////
void CReleaseToolDlg::OnBnClickedBtnStart()
{
	// TODO: Add your control notification handler code here

	if ( "" == m_ReleaseVerFilePath )
	{	
		if ( IDOK == AfxMessageBox( _T("ִ���ļ�·������"),MB_OK | MB_ICONSTOP ) )
			return;
	}

	ShellExecute(NULL, L"open", m_ReleaseVerFilePath, NULL,  NULL, SW_MAXIMIZE);

	//״̬��ʾ
	int len = m_EditStutas.GetWindowTextLength();
	m_EditStutas.SetSel(len, -1);
	m_EditStutas.ReplaceSel(_T("..�ű��ļ���ʼִ��......\r\n")); 

}
///////////////////////////////////////////////////////////////////////
void CReleaseToolDlg::OnBnClickedBtnCheck()
{
	// TODO: Add your control notification handler code here

	char szVer[10] = {0};
	switch (m_dwVerNum)
	{
	case 1:
		fpSysVerFile = fopen(RELEASE_PRODUCT_SYSTEM_VERSION_PATH,"rb+");
		break;
	case 2:
		fpSysVerFile = fopen(RELEASE_DESKTOP_SYSTEM_VERSION_PATH,"rb+");
		break;
	case 3:
		fpSysVerFile = fopen(RELEASE_DEVELOP_SYSTEM_VERSION_PATH,"rb+");
		break;
	default:
		if ( IDOK ==  AfxMessageBox(_T("����ѡ��Release�汾��"),MB_OK | MB_ICONWARNING) )
		{
			return ;
		}	
		break;
	}
	
	if ( NULL == fpSysVerFile)
	{
		if ( IDOK ==  AfxMessageBox(_T("��Release�汾��ʧ�ܣ�"),MB_OK | MB_ICONWARNING) )
		{
			return ;
		}	
	}
	iSize = fread( pBuff,sizeof(char), 2000, fpSysVerFile);

	szpDest = strstr(pBuff,"Release Version");
	if (NULL == szpDest )
	{
		if ( IDOK ==  AfxMessageBox(_T("Release Version Not Found!"),MB_OK | MB_ICONWARNING) )
		{
			fclose(fpSysVerFile);
			return ;
		}	
	}
	else
		szpDest += 19;

	if ( strncpy(szVer, szpDest, 8) )
	{
		// multi char to wchar
		wchar_t * pUnicode;	
		DWORD dwNum = MultiByteToWideChar (CP_ACP, 0, szVer, -1, NULL, 0);
		pUnicode = new wchar_t[dwNum];
		if(!pUnicode)
		{
			delete []pUnicode;
		}  
		MultiByteToWideChar (CP_ACP, 0, szVer, -1, pUnicode, dwNum);
		CString strText = _T("���ɵ�Release�汾��Ϊ��");
		strText +=  pUnicode; 

		//״̬��ʾ
		int len = m_EditStutas.GetWindowTextLength();
		m_EditStutas.SetSel(len, -1);
		m_EditStutas.ReplaceSel(_T("..")+strText +_T("\r\n")); 
	}



}
///////////////////////////////////////////////////////////////////////
void CReleaseToolDlg::OnBnClickedBtnSave()
{
	// TODO: Add your control notification handler code here
	SYSTEMTIME SysTime;
	char szTime[30] = {'0'};
	CString DirName;

	GetLocalTime(& SysTime) ;
	SysTime.wYear -= 2000 ; //
	sprintf(szTime,"_%02d%02d%02d_%02d%02d%02d", SysTime.wYear, SysTime.wMonth, SysTime.wDay, SysTime.wHour, SysTime.wMinute, SysTime.wSecond) ;
	
	switch (m_dwVerNum)
	{
	case 1:
		DirName = "PRO";
		break;
	case 2:
		DirName = "DES";
		break;
	case 3:
		DirName = "DEV";
		break;
	default:
		if ( IDOK ==  AfxMessageBox(_T("����ѡ��Release�汾��"),MB_OK | MB_ICONWARNING) )
		{
			return ;
		}	
		break;
	}

	wchar_t * pUnicode;	
	DWORD dwNum = MultiByteToWideChar (CP_ACP, 0, szTime, -1, NULL, 0);
	pUnicode = new wchar_t[dwNum];
	if(!pUnicode)
	{
		delete []pUnicode;
	}  
	MultiByteToWideChar (CP_ACP, 0, szTime, -1, pUnicode, dwNum);
	DirName += pUnicode;

	DirName = T_SAVE_PATH + DirName;

//	CStringתstd::wstring
	std::wstring str = DirName.GetString();

	m_CDTDir.CreateDirectorySerial( str );
	if (m_CDTDir.CheckDirectoryExist( str ))
	{
		CopyFolder(m_ReleaseSourcePath ,DirName);
	}
	else{
		if ( IDOK == AfxMessageBox( _T("T�̴���Ŀ¼ʧ�ܣ�"),MB_OK | MB_ICONSTOP ) )
			return;
	}

	//״̬��ʾ
	int len = m_EditStutas.GetWindowTextLength();
	m_EditStutas.SetSel(len, -1);
	m_EditStutas.ReplaceSel(_T("..����·����")+DirName+_T("\r\n")); 
	

}

void CReleaseToolDlg::OnBnClickedBtnOpenfile()
{
	// TODO: Add your control notification handler code here

	if ( "" == m_ReleaseVerFilePath )
	{	
		if ( IDOK == AfxMessageBox( _T("���ļ�·������"),MB_OK | MB_ICONSTOP ) )
			return;
	}
	
//	char strName[50]={'0'};
//	char strOpen[100]={"notepad.exe "};
//	DWORD dwNum = WideCharToMultiByte(CP_OEMCP,NULL,m_ReleaseVerFilePath,-1,NULL,0,NULL,FALSE);
//	WideCharToMultiByte (CP_OEMCP,NULL,m_ReleaseVerFilePath,-1,strName,dwNum,NULL,FALSE);
//	strcat(strOpen, strName);
//	WinExec(strOpen, SW_SHOW);
////WinExec( "notepad.exe ReadMe.txt", SW_SHOW );

	ShellExecute(NULL, _T("open"),_T("notepad"),m_ReleaseVerFilePath, NULL, SW_SHOW);


	//״̬��ʾ
	int len = m_EditStutas.GetWindowTextLength();
	m_EditStutas.SetSel(len, -1);
	m_EditStutas.ReplaceSel(_T("..ִ�нű��ļ��Ѵ�......\r\n")); 

}



///////////////////////////////////////////////////////////////////////
void CReleaseToolDlg::OnBnClickedBtnExit()
{
	// TODO: Add your control notification handler code here
	
	EndDialog(IDCANCEL); 

}
