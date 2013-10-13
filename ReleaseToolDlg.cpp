// ReleaseToolDlg.cpp : 实现文件
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

 /*判断一个路径是否是已存在的目录*/
 BOOL IsDirectory(LPCTSTR pstrPath)
 {
    if (NULL == pstrPath)
     {
        return FALSE;   
	}
 
     /*去除路径末尾的反斜杠*/
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
 
     /*判断该路径是否是目录*/
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
 
     /*检查源目录是否是合法目录*/
     if (!IsDirectory(pstrSrcFolder))
     {
         return FALSE;
     }
 
     /*把文件名称转换为CString类型，并确认目录的路径末尾为反斜杠*/
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
 
     /*如果是目录自身复制，直接返回复制成功*/
     if (0 == _tcscmp(strSrcFolder, strDstFolder))
     {
         return TRUE;
     }
 
     /*如果目的目录不存在,则创建目录*/
     if (!IsDirectory(strDstFolder))
     {
         if (!CreateDirectory(strDstFolder, NULL))
         {
             /*创建目的目录失败*/
             return FALSE;
         }
     }
 
     /*创建源目录中查找文件的通配符*/
     CString strWildcard(strSrcFolder);
     strWildcard += _T("*.*");
 
     /*打开文件查找，查看源目录中是否存在匹配的文件*/
     /*调用FindFile后，必须调用FindNextFile才能获得查找文件的信息*/
     CFileFind finder;
     BOOL bWorking = finder.FindFile(strWildcard);
 
     while (bWorking)
     {
         /*查找下一个文件*/
         bWorking = finder.FindNextFile();
 
         /*跳过当前目录“.”和上一级目录“..”*/
         if (finder.IsDots())
        {
             continue;
         }
 
         /*得到当前要复制的源文件的路径*/
         CString strSrcFile = finder.GetFilePath();
 
         /*得到要复制的目标文件的路径*/
         CString strDstFile(strDstFolder);
         strDstFile += finder.GetFileName();
 
         /*判断当前文件是否是目录,*/
         /*如果是目录，递归调用复制目录,*/
         /*否则，直接复制文件*/
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
 
     /*关闭文件查找*/
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


// CReleaseToolDlg 对话框
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


// CReleaseToolDlg 消息处理程序






BOOL ERCFuncW()
{
	CMarkup xml;

	//if (xml.Load(ERC_PATH))
	//{
	//	xml.ResetMainPos();
	//	if (xml.FindElem(_T("三类事件")))
	//	{
	//		CString strNum=xml.GetAttrib(_T("TotalNum"));
	//		memset(tmp,0,10);
	//		CString_To_Char(strNum,tmp);
	//		totalEvtNum = atoi(tmp);//总ERC个数

	//		xml.IntoElem();
	//		for (int i=0;i<totalEvtNum;i++)
	//		{
	//			if (xml.FindElem(_T("发生时间")))
	//			{

	//				int rptflag=0,indxnum=0;;
	//				CString strtime=xml.GetAttrib(_T("time"));

	//				//读取发生时间
	//				_stscanf(strtime, _T("%d-%d-%d %d:%d:%d"), &occurTime.wYear,&occurTime.wMonth,&occurTime.wDay, 
	//					&occurTime.wHour,&occurTime.wMinute,&occurTime.wSecond); 
	//				occurTime.wMilliseconds=0;
	//				memset(tmp,0,10);
	//				CString strtime1=xml.GetAttrib(_T("report"));//获取事件是否上报属性
	//				CString erctype=xml.GetAttrib(_T("ERCType"));//获取事件的类型属性
	//				CString idxtemp=xml.GetAttrib(_T("IndexNum"));//获取事件编号属性
	//				CString_To_Char(strtime1,tmp);
	//				rptflag = atoi(tmp);//是否已经上报

	//				CString_To_Char(idxtemp,tmp);
	//				indxnum = atoi(tmp);//事件编号属性


	CString Path = TEXT("d:\\Temp\\config.xml") ;
	if (xml.Load(Path))
	{
		if (xml.FindElem(_T("sysconfig")))
		{
			szPrjName = xml.GetAttrib(_T("Project"));

			while (xml.FindElem(_T("Item")))//三类事件
			{
				CString szType=xml.GetAttrib(_T("Type"));



			//	//totalNum
			//	CString dat=xml.GetAttrib(_T("TotalNum"));

			//	//发生时间属性-time
			//	TCHAR szDateTime[50] = {0};    
			//	_stprintf(szDateTime, _T("%02d-%02d-%02d %02d:%02d:%02d"), time.wYear,time.wMonth, time.wDay, 
			//		time.wHour, time.wMinute, time.wSecond);    
			//	dat = szDateTime;
			//	xml.SetAttrib(_T("time"),dat); //发生时间属性 添加时间

			//	//发生时间属性-是否上报
			//	xml.SetAttrib(_T("report"),_T("0")); //发生时间属性 是否上报，deflaut ：0--no，1-yes

			//	//发生时间属性-事件类型
			//	xml.SetAttrib(_T("ERCType"),ERC_Type);//发生时间属性 事件类型，

			//	//发生时间属性-事件编号（用于上报查询指针）
			//	dat.Empty();
			////	dat.Format(TEXT("%d"),number);
			//	xml.SetAttrib(_T("IndexNum"),dat);

			//	//增加 事件名称 节点
			//	CString strtemp=para->GetAt(0);
			//	xml.AddChildElem(_T("事件名称"),strtemp);
			//	strtemp.Empty();
			//	int paraNum=(para->GetSize()-1);//!!!!
			//	strtemp.Format(_T("%d"),paraNum);
			//	xml.SetChildAttrib(_T("paranum"),strtemp); //事件名称属性 参数个数

			//	//事件的参数
			//	for (int i=0;i<paraNum;i++)//添加参数
			//	{
			//		strtemp.Empty();
			//		CString str=_T("参数");
			//		dat.Format(TEXT("%d"),i);
			//		str+=dat;
			//		strtemp=para->GetAt(i+1);//!!!!
			//		xml.AddChildElem(str,strtemp);	//添加参数
			//	}
	//		xml.Save(_T("\\nandflash\\afn0E.xml"));
				return 1;}
		}
	}
	else
		if ( IDOK ==  AfxMessageBox(_T("打开路径配置文件失败！"),MB_OK | MB_ICONWARNING) )
		{
			return 0;
		}	
	return 0;
}



BOOL CReleaseToolDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	//m_Cmb_ReleaseVer.AddString(VER_PRODUCT);
	//m_Cmb_ReleaseVer.AddString(VER_DEVELOP);
	//m_Cmb_ReleaseVer.AddString(VER_DESKTOP);
	//m_Cmb_ReleaseVer.SetCurSel( 0 );


////显示状态栏
//	m_StatBar=new   CStatusBarCtrl;
//	RECT     m_Rect;   
//	GetClientRect(&m_Rect);         //获取对话框的矩形区域
//	m_Rect.top=m_Rect.bottom-20;    //设置状态栏的矩形区域
//	m_StatBar->Create(WS_BORDER|WS_VISIBLE|CBRS_BOTTOM,m_Rect,this,3);  
//	int nParts[4]= {100, 200, 300,-1};      //分割尺寸
//	m_StatBar->SetParts(4, nParts); //分割状态栏
//	m_StatBar->SetText(_T("这是第一个指示器"),0,0); //第一个分栏加入"这是第一个指示器"
//	m_StatBar->SetText(_T("这是第二个指示器"),1,0); //以下类似
//	/*也可使用以下方式加入指示器文字
//	m_StatBar.SetPaneText(0,"这是第一个指示器",0);
//	其他操作：m_StatBar->SetIcon(3,SetIcon(AfxGetApp()->LoadIcon(IDI_ICON3),FALSE));
//	//在第四个分栏中加入ID为IDI_ICON3的图标
//	*********************************************/
//	m_StatBar->ShowWindow(SW_SHOW); 
	ERCFuncW();
	
	
	
	


	SetDlgItemText(IDC_EDIT_OPENFILE, _T("    ---点击选择Release类型---"));

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CReleaseToolDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
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
		if ( IDOK ==  AfxMessageBox(_T("打开系统版本号失败！"),MB_OK | MB_ICONWARNING) )
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
		if ( IDOK  == AfxMessageBox(_T("请先检查系统版本号！"),MB_OK   | MB_ICONSTOP | MB_DEFBUTTON1 ))
		{
			return;
		}
	}

	len = GetDlgItemText(IDC_EDIT_SET,cstrNewVer);
	if ( len < 8)
	{
		if ( IDOK == AfxMessageBox( _T("输入版本号错误！"),MB_OK | MB_ICONSTOP ) )
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

	//状态显示
	len = m_EditStutas.GetWindowTextLength();
	m_EditStutas.SetSel(len, -1);
	m_EditStutas.ReplaceSel(_T("..设置系统版本号OK \r\n") );


}

void CReleaseToolDlg::OnEnSetfocusEditOpenfile()
{
	// TODO: Add your control notification handler code here

	//焦点设置为其他控件
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


	//打开文件的类型
	TCHAR szFilter[]=_T("files(*.bat)|*.bat|files(*.py)|*.py|所有文件(*.*)|*.*||");
	CFileDialog dlg(1,NULL,NULL,NULL,szFilter);

	//设置默认路径
	dlg.m_ofn.lpstrInitialDir  = _T("C://build_nk//") ;

	if(dlg.DoModal()==IDOK) 
	{
		m_ReleaseVerFilePath = dlg.GetPathName();
	}
	//状态显示
	int len = m_EditStutas.GetWindowTextLength();
	m_EditStutas.SetSel(len, -1);
	m_EditStutas.ReplaceSel(_T("..选择Release 版本为：") + m_ReleaseVerFilePath + _T("\r\n"));

	//显示选择文件
	int Len = 0,index = 0;
	CString strName;
	Len = m_ReleaseVerFilePath.GetLength();
	index = m_ReleaseVerFilePath.ReverseFind(_T('\\'));
	strName = m_ReleaseVerFilePath.Right(Len - index - 1);
	SetDlgItemText(IDC_EDIT_OPENFILE, strName);	

	//获取选择类型
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
		if ( IDOK == AfxMessageBox( _T("编译版本解析错误！"),MB_OK | MB_ICONSTOP ) )
			return;	
	}


}



