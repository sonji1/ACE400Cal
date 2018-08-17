// FileOperation.h: interface for the CFileOperation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILEOPERATION_H__9704159D_7734_4244_B6A9_84AFF7F1EE41__INCLUDED_)
#define AFX_FILEOPERATION_H__9704159D_7734_4244_B6A9_84AFF7F1EE41__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFileOperation  
{
public:
	CFileOperation();
	virtual ~CFileOperation();

	CString m_CurModelName;
	CString GetModelName();

	void SearchDir();
	BOOL MakeNewDirectory(CString DirPath);
	BOOL DeleteDirectory(CString dirPath);
	BOOL CopyDirectory(CString SourceDirPath, CString TargetDirPath);
	BOOL CheckDirectory(CString CheckDirectoryPath, CString CheckName);

	void MakeNewModel(CString NewModelName);
	BOOL SaveAsModel(CString SourceModelName, CString TargetModelName);
	BOOL DeleteModel(CString ModelName);
	BOOL RenameModel(CString SourceModelName, CString TargetModelName);
	BOOL CheckExistFileName(CString PathFileName);
};

#endif // !defined(AFX_FILEOPERATION_H__9704159D_7734_4244_B6A9_84AFF7F1EE41__INCLUDED_)
