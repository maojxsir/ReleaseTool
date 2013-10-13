#include "stdafx.h"
#include "CDirectory.h"


CDirectory::CDirectory()
{

}
CDirectory::~CDirectory()
{
}

TCHAR *CDirectory::GetCurrentDirectory(TCHAR *pszPath, ULONG ulSize)
{
	memset(pszPath, 0, sizeof(TCHAR) * ulSize);

	TCHAR szBuf[MAX_PATH] = {0};
	GetModuleFileName(NULL,szBuf,sizeof(szBuf)/sizeof(TCHAR));

	int ulCount = _tcslen(szBuf);

	while(--ulCount >= 0)
	{
		if(szBuf[ulCount] == TEXT('//'))
		{
			break;
		}
		else
		{
			continue;
		}
	}

	if(ulSize > (DWORD)ulCount)
	{
		_tcsncpy(pszPath,szBuf,(ulCount + 1));
	}

	return pszPath;
}

void CDirectory::CreateDirectorySerial(const TSTRING &strPath)
{

	TSTRING SourcePath = strPath;

	if(strPath.size() >= MAX_PATH)
		return;

	if(SourcePath[SourcePath.size() -1 ] == TEXT('//'))
	{
		SourcePath[SourcePath.size() -1 ] = TEXT('/0');
	}

	if(CheckDirectoryExist(strPath))
		return;

	TSTRING::size_type idx = SourcePath.find(TEXT('//'));

	while(idx !=TSTRING::npos)
	{
		TSTRING strCreateDirPath;
		strCreateDirPath.assign(SourcePath,0,idx);
		if(!CheckDirectoryExist(strCreateDirPath))
		{
			CreateDirectory(strCreateDirPath.c_str(),NULL);
		}

		idx = SourcePath.find(TEXT('//'),idx + 1);
	}

	CreateDirectory(SourcePath.c_str(),NULL);
}

void CDirectory::DeleteDirectory(const TSTRING &strPath)
{
	TSTRING strDirPath = strPath;
#ifdef _WIN32_WCE
	if(strDirPath[0] != TEXT('//'))
	{
		return;
	}
#endif
	if(strDirPath[strDirPath.size() - 1] == TEXT('//'))
	{
		strDirPath += TEXT("*.*");
	}
	else
	{
		strDirPath += TEXT("//*.*");
	}

	WIN32_FIND_DATA fd;
	HANDLE hdFind;
	hdFind = FindFirstFile(strDirPath.c_str(),&fd);
	if(hdFind != INVALID_HANDLE_VALUE)
	{
		do{
			//At winXP system the "." means current directory, the ".."means parent directory.
			if(fd.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY && fd.cFileName[0] != TEXT('.'))
			{		
				//It is directory	
				TSTRING strNextDir = strPath;
				if(strNextDir[strNextDir.size() -1] != TEXT('//'))
					strNextDir += TEXT("//");

				strNextDir += fd.cFileName;	
				DeleteDirectory(strNextDir);
				RemoveDirectory(strNextDir.c_str());
			}
			else if(fd.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY)
			{	
				//It is file 
				TSTRING strPathFile = strPath;
				if(strPathFile[strPathFile.size() - 1] !=TEXT( '//'))
					strPathFile += TEXT("//");

				strPathFile += fd.cFileName;
				DeleteFile(strPathFile.c_str());
			}
		}while(FindNextFile(hdFind,&fd));
	}
	FindClose(hdFind);

	RemoveDirectory(strPath.c_str());
}

int CDirectory::FindString(const TCHAR *szSource, const TCHAR *szFind, const int iBeginPos)
{
	int iLenSource = _tcslen(szSource);
	int iLenFind = _tcslen(szFind);

	if(iLenSource - 1 < iBeginPos)
	{
		return -1;
	}

	int iCount = 0;
	int iFindCount = 0;
	BOOL bPair = FALSE;
	for(iCount = 0; iCount < iLenSource - iBeginPos; iCount++)
	{
		if(szSource[iCount + iBeginPos] == szFind[iFindCount])
		{
			if(iFindCount == iLenFind - 1)
			{
				bPair = TRUE;
				break;
			}
			iFindCount++;			
		}
		else
		{
			iFindCount = 0;
		}
	}

	int iFindPos ;

	if(bPair == FALSE)
	{
		iFindPos = -1;
	}
	else
	{
		iFindPos = iCount + iBeginPos - iLenFind + 1;
	}
	return iFindPos;
}