///////////////////////////////////////////////////////////////////////
void CReleaseToolDlg::OnBnClickedBtnStart()
{
	// TODO: Add your control notification handler code here

	if ( "" == m_ReleaseVerFilePath )
	{	
		if ( IDOK == AfxMessageBox( _T("执行文件路径错误！"),MB_OK | MB_ICONSTOP ) )
			return;
	}

	ShellExecute(NULL, L"open", m_ReleaseVerFilePath, NULL,  NULL, SW_MAXIMIZE);

	//状态显示
	int len = m_EditStutas.GetWindowTextLength();
	m_EditStutas.SetSel(len, -1);
	m_EditStutas.ReplaceSel(_T("..脚本文件开始执行......\r\n")); 

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
		if ( IDOK ==  AfxMessageBox(_T("请先选择Release版本！"),MB_OK | MB_ICONWARNING) )
		{
			return ;
		}	
		break;
	}
	
	if ( NULL == fpSysVerFile)
	{
		if ( IDOK ==  AfxMessageBox(_T("打开Release版本号失败！"),MB_OK | MB_ICONWARNING) )
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
		CString strText = _T("生成的Release版本号为：");
		strText +=  pUnicode; 

		//状态显示
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
		if ( IDOK ==  AfxMessageBox(_T("请先选择Release版本！"),MB_OK | MB_ICONWARNING) )
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

//	CString转std::wstring
	std::wstring str = DirName.GetString();

	m_CDTDir.CreateDirectorySerial( str );
	if (m_CDTDir.CheckDirectoryExist( str ))
	{
		CopyFolder(m_ReleaseSourcePath ,DirName);
	}
	else{
		if ( IDOK == AfxMessageBox( _T("T盘创建目录失败！"),MB_OK | MB_ICONSTOP ) )
			return;
	}

	//状态显示
	int len = m_EditStutas.GetWindowTextLength();
	m_EditStutas.SetSel(len, -1);
	m_EditStutas.ReplaceSel(_T("..保存路径：")+DirName+_T("\r\n")); 
	

}

void CReleaseToolDlg::OnBnClickedBtnOpenfile()
{
	// TODO: Add your control notification handler code here

	if ( "" == m_ReleaseVerFilePath )
	{	
		if ( IDOK == AfxMessageBox( _T("打开文件路径错误！"),MB_OK | MB_ICONSTOP ) )
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


	//状态显示
	int len = m_EditStutas.GetWindowTextLength();
	m_EditStutas.SetSel(len, -1);
	m_EditStutas.ReplaceSel(_T("..执行脚本文件已打开......\r\n")); 

}



///////////////////////////////////////////////////////////////////////
void CReleaseToolDlg::OnBnClickedBtnExit()
{
	// TODO: Add your control notification handler code here
	
	EndDialog(IDCANCEL); 

}
