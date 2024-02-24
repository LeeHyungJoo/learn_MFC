#include "pch.h"
#include "JUtill.h"

BOOL JUtill::DirectoryExist(const CString & path)
{
	CFileFind finder;
	BOOL bFound = FALSE;
	if (finder.FindFile(path) && finder.FindNextFile()) 
		bFound = finder.IsDirectory();

	finder.Close();
	return bFound;
}

BOOL JUtill::LoadDAO(const CString& filePath, DAO * dao)
{
	CFile file;
	CFileException ex;

	if (!file.Open(filePath, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeRead, &ex))
	{
		ex.ReportError();
		return FALSE;
	}

	CFileStatus status;
	if (file.GetStatus(status) && status.m_size != 0)
	{
		CArchive ar(&file, CArchive::load);
		dao->Serialize(ar);
	}

	return TRUE;
}

BOOL JUtill::SaveDAO(const CString& filePath, DAO * dao)
{
	CFile file;
	CFileException ex;

	if (!file.Open(filePath, CFile::modeCreate | CFile::modeWrite, &ex))
	{
		ex.ReportError();
		return FALSE;
	}

	CArchive ar(&file, CArchive::store);
	dao->Serialize(ar);
	return TRUE;
}
