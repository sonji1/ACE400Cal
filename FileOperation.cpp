// FileOperation.cpp: implementation of the CFileOperation class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ACE400.h"
#include "FileOperation.h"
#include <direct.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFileOperation::CFileOperation()
{

}

CFileOperation::~CFileOperation()
{

}

CString CFileOperation::GetModelName()
{
//	m_CurModelName = g_sFileData.ModelDataName;
	return m_CurModelName;
}

// test 항목임.. 나중에 어디다 사용할지 ...
void CFileOperation::SearchDir()
{
	CFileFind cff;
	CString path, rootPath, strFullPath, tmp, ModelName;
	
	ModelName = "AAAA";
	//이거는 현재의 디렉토리명을 읽어 오는거임. 어디다 사용하냐 함.. 비베 프로그램 실행할때 사용함 된당.
	GetCurrentDirectory(80, (char*)(LPCTSTR(rootPath)));
	path.Format("%s\\Model\\%s\\*.*", rootPath, ModelName);
	strFullPath.Format("%s\\Model\\%s", rootPath, ModelName);


	if(cff.FindFile((char*)(LPCTSTR(path)), 0))
	{
		while(cff.FindNextFile())
		{
			if(!cff.IsDirectory())  //디렉토리가 아니면 즉 파일이면 ...
			{
				tmp= cff.GetFilePath();    //파일 이름 및 경로를 주서와서 
//				DeleteFile(tmp);
			}
			else{
				tmp = cff.GetFileName();
			}
		}
		if(!cff.IsDirectory())  //디렉토리가 아니면 즉 파일이면 ...
		{
			tmp = cff.GetFilePath();     //파일 이름 및 경로를 주서와서 
			//DeleteFile(tmp);
		}
		else
				tmp = cff.GetFileName();
		cff.Close();
	}

}

//새로은 디렉토리를 만든다.
BOOL CFileOperation::MakeNewDirectory(CString DirPath)
{
	if(_mkdir((char*)(LPCTSTR(DirPath))) == -1) return FALSE;

	return TRUE;
}

//Directory안에 있는 모든 파일이든 디렉토리든 다 지운다.
BOOL CFileOperation::DeleteDirectory(CString dirPath)
{
	CFileFind cff;	
	CString path, dFile, tmp;

	path.Format("%s\\*.*", dirPath);
	
	if(cff.FindFile((char*)(LPCTSTR(path)), 0))
	{
		while(cff.FindNextFile())
		{
			if(!cff.IsDirectory()) //디렉토리가 아니면 즉 파일이면 ...
			{
				tmp= cff.GetFileName();    //이름을 주서와서 
				dFile.Format("%s\\%s", dirPath, tmp);
				SetFileAttributes(dFile, FILE_ATTRIBUTE_NORMAL);
				DeleteFile(dFile);
			}
			else{	// 만약 디렉토리이면 DeleteDirectory()함수 재귀 호출....
				tmp= cff.GetFileName();    //이름을 주서와서 
				if(tmp != "." && tmp != ".."){
					dFile.Format("%s\\%s", dirPath, tmp);
					//재귀 호출
					DeleteDirectory(dFile);
				}
			}
		}
		if(!cff.IsDirectory()) //디렉토리가 아니면 즉 파일이면 ...
		{
			tmp= cff.GetFileName();    //이름을 주서와서 
			dFile.Format("%s\\%s", dirPath, tmp);
			SetFileAttributes(dFile, FILE_ATTRIBUTE_NORMAL);
			DeleteFile(dFile);
		}
		else{	// 만약 디렉토리이면 DeleteDirectory()함수 재귀 호출....
			tmp= cff.GetFileName();    //이름을 주서와서 
			if(tmp != "." && tmp != ".."){
				dFile.Format("%s\\%s", dirPath, tmp);
				//재귀 호출
				DeleteDirectory(dFile);
			}
		}
		cff.Close();
	}
	RemoveDirectory(dirPath);

	return TRUE;
}

