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

// test �׸���.. ���߿� ���� ������� ...
void CFileOperation::SearchDir()
{
	CFileFind cff;
	CString path, rootPath, strFullPath, tmp, ModelName;
	
	ModelName = "AAAA";
	//�̰Ŵ� ������ ���丮���� �о� ���°���. ���� ����ϳ� ��.. �� ���α׷� �����Ҷ� ����� �ȴ�.
	GetCurrentDirectory(80, (char*)(LPCTSTR(rootPath)));
	path.Format("%s\\Model\\%s\\*.*", rootPath, ModelName);
	strFullPath.Format("%s\\Model\\%s", rootPath, ModelName);


	if(cff.FindFile((char*)(LPCTSTR(path)), 0))
	{
		while(cff.FindNextFile())
		{
			if(!cff.IsDirectory())  //���丮�� �ƴϸ� �� �����̸� ...
			{
				tmp= cff.GetFilePath();    //���� �̸� �� ��θ� �ּ��ͼ� 
//				DeleteFile(tmp);
			}
			else{
				tmp = cff.GetFileName();
			}
		}
		if(!cff.IsDirectory())  //���丮�� �ƴϸ� �� �����̸� ...
		{
			tmp = cff.GetFilePath();     //���� �̸� �� ��θ� �ּ��ͼ� 
			//DeleteFile(tmp);
		}
		else
				tmp = cff.GetFileName();
		cff.Close();
	}

}

//������ ���丮�� �����.
BOOL CFileOperation::MakeNewDirectory(CString DirPath)
{
	if(_mkdir((char*)(LPCTSTR(DirPath))) == -1) return FALSE;

	return TRUE;
}

//Directory�ȿ� �ִ� ��� �����̵� ���丮�� �� �����.
BOOL CFileOperation::DeleteDirectory(CString dirPath)
{
	CFileFind cff;	
	CString path, dFile, tmp;

	path.Format("%s\\*.*", dirPath);
	
	if(cff.FindFile((char*)(LPCTSTR(path)), 0))
	{
		while(cff.FindNextFile())
		{
			if(!cff.IsDirectory()) //���丮�� �ƴϸ� �� �����̸� ...
			{
				tmp= cff.GetFileName();    //�̸��� �ּ��ͼ� 
				dFile.Format("%s\\%s", dirPath, tmp);
				SetFileAttributes(dFile, FILE_ATTRIBUTE_NORMAL);
				DeleteFile(dFile);
			}
			else{	// ���� ���丮�̸� DeleteDirectory()�Լ� ��� ȣ��....
				tmp= cff.GetFileName();    //�̸��� �ּ��ͼ� 
				if(tmp != "." && tmp != ".."){
					dFile.Format("%s\\%s", dirPath, tmp);
					//��� ȣ��
					DeleteDirectory(dFile);
				}
			}
		}
		if(!cff.IsDirectory()) //���丮�� �ƴϸ� �� �����̸� ...
		{
			tmp= cff.GetFileName();    //�̸��� �ּ��ͼ� 
			dFile.Format("%s\\%s", dirPath, tmp);
			SetFileAttributes(dFile, FILE_ATTRIBUTE_NORMAL);
			DeleteFile(dFile);
		}
		else{	// ���� ���丮�̸� DeleteDirectory()�Լ� ��� ȣ��....
			tmp= cff.GetFileName();    //�̸��� �ּ��ͼ� 
			if(tmp != "." && tmp != ".."){
				dFile.Format("%s\\%s", dirPath, tmp);
				//��� ȣ��
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

	//�켱 ���丮�� �����
	flag = MakeNewDirectory(TargetDirPath);
	if(flag == FALSE) //Derectory�� ������ ����.. ���� �߻�
		return FALSE;
	//������ �о� �ͼ� �����ϰ�
	sPath.Format("%s\\*.*", SourceDirPath);
	
	if(cff.FindFile((char*)(LPCTSTR(sPath)), 0))
	{
		while(cff.FindNextFile())
		{
			if(!cff.IsDirectory())  //���丮�� �ƴϸ� �� �����̸� ...
			{
				tmp= cff.GetFileName();    //�̸��� �ּ��ͼ� 
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
					//��� ȣ��
					CopyDirectory(sFile, tFile);
				}
			}
		}
		if(!cff.IsDirectory())  //���丮�� �ƴϸ� �� �����̸� ...
		{
			tmp= cff.GetFileName();    //�̸��� �ּ��ͼ� 
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
				//��� ȣ��
				CopyDirectory(sFile, tFile);
			}
		}
		cff.Close();
	}

	//��������
	return TRUE;
}

//CheckDirectoryPath : ���͸� �̸��� �˻��ϰ��� �ϴ� ���, CheckName : �˻��ϰ��� �ϴ� ���͸� ��
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
			if(cff.IsDirectory())  //���丮�̸� ...
			{
				tmp= cff.GetFileName();    //�̸��� �ּ��ͼ� 
				target = _strupr( _strdup(tmp));
 				if(source == target){
					cff.Close();
					return TRUE;
				}
			}
		}
		//while ������ FindNextFile�� ������ �������� �ϳ��� ���� ���ؼ� �������� �ٽ� �а� �ؾ� ��
		if(cff.IsDirectory())  //���丮�̸� ...
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