DWORD CDirectory::GetDirectorySize(const TSTRING &strPath)
{
	DWORD dSize = 0;
	//查找路径下的文件夹和所需文件
	TSTRING strDirPath = strPath;

#ifdef _WIN32_WCE
	if(strDirPath[0] != TEXT('//'))
	{
		return FALSE;
	}
#endif

	if(strDirPath[strDirPath.size() - 1] == TEXT('//'))
	{
		strDirPath += TEXT("*.*");
	}
	else
	{
		strDirPath += TEXT("//*.*");
	}

	WIN32_FIND_DATA fd;
	HANDLE hdFind;
	hdFind = FindFirstFile(strDirPath.c_str(),&fd);
	if(hdFind != INVALID_HANDLE_VALUE)
	{
		do{
			//At winXP system the "." means current directory, the ".."means parent directory.
			if(fd.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY && fd.cFileName[0] != TEXT('.'))
			{			
				//It is directory	
				TSTRING strNextDir = strPath;
				if(strNextDir[strNextDir.size() -1] != TEXT('//'))
					strNextDir += TEXT("//");

				strNextDir += fd.cFileName;

				DWORD dDirectorySize = GetDirectorySize(strNextDir);
				dSize += dDirectorySize;		
			}
			else if(fd.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY)
			{	
				//It is file 
				TSTRING strPathFile = strPath;
				if(strPathFile[strPathFile.size() - 1] != TEXT('//'))
					strPathFile += TEXT("//");

				strPathFile += fd.cFileName;

				HANDLE hFile = CreateFile(strPathFile.c_str(),GENERIC_READ,0,NULL,OPEN_EXISTING,0,NULL);
				dSize += ::GetFileSize(hFile,NULL);
				CloseHandle(hFile);
			}
		}while(FindNextFile(hdFind,&fd));
	}
	FindClose(hdFind);
	return dSize;
}