BOOL CFileOperation::CopyDirectory(CString SourceDirPath, CString TargetDirPath)
{
	CFileFind cff;
	BOOL flag;
	CString sPath, tPath, sFile, tFile, tmp;
	CString nextSourcePath, nextTargetPath;

	//우선 디렉토리를 만들고
	flag = MakeNewDirectory(TargetDirPath);
	if(flag == FALSE) //Derectory를 만들지 못함.. 에러 발생
		return FALSE;
	//파일을 읽어 와서 복사하고
	sPath.Format("%s\\*.*", SourceDirPath);
	
	if(cff.FindFile((char*)(LPCTSTR(sPath)), 0))
	{
		while(cff.FindNextFile())
		{
			if(!cff.IsDirectory())  //디렉토리가 아니면 즉 파일이면 ...
			{
				tmp= cff.GetFileName();    //이름을 주서와서 
				sFile.Format("%s\\%s", SourceDirPath, tmp);
				tFile.Format("%s\\%s", TargetDirPath, tmp);
				CopyFile(sFile, tFile, TRUE);
//				DeleteFile(tmp);
			}
			else{
				tmp = cff.GetFileName();
				if(tmp != "." && tmp != ".."){
					sFile.Format("%s\\%s", SourceDirPath, tmp);
					tFile.Format("%s\\%s", TargetDirPath, tmp);
					//재귀 호출
					CopyDirectory(sFile, tFile);
				}
			}
		}
		if(!cff.IsDirectory())  //디렉토리가 아니면 즉 파일이면 ...
		{
			tmp= cff.GetFileName();    //이름을 주서와서 
			sFile.Format("%s\\%s", SourceDirPath, tmp);
			tFile.Format("%s\\%s", TargetDirPath, tmp);
			CopyFile(sFile, tFile, TRUE);
//				DeleteFile(tmp);
		}
		else{
			tmp = cff.GetFileName();
			if(tmp != "." && tmp != ".."){
				sFile.Format("%s\\%s", SourceDirPath, tmp);
				tFile.Format("%s\\%s", TargetDirPath, tmp);
				//재귀 호출
				CopyDirectory(sFile, tFile);
			}
		}
		cff.Close();
	}

	//종료하자
	return TRUE;
}

//CheckDirectoryPath : 디렉터리 이름을 검사하고자 하는 경로, CheckName : 검사하고자 하는 디렉터리 명
BOOL CFileOperation::CheckDirectory(CString CheckDirectoryPath, CString CheckName)
{
	CFileFind cff;
	CString tmp, dirPath, source, target;

	source = _strupr( _strdup(CheckName));

	dirPath.Format("%s\\*.*", CheckDirectoryPath);
	if(cff.FindFile((char*)(LPCTSTR(dirPath)), 0))
	{
		while(cff.FindNextFile())
		{
			if(cff.IsDirectory())  //디렉토리이면 ...
			{
				tmp= cff.GetFileName();    //이름을 주서와서 
				target = _strupr( _strdup(tmp));
 				if(source == target){
					cff.Close();
					return TRUE;
				}
			}
		}
		//while 문으로 FindNextFile을 돌리면 마지막의 하나는 읽지 못해서 마지막거 다시 읽게 해야 함
		if(cff.IsDirectory())  //디렉토리이면 ...
		{
			tmp = cff.GetFileName();
			target = _strupr( _strdup(tmp));
			if(source == target){
				cff.Close();
				return TRUE;
			}
		}
		cff.Close();
	}
	return FALSE;
}

//새로운 모델을 맹근다....
void CFileOperation::MakeNewModel(CString NewModelName)
{
	int ret;
	CString strFullPath, rootPath;

	GetCurrentDirectory(80, (char*)(LPCTSTR(rootPath)));
	strFullPath.Format("%s\\Model\\%s\\", rootPath, NewModelName);
	ret = _mkdir((char*)(LPCTSTR(strFullPath)));
}