//���ο� ���� �ͱٴ�....
void CFileOperation::MakeNewModel(CString NewModelName)
{
	int ret;
	CString strFullPath, rootPath;

	GetCurrentDirectory(80, (char*)(LPCTSTR(rootPath)));
	strFullPath.Format("%s\\Model\\%s\\", rootPath, NewModelName);
	ret = _mkdir((char*)(LPCTSTR(strFullPath)));
}

//�̰Ŵ� ������ ���� �ٸ� �𵨸����� ���� �����ϴ� ��
BOOL CFileOperation::SaveAsModel(CString SourceModelName, CString TargetModelName)
{
	CFileFind cff;
	BOOL checkFlag;
	CString fullPath, rootPath, sourcePath, targetPath, tmp;


	// ���� ��ü ��θ� �ͱ۰�
	GetCurrentDirectory(80, (char*)(LPCTSTR(tmp)));
	rootPath.Format("%s\\Model\\", tmp);
	// source Model���� ���� �ϴ����� Ȯ���ϰ�, �������� ������ ����¡
	sourcePath.Format("%s\\Model", tmp);
	checkFlag = CheckDirectory(sourcePath, SourceModelName);
	//checkFlag�� FALSE�̸� sourcePath�� SourceModelName�� �����ϴ� ���̹Ƿ� ���� ����
	if(checkFlag == FALSE){
//		Message("�����ϰ����ϴ� �ҽ� �𵨸��� �������� �ʽ��ϴ�.");
		Message("There is no such source model file exist.");
		return FALSE;
	}

	// Target Model���� �����ϴ����� Ȯ���ϰ�, �����ϸ� ����¡
	targetPath.Format("%s\\Model", tmp);
	checkFlag = CheckDirectory(targetPath, TargetModelName);
	//checkFlag�� TRUE�̸� targetPath�� TargetModelName�� �����ϴ� ���̹Ƿ� ���� ����
	if(checkFlag == TRUE){
//		Message("���� �̸��� �𵨸��� ���� �մϴ�.");
		Message("There is same model name exist.");
		return FALSE;
	}

	sourcePath = rootPath + SourceModelName;
	targetPath = rootPath + TargetModelName;

	// ���� ������ ����� Source Model�� �����ϴ� ������ �����Ѵ�.
	CopyDirectory(sourcePath, targetPath);

	return TRUE;
}

//���� �����....
BOOL CFileOperation::DeleteModel(CString ModelName)
{
	CString strFullPath, rootPath, tmp;

	// ���� ��ü ��θ� �ͱ۰�
	GetCurrentDirectory(80, (char*)(LPCTSTR(tmp)));
	strFullPath.Format("%s\\Model\\%s", tmp, ModelName);

	//Directory�� �����
	DeleteDirectory(strFullPath);
	return TRUE;
}

//���α׷����� ����Ǿ� �ִ� �����͸� �о� �ö� ����Ǿ� �ִ� ������ �����ϴ����� Ȯ���ϴ� �Լ�.
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


	// ���� ��ü ��θ� �ͱ۰�
	GetCurrentDirectory(80, (char*)(LPCTSTR(tmp)));
	rootPath.Format("%s\\Model\\", tmp);
	// source Model���� ���� �ϴ����� Ȯ���ϰ�, �������� ������ ����¡
	sourcePath.Format("%s\\Model", tmp);
	checkFlag = CheckDirectory(sourcePath, SourceModelName);
	//checkFlag�� FALSE�̸� sourcePath�� SourceModelName�� �����ϴ� ���̹Ƿ� ���� ����
	if(checkFlag == FALSE){
//		Message("�����ϰ����ϴ� �ҽ� �𵨸��� �������� �ʽ��ϴ�.",MB_OK);
		Message("There is no such source model file exist.",MB_OK);
		return FALSE;
	}

	// Target Model���� �����ϴ����� Ȯ���ϰ�, �����ϸ� ����¡
	targetPath.Format("%s\\Model", tmp);
	checkFlag = CheckDirectory(targetPath, TargetModelName);
	//checkFlag�� TRUE�̸� targetPath�� TargetModelName�� �����ϴ� ���̹Ƿ� ���� ����
	if(checkFlag == TRUE){
//		Message("���� �̸��� �𵨸��� ���� �մϴ�.",MB_OK);
		Message("There is same model name exist.",MB_OK);
		return FALSE;
	}

	sourcePath = rootPath + SourceModelName;
	targetPath = rootPath + TargetModelName;

   int result = rename(sourcePath, targetPath);
   if( result != 0 )
//     Message( "�𵨸��� ������ �� �����ϴ�.");
     Message( "The model name can not be changed.");

	return TRUE;
}
