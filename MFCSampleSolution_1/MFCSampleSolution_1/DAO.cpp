#include "pch.h"
#include "DAO.h"


DAO::DAO()
{
}

DAO::~DAO()
{
}

IMPLEMENT_SERIAL(DAO, CObject, 1);

void DAO::Serialize(CArchive & ar)
{
	CObject::Serialize(ar);

	if (ar.IsStoring())
	{
		ar	<< m_urbtActiveIdx 
			<< m_ubitOptIdx
			<< m_uCnt
			<< m_uElapsed
			<< m_uhsbPos
			<< m_uvsbPos;
	}
	else
	{
		ar	>> m_urbtActiveIdx
			>> m_ubitOptIdx
			>> m_uCnt
			>> m_uElapsed
			>> m_uhsbPos
			>> m_uvsbPos;
	}
}
