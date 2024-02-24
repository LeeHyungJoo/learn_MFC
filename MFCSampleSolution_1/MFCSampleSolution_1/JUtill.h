#pragma once

class JUtill
{
public:
	static BOOL DirectoryExist(const CString& path);
	static BOOL LoadDAO(const CString& filePath, DAO* dao);
	static BOOL SaveDAO(const CString& filePath, DAO* dao);
};