BOOL CDirectory::CheckDirectoryExist(const TSTRING &strPath)
{
	BOOL bReturn = FALSE;
	if(strPath.size() >= MAX_PATH)
	{
		return FALSE;
	}
	WIN32_FIND_DATA fd;
	HANDLE hdFind = FindFirstFile(strPath.c_str(),&fd);
	if(hdFind != INVALID_HANDLE_VALUE)
	{
		if(fd.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
		{
			bReturn = TRUE;
		}
	}
	FindClose(hdFind);
	return bReturn;
}

BOOL CDirectory::CopyDirectory(const TSTRING &strDestinationPath,const TSTRING &strSourcePath) 
{
	TSTRING strDirPath = strSourcePath;
	BOOL bResult = TRUE;
#ifdef _WIN32_WCE
	if(strDirPath[0] != '//')
	{
		return FALSE;
	}
#endif
	if(strDirPath[strDirPath.size() - 1] == '//')
	{
		strDirPath += TEXT("*.*");
	}
	else
	{
		strDirPath += TEXT("//*.*");
	}

	WIN32_FIND_DATA fd;
	HANDLE hdFind;
	hdFind = FindFirstFile(strDirPath.c_str(),&fd);
	if(hdFind != INVALID_HANDLE_VALUE)
	{
		do{
			if(!CheckDirectoryExist(strDestinationPath))
			{
				CreateDirectory(strDestinationPath.c_str(),NULL);

			}

			//At winXP system the "." means current directory, the ".."means parent directory.
			if(fd.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY && fd.cFileName[0] != TEXT('.'))
			{		
				//It is directory	
				TSTRING strSourceNextDir = strSourcePath;
				TSTRING strDestinationNextDir = strDestinationPath;

				if(strSourceNextDir[strSourceNextDir.size() -1] != TEXT('//'))
				{
					strSourceNextDir += TEXT("//");
				}

				if(strDestinationNextDir[strDestinationNextDir.size() -1] != TEXT('//'))
				{
					strDestinationNextDir += TEXT("//");
				}

				strSourceNextDir += fd.cFileName;
				strDestinationNextDir += fd.cFileName;

				CreateDirectory(strDestinationNextDir.c_str(),NULL);

				CopyDirectory(strDestinationNextDir,strSourceNextDir);
			}
			else if(fd.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY)
			{	
				//It is file 
				TSTRING strSourceFilePath = strSourcePath;
				TSTRING strDestinationFilePath = strDestinationPath;

				if(strSourceFilePath[strSourceFilePath.size() - 1] != TEXT('//'))
				{
					strSourceFilePath += TEXT("//");
				}

				if(strDestinationFilePath[strDestinationFilePath.size() - 1] != TEXT('//'))
				{
					strDestinationFilePath += TEXT("//");
				}

				strSourceFilePath += fd.cFileName;
				strDestinationFilePath += fd.cFileName;

				//The file is read-only ,cancle the attributes of read-only
				if(GetFileAttributes(strSourceFilePath.c_str()) & FILE_ATTRIBUTE_READONLY) 
					SetFileAttributes(strSourceFilePath.c_str(), FILE_ATTRIBUTE_NORMAL); 

				if(GetFileAttributes(strDestinationFilePath.c_str()) &FILE_ATTRIBUTE_READONLY) 
					SetFileAttributes(strDestinationFilePath.c_str(), FILE_ATTRIBUTE_NORMAL);

				if(CopyFile(strSourceFilePath.c_str(),strDestinationFilePath.c_str(),FALSE)==FALSE)
				{ 	
#ifdef _WIN32_WCE
					swprintf(TEXT("Copy File  %s   Failed ! Error Code : 0x%x/r/n"),strSourceFilePath.c_str(),GetLastError());
					ASSERT(FALSE);
#else 
					TSTRING strInfo;
					strInfo = TEXT("Copy File/ ");
					strInfo += strSourceFilePath;
					strInfo += TEXT("/  Failed !");
					OutputDebugString(strInfo.c_str());
#endif
					return FALSE;
				}
			}
		}while(FindNextFile(hdFind,&fd));
	}
	else
	{
		bResult = FALSE;
	}
	FindClose(hdFind);
	return bResult;
}

BOOL CDirectory::FindFileFromDirectory(const TSTRING &DirectoryPath,const TSTRING &SuffixName,std::vector<TSTRING> &vFilePathList)
{
	if(DirectoryPath.size() >= MAX_PATH)
	{
		return FALSE;
	}

	TSTRING FindDirPath = DirectoryPath;

	if(FindDirPath[FindDirPath.size() - 1] == TEXT('//'))
	{
		FindDirPath += TEXT("*.*");
	}
	else
	{
		FindDirPath += TEXT("//*.*");
	}

	WIN32_FIND_DATA fd;
	HANDLE hdFind;
	hdFind = FindFirstFile(FindDirPath.c_str(),&fd);
	if(hdFind != INVALID_HANDLE_VALUE)
	{
		do{
			//At winXP system the "." means current directory, the ".."means parent directory.
			if(fd.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY && fd.cFileName[0] != TEXT('.'))
			{		
				//it must be directory	
				TSTRING strFindNextDir = FindDirPath;
				if(strFindNextDir[strFindNextDir.size() -1] != TEXT('//'))
					strFindNextDir += TEXT("//");

				strFindNextDir += fd.cFileName;

				FindFileFromDirectory(strFindNextDir,SuffixName,vFilePathList);
			}
			else if(fd.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY)
			{	
				//it is file 
				TSTRING strFindPathFile = DirectoryPath;
				if(strFindPathFile[strFindPathFile.size() - 1] != TEXT('//'))
					strFindPathFile += TEXT("//");

				strFindPathFile += fd.cFileName;
				if(CheckFileSuffix(fd.cFileName,SuffixName) == TRUE)
				{
					vFilePathList.push_back(strFindPathFile);
				}
			}
		}while(FindNextFile(hdFind,&fd));
	}
	FindClose(hdFind);

	return TRUE;
}

BOOL CDirectory:: CheckFileSuffix(const TSTRING &FileName,const TSTRING &SuffixName)
{
	if(SuffixName.empty())
	{
		return FALSE;
	}

	if(FileName.empty())
	{
		return FALSE;
	}

	if(SuffixName[0] != TEXT('*'))
	{	
		return (FileName.compare(SuffixName) == 0) ? TRUE : FALSE;
	}
	else
	{
		if(FileName.rfind(&SuffixName[1]) != TSTRING::npos)
			return TRUE;
	}
	return FALSE;
}

#ifndef _WIN32_WCE
void CDirectory::GetDiskPartition(std::multimap<FileDeviceType,std::wstring> &mDiskParition)
{
	HANDLE   hDevice;     
	std::wstring strBaseDisk = TEXT("////.//");

	TCHAR BaseNumber = TEXT('A');

	//The Disk partition from'A' to 'Z'
	for(int i = 0; i<26;i++)
	{
		std::wstring strDiskNumber;
		strDiskNumber += (BaseNumber + i);
		strDiskNumber += TEXT(':');


		std::wstring strDiskName = strBaseDisk;
		strDiskName +=strDiskNumber;


		hDevice =  CreateFile(strDiskName.c_str(), GENERIC_READ,   
			FILE_SHARE_READ | FILE_SHARE_WRITE,   
			NULL, OPEN_EXISTING,   
			FILE_ATTRIBUTE_NORMAL,NULL);   

		if(hDevice != INVALID_HANDLE_VALUE)  
		{
			strDiskNumber += TEXT("//");

			if(DRIVE_CDROM == GetDriveType(strDiskNumber.c_str()))
			{
				mDiskParition.insert(make_pair(CDROM,strDiskNumber));

			}else if(DRIVE_REMOVABLE == GetDriveType(strDiskNumber.c_str()))
			{

				mDiskParition.insert(make_pair(DISK_REMOVABLE,strDiskNumber));

			}
			else if(DRIVE_FIXED == GetDriveType(strDiskNumber.c_str()))
			{

				mDiskParition.insert(make_pair(DISK_FIXED,strDiskNumber));

			}

		}
		CloseHandle(hDevice);
	}
}
#endif






// 	CDirector DirOperator;
// 	//根据绝对路径创建文件夹，如果绝对路径名有不存在的目录将被创建。如在C盘下有个Dir的文件//夹，但是在Dir子目录文件夹中没有A文件夹通过下面的调用：
// 	DirOperator. CreateDirectorySerial(TEXT("c://Dir//A//B"));
// 	//将会为Dir创建一个A的子目录同时也会为为A创建一个B的子目录。
// 	//获取指定文件夹的大小，将返回B文件夹的大小
// 	DirOperator. GetDirectorySize(TEXT("c://Dir//A//B"));
// 	//检查目录是否存在，如果B目录存在将返回TRUE，否则返回FALSE
// 	if(CheckDirectoryExist(TEXT("c://Dir//A//B")))
// 	{
// 		RETAILMSG(TRUE,(TEXT("Directory exist!/r/n")));
// 	}
// 	else
// 	{
// 		RETAILMSG(TRUE,(TEXT("Directory does not exist!/r/n")));
// 	}
// 	//拷贝文件夹,将C盘下的B目录下的所有文件拷贝到D下的C文件夹下，如果目标路径中有不存在//的文件夹将被创建
// 	DirOperator. CopyDirectory (TEXT("D://Dir//A//C"),TEXT("c://Dir//A//B")):
// 
// 	//在文件夹中查找文件, vFilePathList中存储了B文件夹下所有File.exe路径名
// 	std::vector<TSTRING> vFilePathList
// 	DirOperator.FindFileFromDirectory(TEXT("c://Dir//A//B"),TEXT("File.exe"),vFilePathList);
// 	// vFilePathList中存储了B文件夹下所有扩展名为“.exe”路径名
// 	DirOperator.FindFileFromDirectory(TEXT("c://Dir//A//B"),TEXT("*.exe"),vFilePathList);
// 	//删除文件夹,将删除文件下的所有文件
// 	DirOperator. DeleteDirectorye(TEXT("c://Dir"));