//이거는 현재의 모델을 다른 모델명으로 변경 저장하는 거
BOOL CFileOperation::SaveAsModel(CString SourceModelName, CString TargetModelName)
{
	CFileFind cff;
	BOOL checkFlag;
	CString fullPath, rootPath, sourcePath, targetPath, tmp;


	// 모델의 전체 경로를 맹글고
	GetCurrentDirectory(80, (char*)(LPCTSTR(tmp)));
	rootPath.Format("%s\\Model\\", tmp);
	// source Model명이 존재 하는지를 확인하고, 존재하지 않으면 에러징
	sourcePath.Format("%s\\Model", tmp);
	checkFlag = CheckDirectory(sourcePath, SourceModelName);
	//checkFlag가 FALSE이면 sourcePath에 SourceModelName이 존재하는 것이므로 에러 리턴
	if(checkFlag == FALSE){
//		Message("복사하고자하는 소스 모델명이 존재하지 않습니다.");
		Message("There is no such source model file exist.");
		return FALSE;
	}

	// Target Model명이 존재하는지를 확인하고, 존재하면 에러징
	targetPath.Format("%s\\Model", tmp);
	checkFlag = CheckDirectory(targetPath, TargetModelName);
	//checkFlag가 TRUE이면 targetPath에 TargetModelName이 존재하는 것이므로 에러 리턴
	if(checkFlag == TRUE){
//		Message("같은 이름의 모델명이 존재 합니다.");
		Message("There is same model name exist.");
		return FALSE;
	}

	sourcePath = rootPath + SourceModelName;
	targetPath = rootPath + TargetModelName;

	// 새로 폴더를 만들고 Source Model에 존재하는 파일을 복사한다.
	CopyDirectory(sourcePath, targetPath);

	return TRUE;
}

//모델을 지운다....
BOOL CFileOperation::DeleteModel(CString ModelName)
{
	CString strFullPath, rootPath, tmp;

	// 모델의 전체 경로를 맹글고
	GetCurrentDirectory(80, (char*)(LPCTSTR(tmp)));
	strFullPath.Format("%s\\Model\\%s", tmp, ModelName);

	//Directory를 지운다
	DeleteDirectory(strFullPath);
	return TRUE;
}

//프로그램에서 저장되어 있는 데이터를 읽어 올때 저장되어 있는 파일이 존재하는지를 확인하는 함수.
BOOL CFileOperation::CheckExistFileName(CString PathFileName)
{
	CFileFind cff;

	if(cff.FindFile((char*)(LPCTSTR(PathFileName)), 0))
		return TRUE;
	else 
		return FALSE;
}

BOOL CFileOperation::RenameModel(CString SourceModelName, CString TargetModelName)
{
	CFileFind cff;
	BOOL checkFlag;
	CString fullPath, rootPath, sourcePath, targetPath, tmp;


	// 모델의 전체 경로를 맹글고
	GetCurrentDirectory(80, (char*)(LPCTSTR(tmp)));
	rootPath.Format("%s\\Model\\", tmp);
	// source Model명이 존재 하는지를 확인하고, 존재하지 않으면 에러징
	sourcePath.Format("%s\\Model", tmp);
	checkFlag = CheckDirectory(sourcePath, SourceModelName);
	//checkFlag가 FALSE이면 sourcePath에 SourceModelName이 존재하는 것이므로 에러 리턴
	if(checkFlag == FALSE){
//		Message("복사하고자하는 소스 모델명이 존재하지 않습니다.",MB_OK);
		Message("There is no such source model file exist.",MB_OK);
		return FALSE;
	}

	// Target Model명이 존재하는지를 확인하고, 존재하면 에러징
	targetPath.Format("%s\\Model", tmp);
	checkFlag = CheckDirectory(targetPath, TargetModelName);
	//checkFlag가 TRUE이면 targetPath에 TargetModelName이 존재하는 것이므로 에러 리턴
	if(checkFlag == TRUE){
//		Message("같은 이름의 모델명이 존재 합니다.",MB_OK);
		Message("There is same model name exist.",MB_OK);
		return FALSE;
	}

	sourcePath = rootPath + SourceModelName;
	targetPath = rootPath + TargetModelName;

   int result = rename(sourcePath, targetPath);
   if( result != 0 )
//     Message( "모델명을 변경할 수 없습니다.");
     Message( "The model name can not be changed.");

	return TRUE;
}
